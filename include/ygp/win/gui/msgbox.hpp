#ifndef _YGP_WIN_GUI_MSGBOX_HPP_
#define _YGP_WIN_GUI_MSGBOX_HPP_
#include"global.hpp"
#include<sstream>
BEGIN_NAMESPACE_YGP
class msgboxa
{
    const char* _title;
    UINT _style;
    HWND _hwnd;
    template<typename _Tp>
    void m(_Tp&& _tp)
    {
        ss<<_tp;
    }
    template<typename _Tp,typename ..._Args>
    void m(_Tp&& _tp,_Args&& ..._args)
    {
        ss<<_tp;
        m(_args...);
    }
    template<typename _Tp>
    void ml(_Tp&& _tp)
    {
        ss<<_tp<<delimeter;
    }
    template<typename _Tp,typename ..._Args>
    void ml(_Tp&& _tp,_Args&& ..._args)
    {
        ss<<_tp<<delimeter;
        ml(_args...);
    }
    public:
        char delimeter;
        std::stringstream ss;
        msgboxa():_title(nullptr),_style(0u),_hwnd(nullptr),delimeter(' '){}
        msgboxa& title(const char* _Title)
        {
            _title=_Title;
            return *this;
        }
        msgboxa& hwnd(HWND _Hwnd)
        {
            _hwnd=_Hwnd;
            return *this;
        }
        msgboxa& style(UINT _Style)
        {
            _style=_Style;
            return *this;
        }
        template<typename ..._Args>
        int operator()(_Args&& ..._args)
        {
            m(_args...);
            return MessageBoxA(_hwnd,ss.str().c_str(),_title,_style);
        }
        int operator()()
        {
            return MessageBoxA(_hwnd,ss.str().c_str(),_title,_style);
        }
        template<typename ..._Args>
        int put(_Args&& ..._args)
        {
            ml(_args...);
            return MessageBoxA(_hwnd,ss.str().c_str(),_title,_style);
        }
};
class msgboxw
{
    const wchar_t* _title;
    UINT _style;
    HWND _hwnd;
    template<typename _Tp>
    void m(_Tp&& _tp)
    {
        ss<<_tp;
    }
    template<typename _Tp,typename ..._Args>
    void m(_Tp&& _tp,_Args&& ..._args)
    {
        ss<<_tp;
        m(_args...);
    }
    template<typename _Tp>
    void ml(_Tp&& _tp)
    {
        ss<<_tp<<delimeter;
    }
    template<typename _Tp,typename ..._Args>
    void ml(_Tp&& _tp,_Args&& ..._args)
    {
        ss<<_tp<<delimeter;
        ml(_args...);
    }
    public:
        wchar_t delimeter;
        std::wstringstream ss;
        msgboxw():_title(nullptr),_style(0u),_hwnd(nullptr),delimeter(L' '){}
        msgboxw& title(const wchar_t* _Title)
        {
            _title=_Title;
            return *this;
        }
        msgboxw& hwnd(HWND _Hwnd)
        {
            _hwnd=_Hwnd;
            return *this;
        }
        msgboxw& style(UINT _Style)
        {
            _style=_Style;
            return *this;
        }
        template<typename ..._Args>
        int operator()(_Args&& ..._args)
        {
            m(_args...);
            return MessageBoxW(_hwnd,ss.str().c_str(),_title,_style);
        }
        int operator()()
        {
            return MessageBoxW(_hwnd,ss.str().c_str(),_title,_style);
        }
        template<typename ..._Args>
        int put(_Args&& ..._args)
        {
            ml(_args...);
            return MessageBoxW(_hwnd,ss.str().c_str(),_title,_style);
        }
};
END_NAMESPACE_YGP
#endif