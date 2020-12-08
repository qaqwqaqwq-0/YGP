#ifndef _YGP_WIN_GUI_DIALOG_HPP_
#define _YGP_WIN_GUI_DIALOG_HPP_
#include"window.hpp"
#include<commctrl.h>
BEGIN_NAMESPACE_YGP
class dialog
{
    HGLOBAL hgbl;
    friend class initializer;
    public:
        static INT_PTR fromres(LPCSTR lpszTemplate,DLGPROC _DlgProc,
            HWND hwndParent=nullptr,HMODULE hModule=currentmodule())
            //Create a dialog from a resource
        {
            return DialogBoxA(hModule,lpszTemplate,hwndParent,
                _DlgProc);
        }
        static INT_PTR fromres(int iResId,DLGPROC _DlgProc,
            HWND hwndParent=nullptr,HMODULE hModule=currentmodule())
            //Create a dialog from a resource
        {
            return DialogBoxA(hModule,MAKEINTRESOURCEA(iResId),hwndParent,
                _DlgProc);
        }
        template<typename _Tp>
        static INT_PTR fromresparam(LPCSTR lpszTemplate,DLGPROC _DlgProc,
            _Tp _param,HWND hwndParent=nullptr,
            HMODULE hModule=currentmodule())
            //Create a dialog from a resource, with parameters specified
        {
            return DialogBoxParamA(hModule,lpszTemplate,hwndParent,
                _DlgProc,(LPARAM)_param);
        }
        template<typename _Tp>
        static INT_PTR fromresparam(int iResId,DLGPROC _DlgProc,
            _Tp _param,HWND hwndParent=nullptr,
            HMODULE hModule=currentmodule())
            //Create a dialog from a resource, with parameters specified
        {
            return DialogBoxParamA(hModule,MAKEINTRESOURCEA(iResId),
                hwndParent,_DlgProc,(LPARAM)_param);
        }
        dialog():hgbl(nullptr){}
        struct initializer
        {
            dialog* dlg;
            LPDLGTEMPLATE lpdt;
            LPARAM _lParam;
            //Using the default constructor is not allowed.
            initializer()=delete;
            //The user must specify the corresponding
            //ygp::dialog object with the constructor below.
            explicit initializer(dialog& _dlg):dlg(&_dlg)
            {
                HGLOBAL hgbl=GlobalAlloc(GMEM_ZEROINIT,sizeof(DLGTEMPLATE));
                if(!hgbl)throw std::runtime_error("ygp::dialog::"
                    "initializer::initializer (function GlobalAlloc): "
                    +lasterror<std::string>());
                lpdt=(LPDLGTEMPLATE)GlobalLock(hgbl);
                if(!lpdt)throw std::runtime_error("ygp::dialog::"
                    "initializer::initializer (function GlobalLock): "
                    +lasterror<std::string>());
                lpdt->style=WS_OVERLAPPEDWINDOW;
                dlg->hgbl=hgbl;
            }
            initializer& style(DWORD dwStyle)
            {
                lpdt->style=dwStyle;
                return *this;
            }
            initializer& exstyle(DWORD dwExStyle)
            {
                lpdt->dwExtendedStyle=dwExStyle;
                return *this;
            }
            initializer& x(short _x)
            {
                lpdt->x=_x;
                return *this;
            }
            initializer& y(short _y)
            {
                lpdt->y=_y;
                return *this;
            }
            initializer& width(short _w)
            {
                lpdt->cx=_w;
                return *this;
            }
            initializer& height(short _h)
            {
                lpdt->cy=_h;
                return *this;
            }
            template<typename _Tp>
            initializer& param(_Tp _val)
            {
                _lParam=(LPARAM)_val;
                return *this;
            }
            INT_PTR operator()(DLGPROC _dp,HWND _hwnd=nullptr)
            //Create the dialog and wait.
            {
                GlobalUnlock(dlg->hgbl);
                INT_PTR res=DialogBoxIndirectParamA(currentmodule(),
                    (LPDLGTEMPLATE)dlg->hgbl,_hwnd,_dp,_lParam);
                lpdt=nullptr;
                return res;
            }
            ~initializer()
            {
                if(lpdt)GlobalUnlock(dlg->hgbl);
            }
        };
        ~dialog()
        {
            if(hgbl)GlobalFree(hgbl);//make sure hgbl!=nullptr
        }
};
//Usage:
//  ygp::dialog dlg;
//  ygp::dialog::initialzer(dlg).x(100).y(100).width(100).height(100)();
//Then the dialog box will appear.
//There is no need to explicitly free any resource.

