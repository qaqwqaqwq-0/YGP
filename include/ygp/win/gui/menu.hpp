#ifndef _YGP_WIN_GUI_MENU_HPP_
#define _YGP_WIN_GUI_MENU_HPP_
#include"frame.hpp"
BEGIN_NAMESPACE_YGP
struct basic_menu
{
    HMENU h;
    operator HMENU()const noexcept{return h;}
    basic_menu(HMENU _h):h(_h){}
    basic_menu()
    {
        if(!(h=CreateMenu()))
            throw std::runtime_error("ygp::basic_menu::basic_menu"
                " (function CreateMenu): "+lasterror<std::string>());
    }
    void append(WORD _id,LPCSTR _str,bool bcheck=false)
    {
        if(!AppendMenuA(h,MF_STRING|bcheck?MF_CHECKED:0,_id,_str))
            throw std::runtime_error("ygp::basic_menu::append"
                " (function AppendMenuA): "+lasterror<std::string>());
    }
    void append(WORD _id,LPCWSTR _str,bool bcheck=false)
    {
        if(!AppendMenuW(h,MF_STRING|bcheck?MF_CHECKED:0,_id,_str))
            throw std::runtime_error("ygp::basic_menu::append"
                " (function AppendMenuW): "+lasterror<std::string>());
    }
    void append(HMENU hm,LPCSTR _str)
    {
        if(!AppendMenuA(h,MF_POPUP,(UINT_PTR)hm,_str))
            throw std::runtime_error("ygp::basic_menu::append"
                " (function AppendMenuA): "+lasterror<std::string>());
    }
    void append(HMENU hm,LPCWSTR _str)
    {
        if(!AppendMenuW(h,MF_POPUP,(UINT_PTR)hm,_str))
            throw std::runtime_error("ygp::basic_menu::append"
                " (function AppendMenuW): "+lasterror<std::string>());
    }
    void appendseparator()
    {
        if(!AppendMenuW(h,MF_SEPARATOR,0,nullptr))
            throw std::runtime_error("ygp::basic_menu::appendseparator"
                " (function AppendMenuW): "+lasterror<std::string>());
    }
    void destroy()
    {
        DestroyMenu(h);
    }
};
struct basic_popupmenu
{
    HMENU h;
    operator HMENU()const noexcept{return h;}
    basic_popupmenu(HMENU _h):h(_h){}
    basic_popupmenu()
    {
        if(!(h=CreatePopupMenu()))
            throw std::runtime_error("ygp::basic_popupmenu::basic_popupmenu"
                " (function CreatePopupMenu): "+lasterror<std::string>());
    }
    void append(WORD _id,LPCSTR _str,bool bcheck=false)
    {
        if(!AppendMenuA(h,MF_STRING|bcheck?MF_CHECKED:0,_id,_str))
            throw std::runtime_error("ygp::basic_popupmenu::append"
                " (function AppendMenuA): "+lasterror<std::string>());
    }
    void append(WORD _id,LPCWSTR _str,bool bcheck=false)
    {
        if(!AppendMenuW(h,MF_STRING|bcheck?MF_CHECKED:0,_id,_str))
            throw std::runtime_error("ygp::basic_popupmenu::append"
                " (function AppendMenuW): "+lasterror<std::string>());
    }
    void append(HMENU hm,LPCSTR _str)
    {
        if(!AppendMenuA(h,MF_POPUP,(UINT_PTR)hm,_str))
            throw std::runtime_error("ygp::basic_popupmenu::append"
                " (function AppendMenuA): "+lasterror<std::string>());
    }
    void append(HMENU hm,LPCWSTR _str)
    {
        if(!AppendMenuW(h,MF_POPUP,(UINT_PTR)hm,_str))
            throw std::runtime_error("ygp::basic_popupmenu::append"
                " (function AppendMenuW): "+lasterror<std::string>());
    }
    void appendseparator()
    {
        if(!AppendMenuW(h,MF_SEPARATOR,0,nullptr))
            throw std::runtime_error("ygp::basic_popupmenu::appendseparator"
                " (function AppendMenuW): "+lasterror<std::string>());
    }
    void destroy()
    {
        DestroyMenu(h);
    }
};
struct menu:public basic_menu
{
    frame* frm;
    menu(frame& _frm):frm(&_frm){}
    menu& set()
    {
        frm->win.setmenu(h);
        return *this;
    }
    menu& draw()
    {
        frm->win.drawmenubar();
        return *this;
    }
    basic_popupmenu createsubmenu()
    {
        return basic_popupmenu();
    }
    menu& add(LPCSTR _str)
    {
        append(++frm->idlist,_str);
        return *this;
    }
    menu& add(LPCWSTR _str)
    {
        append(++frm->idlist,_str);
        return *this;
    }
};
END_NAMESPACE_YGP
#endif