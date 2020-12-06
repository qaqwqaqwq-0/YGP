#define YGP_DISABLE_BROOM
#define YGP_DISABLE_DEMENTOR
#define YGP_DISABLE_BROWSER
#define YGP_DISABLE_PLAYER
#include"../include/ygp.hpp"
YGP_INIT
YGP_WNDPROC
{
    static window win;
    YGP_BEGIN_MSG_MAP
    case WM_CREATE:
    {
        win=hwnd;
        break;
    }
    case WM_DESTROY:
    {
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