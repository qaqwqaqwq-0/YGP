#define YGP_DISABLE_BROOM
#define YGP_DISABLE_DEMENTOR
#define YGP_DISABLE_BROWSER
#define YGP_DISABLE_PLAYER
#include"../include/ygp.hpp"
YGP_INIT
YGP_DLGPROC
{
    static window win;
    YGP_BEGIN_MSG_MAP
    case WM_INITDIALOG:
    {
        win=hwnd;
        break;
    }
    case WM_COMMAND:
    {
        switch(wParam)
        {
            case IDCANCEL:
            {
                win.enddlg(0);
                break;
            }
        }
        break;
    }
    YGP_END_MSG_MAP
    return 0;
}
YGP_WINMAIN
{
    YGP_TRY
        dialog dlg;
        dialog::initializer(dlg).x(200).y(200)
            .width(200).height(200)(DlgProc);
    YGP_CATCH_MSGBOX
    return 0;
}