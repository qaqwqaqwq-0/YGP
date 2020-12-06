#ifndef _YGP_WIN_GUI_GDIOBJ_HPP_
#define _YGP_WIN_GUI_GDIOBJ_HPP_
#include"../winerr.hpp"
BEGIN_NAMESPACE_YGP
template<typename _Tp>
struct basic_gdiobj
{
    _Tp h;
    operator _Tp()const noexcept
    {
        return h;
    }
    void del()//delete the object
    {
        DeleteObject(h);
        h=nullptr;
    }
    HGDIOBJ selectobject(HDC hDC)
    {
        return SelectObject(hDC,h);
    }
};
template<typename _Tp>
struct gdiobj:basic_gdiobj<_Tp>
{
    ~gdiobj(){this->del();}
};
END_NAMESPACE_YGP
#endif