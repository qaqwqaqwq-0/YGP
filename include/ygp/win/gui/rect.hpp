#ifndef _YGP_WIN_GUI_RECT_HPP_
#define _YGP_WIN_GUI_RECT_HPP_
#include"global.hpp"
BEGIN_NAMESPACE_YGP
struct rect:RECT
{
    rect():RECT{0l,0l,0l,0l}{}
    rect(LONG _lft,LONG _tp,LONG _rght,LONG _btm):
        RECT{_lft,_tp,_rght,_btm}{}
    rect(POINT _center,LONG _rad):
        RECT{_center.x-_rad,_center.y-_rad,_center.x+_rad,
        _center.y+_rad}{}
    rect(LONG _w,LONG _h):
        RECT{0l,0l,_w,_h}{}
    rect(const RECT& _rct):RECT{_rct}{}
    rect(const RECT&& _rct):RECT{_rct}{}
    rect(HWND hwnd)
    {
        if(!GetWindowRect(hwnd,this))
            throw std::runtime_error("ygp::rect::rect (function GetWindowRect): "
                +lasterror<std::string>());
    }
    rect& operator()(LONG _lft,LONG _tp,LONG _rght,LONG _btm)
    {
        left=_lft;
        top=_tp;
        right=_rght;
        bottom=_btm;
        return *this;
    }
    LONG w()const noexcept
    {
        return right-left;
    }
    LONG h()const noexcept
    {
        return bottom-top;
    }
    LONGLONG a()const noexcept
    {
        return w()*h();
    }
    bool intersect(const RECT& _rct)const noexcept
    {
        RECT _rect;
        return (bool)IntersectRect(&_rect,this,&_rct);
    }
    bool intersect(const RECT& _rct,RECT& _dst)const noexcept
    {
        return (bool)IntersectRect(&_dst,this,&_rct);
    }
    rect& operator=(const RECT& _anth)noexcept
    {
        CopyRect(this,&_anth);
        return *this;
    }
    bool operator==(const RECT& _anth)const noexcept
    {
        return (bool)EqualRect(this,&_anth);
    }
    bool empty()const noexcept
    {
        return (bool)IsRectEmpty(this);
    }
    rect& move(int dx,int dy=0)noexcept
    {
        OffsetRect(this,dx,dy);
        return *this;
    }
    bool contains(POINT pt)const noexcept
    {
        return (bool)PtInRect(this,pt);
    }
    rect& setempty()noexcept
    {
        SetRectEmpty(this);
        return *this;
    }
    rect& expand(LONG _delta)noexcept
    {
        left-=_delta;
        right+=_delta;
        top-=_delta;
        bottom+=_delta;
        return *this;
    }
    bool subtract(const RECT& _anth,RECT& _dst)const noexcept
    {
        return (bool)SubtractRect(&_dst,this,&_anth);
    }
    bool combine(const RECT& _anth,RECT& _dst)const noexcept
    {
        return (bool)UnionRect(&_dst,this,&_anth);
    }
    rect operator|(const RECT& _anth)
    {
        rect _dst;
        combine(_anth,_dst);
        return _dst;
    }
    operator std::string()const noexcept
    {
        return strfmt("Rectangle(left=%ld, top=%ld, right=%ld, bottom=%ld)",left,top,right,bottom);
    }
};
END_NAMESPACE_YGP
#endif