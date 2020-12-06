#ifndef _YGP_WIN_WNDUTIL_HPP_
#define _YGP_WIN_WNDUTIL_HPP_
#ifdef _MSC_VER
#pragma comment(lib,"user32")
#endif
#include"winerr.hpp"
BEGIN_NAMESPACE_YGP
template<typename _Container>
static _Container& enumwindows(_Container& _con)
{
    if(!EnumWindows(
        [](HWND _hwnd,LPARAM _lpa)->BOOL
        {
            ((_Container*)_lpa)->push_back(_hwnd);
            return TRUE;
        },(LPARAM)&_con))
        throw std::runtime_error("ygp::enumwindows (function EnumWindows): "+lasterror<std::string>());
    return _con;
}
template<typename _Container>
static _Container& enumchildwindows(HWND _hwnd,_Container& _con)
{
    EnumChildWindows(_hwnd,
        [](HWND _hwn,LPARAM _lpa)->BOOL
        {
            ((_Container*)_lpa)->push_back(_hwn);
            return TRUE;
        },(LPARAM)&_con);//The return value of EnumChildWindows is not used.
    return _con;
}
template<typename _Tp>
static std::shared_ptr<_Tp> getwindowtext(HWND);
template<>
static std::shared_ptr<char> getwindowtext<char>(HWND _hwnd)
{
    int nlength=GetWindowTextLengthA(_hwnd);
    if(nlength<0)throw std::runtime_error("ygp::getwindowtext (function GetWindowTextLengthA): "+lasterror<std::string>());
    char* buf=new char[nlength+1];
    if(GetWindowTextA(_hwnd,buf,nlength+1)<0)
        throw std::runtime_error("ygp::getwindowtext (function GetWindowTextA): "+lasterror<std::string>());
    return std::shared_ptr<char>(buf);
}
template<>
static std::shared_ptr<wchar_t> getwindowtext<wchar_t>(HWND _hwnd)
{
    int nlength=GetWindowTextLengthW(_hwnd);
    if(nlength<0)throw std::runtime_error("ygp::getwindowtext (function GetWindowTextLengthW): "+lasterror<std::string>());
    wchar_t* buf=new wchar_t[nlength+1];
    if(GetWindowTextW(_hwnd,buf,nlength+1)<0)
        throw std::runtime_error("ygp::getwindowtext (function GetWindowTextW): "+lasterror<std::string>());
    return std::shared_ptr<wchar_t>(buf);
}
static std::pair<DWORD,DWORD> windowthreadprocid(HWND _hwnd)
{
    DWORD dwPID,dwTID;
    dwTID=GetWindowThreadProcessId(_hwnd,&dwPID);
    return std::pair<DWORD,DWORD>(dwPID,dwTID);
}
static HWND findwindow(LPCSTR windowname,LPCSTR classname=nullptr)
{
    HWND ret=FindWindowA(classname,windowname);
    if(!ret)
        throw std::runtime_error("ygp::findwindow (function FindWindowA): "+lasterror<std::string>());
    return ret;
}
static HWND findwindow(LPCWSTR windowname,LPCWSTR classname=nullptr)
{
    HWND ret=FindWindowW(classname,windowname);
    if(!ret)
        throw std::runtime_error("ygp::findwindow (function FindWindowW): Cannot find the specific window.");
    return ret;
}
END_NAMESPACE_YGP
#endif