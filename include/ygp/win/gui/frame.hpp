#ifndef _YGP_WIN_GUI_FRAME_HPP_
#define _YGP_WIN_GUI_FRAME_HPP_
#include"window.hpp"
#include"messagehandler.hpp"
#include<ctime>
#include<vector>
#include<unordered_map>
BEGIN_NAMESPACE_YGP
class frame
{
    public:
        class initializer
        {
            public:
                WNDCLASSEXA cls;
                std::string classname;
                std::string windowname;
                DWORD dwStyle;
                DWORD dwExStyle;
                int X,Y,nWidth,nHeight;
                initializer()
                {
                    zmem(cls);
                    cls.cbSize=sizeof(cls);
                    classname=strfmt("YGPWindowClass_Thread%luTickCount%lu",
                        GetCurrentThreadId(),GetTickCount());
                    cls.lpszClassName=classname.c_str();
                    cls.lpfnWndProc=WndProc;
                    cls.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
                    cls.hCursor=LoadCursorW(nullptr,MAKEINTRESOURCEW(32512));
                    cls.hIcon=cls.hIconSm=
                        LoadIconW(nullptr,MAKEINTRESOURCEW(32512));
                    cls.hInstance=currentmodule();
                    dwStyle=WS_VISIBLE|WS_OVERLAPPEDWINDOW;
                    dwExStyle=WS_EX_CLIENTEDGE;
                    X=Y=nWidth=nHeight=CW_USEDEFAULT;
                }
                initializer& brush(HBRUSH hbr)
                {
                    cls.hbrBackground=hbr;
                    return *this;
                }
                initializer& background(BYTE _r,BYTE _g,BYTE _b)
                {
                    cls.hbrBackground=CreateSolidBrush(RGB(_r,_g,_b));
                    return *this;
                }
                initializer& cursor(HCURSOR hcs)
                {
                    cls.hCursor=hcs;
                    return *this;
                }
                initializer& icon(HICON hi)
                {
                    cls.hIcon=hi;
                    return *this;
                }
                initializer& iconsm(HICON hi)
                {
                    cls.hIconSm=hi;
                    return *this;
                }
                initializer& name(const char* _name)
                {
                    windowname=_name;
                    return *this;
                }
                initializer& style(DWORD _style)
                {
                    dwStyle=_style;
                    return *this;
                }
                initializer& exstyle(DWORD _exstyle)
                {
                    dwExStyle=_exstyle;
                    return *this;
                }
                initializer& setrect(const rect& rct)
                {
                    X=rct.left;
                    Y=rct.top;
                    nWidth=rct.w();
                    nHeight=rct.h();
                    return *this;
                }
                initializer& x(int _x)
                {
                    X=_x;
                    return *this;
                }
                initializer& y(int _y)
                {
                    Y=_y;
                    return *this;
                }
                initializer& width(int _w)
                {
                    nWidth=_w;
                    return *this;
                }
                initializer& height(int _h)
                {
                    nHeight=_h;
                    return *this;
                }
                frame& create(frame& ret)
                {
                    if(!RegisterClassExA(&cls))
                        throw std::runtime_error(
                            "ygp::frame::initializer::create "
                            "(function RegisterClassExA): "
                            +lasterror<std::string>());
                    ret.win.create(classname.c_str(),windowname.c_str(),
                        dwStyle,dwExStyle,X,Y,nWidth,nHeight);
                    ret.initialize();
                    return ret;
                }
        };
        window win;
        WORD idlist;
        struct msghandlerdata
        {
            detail::WM_COMMAND_handler* pwmcommandh;
            msghandlerdata():pwmcommandh(nullptr){}
            ~msghandlerdata()
            {
                if(pwmcommandh)delete pwmcommandh;
                pwmcommandh=nullptr;
            }
        }mhd;
        frame():idlist(0){}
        frame(HWND _hwnd):win(_hwnd){}
        void initialize()//The user should not call this method apparently
        {
            map[WM_DESTROY].builtin=[](HWND,WPARAM,LPARAM)
            {
                PostQuitMessage(0);
            };
            auto wm_command_func=[](HWND hwnd,WPARAM wParam,
                LPARAM lParam)
            {
                static detail::WM_COMMAND_handler** ppghbi=nullptr;
                if(!ppghbi)
                {
                    ppghbi=(decltype(ppghbi))wParam;
                    if(lParam!=0x3e75264f)return;
                    *ppghbi=new detail::WM_COMMAND_handler;
                    return;
                }
                (*ppghbi)->invoke(LOWORD(wParam),HIWORD(wParam));
            };
            map[WM_COMMAND].builtin=wm_command_func;
            wm_command_func(nullptr,(WPARAM)&mhd.pwmcommandh,0x3e75264f);
            win.setlonga(GWLP_USERDATA,&map);
        }
        operator HWND()const noexcept{return win.hwnd;}
        operator window&()noexcept{return win;}
        static constexpr size_t lowpart=0x3000u;
        struct map_t
        {
            messagehandler* ptr;
            std::unordered_map<UINT,messagehandler> mp;
            map_t():ptr(new messagehandler[lowpart]){}
            messagehandler& operator[](UINT Msg)
            {
                if(Msg<lowpart)return ptr[Msg];
                else return mp[Msg];
            }
            bool operator()(UINT Msg,messagehandler*& pmh)
            {
                if(Msg<lowpart)
                {
                    pmh=ptr+Msg;
                    return true;
                }
                std::unordered_map<UINT,messagehandler>::iterator
                    it=mp.find(Msg);
                if(it==mp.end())
                    return false;
                pmh=&it->second;
                return true;
            }
            ~map_t(){if(ptr)delete []ptr;}
        }map;
        static YGP_WNDPROC
        {
            static map_t* pmap=nullptr;
            if(!pmap)pmap=(map_t*)GetWindowLongPtrA(hwnd,GWLP_USERDATA);
            if(!pmap)return DefWindowProcA(hwnd,Message,wParam,lParam);
            messagehandler* pmh;
            if(!pmap->operator()(Message,pmh))
                return DefWindowProcA(hwnd,Message,wParam,lParam);
            return pmh->operator()(hwnd,Message,wParam,lParam);
        }
};
END_NAMESPACE_YGP
#endif