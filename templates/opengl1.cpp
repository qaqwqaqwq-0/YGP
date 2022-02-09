#include"../include/ygp/base.hpp"
#include"../include/ygp/win/gui/window.hpp"
#include"../include/ygp/win/gui/opengl.hpp"
#include"../include/ygp/win/gui/msgbox.hpp"
#include"../include/ygp/win/gui/wndclass.hpp"
YGP_INIT
#define MAINWINDOWCLASSNAME "WndClass"
HDC hDC;
YGP_WNDPROC
{
    YGP_BEGIN_MSG_MAP
    case WM_DESTROY:
    {
        ReleaseDC(hwnd,hDC);
        wglMakeCurrent(nullptr,nullptr);
        PostQuitMessage(0);
        break;
    }
    YGP_END_MSG_MAP
    return 0;
}
YGP_WINMAIN
{
    YGP_TRY
        SetProcessDPIAware();
        YGP_REGCLS(MAINWINDOWCLASSNAME)
        window win(MAINWINDOWCLASSNAME,"YGP OpenGL Window",WS_VISIBLE|WS_OVERLAPPEDWINDOW,0,
            100,100,1200,800);
        hDC=win.getdc();
        int iFormat;
        PIXELFORMATDESCRIPTOR pfd;
        zmem(pfd);
        pfd.nSize=sizeof(pfd);
        pfd.nVersion=1;
        pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
        pfd.iPixelType=PFD_TYPE_RGBA;
        pfd.cColorBits=24;
        pfd.cDepthBits=16;
        pfd.iLayerType=PFD_MAIN_PLANE;
        iFormat=ChoosePixelFormat(hDC,&pfd);
        SetPixelFormat(hDC,iFormat,&pfd);
        gl::context ctx;
        ctx.create(hDC);
        ctx.setcurrent(hDC);
        return messageloop();
    YGP_CATCH_MSGBOX
    return -1;
}
//Don't forget to call SwapBuffers after drawing sth!