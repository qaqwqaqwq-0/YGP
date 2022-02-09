#ifndef _YGP_WIN_GUI_OPENGL_HPP
#define _YGP_WIN_GUI_OPENGL_HPP
#include"../winerr.hpp"
#include<gl/gl.h>
#ifdef _MSC_VER
#pragma comment(lib,"opengl32")
#endif
BEGIN_NAMESPACE_YGP
namespace gl
{
    class context
    {
        public:
            HGLRC h;
            context():h(nullptr){}
            context(HDC hDC)
            {
                create(hDC);
            }
            context& create(HDC hDC)
            {
                h=wglCreateContext(hDC);
                if(!h)
                    throw std::runtime_error("ygp::gl::context::create (function "
                        "wglCreateContext): "+lasterror<std::string>());
                return *this;
            }
            void setcurrent(HDC hDC)
            {
                if(!wglMakeCurrent(hDC,h))
                    throw std::runtime_error("ygp::gl::context::setcurrent (function "
                        "wglMakeCurrent): "+lasterror<std::string>());
            }
            ~context()
            {
                if(h)wglDeleteContext(h);
            }
    };
    template<typename _Ps>
    void line3d(const _Ps& p1,const _Ps& p2)
    {
        glBegin(GL_LINES);
            glVertex3f(p1.x,p1.y,p1.z);
            glVertex3f(p2.x,p2.y,p2.z);
        glEnd();
    }
}
END_NAMESPACE_YGP
#endif