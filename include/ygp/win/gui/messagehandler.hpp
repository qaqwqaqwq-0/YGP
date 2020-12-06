#ifndef _YGP_MESSAGEHANDLER_HPP_
#define _YGP_MESSAGEHANDLER_HPP_
#include"../winerr.hpp"
#include<functional>
BEGIN_NAMESPACE_YGP
class messagehandler
{
    struct handler
    {
        struct func
        {
            enum functype:BYTE
            {
                null_t,
                v_t,
                vrf_t,
                lrf_t,
                hvrf_t,
                hlrf_t
            };
            functype tp;//functor type
            typedef std::function<void()> vtp;
            typedef std::function<void(WPARAM,LPARAM)> vrftp;
            typedef std::function<LRESULT(WPARAM,LPARAM)> lrftp;
            typedef std::function<void(HWND,WPARAM,LPARAM)> hvrftp;
            typedef std::function<LRESULT(HWND,WPARAM,LPARAM)> hlrftp;
            union
            {
                vtp* v;
                vrftp* vrf;
                lrftp* lrf;
                hvrftp* hvrf;
                hlrftp* hlrf;
            };
            template<typename _Tp>
            struct identity//helper structure
            {
                typedef _Tp type;
            };
            func():vrf(nullptr),tp(null_t){}
            func(const typename identity<vtp>::type& _v):
                tp(v_t),v(new vtp(_v)){}
            func(const typename identity<vrftp>::type& _vrf):
                tp(vrf_t),vrf(new vrftp(_vrf)){}
            func(const typename identity<lrftp>::type& _lrf):
                tp(lrf_t),lrf(new lrftp(_lrf)){}
            func(const typename identity<hvrftp>::type& _hvrf):
                tp(hvrf_t),hvrf(new hvrftp(_hvrf)){}
            func(const typename identity<hlrftp>::type& _hlrf):
                tp(hlrf_t),hlrf(new hlrftp(_hlrf)){}
            func(const func& _other)//copy constructor
            {
                tp=_other.tp;
                switch(tp)
                {
                    case v_t:
                        v=new vtp(*_other.v);
                        break;
                    case vrf_t:
                        vrf=new vrftp(*_other.vrf);
                        break;
                    case lrf_t:
                        lrf=new lrftp(*_other.lrf);
                        break;
                    case hvrf_t:
                        hvrf=new hvrftp(*_other.hvrf);
                        break;
                    case hlrf_t:
                        hlrf=new hlrftp(*_other.hlrf);
                        break;
                    case null_t:
                        vrf=nullptr;
                        break;
                }
            }
            func& clear()
            {
                switch(tp)
                {
                    case null_t:break;
                    case v_t:delete v;break;
                    case vrf_t:delete vrf;break;
                    case lrf_t:delete lrf;break;
                    case hvrf_t:delete hvrf;break;
                    case hlrf_t:delete hlrf;break;
                }
                tp=null_t;
                return *this;
            }
            func& reset(const vtp& _v)
            {
                clear();
                tp=v_t;
                v=new vtp(_v);
                return *this;
            }
            func& reset(const vrftp& _vrf)
            {
                clear();
                tp=vrf_t;
                vrf=new vrftp(_vrf);
                return *this;
            }
            func& reset(const lrftp& _lrf)
            {
                clear();
                tp=lrf_t;
                lrf=new lrftp(_lrf);
                return *this;
            }
            func& reset(const hvrftp& _hvrf)
            {
                clear();
                tp=hvrf_t;
                hvrf=new hvrftp(_hvrf);
                return *this;
            }
            func& reset(const hlrftp& _hlrf)
            {
                clear();
                tp=hlrf_t;
                hlrf=new hlrftp(_hlrf);
                return *this;
            }
            func& reset()
            {
                clear();
                return *this;
            }
            template<typename _Tp>
            func& operator=(const _Tp& _tp)
            {
                return reset(_tp);
            }
            ~func(){clear();}
        };
        std::vector<func> vec;
    };
    public:
        std::function<void(HWND,WPARAM,LPARAM)> builtin;
        handler userdef;
        LRESULT operator()(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam)
        {
            if(builtin)builtin(hwnd,wParam,lParam);
            if(userdef.vec.empty())
                if(!builtin)return DefWindowProcA(hwnd,Msg,wParam,lParam);
            else for(auto& i:userdef.vec)
            {
                
                switch(i.tp)
                {
                    case handler::func::v_t:
                        i.v->operator()();
                        break;
                    case handler::func::vrf_t:
                        i.vrf->operator()(wParam,lParam);
                        break;
                    case handler::func::lrf_t:
                        return i.lrf->operator()(wParam,lParam);
                    case handler::func::hvrf_t:
                        i.hvrf->operator()(hwnd,wParam,lParam);
                        break;
                    case handler::func::hlrf_t:
                        return i.hlrf->operator()(hwnd,wParam,lParam);
                }
            }
            return 0;
        }
        template<typename _Fn>
        messagehandler& add(_Fn&& _fn)
        {
            handler::func fc(_fn);
            userdef.vec.push_back(fc);
            return *this;
        }
};
namespace detail
{
    struct WM_COMMAND_handler
    {
        struct func
        {
            enum functype
            {
                null_t,
                vr_t,
                wvr_t
            }tp;
            typedef std::function<void()> vr;
            typedef std::function<void(WORD)> wvr;
            union
            {
                vr* pvr;
                wvr* pwvr;
            };
            func():tp(null_t),pvr(nullptr){}
            func(const vr& refvr):tp(vr_t),pvr(new vr(refvr)){}
            func(const wvr& refwvr):tp(wvr_t),pwvr(new wvr(refwvr)){}
            func& operator=(const func& _other)
            {
                tp=_other.tp;
                switch(tp)
                {
                    case vr_t:pvr=new vr(*_other.pvr);
                    case wvr_t:pwvr=new wvr(*_other.pwvr);
                }
                return *this;
            }
            void clear()
            {
                switch(tp)
                {
                    case null_t:break;
                    case vr_t:delete pvr;break;
                    case wvr_t:delete pwvr;break;
                }
                tp=null_t;
            }
            ~func(){clear();}
        };
        void invoke(WORD _id,WORD wParam=0)
        {
            std::unordered_map<WORD,func>::iterator it=map.find(_id);
            if(it!=map.end())
            {
                switch(it->second.tp)
                {
                    case func::null_t:break;
                    case func::vr_t:it->second.pvr->operator()();
                        break;
                    case func::wvr_t:it->second.pwvr->
                        operator()(wParam);break;
                }
            }
        }
        std::unordered_map<WORD,func> map;
    };
}
END_NAMESPACE_YGP
#endif