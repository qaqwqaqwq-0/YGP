#ifndef _YGP_WIN_GUI_WNDCLASS_HPP_
#define _YGP_WIN_GUI_WNDCLASS_HPP_
#include"global.hpp"
BEGIN_NAMESPACE_YGP
static void registerclass(const char* classname="YGP Window Class",WNDPROC wndproc=nullptr,
    HBRUSH hbr=(HBRUSH)(COLOR_WINDOW+1),HCURSOR hcs=LoadCursorA(nullptr,MAKEINTRESOURCEA(32512)),
    HICON hico=LoadIconA(nullptr,MAKEINTRESOURCEA(32512)),
    HICON hicosm=LoadIconA(nullptr,MAKEINTRESOURCEA(32512)),
    HINSTANCE hinst=currentmodule())
{
    WNDCLASSEXA wc={sizeof(WNDCLASSEXA),0u,wndproc,0,0,hinst,hico,hcs,hbr,nullptr,
        classname,hicosm};
    if(!RegisterClassExA(&wc))
        throw std::runtime_error("ygp::registerclass (function RegisterClassExA): "
            +lasterror<std::string>());
}
static void registerclass(const wchar_t* classname=L"YGP Window Class",WNDPROC wndproc=nullptr,
    HBRUSH hbr=(HBRUSH)(COLOR_WINDOW+1),
    HCURSOR hcs=LoadCursorW(nullptr,MAKEINTRESOURCEW(32512)),
    HICON hico=LoadIconW(nullptr,MAKEINTRESOURCEW(32512)),
    HICON hicosm=LoadIconW(nullptr,MAKEINTRESOURCEW(32512)),
    HINSTANCE hinst=currentmodule())
{
    WNDCLASSEXW wc={sizeof(WNDCLASSEXW),0u,wndproc,0,0,hinst,hico,hcs,hbr,nullptr,
        classname,hicosm};
    if(!RegisterClassExW(&wc))
        throw std::runtime_error("ygp::registerclass (function RegisterClassExA): "
            +lasterror<std::string>());
}
static void registerclassifnotexists(const char* classname="YGP Window Class",WNDPROC wndproc=nullptr,
    HBRUSH hbr=(HBRUSH)(COLOR_WINDOW+1),HCURSOR hcs=LoadCursorA(nullptr,MAKEINTRESOURCEA(32512)),
    HICON hico=LoadIconA(nullptr,MAKEINTRESOURCEA(32512)),
    HICON hicosm=LoadIconA(nullptr,MAKEINTRESOURCEA(32512)),
    HINSTANCE hinst=currentmodule())
{
    WNDCLASSEXA wc={sizeof(WNDCLASSEXA),0u,wndproc,0,0,hinst,hico,hcs,hbr,nullptr,
        classname,hicosm};
    RegisterClassExA(&wc);
    DWORD dwLastError=GetLastError();
    if(dwLastError)if(dwLastError!=ERROR_ALREADY_EXISTS)
        throw std::runtime_error("ygp::registerclassifnotexists (function RegisterClassExA): "
            +lasterror<std::string>());
}
static void registerclassifnotexists(const wchar_t* classname=L"YGP Window Class",WNDPROC wndproc=nullptr,
    HBRUSH hbr=(HBRUSH)(COLOR_WINDOW+1),
    HCURSOR hcs=LoadCursorW(nullptr,MAKEINTRESOURCEW(32512)),
    HICON hico=LoadIconW(nullptr,MAKEINTRESOURCEW(32512)),
    HICON hicosm=LoadIconW(nullptr,MAKEINTRESOURCEW(32512)),
    HINSTANCE hinst=currentmodule())
{
    WNDCLASSEXW wc={sizeof(WNDCLASSEXW),0u,wndproc,0,0,hinst,hico,hcs,hbr,nullptr,
        classname,hicosm};
    RegisterClassExW(&wc);
    DWORD dwLastError=GetLastError();
    if(dwLastError)if(dwLastError!=ERROR_ALREADY_EXISTS)
        throw std::runtime_error("ygp::registerclassifnotexists (function RegisterClassExA): "
            +lasterror<std::string>());
}
END_NAMESPACE_YGP
#endif