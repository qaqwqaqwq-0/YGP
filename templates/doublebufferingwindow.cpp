#define YGP_DISABLE_BROOM
#define YGP_DISABLE_DEMENTOR
#define YGP_DISABLE_BROWSER
#define YGP_DISABLE_PLAYER
#include"../include/ygp.hpp"
YGP_INIT
YGP_WNDPROC
{
    static HDC hMemDC;
    static HBITMAP hMemBmp,hPreBmp;
    static window win;
    YGP_BEGIN_MSG_MAP
    case WM_CREATE:
    {
        win=hwnd;
        HDC hDC=win.getdc();
        rect rct=win.clientrect();
        hMemDC=CreateCompatibleDC(hDC);
        hMemBmp=CreateCompatibleBitmap(hMemDC,rct.w(),
            rct.h());
        hPreBmp=(HBITMAP)SelectObject(hMemDC,hMemBmp);
        win.releasedc(hDC);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hDC=win.beginpaint(ps);
        rect rct=win.clientrect();
        BitBlt(hDC,0,0,rct.w(),rct.h(),hMemDC,0,0,SRCCOPY);
        win.endpaint(ps);
        break;
    }
    case WM_DESTROY:
    {
        SelectObject(hMemDC,hPreBmp);
        DeleteObject(hMemBmp);
        DeleteDC(hMemDC);
        PostQuitMessage(0);
        break;
    }
    YGP_END_MSG_MAP
    return 0;
}
YGP_WINMAIN
{
    YGP_TRY
        YGP_REGCLS("YGPWindowClass")
        window win("YGPWindowClass","Caption");
        messageloop();
    YGP_CATCH_MSGBOX
}