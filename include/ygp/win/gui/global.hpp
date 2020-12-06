#ifndef _YGP_WIN_GUI_GLOBAL_HPP_
#define _YGP_WIN_GUI_GLOBAL_HPP_
#include"../winerr.hpp"
#include<functional>
#pragma comment(lib,"gdi32")
#pragma comment(lib,"comctl32")
BEGIN_NAMESPACE_YGP
#define YGP_WINMAIN int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR lpCmdLine,int)
#define YGP_WWINMAIN int WINAPI wWinMain(HINSTANCE,HINSTANCE,LPWSTR lpCmdLine,int)
#define YGP_WNDPROC LRESULT CALLBACK WndProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
#define YGP_WNDPROC_(funcname) LRESULT CALLBACK funcname(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
#define YGP_DLGPROC INT_PTR CALLBACK DlgProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
#define YGP_DLGPROC_(funcname) INT_PTR CALLBACK funcname(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
#define YGP_REGCLS(classname) ygp::registerclass(classname,WndProc);
#define YGP_MANIFEST comment(linker,"\"/manifestdependency:type='win32' \
    name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
    processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define YGP_INIT_RICHEDITCTRL LoadLibraryA("msftedit.dll");
#define YGP_BEGIN_MSG_MAP switch(Message){
#define YGP_END_MSG_MAP default:return DefWindowProcA(hwnd,Message,wParam,lParam);}
#define YGP_GDIPLUS_INIT ULONG_PTR GdiToken;                \
    Gdiplus::GdiplusStartupInput GdiInput;                  \
    Gdiplus::GdiplusStartup(&GdiToken,&GdiInput,nullptr);
#define YGP_GDIPLUS_UNINIT Gdiplus::GdiplusShutdown(GdiToken);
#define YGP_INIT_GUI SetProcessDPIAware();InitCommonControls();
static HMODULE getmodulehandle(LPCSTR mdstr)
{
    HMODULE ret=GetModuleHandleA(mdstr);
    if(!ret)throw std::runtime_error(
        "ygp::getmodulehandle (function GetModuleHandleA): "
        +lasterror<std::string>());
    return ret;
}
static HMODULE getmodulehandle(LPCWSTR mdstr)
{
    HMODULE ret=GetModuleHandleW(mdstr);
    if(!ret)throw std::runtime_error(
        "ygp::getmodulehandle (function GetModuleHandleA): "
        +lasterror<std::string>());
    return ret;
}
static HMODULE currentmodule()
{
    return getmodulehandle((LPCSTR)nullptr);
}
static WPARAM messageloop()
{
    MSG msg;
    while(GetMessageA(&msg,nullptr,0u,0u))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return msg.wParam;
}
static WPARAM messageloop(const std::function<void(HWND,UINT,WPARAM,LPARAM)>& _pre)
{
    MSG msg;
    while(GetMessageA(&msg,nullptr,0u,0u))
    {
        _pre(msg.hwnd,msg.message,msg.wParam,msg.lParam);
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return msg.wParam;
}
static WPARAM messageloop(const std::function<void(HWND,UINT,WPARAM,LPARAM,bool&)>& _pre)
{
    MSG msg;
    while(GetMessageA(&msg,nullptr,0u,0u))
    {
        bool bcont=true;
        _pre(msg.hwnd,msg.message,msg.wParam,msg.lParam,bcont);
        if(!bcont)continue;
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return msg.wParam;
}
static WPARAM messageloop(HWND _hwnd)
{
    MSG msg;
    while(GetMessageA(&msg,_hwnd,0u,0u))
    {
        if(IsDialogMessageA(_hwnd,&msg))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }
    return msg.wParam;
}
END_NAMESPACE_YGP
#endif