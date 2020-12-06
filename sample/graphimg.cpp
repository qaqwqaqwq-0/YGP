/*
    Known issues:
        1.Cannot keep the input box floating on parent.
*/
#define YGP_DISABLE_BROOM
#define YGP_DISABLE_DEMENTOR
#define YGP_DISABLE_PLAYER
#define YGP_DISABLE_BROWSER
#include"../include/ygp.hpp"
#include"../include/ygp/win/gui/menu.hpp"
#include<gdiplus.h>
#pragma YGP_LIB(gdiplus)
YGP_INIT
static int radius=10;
struct vertex:POINT
{
    int r;
};
std::list<vertex> nodes;
int cursel=-1;
static const Gdiplus::Color SelColor(0,0,255);
static constexpr int SelWidth=3;
struct inputboxinfo
{
    int* val;
    LPCSTR lpContent;
};
INT_PTR CALLBACK InputBoxProc(HWND hwnd,UINT Message,
    WPARAM wParam,LPARAM lParam)
{
    static window hInput;
    static window hStatic;
    static inputboxinfo* info=nullptr;
    if(!info)
    {
        info=(inputboxinfo*)GetWindowLongPtrA(hwnd,
            GWLP_USERDATA);
        if(info)SetWindowTextA(hStatic,info->lpContent);
    }
    switch(Message){
    case WM_INITDIALOG:
    {
        hStatic.create(hwnd,"static","",0,
            WS_CHILD|WS_VISIBLE,0,0,0,600,30);
        hInput.create(hwnd,"edit",nullptr,0,
            WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_WANTRETURN|
            ES_AUTOVSCROLL|ES_AUTOHSCROLL|ES_NUMBER,IDOK,
            0,30,600,60);
        LOGFONTA lf;
        zmem(lf);
        lf.lfHeight=30;
        strcpy(lf.lfFaceName,"Courier New");
        HFONT hf=CreateFontIndirectA(&lf);
        hStatic.sendmsga(WM_SETFONT,(WPARAM)hf);
        hInput.sendmsga(WM_SETFONT,(WPARAM)hf);
        info=nullptr;
        break;
    }
    case WM_COMMAND:
    {
        if(SendMessageA(hInput,EM_GETLINECOUNT,
            0,0)>1)
        {
            int sz=GetWindowTextLengthA(hInput);
            char* txt=(char*)malloc(sz+1);
            GetWindowTextA(hInput,txt,sz+1);
            *(info->val)=atoi(txt);
            free(txt);
            DestroyWindow(hwnd);
            break;
        }
        break;
    }
    case WM_CLOSE:
    {
        DestroyWindow(hwnd);
        break;
    }
    default:return FALSE;
    }
    return TRUE;
}
void InputBoxThreadProc(HGLOBAL hglb,HWND hParent)
{
    DialogBoxIndirectA(currentmodule(),
        (LPDLGTEMPLATE)hglb,hParent,InputBoxProc);
}
int inputbox(HWND hParent,LPCSTR lpTitle,LPCSTR lpContent)
{
    HGLOBAL hglb=GlobalAlloc(GMEM_ZEROINIT,1024);
    LPDLGTEMPLATE lpdt=(LPDLGTEMPLATE)GlobalLock(hglb);
    lpdt->style=WS_POPUP|WS_BORDER|WS_SYSMENU|
        DS_MODALFRAME|WS_CAPTION;
    lpdt->x=lpdt->y=400;
    lpdt->cx=600;
    lpdt->cy=140;
    GlobalUnlock(hglb);
    window inputwin;
    DialogBoxIndirectA(currentmodule(),
        (LPDLGTEMPLATE)hglb,hParent,InputBoxProc);
    int _ret=-1;
    inputboxinfo info{&_ret,lpContent};
    inputwin.setlonga(GWLP_USERDATA,&info);
    GlobalFree(hglb);
    return _ret;
}
int Emphasize(window& win,HDC hMemDC,POINT pt)
{
    int ret=-1;
    for(auto& i:nodes)
    {
        ++ret;
        rect rct(pt,radius);
        if(rct.contains(pt))
        {
            Gdiplus::Rect gr(rct.left,rct.top,
                rct.w(),rct.h());
            Gdiplus::Pen* pen=
                new Gdiplus::Pen(SelColor,SelWidth);
            Gdiplus::Graphics* gp=
                new Gdiplus::Graphics(hMemDC);
            gp->DrawEllipse(pen,gr);
            delete gp;
            delete pen;
            rct.expand(SelWidth-1);
            win.invalidaterect(rct);
            break;
        }
    }
    return ret;
}
YGP_WNDPROC
{
    static HDC hMemDC;
    static HBITMAP hMemBmp,hPreBmp;
    YGP_BEGIN_MSG_MAP
    case WM_CREATE:
    {
        HDC hDC=GetDC(hwnd);
        rect rct;
        GetClientRect(hwnd,&rct);
        hMemDC=CreateCompatibleDC(hDC);
        hMemBmp=CreateCompatibleBitmap(hMemDC,rct.w(),
            rct.h());
        hPreBmp=(HBITMAP)SelectObject(hMemDC,hMemBmp);
        ReleaseDC(hwnd,hDC);
        FillRect(hMemDC,&rct,
            (HBRUSH)GetStockObject(WHITE_BRUSH));
        break;
    }
    case WM_LBUTTONDOWN:
    {
        window win(hwnd);
        rect rct;
        POINT pt{GET_X_LPARAM(lParam),
            GET_Y_LPARAM(lParam)};
        if(cursel==-1)cursel=Emphasize(win,hMemDC,pt);
        else break;
        cursel=-1;
        rct=rect(pt,radius);
        Ellipse(hMemDC,rct.left,rct.top,rct.right,
            rct.bottom);
        win.invalidaterect(rct);
        nodes.push_back(vertex{pt,radius});
        break;
    }
    case WM_COMMAND:
    {
        switch(wParam)
        {
            case 1:
            {
                int ret=inputbox(hwnd,"YGP Graph Canvas",
                    "Enter desired radius here:");
                if(ret!=-1)radius=ret;
                break;
            }
        }
        break;
    }
    case WM_PAINT:
    {
        window win(hwnd);
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
        SetProcessDPIAware();
        YGP_GDIPLUS_INIT
        registerclass("YGPGRAPHIMG",WndProc);
        window win("YGPGRAPHIMG","YGP Graph Canvas",
            WS_VISIBLE|WS_OVERLAPPEDWINDOW,
            WS_EX_CLIENTEDGE,200,200,1620,1006);
        basic_menu mn;
        mn.append(1,"Set Node Radius");
        win.setmenu(mn);
        messageloop();
        YGP_GDIPLUS_UNINT
    YGP_CATCH_MSGBOX
    return 0;
}