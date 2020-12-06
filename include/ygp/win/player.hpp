#ifndef _YGP_WIN_PALYER_HPP_
#define _YGP_WIN_PLAYER_HPP_
#include<functional>
#include<initguid.h>//Initialize GUIDs or there will be linking problems.
#include"sync.hpp"
#include<mfapi.h>
#include<mfplay.h>
#include<shlwapi.h>
#ifdef _MSC_VER
#pragma comment(lib,"mfplay")
#pragma comment(lib,"shlwapi")
#endif
BEGIN_NAMESPACE_YGP
#ifndef YGP_DISABLE_COM
class player;//forward declaration of ygp::player class (for ygp::detail::playercallback)
namespace detail
{
    class playercallback:public IMFPMediaPlayerCallback
    {
        illong m_cRef;//reference count
        public:
            BOOL b;//has video
            player* p;//corresponding player pointer
            playercallback(player* _p);//constructor
            //Methods below are COM Interface implementations.
            IFACEMETHODIMP QueryInterface(REFIID riid,void** ppv)noexcept override
            {
                static const QITAB qit[]={QITABENT(playercallback,
                    IMFPMediaPlayerCallback),{0}};
                return QISearch(this,qit,riid,ppv);
            }
            IFACEMETHODIMP_(ULONG) AddRef()noexcept override
            {
                return ++m_cRef;
            }
            IFACEMETHODIMP_(ULONG) Release()noexcept override
            {
                if(!--m_cRef)
                {
                    delete this;
                    return 0;
                }
                return (ULONG)(LONG)m_cRef;
            }
            virtual void STDMETHODCALLTYPE OnMediaPlayerEvent
                (MFP_EVENT_HEADER *pEventHeader)override;
            BOOL HasVideo()const noexcept
            {
                return b;
            }
    };
}
class player
{
    detail::playercallback* pc;
    public:
        IMFPMediaPlayer* p;
        struct basic_item//wrapper of IMFPMediaItem without destructor
        {
            IMFPMediaItem* p;
            basic_item():p(nullptr){}
            basic_item(IMFPMediaItem* _p):p(_p){}
            operator IMFPMediaItem*()const noexcept
            {
                return p;
            }
            operator bool()const noexcept
            {
                return (bool)p;
            }
            MFP_MEDIAITEM_CHARACTERISTICS characteristics()
            {
                MFP_MEDIAITEM_CHARACTERISTICS ret;
                throw_com_error(p->GetCharacteristics(&ret),
                    "player::item::characteristics",
                    "IMFPMediaItem::GetCharacteristics");
                return ret;
            }
            ULONGLONG duration()
            {
                PROPVARIANT ret;
                throw_com_error(p->GetDuration(MFP_POSITIONTYPE_100NS,&ret),
                    "player::item::duration","IMFPMediaItem::GetDuration");
                ULONGLONG retval=ret.uhVal.QuadPart;
                PropVariantClear(&ret);
                return retval;
            }
            IMFPMediaPlayer* playerptr()
            {
                IMFPMediaPlayer* ret;
                throw_com_error(p->GetMediaPlayer(&ret),
                    "player::item::playerptr",
                    "IMFPMediaItem::GetMediaPlayer");
                return ret;
            }
            DWORD numberofstreams()
            {
                DWORD ret;
                throw_com_error(p->GetNumberOfStreams(&ret),
                    "player::item::numberofstreams",
                    "IMFPMediaItem::GetNumberOfStreams");
                return ret;
            }
            void startstoppos(LONGLONG& _start,LONGLONG& _stop)
            {
                PROPVARIANT _pst,_psp;
                IID mfp100ns=MFP_POSITIONTYPE_100NS;
                throw_com_error(p->GetStartStopPosition(&mfp100ns,
                    &_pst,&mfp100ns,&_psp),
                    "player::item::startstoppos",
                    "IMFPMediaItem::GetStartStopPosition");
                _start=_pst.hVal.QuadPart;
                _stop=_psp.hVal.QuadPart;
                PropVariantClear(&_pst);
                PropVariantClear(&_psp);
            }
            void getstreamattr(DWORD dwindex,REFGUID gattr,PROPVARIANT& var)
            {
                throw_com_error(p->GetStreamAttribute(dwindex,gattr,&var),
                    "player::item::getstreamattr",
                    "IMFPMediaItem::GetStreamAttribute");
            }
            bool selected(DWORD dwindex)
            {
                BOOL bret;
                throw_com_error(p->GetStreamSelection(dwindex,&bret),
                    "player::item::selected",
                    "IMFPMediaItem::GetStreamSelection");
                return (bool)bret;
            }
            std::shared_ptr<WCHAR> url()
            {
                LPWSTR ret;
                throw_com_error(p->GetURL(&ret),
                    "player::item::url","IMFPMediaItem::GetURL");
                return std::shared_ptr<WCHAR>(ret,
                    [](LPWSTR _w){CoTaskMemFree(_w);});
            }
            std::pair<bool,bool> hasaudio()
            {
                BOOL ret1,ret2;
                throw_com_error(p->HasAudio(&ret1,&ret2),
                    "player::item::hasaudio","IMFPMediaItem::HasAudio");
                return {ret1,ret2};
            }
            std::pair<bool,bool> hasvideo()
            {
                BOOL ret1,ret2;
                throw_com_error(p->HasVideo(&ret1,&ret2),
                    "player::item::hasvideo","IMFPMediaItem::HasVideo");
                return {ret1,ret2};
            }
            void setstartstoppos(LONGLONG _start,LONGLONG _stop)
            {
                PROPVARIANT varst,varsp;
                PropVariantInit(&varst);
                PropVariantInit(&varsp);
                varst.vt=VT_I8;
                varsp.vt=VT_I8;
                varst.hVal.QuadPart=_start;
                varsp.hVal.QuadPart=_stop;
                throw_com_error(p->SetStartStopPosition(&MFP_POSITIONTYPE_100NS,
                    &varst,&MFP_POSITIONTYPE_100NS,&varsp),
                    "player::item::setstartstoppos",
                    "IMFPMediaItem::SetStartStopPosition");
            }
            void select(DWORD dwindex,bool fenb)//Set stream selection
            {
                throw_com_error(p->SetStreamSelection(dwindex,(BOOL)fenb),
                    "player::item::select",
                    "IMFPMediaItem::SetStreamSelection");
            }
            float framerate()
            {
                PROPVARIANT r;
                getstreamattr(1ul,MF_MT_FRAME_RATE,r);
                float ret=float(r.uhVal.HighPart)/float(r.uhVal.LowPart);
                PropVariantClear(&r);
                return ret;
            }
            IPropertyStore* metadata()
            {
                IPropertyStore* ret;
                throw_com_error(p->GetMetadata(&ret),"player::item::metadata",
                    "IMFPMediaPlayer::GetMetaData");
                return ret;
            }
        };
        struct item:public basic_item//basic_item with destructor
        {
            item(){}
            item(IMFPMediaItem* _p):basic_item(_p){}
            item(const basic_item& itm){p=itm.p;}
            item(const basic_item&& itm){p=itm.p;}
            item& operator=(const basic_item& _bi)
            {
                p=_bi.p;
                return *this;
            }
            item& operator=(const basic_item&& _bi)
            {
                p=_bi.p;
                return *this;
            }
            ~item()
            {
                if(p)p->Release();
                p=nullptr;
            }
        };
        /*
            The list below are callback functions.
            The syntax is: std::function<void(MFP_***_EVENT*)>.
            It is deprecated to throw exceptions in these functions.
            Sample:
                ygp::player* p=new ygp::player(hwnd);
                p->MEDIAITEM_CREATED_callback=[=]
				    (MFP_MEDIAITEM_CREATED_EVENT* pEvent)
			    {
				    p->setitem(pEvent->pMediaItem);
			    };
			    p->MEDIAITEM_SET_callback=[=]
				    (MFP_MEDIAITEM_SET_EVENT* pEvent)
			    {
				    p->play();
			    };
                p->STOP_callback=[=]
				    (MFP_MEDIAITEM_SET_EVENT* pEvent)
			    {
				    p->play();//Loop playback
			    };
        */
        #define YGP_PLAYER_CALLBACK(t) \
            std::function<void(MFP_##t##_EVENT*)> t##_callback;
        YGP_PLAYER_CALLBACK(PLAY)
        YGP_PLAYER_CALLBACK(PAUSE)
        YGP_PLAYER_CALLBACK(STOP)
        YGP_PLAYER_CALLBACK(POSITION_SET)
        YGP_PLAYER_CALLBACK(RATE_SET)
        YGP_PLAYER_CALLBACK(MEDIAITEM_CREATED)
        YGP_PLAYER_CALLBACK(MEDIAITEM_SET)
        YGP_PLAYER_CALLBACK(FRAME_STEP)
        YGP_PLAYER_CALLBACK(MEDIAITEM_CLEARED)
        YGP_PLAYER_CALLBACK(MF)
        YGP_PLAYER_CALLBACK(ERROR)
        YGP_PLAYER_CALLBACK(PLAYBACK_ENDED)
        YGP_PLAYER_CALLBACK(ACQUIRE_USER_CREDENTIAL)
        #undef YGP_PLAYER_CALLBACK
        std::function<void(std::string/*Error Message*/)> error_handler;
        player(HWND _h,LPCWSTR _u,bool bautostart=false)
        {
            pc=new (std::nothrow) detail::playercallback(this);
            throw_com_error(MFPCreateMediaPlayer(_u,(BOOL)bautostart,0,pc,_h,&p),
                "player::player","MFPCreateMediaPlayer");
        }
        player(HWND _h)
        {
            pc=new (std::nothrow) detail::playercallback(this);
            throw_com_error(MFPCreateMediaPlayer(nullptr,FALSE,0,pc,_h,&p),
                "player::player","MFPCreateMediaPlayer");
        }
        basic_item createitem(LPCWSTR _w,bool bsync=false)
        {
            IMFPMediaItem* ret;
            throw_com_error(p->CreateMediaItemFromURL(_w,(BOOL)bsync,0,&ret),
                "player::createitem",
                "IMFPMediaPlayer::CreateMediaItemFromURL");
            return basic_item(ret);
        }
        void setitem(IMFPMediaItem* _mp)
        {
            throw_com_error(p->SetMediaItem(_mp),"player::setitem",
                "IMFPMediaPlayer::SetMediaItem");
        }
        void setitem(const item& _mp)
        {
            throw_com_error(p->SetMediaItem(_mp.p),"player::setitem",
                "IMFPMediaPlayer::SetMediaItem");
        }
        void play()
        {
            throw_com_error(p->Play(),"player::play","IMFPMediaPlayer::Play");
        }
        void pause()
        {
            throw_com_error(p->Pause(),"player::Pause","IMFPMediaPlayer::Pause");
        }
        void stop()
        {
            throw_com_error(p->Stop(),"player::stop","IMFPMediaItem::Stop");
        }
        void updatevideo()
        {
            throw_com_error(p->UpdateVideo(),"player::UpdateVideo",
                "IMFPMediaPlayer::UpdateVideo");
        }
        BOOL hasvideo()const noexcept
        {
            return pc->HasVideo();
        }
        void clearitem()
        {
            throw_com_error(p->ClearMediaItem(),"player::clearitem",
                "IMFPMediaPlayer::ClearMediaItem");
        }
        void framestep()
        {
            throw_com_error(p->FrameStep(),"player::framestep",
                "IMFPMediaPlayer::FrameStep");
        }
        float balance()
        {
            float ret;
            throw_com_error(p->GetBalance(&ret),"player::balance",
                "IMFPMediaPlayer::GetBalance");
            return ret;
        }
        COLORREF bordercolor()
        {
            COLORREF ret;
            throw_com_error(p->GetBorderColor(&ret),"player::bordercolor",
                "IMFPMediaPlayer::GetBorderColor");
            return ret;
        }
        ULONGLONG duration()//in 100 ns
        {
            PROPVARIANT ret;
            throw_com_error(p->GetDuration(MFP_POSITIONTYPE_100NS,&ret),
                "player::duration","IMFPMediaPlayer::GetDuration");
            ULONGLONG retval=ret.uhVal.QuadPart;
            PropVariantClear(&ret);
            return retval;
        }
        std::pair<SIZE,SIZE> idealvideosize()
        {
            SIZE pMin;
            SIZE pMax;
            throw_com_error(p->GetIdealVideoSize(&pMin,&pMax),
                "player::idealvideosize",
                "IMFPMediaPlayer::GetIdealVideoSize");
            return {pMin,pMax};
        }
        item currentitem()
        {
            IMFPMediaItem* ret;
            throw_com_error(p->GetMediaItem(&ret),
                "player::currentitem","IMFPMediaPlayer::GetMediaItem");
            return item(ret);
        }
        bool ismuted()
        {
            BOOL ret;
            throw_com_error(p->GetMute(&ret),
                "player::ismuted","IMFPMediaPlayer::GetMute");
            return ret;
        }
        SIZE videosize()
        {
            SIZE ret;
            throw_com_error(p->GetNativeVideoSize(&ret,nullptr),
                "player::videosize",
                "IMFPMediaPlayer::GetNativeVideoSize");
            return ret;
        }
        SIZE pictureaspectratio()
        {
            SIZE ret;
            throw_com_error(p->GetNativeVideoSize(nullptr,&ret),
                "player::videosize",
                "IMFPMediaPlayer::GetNativeVideoSize");
            return ret;
        }
        LONGLONG position()
        {
            PROPVARIANT ret;
            throw_com_error(p->GetPosition(MFP_POSITIONTYPE_100NS,&ret),
                "player::position","IMFPMediaPlayer::GetPosition");
            LONGLONG retval=ret.hVal.QuadPart;
            PropVariantClear(&ret);
            return retval;
        }
        float rate()
        {
            float f;
            throw_com_error(p->GetRate(&f),"player::rate",
                "IMFPMediaPlayer::GetRate");
            return f;
        }
        MFP_MEDIAPLAYER_STATE state()
        {
            MFP_MEDIAPLAYER_STATE ret;
            throw_com_error(p->GetState(&ret),
                "player::state","IMFPMediaPlayer::GetState");
            return ret;
        }
        void supportedrates(bool bforward,float flowest,float ffastest)
        {
            throw_com_error(p->GetSupportedRates((BOOL)bforward,
                &flowest,&ffastest),"player::supportedrates",
                "IMFPMediaPlayer::GetSupportedRates");
        }
        MFVideoNormalizedRect videosourcerect()
        {
            MFVideoNormalizedRect ret;
            throw_com_error(p->GetVideoSourceRect(&ret),
                "player::videosourcerect",
                "IMFPMediaPlayer::GetVideoSourceRect");
            return ret;
        }
        HWND videowindow()
        {
            HWND ret;
            throw_com_error(p->GetVideoWindow(&ret),
                "player::videowindow",
                "IMFPMediaPlayer::GetVideoWindow");
            return ret;
        }
        float volume()
        {
            float f;
            throw_com_error(p->GetVolume(&f),"player::volume",
                "IMFPMediaPlayer::GetVolume");
            return f;
        }
        void aspectvideomode(DWORD dwval)
        {
            throw_com_error(p->SetAspectRatioMode(dwval),
                "player::aspectvideomode","IMFPMediaPlayer::SetAspectRatioMode");
        }
        void balance(float f)
        {
            throw_com_error(p->SetBalance(f),
                "player::balance","IMFPMediaPlayer::SetBalance");
        }
        void bordercolor(COLORREF clval)
        {
            throw_com_error(p->SetBorderColor(clval),
                "player::bordercolor","IMFPMediaPlayer::SetBorderColor");
        }
        void bordercolor(BYTE _r,BYTE _g,BYTE _b)
        {
            throw_com_error(p->SetBorderColor(RGB(_r,_g,_b)),
                "player::bordercolor","IMFPMediaPlayer::SetBorderColor");
        }
        void volume(float f)
        {
            throw_com_error(p->SetVolume(f),"player::volume",
                "IMFPMediaPlayer::SetVolume");
        }
        void mute(bool b=true)//use pause()
        {
            throw_com_error(p->SetMute(b),"player::mute",
                "IMFPMediaPlayer::SetMute");
        }
        void position(LONGLONG lpos)
        {
            PROPVARIANT var;
            PropVariantInit(&var);
            var.vt=VT_I8;
            var.hVal.QuadPart=lpos;
            throw_com_error(p->SetPosition(MFP_POSITIONTYPE_100NS,&var),
                "player::position","IMFPMediaPlayer::SetPosition");
            PropVariantClear(&var);
        }
        void rate(float f)
        {
            throw_com_error(p->SetRate(f),"player::rate",
                "IMFPMediaPlayer::SetRate");
        }
        void videosourcerect(float _left,float _right,float _top,float _bottom)
        {
            MFVideoNormalizedRect rect{_left,_right,_top,_bottom};
            throw_com_error(p->SetVideoSourceRect(&rect),
                "player::videosourcerect","IMFPMediaPlayer::SetRate");
        }
        ~player()
        {
            p->Shutdown();
            p->Release();
            pc->Release();
        }
};
namespace detail
{
    playercallback::playercallback(player* _p):p(_p),m_cRef(1l),b(FALSE){}
    inline void STDMETHODCALLTYPE playercallback::OnMediaPlayerEvent
        (MFP_EVENT_HEADER *pEventHeader)
    {
        if(p->error_handler)
        {
            p->error_handler(com_callback_err_str(pEventHeader->hrEvent,
                "detail::playercallback::OnMediaPlayerEvent"));
        }
        switch(pEventHeader->eEventType)
        {
            case MFP_EVENT_TYPE_MEDIAITEM_CREATED:
            {
                MFP_MEDIAITEM_CREATED_EVENT *pEvent=
                    MFP_GET_MEDIAITEM_CREATED_EVENT(pEventHeader);
                if(p)
                {
                    BOOL bh=FALSE,bi=FALSE;
                    if(p->error_handler)
                    {
                        p->error_handler(
                            "ygp::detail::playercallback::OnMediaPlayerEvent"
                            " (function IMFPMediaItem::HasVideo): "+
                            lasterror<std::string>(pEvent->pMediaItem->HasVideo
                            (&bh,&bi)));
                    }
                    b=bh&&bi;
                    if(p->MEDIAITEM_CREATED_callback)
                        p->MEDIAITEM_CREATED_callback(pEvent);
                        //typically MEDIAITEM_CREATED_callback calls player::setitem method
                }
                break;
            }
            #define YGP_PLAYER_PERFORM_CALLBACK(t)                            \
                case MFP_EVENT_TYPE_##t:                                      \
                {                                                             \
                    if(p->t##_callback)                                       \
                    {                                                         \
                        p->t##_callback(MFP_GET_##t##_EVENT(pEventHeader));   \
                    }                                                         \
                    break;                                                    \
                }
            YGP_PLAYER_PERFORM_CALLBACK(PLAY)
            YGP_PLAYER_PERFORM_CALLBACK(PAUSE)
            YGP_PLAYER_PERFORM_CALLBACK(STOP)
            YGP_PLAYER_PERFORM_CALLBACK(POSITION_SET)
            YGP_PLAYER_PERFORM_CALLBACK(RATE_SET)
            YGP_PLAYER_PERFORM_CALLBACK(MEDIAITEM_SET)//you should call player::play here
            YGP_PLAYER_PERFORM_CALLBACK(FRAME_STEP)
            YGP_PLAYER_PERFORM_CALLBACK(MEDIAITEM_CLEARED)
            YGP_PLAYER_PERFORM_CALLBACK(MF)
            YGP_PLAYER_PERFORM_CALLBACK(ERROR)
            YGP_PLAYER_PERFORM_CALLBACK(PLAYBACK_ENDED)
            YGP_PLAYER_PERFORM_CALLBACK(ACQUIRE_USER_CREDENTIAL)
            #undef YGP_PLAYER_PERFORM_CALLBACK
        }
    }
}
#endif
END_NAMESPACE_YGP
#endif