class taskdlg
    //You must specify #pragma YGP_MANIFEST when before using this class, or 
    //DLL linking problems will occur.
{
    TASKDIALOGCONFIG c;
    int nbutton,nradiobutton;
    BOOL bverificationflagchecked;
    struct _Param
    {
        const taskdlg* dlg;
        LONG_PTR param;
    }m_Param;
    public:
        class taskdlgwindow:public window
        {
            friend class taskdlg;
            taskdlgwindow(HWND _hwnd):window(_hwnd){}
            public:
                void clickbutton(int nid)noexcept
                {
                    sendmsgw(TDM_CLICK_BUTTON,nid);
                }
                void clickradiobutton(int nid)noexcept
                {
                    sendmsgw(TDM_CLICK_RADIO_BUTTON,nid);
                }
                void clickverification(bool bcheck,bool bfocus)noexcept
                {
                    sendmsgw(TDM_CLICK_VERIFICATION,bcheck,bfocus);
                }
                void enablebutton(int nid)noexcept
                {
                    sendmsgw(TDM_ENABLE_BUTTON,nid,TRUE);
                }
                void disablebutton(int nid)noexcept
                {
                    sendmsgw(TDM_ENABLE_BUTTON,nid,FALSE);
                }
                void enableradiobutton(int nid)noexcept
                {
                    sendmsgw(TDM_ENABLE_RADIO_BUTTON,nid,TRUE);
                }
                void disableradiobutton(int nid)noexcept
                {
                    sendmsgw(TDM_ENABLE_RADIO_BUTTON,nid,FALSE);
                }
                void navigatepage(const taskdlg& td)noexcept
                {
                    sendmsgw(TDM_NAVIGATE_PAGE,0,(LPARAM)&td.c);
                }
                void showshield(int nid,bool bshow=true)noexcept
                    //indicates whether a given task dialog button 
                    //or command link should have a User Account 
                    //Control (UAC) shield icon; that is, whether 
                    //the action invoked by the button requires 
                    //elevation
                {
                    sendmsgw(
                        TDM_SET_BUTTON_ELEVATION_REQUIRED_STATE,
                        nid,bshow);
                }
                void updatecontent(LPCWSTR lpszTxt)noexcept
                {
                    sendmsgw(TDM_SET_ELEMENT_TEXT,TDE_CONTENT,
                        (LPARAM)lpszTxt);
                }
                void updateexpandedinfo(LPCWSTR lpszTxt)noexcept
                {
                    sendmsgw(TDM_SET_ELEMENT_TEXT,
                        TDE_EXPANDED_INFORMATION,(LPARAM)lpszTxt);
                }
                void updatefooter(LPCWSTR lpszTxt)noexcept
                {
                    sendmsgw(TDM_SET_ELEMENT_TEXT,
                        TDE_FOOTER,(LPARAM)lpszTxt);
                }
                void updateinstruction(LPCWSTR lpszTxt)noexcept
                {
                    sendmsgw(TDM_SET_ELEMENT_TEXT,
                        TDE_MAIN_INSTRUCTION,(LPARAM)lpszTxt);
                }
                void setmarqueeprogressbarmode(bool bturnon=true)noexcept
                {
                    sendmsgw(TDM_SET_MARQUEE_PROGRESS_BAR,bturnon);
                }
                //How to enable marquee progress bar mode:
                //  (win: an instance of taskdlgwindow)
                //  win.setmarqueeprogressbarmode();
                //  win.configprogressbarmarquee(true);
                void configprogressbarmarquee(bool bdisplay,
                    UINT millisecondsbetweenmarqueeanimationupdates=1u
                    )noexcept
                {
                    sendmsgw(TDM_SET_PROGRESS_BAR_MARQUEE,bdisplay,
                        millisecondsbetweenmarqueeanimationupdates);
                }
                void setprogressbarpos(int newpos)noexcept
                {
                    sendmsgw(TDM_SET_PROGRESS_BAR_POS,newpos);
                }
                std::pair<WORD,WORD> setprogressbarrange(WORD _min,
                    WORD _max)//(0,100) by default
                {
                    auto res=sendmsgw(TDM_SET_PROGRESS_BAR_RANGE,0,
                        MAKELONG(_min,_max));
                    if(!res)
                        throw std::runtime_error("ygp::taskdlg::taskdlgwindow: "
                            "The response to TDM_SET_PROGRESS_BAR_RANGE is 0, "
                            "which means the task dialog failed to set the "
                            "range of the progress bar.");
                    return std::make_pair(LOWORD(res),HIWORD(res));
                }
                void setprogressbarstate(int nState)
                    //PBST_NORMAL, PBST_PAUSED, PBST_ERROR
                    //green        yellow       red
                {
                    sendmsgw(TDM_SET_PROGRESS_BAR_STATE,nState);
                }
                template<typename _Tp>
                void updatemainicon(_Tp hi)//HICON, int or LPCWSTR
                {
                    sendmsgw(TDM_UPDATE_ICON,TDIE_ICON_MAIN,(LPARAM)hi);
                }
                template<typename _Tp>
                void updatefootericon(_Tp hi)//HICON, int or LPCWSTR
                {
                    sendmsgw(TDM_UPDATE_ICON,TDIE_ICON_FOOTER,(LPARAM)hi);
                }
        };
    private:
        static HRESULT CALLBACK callback(HWND hwnd,UINT Message,WPARAM wParam,
            LPARAM lParam,LONG_PTR lpParam)
        {
            _Param* prm=(_Param*)lpParam;
            taskdlgwindow tdw(hwnd);
            switch(Message)
            {
                case TDN_BUTTON_CLICKED:
                {
                    if(prm->dlg->onbuttonclicked)
                        return prm->dlg->onbuttonclicked(tdw,wParam,prm->param)?
                            S_OK:S_FALSE;
                    break;
                }
                case TDN_CREATED:
                {
                    if(prm->dlg->oncreated)
                        prm->dlg->oncreated(tdw,prm->param);
                    break;
                }
                case TDN_DESTROYED:
                {
                    if(prm->dlg->ondestroyed)
                        prm->dlg->ondestroyed(tdw,prm->param);
                    break;
                }
                case TDN_DIALOG_CONSTRUCTED:
                {
                    if(prm->dlg->ondialogconstructed)
                        prm->dlg->ondialogconstructed(tdw,prm->param);
                    break;
                }
                case TDN_EXPANDO_BUTTON_CLICKED:
                {
                    if(prm->dlg->onexpandobuttonclicked)
                        prm->dlg->onexpandobuttonclicked(tdw,(bool)wParam,
                            prm->param);
                    break;
                }
                case TDN_HELP:
                {
                    if(prm->dlg->onhelp)
                        prm->dlg->onhelp(tdw,prm->param);
                    break;
                }
                case TDN_HYPERLINK_CLICKED:
                {
                    if(prm->dlg->onhyperlinkclicked)
                        prm->dlg->onhyperlinkclicked(tdw,(LPCWSTR)lParam,
                            prm->param);
                    break;
                }
                case TDN_NAVIGATED:
                {
                    if(prm->dlg->onnavigated)
                        prm->dlg->onnavigated(tdw,prm->param);
                    break;
                }
                case TDN_RADIO_BUTTON_CLICKED:
                {
                    if(prm->dlg->onradiobuttonclicked)
                        prm->dlg->onradiobuttonclicked(tdw,wParam,prm->param);
                    break;
                }
                case TDN_TIMER:
                {
                    if(prm->dlg->ontimer)
                        return prm->dlg->ontimer(tdw,wParam,prm->param)?
                            S_OK:S_FALSE;
                    break;
                }
                case TDN_VERIFICATION_CLICKED:
                {
                    if(prm->dlg->onverificationchecked)
                        prm->dlg->onverificationchecked(tdw,
                            (bool)wParam,prm->param);
                    break;
                }
            }
            return S_OK;
        }
    public:
        #ifdef _MSC_VER
        #pragma region configurations
        #endif
        std::function<bool(taskdlgwindow&,int/*button id*/,LONG_PTR)> onbuttonclicked;
            //return false to prevent the task dialog from closing
        std::function<void(taskdlgwindow&,LONG_PTR)/*user data*/> oncreated;
        std::function<void(taskdlgwindow&,LONG_PTR)> ondestroyed;
        std::function<void(taskdlgwindow&,LONG_PTR)> ondialogconstructed;
        std::function<void(taskdlgwindow&,bool/*expanded or not*/,LONG_PTR)> 
            onexpandobuttonclicked;
        std::function<void(taskdlgwindow&,LONG_PTR)> onhelp;
        std::function<void(taskdlgwindow&,LPCWSTR/*the URL of the link*/,LONG_PTR)> 
            onhyperlinkclicked;
        std::function<void(taskdlgwindow&,LONG_PTR)> onnavigated;
        std::function<void(taskdlgwindow&,int/*button id*/,LONG_PTR)> 
            onradiobuttonclicked;
        std::function<bool(taskdlgwindow&,DWORD/*milliseconds since the */
            /*dialog was created*/,LONG_PTR)> ontimer;
            //return false to reset the timer
        std::function<void(taskdlgwindow&,bool/*checked or not*/,
            LONG_PTR)> onverificationchecked;
        taskdlg():c{sizeof(c)},nbutton{-1},nradiobutton{-1},
            m_Param{this},bverificationflagchecked{FALSE},
            onhyperlinkclicked{[](taskdlgwindow& win,LPCWSTR url,LONG_PTR)
            {
                ShellExecuteW(win.hwnd,L"open",url,nullptr,nullptr,SW_SHOWNORMAL);
            }}
        {
            c.pfCallback=callback;
            c.lpCallbackData=(LONG_PTR)&m_Param;
        }
        taskdlg& sethwndparent(HWND hParent)noexcept
        {
            c.hwndParent=hParent;
            return *this;
        }
        taskdlg& sethinstance(HINSTANCE hInstance=currentmodule())
            noexcept
        {
            c.hInstance=hInstance;
            return *this;
        }
        taskdlg& enablehyperlinks()noexcept
        {
            c.dwFlags|=TDF_ENABLE_HYPERLINKS;
            return *this;
        }
        taskdlg& allowdialogcancellation()noexcept
            //Indicates that the dialog should be able to be closed using 
            //Alt-F4, Escape, and the title bar's close button even if no 
            //cancel button is specified in either the dwCommonButtons or 
            //pButtons members.
        {
            c.dwFlags|=TDF_ALLOW_DIALOG_CANCELLATION;
            return *this;
        }
        taskdlg& usecommandlinks(bool bNoGlyph=false)noexcept
        {
            c.dwFlags|=bNoGlyph?TDF_USE_COMMAND_LINKS_NO_ICON:
                TDF_USE_COMMAND_LINKS;
            return *this;
        }
        taskdlg& useexpandfootarea()noexcept
            //Indicates that the expanded information 
            //is displayed at the bottom of the dialog's footer area 
            //instead of immediately after the dialog's content.
        {
            c.dwFlags|=TDF_EXPAND_FOOTER_AREA;
            return *this;
        }
        taskdlg& expandedbydefault()noexcept
        {
            c.dwFlags|=TDF_EXPANDED_BY_DEFAULT;
            return *this;
        }
        taskdlg& verificationflagcheckedinitially()noexcept
        {
            c.dwFlags|=TDF_VERIFICATION_FLAG_CHECKED;
            return *this;
        }
        taskdlg& showprogressbar()noexcept
        {
            c.dwFlags|=TDF_SHOW_PROGRESS_BAR;
            return *this;
        }
        taskdlg& showmarqueeprogressbar()noexcept
        {
            c.dwFlags|=TDF_SHOW_MARQUEE_PROGRESS_BAR;
            return *this;
        }
        taskdlg& setcallbacktimer()noexcept
        {
            c.dwFlags|=TDF_CALLBACK_TIMER;
            return *this;
        }
        taskdlg& centeredrelativetoparent()noexcept
        {
            c.dwFlags|=TDF_POSITION_RELATIVE_TO_WINDOW;
            return *this;
        }
        taskdlg& textdisplayedreadrighttoleft()noexcept
        {
            c.dwFlags|=TDF_RTL_LAYOUT;
            return *this;
        }
        taskdlg& nodefaultradiobutton()noexcept
            //no default radio button
        {
            c.dwFlags|=TDF_NO_DEFAULT_RADIO_BUTTON;
            return *this;
        }
        taskdlg& canbeminimized()noexcept
        {
            c.dwFlags|=TDF_CAN_BE_MINIMIZED;
            return *this;
        }
        taskdlg& widthdeterminedbycontentarea()noexcept
        {
            c.dwFlags|=TDF_SIZE_TO_CONTENT;
            return *this;
        }
        enum commonbuttons:DWORD
            //These flags are not the IDs of the buttons (if created).
            //Their IDs are: IDOK, IDYES, IDNO, IDCANCEL, 
            //IDRETRY, IDCLOSE respectively.
        {
            OKButton=TDCBF_OK_BUTTON,
            YesButton=TDCBF_YES_BUTTON,
            NoButton=TDCBF_NO_BUTTON,
            CancelButton=TDCBF_CANCEL_BUTTON,
            RetryButton=TDCBF_RETRY_BUTTON,
            CloseButton=TDCBF_CLOSE_BUTTON
        };
        taskdlg& setcommonbuttons(DWORD dwCommonButtons)noexcept
        {
            c.dwCommonButtons=dwCommonButtons;
            return *this;
        }
        taskdlg& title(PCWSTR title)noexcept
        {
            c.pszWindowTitle=title;
            return *this;
        }
        taskdlg& setmainicon(HICON hi)noexcept
        {
            c.dwFlags|=TDF_USE_HICON_MAIN;
            c.hMainIcon=hi;
            return *this;
        }
        taskdlg& setmainicon(PCWSTR iconres)noexcept
        {
            c.pszMainIcon=iconres;
            return *this;
        }
        taskdlg& setmainicon(int resid)noexcept
        {
            c.pszMainIcon=MAKEINTRESOURCEW(resid);
            return *this;
        }
        taskdlg& instruction(PCWSTR txt)noexcept
        {
            c.pszMainInstruction=txt;
            return *this;
        }
        taskdlg& content(PCWSTR txt)noexcept
        {
            c.pszContent=txt;
            return *this;
        }
        taskdlg& setbuttons(const TASKDIALOG_BUTTON* btns,UINT cnt)
        {
            c.pButtons=btns;
            c.cButtons=cnt;
            return *this;
        }
        taskdlg& setdefaultbuttonid(int nid)
        {
            c.nDefaultButton=nid;
            return *this;
        }
        taskdlg& setradiobuttons(const TASKDIALOG_BUTTON* btns,UINT cnt)
        {
            c.pRadioButtons=btns;
            c.cRadioButtons=cnt;
            return *this;
        }
        taskdlg& setdefaultradiobuttonid(int nid)
        {
            c.nDefaultRadioButton=nid;
            return *this;
        }
        taskdlg& verificationtext(PWSTR txt)
        {
            c.pszVerificationText=txt;
            return *this;
        }
        taskdlg& expandedinfo(PWSTR txt)
        {
            c.pszExpandedInformation=txt;
            return *this;
        }
        taskdlg& expandedcontroltext(PWSTR txt)
        {
            c.pszExpandedControlText=txt;
            return *this;
        }
        taskdlg& collapsedcontroltext(PWSTR txt)
        {
            c.pszCollapsedControlText=txt;
            return *this;
        }
        taskdlg& setfootericon(HICON hi)noexcept
        {
            c.dwFlags|=TDF_USE_HICON_FOOTER;
            c.hFooterIcon=hi;
            return *this;
        }
        taskdlg& setfootericon(PCWSTR iconres)noexcept
        {
            c.pszFooterIcon=iconres;
            return *this;
        }
        taskdlg& setfootericon(int resid)noexcept
        {
            c.pszFooterIcon=MAKEINTRESOURCEW(resid);
            return *this;
        }
        taskdlg& setfooter(PCWSTR txt)noexcept
        {
            c.pszFooter=txt;
            return *this;
        }
        template<typename _Tp>
        taskdlg& setcallbackdata(_Tp _data)noexcept
        {
            m_Param.param=(LONG_PTR)_data;
            return *this;
        }
        taskdlg& width(int nWidth)noexcept
        {
            c.cxWidth=nWidth;
            return *this;
        }
        #ifdef _MSC_VER
        #pragma endregion
        #endif
        taskdlg& operator()()//show the dialog
        {
            HRESULT hr=TaskDialogIndirect(&c,&nbutton,
                &nradiobutton,&bverificationflagchecked);
            if(FAILED(hr))
                throw std::runtime_error("ygp::taskdlg::operator() "
                    " (function TaskDialogIndirect): "
                    +lasterror<std::string>());
            return *this;
        }
        int pressedbuttonid()const noexcept
            //get the clicked button ID
            //(Call this method after the dialog is shown)
        {
            return nbutton;
        }
        int selectedradiobuttonid()const noexcept
            //get the selected radio button ID
            //(Call this method after the dialog is shown)
        {
            return nradiobutton;
        }
        bool verificationflagchecked()const noexcept
            //(Call this method after the dialog is shown)
        {
            return (bool)bverificationflagchecked;
        }
};
END_NAMESPACE_YGP
#endif