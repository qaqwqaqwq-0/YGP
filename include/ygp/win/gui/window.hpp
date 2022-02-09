#ifndef _YGP_WIN_GUI_WINDOW_HPP_
#define _YGP_WIN_GUI_WINDOW_HPP_
#include"rect.hpp"
#include"font.hpp"
#include<memory>
BEGIN_NAMESPACE_YGP
class window
{
    public:
        HWND hwnd;
        window& create(LPCSTR lpClassName,LPCSTR lpWindowName,
            DWORD dwStyle=WS_VISIBLE|WS_OVERLAPPEDWINDOW,
            DWORD dwExStyle=0,
            int X=CW_USEDEFAULT,int Y=CW_USEDEFAULT,
            int nWidth=CW_USEDEFAULT,int nHeight=CW_USEDEFAULT)
        {
            hwnd=CreateWindowExA(dwExStyle,lpClassName,
                lpWindowName,dwStyle,X,Y,nWidth,nHeight,nullptr,
                nullptr,currentmodule(),nullptr);
            if(!hwnd)
                throw std::runtime_error("ygp::window::create"
                    " (function CreateWindowExA): "+
                    lasterror<std::string>());
            return *this;
        }
        window& create(HWND hwndParent,
            LPCSTR lpClassName,LPCSTR lpWindowName,int nID,
            DWORD dwStyle=WS_CHILD|WS_VISIBLE,DWORD dwExStyle=0,
            int X=CW_USEDEFAULT,int Y=CW_USEDEFAULT,
            int nWidth=CW_USEDEFAULT,int nHeight=CW_USEDEFAULT)
        {
            hwnd=CreateWindowExA(dwExStyle,lpClassName,
                lpWindowName,dwStyle,X,Y,nWidth,nHeight,
                hwndParent,(HMENU)nID,currentmodule(),
                nullptr);
            if(!hwnd)
                throw std::runtime_error("ygp::window::create"
                    " (function CreateWindowExA): "+
                    lasterror<std::string>());
            return *this;
        }
        window& create(LPCWSTR lpClassName,LPCWSTR lpWindowName,
            DWORD dwStyle=WS_VISIBLE,DWORD dwExStyle=0,
            int X=CW_USEDEFAULT,int Y=CW_USEDEFAULT,
            int nWidth=CW_USEDEFAULT,int nHeight=CW_USEDEFAULT)
        {
            hwnd=CreateWindowExW(dwExStyle,lpClassName,
                lpWindowName,dwStyle,X,Y,nWidth,nHeight,nullptr,
                nullptr,currentmodule(),nullptr);
            if(!hwnd)
                throw std::runtime_error("ygp::window::create"
                    " (function CreateWindowExA): "+
                    lasterror<std::string>());
            return *this;
        }
        window& create(HWND hwndParent,
            LPCWSTR lpClassName,LPCWSTR lpWindowName,int nID,
            DWORD dwStyle=WS_CHILD|WS_VISIBLE,DWORD dwExStyle=0,
            int X=CW_USEDEFAULT,int Y=CW_USEDEFAULT,
            int nWidth=CW_USEDEFAULT,int nHeight=CW_USEDEFAULT)
        {
            hwnd=CreateWindowExW(dwExStyle,lpClassName,
                lpWindowName,dwStyle,X,Y,nWidth,nHeight,
                hwndParent,(HMENU)nID,currentmodule(),
                nullptr);
            if(!hwnd)
                throw std::runtime_error("ygp::window::create"
                    " (function CreateWindowExA): "+
                    lasterror<std::string>());
            return *this;
        }
        operator HWND()const noexcept
        {
            return hwnd;
        }
        window(LPCSTR lpClassName,LPCSTR lpWindowName,
            DWORD dwStyle=WS_VISIBLE|WS_OVERLAPPEDWINDOW,DWORD dwexStyle=0,
            int X=CW_USEDEFAULT,int Y=CW_USEDEFAULT,
            int nWidth=CW_USEDEFAULT,int nHeight=CW_USEDEFAULT)
        {
            create(lpClassName,lpWindowName,dwStyle,
                dwexStyle,X,Y,nWidth,nHeight);
        }
        window(HWND hwndParent,
            LPCSTR lpClassName,LPCSTR lpWindowName,int nID,
            DWORD dwStyle=WS_CHILD|WS_VISIBLE,DWORD dwexStyle=0,
            int X=CW_USEDEFAULT,int Y=CW_USEDEFAULT,
            int nWidth=CW_USEDEFAULT,int nHeight=CW_USEDEFAULT)
        {
            create(hwndParent,lpClassName,lpWindowName,
                nID,dwStyle,dwexStyle,X,Y,nWidth,nHeight);
        }
        window(LPCWSTR lpClassName,LPCWSTR lpWindowName,
            DWORD dwStyle=WS_VISIBLE|WS_OVERLAPPEDWINDOW,DWORD dwexStyle=0,
            int X=CW_USEDEFAULT,int Y=CW_USEDEFAULT,
            int nWidth=CW_USEDEFAULT,int nHeight=CW_USEDEFAULT)
        {
            create(lpClassName,lpWindowName,dwStyle,
                dwexStyle,X,Y,nWidth,nHeight);
        }
        window(HWND hwndParent,
            LPCWSTR lpClassName,LPCWSTR lpWindowName,int nID,
            DWORD dwStyle=WS_CHILD|WS_VISIBLE,DWORD dwexStyle=0,
            int X=CW_USEDEFAULT,int Y=CW_USEDEFAULT,
            int nWidth=CW_USEDEFAULT,int nHeight=CW_USEDEFAULT)
        {
            create(hwndParent,lpClassName,lpWindowName,
                nID,dwStyle,dwexStyle,X,Y,nWidth,nHeight);
        }
        window(HWND hDlg,int nID)
        {
            hwnd=GetDlgItem(hDlg,nID);
            if(!hwnd)
                throw std::runtime_error("ygp::window::window "
                    "(function GetDlgItem): "+lasterror<std::string>());
        }
        window(HDC hDC)//window from DC
        {
            hwnd=WindowFromDC(hDC);
            if(!hwnd)
                throw std::runtime_error("ygp::window::window "
                    "(function WindowFromDC): "+lasterror<std::string>());
        }
        window(POINT pt)//window from point
        {
            hwnd=WindowFromPoint(pt);
            if(!hwnd)
                throw std::runtime_error("ygp::window::window "
                    "(function WindowFromPoint): "+
                    lasterror<std::string>());
        }
        window(HWND hParent,POINT pt)//child window from point
        {
            hwnd=ChildWindowFromPoint(hParent,pt);
            if(!hwnd)
                throw std::runtime_error("ygp::window::window "
                    "(function ChildWindowFromPoint): "+
                    lasterror<std::string>());
        }
        window(HWND hParent,POINT pt,UINT flags)//child window from point, with flags
        {
            hwnd=ChildWindowFromPointEx(hParent,pt,flags);
            if(!hwnd)
                throw std::runtime_error("ygp::window::window "
                    "(function ChildWindowFromPointEx): "+
                    lasterror<std::string>());
        }
        static window fromphysicalpoint(POINT PhysicalPoint)
        {
            HWND _hwnd=WindowFromPhysicalPoint(PhysicalPoint);
            if(!_hwnd)
                throw std::runtime_error("ygp::window::fromphysicalpoint "
                    "(function WindowFromPhysicalPoint): "+
                    lasterror<std::string>());
            return _hwnd;
        }
        window(HWND _hwnd)
        {
            if(!(hwnd=_hwnd))
                throw std::runtime_error("ygp::window::window: "
                    "The constructor received an empty window handle. "
                    "Reason: "+lasterror<std::string>());
        }
        window():hwnd(nullptr){}
        window& operator=(HWND _hwnd)
        {
            if(!(hwnd=_hwnd))
                throw std::runtime_error("ygp::window::operator=: "
                    "Received an empty window handle. "
                    "Reason: "+lasterror<std::string>());
            return *this;
        }
        window firstchild()
        {
            HWND hwndRet;
            if(!(hwndRet=GetTopWindow(hwnd)))
                throw std::runtime_error("ygp::window::firstchild "
                    "(function GetTopWindow): "
                    +lasterror<std::string>());
            return hwndRet;
        }
        window firstsibling()
        {
            HWND hwndRet;
            if(!(hwndRet=GetWindow(hwnd,GW_HWNDFIRST)))
                throw std::runtime_error("ygp::window::firstsibling "
                    "(function GetWindow): "
                    +lasterror<std::string>());
            return hwndRet;
        }
        window lastsibling()
        {
            HWND hwndRet;
            if(!(hwndRet=GetWindow(hwnd,GW_HWNDLAST)))
                throw std::runtime_error("ygp::window::lastsibling "
                    "(function GetWindow): "
                    +lasterror<std::string>());
            return hwndRet;
        }
        window prevsibling()
        {
            HWND hwndRet;
            if(!(hwndRet=GetWindow(hwnd,GW_HWNDPREV)))
                throw std::runtime_error("ygp::window::prevsibling "
                    "(function GetWindow): "
                    +lasterror<std::string>());
            return hwndRet;
        }
        window nextsibling()
        {
            HWND hwndRet;
            if(!(hwndRet=GetWindow(hwnd,GW_HWNDPREV)))
                throw std::runtime_error("ygp::window::nextsibling "
                    "(function GetWindow): "
                    +lasterror<std::string>());
            return hwndRet;
        }
        window next()
        {
            return nextsibling();
        }
        rect clientrect()
        {
            rect ret;
            if(!GetClientRect(hwnd,&ret))
                throw std::runtime_error("ygp::window::clientrect "
                    "(function GetClientRect): "+lasterror<std::string>());
            return ret;
        }
        rect windowrect()
        {
            rect ret;
            if(!GetWindowRect(hwnd,&ret))
                throw std::runtime_error("ygp::window::windowrect "
                    "(function GetWindowRect): "+lasterror<std::string>());
            return ret;
        }
        window ancestor(UINT uFlags=GA_ROOTOWNER)noexcept
        {
            return GetAncestor(hwnd,uFlags);
        }
        window parent()noexcept
        {
            return GetParent(hwnd);
        }
        window& text(std::shared_ptr<char>& _str)//retrive text
            //This function cannot retrieve the text of an edit control 
            //in another application.
        {
            int nlen=GetWindowTextLengthA(hwnd);
            if(!nlen)throw std::runtime_error("ygp::window::text (function "
                "GetWindowTextLengthA): "+lasterror<std::string>());
            char* p=new char[nlen+1];
            if(!GetWindowTextA(hwnd,p,nlen+1))
                throw std::runtime_error("ygp::window::text (function GetWindowTextA): "
                    +lasterror<std::string>());
            _str.reset(p);
            return *this;
        }
        window& text(std::shared_ptr<wchar_t>& _str)//retrieve text
        {
            int nlen=GetWindowTextLengthW(hwnd);
            if(!nlen)throw std::runtime_error("ygp::window::text (function "
                "GetWindowTextLengthW): "+lasterror<std::string>());
            wchar_t* p=new wchar_t[nlen+1];
            if(!GetWindowTextW(hwnd,p,nlen+1))
                throw std::runtime_error("ygp::window::text (function GetWindowTextW): "
                    +lasterror<std::string>());
            _str.reset(p);
            return *this;
        }
        window& text(std::unique_ptr<char>& _str)//retrive text
        {
            int nlen=GetWindowTextLengthA(hwnd);
            if(!nlen)throw std::runtime_error("ygp::window::text (function "
                "GetWindowTextLengthA): "+lasterror<std::string>());
            char* p=new char[nlen+1];
            if(!GetWindowTextA(hwnd,p,nlen+1))
                throw std::runtime_error("ygp::window::text (function GetWindowTextA): "
                    +lasterror<std::string>());
            _str.reset(p);
            return *this;
        }
        window& text(std::unique_ptr<wchar_t>& _str)//retrieve text
        {
            int nlen=GetWindowTextLengthW(hwnd);
            if(!nlen)throw std::runtime_error("ygp::window::text (function "
                "GetWindowTextLengthW): "+lasterror<std::string>());
            wchar_t* p=new wchar_t[nlen+1];
            if(!GetWindowTextW(hwnd,p,nlen+1))
                throw std::runtime_error("ygp::window::text (function GetWindowTextW): "
                    +lasterror<std::string>());
            _str.reset(p);
            return *this;
        }
        std::string gettexta()
        {
            int nlen=GetWindowTextLengthA(hwnd);
            if(!nlen)throw std::runtime_error("ygp::window::gettexta (function "
                "GetWindowTextLengthA): "+lasterror<std::string>());
            char* p=new char[nlen+1];
            if(!GetWindowTextA(hwnd,p,nlen+1))
                throw std::runtime_error("ygp::window::gettexta (function GetWindowTextA): "
                    +lasterror<std::string>());
            std::string _str(p);
            delete []p;
            return _str;
        }
        std::wstring gettextw()
        {
            int nlen=GetWindowTextLengthW(hwnd);
            if(!nlen)throw std::runtime_error("ygp::window::gettextw (function "
                "GetWindowTextLengthW): "+lasterror<std::string>());
            wchar_t* p=new wchar_t[nlen+1];
            if(!GetWindowTextW(hwnd,p,nlen+1))
                throw std::runtime_error("ygp::window::gettextw (function GetWindowTextW): "
                    +lasterror<std::string>());
            std::wstring _str(p);
            delete []p;
            return _str;
        }
        window& text(const char* _txt)//set text
        {
            if(!SetWindowTextA(hwnd,_txt))
                throw std::runtime_error("ygp::window::text (function SetWindowTextA): "
                    +lasterror<std::string>());
            return *this;
        }
        window& text(const wchar_t* _txt)//set text
        {
            if(!SetWindowTextW(hwnd,_txt))
                throw std::runtime_error("ygp::window::text (function SetWindowTextW): "
                    +lasterror<std::string>());
            return *this;
        }
        window& text(const std::string& _txt)//set text
        {
            if(!SetWindowTextA(hwnd,_txt.c_str()))
                throw std::runtime_error("ygp::window::text (function SetWindowTextA): "
                    +lasterror<std::string>());
            return *this;
        }
        window& text(const std::wstring& _txt)//set text
        {
            if(!SetWindowTextW(hwnd,_txt.c_str()))
                throw std::runtime_error("ygp::window::text (function SetWindowTextW): "
                    +lasterror<std::string>());
            return *this;
        }
        bool iswindow()noexcept
        {
            return (bool)IsWindow(hwnd);
        }
        bool isvisible()noexcept
        {
            return (bool)IsWindowVisible(hwnd);
        }
        bool ischild(HWND hChild)noexcept
        {
            return (bool)IsChild(hwnd,hChild);
        }
        window& bringtotop()
        {
            if(!BringWindowToTop(hwnd))
                throw std::runtime_error("ygp::window::bringtotop (function "
                    "BringWindowToTop): "+lasterror<std::string>());
            return *this;
        }
        window& move(int X,int Y,int nW,int nH,bool bRepaint=true)
        {
            if(!MoveWindow(hwnd,X,Y,nW,nH,(BOOL)bRepaint))
                throw std::runtime_error("ygp::window::move (function MoveWindow): "
                    +lasterror<std::string>());
            return *this;
        }
        window& showwindow(int nCmdShow=SW_SHOW)
        {
            ShowWindow(hwnd,nCmdShow);
            DWORD dwLastError=GetLastError();
            if(dwLastError)
                throw std::runtime_error("ygp::window::showwindow"
                    " (function ShowWindow): "+lasterror<std::string>(dwLastError));
            return *this;
        }
        window& minimize()
        {
            return showwindow(SW_MINIMIZE);
        }
        window& maximize()
        {
            return showwindow(SW_MAXIMIZE);
        }
        window& hide()
        {
            return showwindow(SW_HIDE);
        }
        LRESULT sendmsga(UINT Msg,WPARAM wParam=0,LPARAM lParam=0)noexcept
        {
            return SendMessageA(hwnd,Msg,wParam,lParam);
        }
        LRESULT sendmsgw(UINT Msg,WPARAM wParam=0,LPARAM lParam=0)noexcept
        {
            return SendMessageW(hwnd,Msg,wParam,lParam);
        }
        window& postmsga(UINT Msg,WPARAM wParam=0,LPARAM lParam=0)
        {
            if(!PostMessageA(hwnd,Msg,wParam,lParam))
                throw std::runtime_error("ygp::window::postmsga "
                    "(function PostMessageA): "+lasterror<std::string>());
        }
        window& postmsgw(UINT Msg,WPARAM wParam=0,LPARAM lParam=0)
        {
            if(!PostMessageW(hwnd,Msg,wParam,lParam))
                throw std::runtime_error("ygp::window::postmsgw "
                    "(function PostMessageW): "+lasterror<std::string>());
        }
        template<typename _Tp>
        LONG_PTR setlonga(int nIndex,_Tp lptr)noexcept
        {
            return SetWindowLongPtrA(hwnd,nIndex,(LONG_PTR)lptr);
        }
        template<typename _Tp>
        LONG_PTR setlongw(int nIndex,_Tp lptr)noexcept
        {
            return SetWindowLongPtrW(hwnd,nIndex,(LONG_PTR)lptr);
        }
        LONG_PTR getlonga(int nIndex)noexcept
        {
            return GetWindowLongPtrA(hwnd,nIndex);
        }
        LONG_PTR getlongw(int nIndex)noexcept
        {
            return GetWindowLongPtrW(hwnd,nIndex);
        }
        template<typename _Tp>
        window& getlonga(int nIndex,_Tp& _val)noexcept
        {
            _val=(_Tp)GetWindowLongPtrA(hwnd,nIndex);
            return *this;
        }
        template<typename _Tp>
        window& getlongw(int nIndex,_Tp& _val)noexcept
        {
            _val=(_Tp)GetWindowLongPtrW(hwnd,nIndex);
            return *this;
        }
        LONG_PTR id()noexcept
        {
            return getlonga(GWLP_ID);
        }
        LONG_PTR id(LONG_PTR _newid)noexcept
        {
            return setlonga(GWLP_ID,_newid);
        }
        HDC beginpaint(PAINTSTRUCT& _ps)noexcept
        {
            return BeginPaint(hwnd,&_ps);
        }
        window& endpaint(const PAINTSTRUCT& _ps)noexcept
        {
            EndPaint(hwnd,&_ps);
            return *this;
        }
        bool invalidaterect(const rect& _rect,bool fErase=false)noexcept
        {
            return (bool)InvalidateRect(hwnd,&_rect,(BOOL)fErase);
        }
        bool invalidaterect(bool fErase=false)noexcept
        {
            return (bool)InvalidateRect(hwnd,nullptr,(BOOL)fErase);
        }
        bool update()noexcept
        {
            return (bool)UpdateWindow(hwnd);
        }
        HDC getdc()
        {
            HDC ret=GetDC(hwnd);
            if(!ret)throw std::runtime_error("ygp::window::getdc "
                "(function GetDC): "+lasterror<std::string>());
            return ret;
        }
        HDC getwindowdc()
        {
            HDC ret=GetWindowDC(hwnd);
            if(!ret)throw std::runtime_error("ygp::window::getwindowdc "
                "(function GetWindowDC): "+lasterror<std::string>());
            return ret;
        }
        bool releasedc(HDC _hDC)noexcept
        {
            return (bool)ReleaseDC(hwnd,_hDC);
        }
        bool enable()noexcept
        {
            return (bool)EnableWindow(hwnd,TRUE);
        }
        bool disable()noexcept
        {
            return (bool)EnableWindow(hwnd,FALSE);
        }
        window focus()noexcept
        {
            return SetFocus(hwnd);
        }
        static window getfocus()noexcept
        {
            return GetFocus();
        }
        void destroy()
        {
            if(!DestroyWindow(hwnd))
                throw std::runtime_error("ygp::window::destroy"
                    " (function DestroyWindow): "+lasterror<std::string>());
        }
        window& setfont(font& _ft)
        {
            if(!_ft.h)_ft.create();
            sendmsga(WM_SETFONT,(WPARAM)_ft.h);
            return *this;
        }
        window& setfont(HFONT _ft)
        {
            sendmsga(WM_SETFONT,(WPARAM)_ft);
            return *this;
        }
        window& setmenu(HMENU hm)
        {
            if(!SetMenu(hwnd,hm))
                throw std::runtime_error("ygp::window::setmenu "
                    "(function SetMenu): "+lasterror<std::string>());
            return *this;
        }
        HMENU getmenu()noexcept
        {
            return GetMenu(hwnd);
        }
        window& drawmenubar()
        {
            if(!DrawMenuBar(hwnd))
                throw std::runtime_error("ygp::window::drawmenubar "
                    "(function DrawMenuBar): "+lasterror<std::string>());
            return *this;
        }
        window& redrawnow()
        {
            if(!RedrawWindow(hwnd,nullptr,nullptr,RDW_INVALIDATE|RDW_UPDATENOW))
                throw std::runtime_error("ygp::window::redrawnow "
                    "(function RedrawWindow): "+lasterror<std::string>());
            return *this;
        }
        template<typename _Tp>
        window& enddlg(_Tp _res)
        {
            if(!EndDialog(hwnd,(INT_PTR)_res))
                throw std::runtime_error("ygp::window::enddlg "
                    "(function EndDialog): "+lasterror<std::string>());
            return *this;
        }
        LONG_PTR userdata()noexcept
        {
            return getlonga(GWLP_USERDATA);
        }
        template<typename _Tp>
        window& getuserdata(_Tp& _val)
        {
            _val=(_Tp)userdata();
            return *this;
        }
        template<typename _Tp>
        window& userdata(_Tp _val)
        {
            setlonga(GWLP_USERDATA,_val);
            return *this;
        }
        window getdlgitem(int nID)
        {
            return window(hwnd,nID);
        }
        struct placement:WINDOWPLACEMENT
        {
            placement():WINDOWPLACEMENT{sizeof(WINDOWPLACEMENT)}{}
        };
        placement getplacement()
        {
            placement ret;
            if(!GetWindowPlacement(hwnd,&ret))
                throw std::runtime_error("ygp::window::getplacement "
                    "(function GetWindowPlacement): "+lasterror<std::string>());
            return ret;
        }
        window& setplacement(const placement& plcm)
        {
            if(!SetWindowPlacement(hwnd,&plcm))
                throw std::runtime_error("ygp::window::setplacement "
                    "(function SetWindowPlacement): "+lasterror<std::string>());
            return *this;
        }
        UINT isdlgbuttonchecked(int nID)
        {
            return IsDlgButtonChecked(hwnd,nID);
        }
        window& appendtext(const char* _txt)
        {
            DWORD dwStartPos, dwEndPos;
            sendmsga(EM_GETSEL, (WPARAM)&dwStartPos, (WPARAM)&dwEndPos);
            int txtlen = GetWindowTextLengthA(hwnd);
            sendmsga(EM_SETSEL, txtlen, txtlen);
            sendmsga(EM_REPLACESEL, TRUE, (LPARAM)_txt);
            sendmsga(EM_SETSEL, dwStartPos, dwEndPos);
            return *this;
        }
        window& appendtext(const wchar_t* _txt)
        {
            DWORD dwStartPos, dwEndPos;
            sendmsgw(EM_GETSEL, (WPARAM)&dwStartPos, (WPARAM)&dwEndPos);
            int txtlen = GetWindowTextLengthW(hwnd);
            sendmsgw(EM_SETSEL, txtlen, txtlen);
            sendmsgw(EM_REPLACESEL, TRUE, (LPARAM)_txt);
            sendmsgw(EM_SETSEL, dwStartPos, dwEndPos);
            return *this;
        }
};
END_NAMESPACE_YGP
#endif