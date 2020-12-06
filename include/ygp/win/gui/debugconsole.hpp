//Attention: Call InitCommonControls function before creating an
//           instance of debugconsole class.
#ifndef _YGP_WIN_GUI_DEBUGCONSOLE_HPP_
#define _YGP_WIN_GUI_DEBUGCONSOLE_HPP_
#include"wndclass.hpp"
BEGIN_NAMESPACE_YGP
class debugconsole
{
    static constexpr UINT YGP_WM_ADDTEXT=WM_USER+1;
    HWND hparent;
    const char* title;
    static YGP_WNDPROC noexcept
    {
        static HWND hedit;
        YGP_BEGIN_MSG_MAP
        case WM_CREATE:
        {
            RECT rect;
            GetClientRect(hwnd,&rect);
            LoadLibraryA("msftedit.dll");
            hedit=CreateWindowExA(0,"RICHEDIT50W",nullptr,WS_CHILD|
                WS_VISIBLE|WS_BORDER|WS_HSCROLL|WS_VSCROLL|ES_MULTILINE|
                ES_WANTRETURN|ES_AUTOHSCROLL|ES_AUTOVSCROLL,0,0,
                rect.right-rect.left,rect.bottom-rect.top,
                hwnd,(HMENU)1,currentmodule(),nullptr);
            break;
        }
        case WM_SIZE:
        {
            RECT rect;
            GetClientRect(hwnd,&rect);
            MoveWindow(hedit,0,0,
                rect.right-rect.left,rect.bottom-rect.top,TRUE);
            break;
        }
        case YGP_WM_ADDTEXT:
        {
            DWORD dwst,dwen;
            SendMessageA(hedit,EM_GETSEL,(WPARAM)&dwst,(LPARAM)&dwen);
            SendMessageA(hedit,EM_SETSEL,(WPARAM)-1,(LPARAM)-1);
            SendMessageA(hedit,EM_REPLACESEL,TRUE,wParam);
            SendMessageA(hedit,EM_SETSEL,dwst,dwen);
            SendMessageA(hedit,EM_SCROLL,SB_PAGEDOWN,0);
            break;
        }
        YGP_END_MSG_MAP
        return 0;
    }
    void initialize()noexcept
    {
        try
        {
            registerclass("YGPDebugConsole",WndProc);
        }
        catch(...){}
        hwnd=CreateWindowExA(WS_EX_CLIENTEDGE,"YGPDebugConsole",
            title,WS_OVERLAPPEDWINDOW|WS_VISIBLE|(hparent?WS_CHILD:0),
            CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
            CW_USEDEFAULT,hparent,nullptr,currentmodule(),nullptr);
    }
    template<typename _Tp>
    void m(std::stringstream& _ss,_Tp&& _tp)
    {
        _ss<<_tp;
    }
    template<typename _Tp,typename ..._Args>
    void m(std::stringstream& _ss,_Tp&& _tp,_Args&& ..._args)
    {
        _ss<<_tp;
        m(_ss,_args...);
    }
    public:
        HWND hwnd;
        debugconsole(HWND _hwnd,const char* _title=
            "YGP Debug Console"):
        hparent(_hwnd),title(_title)
        {
            initialize();
        }
        debugconsole(const char* _title="YGP Debug Console"):
        hparent(nullptr),title(_title)
        {
            initialize();
        }
        debugconsole& add(const char* _txt)noexcept
        {
            SendMessageA(hwnd,YGP_WM_ADDTEXT,(WPARAM)_txt,0);
            return *this;
        }
        debugconsole& add(std::string& _txt)noexcept
        {
            SendMessageA(hwnd,YGP_WM_ADDTEXT,(WPARAM)_txt.c_str(),0);
            return *this;
        }
        debugconsole& add(std::stringstream& _txt)noexcept
        {
            SendMessageA(hwnd,YGP_WM_ADDTEXT,(WPARAM)_txt.str().c_str(),0);
            return *this;
        }
        template<typename ..._Args>
        debugconsole& operator()(_Args&& ..._args)
        {
            std::stringstream _ss;
            m(_ss,_args...);
            add(_ss);
            return *this;
        }
        template<typename ..._Args>
        debugconsole& line(_Args&& ..._args)
        {
            std::stringstream _ss;
            m(_ss,_args...,'\n');
            add(_ss);
            return *this;
        }
        debugconsole& pos(int _left,int _top)noexcept
        {
            RECT rect;
            GetWindowRect(hwnd,&rect);
            MoveWindow(hwnd,_left,_top,rect.right-rect.left,
                rect.bottom-rect.top,TRUE);
            return *this;
        }
        debugconsole& size(int _width,int _height)noexcept
        {
            RECT rect;
            GetWindowRect(hwnd,&rect);
            MoveWindow(hwnd,rect.left,rect.top,_width,
                _height,TRUE);
            return *this;
        }
        debugconsole& icon(HICON _hicon)noexcept
        {
            SendMessageA(hwnd,WM_SETICON,ICON_SMALL,(LPARAM)_hicon);
            SendMessageA(hwnd,WM_SETICON,ICON_BIG,(LPARAM)_hicon);
            return *this;
        }
};
END_NAMESPACE_YGP
#endif