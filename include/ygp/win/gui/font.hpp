#ifndef _YGP_WIN_GUI_FONT_HPP_
#define _YGP_WIN_GUI_FONT_HPP_
#include"gdiobj.hpp"
BEGIN_NAMESPACE_YGP
class font:public gdiobj<HFONT>
{
    LOGFONTA l;
    public:
        font()
        {
            h=nullptr;
            zmem(l);
            l.lfCharSet=DEFAULT_CHARSET;
        }
        font& height(LONG lfHeight)noexcept
        {
            l.lfHeight=lfHeight;
            return *this;
        }
        font& width(LONG lfWidth)noexcept
        {
            l.lfWidth=lfWidth;
            return *this;
        }
        font& weight(LONG lfWeight)noexcept
        {
            l.lfWeight=lfWeight;
            return *this;
        }
        font& italic(bool _b)noexcept
        {
            l.lfItalic=(BOOL)_b;
            return *this;
        }
        font& underline(bool _b)noexcept
        {
            l.lfUnderline=(BOOL)_b;
            return *this;
        }
        font& strikeout(bool _b)noexcept
        {
            l.lfStrikeOut=(BOOL)_b;
            return *this;
        }
        font& name(const char* _name)
        {
            for(std::size_t i=0;;++i)
            {
                l.lfFaceName[i]=_name[i];
                if(i==31&&_name[i])
                    throw std::runtime_error("ygp::font::name: "
                        "The length of the face name must not exceed "
                        "32, including the terminating '\\0'.");
                if(!_name[i])break;
            }
            return *this;
        }
        font& create()
        {
            if(!(h=CreateFontIndirectA(&l)))
                throw std::runtime_error("ygp::font::create "
                    "(function CreateFontIndirectA): "
                    "Failed to create the font.");
            return *this;
        }
};
END_NAMESPACE_YGP
#endif