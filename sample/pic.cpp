/*
    问题：
    7.内存不足后会糊
    8.unsplash缩小后不动了: WM_SIZE
*/
#define YGP_DISABLE_BROOM
#define YGP_DISABLE_DEMENTOR
#define YGP_DISABLE_PLAYER
#include"../include/ygp.hpp"
#include<gdiplus.h>
#include<math.h>
#include<ppl.h>
#pragma YGP_LIB(gdiplus)
#pragma YGP_LIB(comctl32)
#pragma YGP_LIB(gdi32)
#pragma YGP_MANIFEST
#define APPNAME L"ZRY Picture Viewer"
#define MBAPPNAME "ZRY Picture Viewer"
#define CHECK if(ct.img->GetLastStatus()!=Gdiplus::Ok)                 \
    {                                                                  \
        msgboxw().title(APPNAME L" - Error").style(MB_ICONERROR)       \
        (L"An Gdiplus error occurred.");                               \
        delete ct.img;                                                 \
        ct.img=nullptr;                                                \
    }
#define TIPSIZE 120,70
#define TIPCOLOR RGB(250,250,250)
#define WM_SETTIP 0x0401
#define SCRWIDTH 20
YGP_INIT
void RedrawScrollbar();
void GetProperRect(RECT&);
struct content
{
    Gdiplus::Bitmap* img;
    Gdiplus::Bitmap* rimg;
    wstring url;
    int scale;
    HWND hwnd;
    HWND htip;
    HWND hsta;
    HWND hhor;
    HWND hver;
    bool bhsh;
    bool bvsh;
    wstring strtip;
    uint32_t* wbits;
    int offx;
    int offy;
    content():img(nullptr),rimg(nullptr),offx(0),offy(0),scale(0),
        wbits(nullptr),bhsh(false),bvsh(false){}
    double sc()
    {
        return pow(2.0f,scale);
    }
    int width()
    {
        return round(sc()*img->GetWidth());
    }
    int height()
    {
        return round(sc()*img->GetHeight());
    }
    void reset(const wstring& ws)
    {
        url=ws;
        scale=offx=offy=0;
        if(img)delete img;
        img=new Gdiplus::Bitmap(url-cstr);
        if(img->GetLastStatus()!=Gdiplus::Ok)
        {
            msgboxw().title(APPNAME L" - Error").style(MB_ICONERROR)
                (L"An Gdiplus error occurred.");
            delete img;
            img=nullptr;
            return;
        }
        SetWindowTextW(hwnd,(wstringstream()<<setprecision(3)<<fixed<<APPNAME
            <<L" - "<<url<<L" ("<<sc()*100.0f<<L"%) "<<img->GetWidth()<<L"×"<<
            img->GetHeight()).str()-cstr);
        RECT rect;
        GetProperRect(rect);
        rimg=img->Clone(Gdiplus::Rect(0,0,img->GetWidth(),img->GetHeight()),
            img->GetPixelFormat());
        ct.wbits=(uint32_t*)0xffffffffffffffffull;
        RedrawScrollbar();
        UpdateWindow(hwnd);
        RedrawWindow(hwnd,&rect,nullptr,RDW_UPDATENOW|RDW_INVALIDATE);
    }
}ct;
void GetProperRect(RECT& rect)
{
    GetClientRect(ct.hwnd,&rect);
    rect.right-=ct.bvsh?SCRWIDTH:0;
    rect.bottom-=ct.bhsh?SCRWIDTH:0;
}
LRESULT CALLBACK TooltipWndProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam);
void UpdateTooltip()
{
    POINT pt;
    GetCursorPos(&pt);
    MoveWindow(ct.htip,pt.x+10,pt.y+10,TIPSIZE,TRUE);
    if(!ct.img)
    {
        ct.strtip=L"Drop an image here";
        PostMessageW(ct.htip,WM_SETTIP,0,0);
        return;
    }
    ScreenToClient(ct.hwnd,&pt);
    RECT rect;
    GetProperRect(rect);
    if(pt.x<0||pt.x>=rect.right||pt.y<0||pt.y>=rect.bottom)
    {
        ShowWindow(ct.htip,SW_HIDE);
        return;
    }
    POINT real;
    real.x=ct.scale>=0?(pt.x+ct.offx)>>ct.scale:(pt.x+ct.offx)<<-ct.scale;
    real.y=ct.scale>=0?(pt.y+ct.offy)>>ct.scale:(pt.y+ct.offy)<<-ct.scale;
    if(real.x>ct.img->GetWidth()||real.y>ct.img->GetHeight())
    {
        ShowWindow(ct.htip,SW_HIDE);
        return;
    }
    ShowWindow(ct.htip,SW_SHOWDEFAULT);
    Gdiplus::Color cl;
    ct.img->GetPixel(real.x,real.y,&cl);
    ct.strtip=strfmt(L"(%d,%d)\n#0x%08X",real.x,real.y,cl.GetValue());
    PostMessageW(ct.htip,WM_SETTIP,0,0);
}
bool ScaleImage()
{
    int ow=ct.img->GetWidth();
    int oh=ct.img->GetHeight();
    int scl=ct.sc();
    int nw=ct.width();
    int nh=ct.height();
    uint32_t* wbits=ct.wbits;
    uint32_t* oldmem=wbits;
    ct.wbits=wbits=(uint32_t*)LocalAlloc(LPTR,nw*nh*sizeof(uint32_t));
    if(!wbits)
    {
        MessageBoxA(ct.hwnd,MBAPPNAME" cannot allocate sufficient memory"
            " while scaling this image.",MBAPPNAME" - Error",MB_ICONERROR);
        return false;
    }
    Gdiplus::BitmapData bmd;
    ct.img->LockBits(&Gdiplus::Rect(0,0,ct.img->GetWidth(),ct.img->GetHeight()),
        Gdiplus::ImageLockMode::ImageLockModeRead,PixelFormat32bppARGB,&bmd);
    uint32_t* rbits=(uint32_t*)bmd.Scan0;
    if(nw<2||nh<2)
    {
        ct.img->UnlockBits(&bmd);
        return false;
    }
    if(oldmem)LocalFree(oldmem);
    if(ct.rimg)delete ct.rimg;
    if(ct.scale>=0)
    {
        signer sn(ow);
        concurrency::parallel_for(0,ow,[&](int x)
        {
            for(int y=0;y<oh;++y)
            {
                uint32_t val=rbits[y*ow+x];
                for(int i=0;i<scl;++i)
                    for(int j=0;j<scl;++j)
                    {
                        int cx=scl*x+i,cy=scl*y+j;
                        wbits[cy*nw+cx]=val;
                    }
            }
            sn.inc();
        });
        sn.wait();
    }
    else
    {
        signer sn(nw);
        concurrency::parallel_for(0,nw,[&](int x)
        {
            for(int y=0;y<nh;++y)
            {
                wbits[y*nw+x]=rbits[(y<<-ct.scale)*ow+(x<<-ct.scale)];
            }
            sn.inc();
        });
        sn.wait();
    }
    ct.rimg=new Gdiplus::Bitmap(nw,nh,nw*sizeof(uint32_t),PixelFormat32bppARGB,
        (BYTE*)wbits);
    ct.img->UnlockBits(&bmd);
    return true;
}
void RedrawScrollbar()
{
    HWND hwnd=ct.hwnd;
    RECT rect;
    GetClientRect(hwnd,&rect);
    int szx=rect.right-rect.left;
    int szy=rect.bottom-rect.top;
    ct.bhsh=!(!ct.img||szx>ct.width());
    ct.bvsh=!(!ct.img||szy>ct.height());
    if(ct.bhsh)MoveWindow(ct.hhor,rect.left,rect.bottom-SCRWIDTH,
        rect.right-rect.left-SCRWIDTH,SCRWIDTH,TRUE);
    if(ct.bvsh)MoveWindow(ct.hver,rect.right-SCRWIDTH,rect.top,SCRWIDTH,
        rect.bottom-rect.top,TRUE);
    SCROLLINFO si={sizeof(si),SIF_PAGE|SIF_POS|SIF_RANGE,0};
    if(!ct.bhsh)ShowWindow(ct.hhor,SW_HIDE);
    else
    {
        ShowWindow(ct.hhor,SW_SHOWDEFAULT);
        si.nMax=ct.width();
        si.nPage=szx;
        si.nPos=ct.offx;
        SendMessageW(ct.hhor,SBM_SETSCROLLINFO,TRUE,(LPARAM)&si);
    }
    if(!ct.bvsh)ShowWindow(ct.hver,SW_HIDE);
    else
    {
        ShowWindow(ct.hver,SW_SHOWDEFAULT);
        si.nMax=ct.height();
        si.nPage=szy;
        si.nPos=ct.offy;
        SendMessageW(ct.hver,SBM_SETSCROLLINFO,TRUE,(LPARAM)&si);
    }
}
YGP_WNDPROC
{
    try
    {
    if(hwnd==ct.htip)return TooltipWndProc(hwnd,Message,wParam,lParam);
    YGP_BEGIN_MSG_MAP
    case WM_CREATE:
    {
        InitCommonControls();
        TRACKMOUSEEVENT te={sizeof(te)};
        te.cbSize=sizeof(te);
        te.dwFlags=TME_LEAVE;
        te.hwndTrack=hwnd;
        _TrackMouseEvent(&te);
        if(ct.url.empty())
        {
            ct.strtip=L"Drop an image here";
            PostMessageW(ct.htip,WM_SETTIP,0,0);
            RedrawScrollbar();
            break;
        }
        ct.img=new Gdiplus::Bitmap(ct.url-cstr);
        SetWindowTextW(hwnd,(wstringstream()<<APPNAME<<L" - "<<ct.url
            <<L" (100.000%) "<<ct.img->GetWidth()<<L"×"<<
            ct.img->GetHeight()).str()-cstr);
        CHECK
        ct.wbits=(uint32_t*)0xffffffffffffffffull;
        RedrawScrollbar();
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_PAINT:
    {
        if(!ct.img||!ct.wbits)break;
        if(!ct.rimg)
            ct.rimg=ct.img->Clone(Gdiplus::Rect(0,0,ct.img->GetWidth(),
                ct.img->GetHeight()),ct.img->GetPixelFormat());
        PAINTSTRUCT ps;
        HDC hdc=BeginPaint(hwnd,&ps);
        Gdiplus::Graphics gr(hdc);
        gr.Clear(Gdiplus::Color::White);
        Gdiplus::Color cl;
        gr.DrawImage(
            ct.rimg,
            Gdiplus::Rect(
                0,
                0,
                ct.width()-ct.offx,
                ct.height()-ct.offy
                ),
            ct.offx,
            ct.offy,
            ct.width()-ct.offx,
            ct.height()-ct.offy,
            Gdiplus::UnitPixel
            );
        EndPaint(hwnd,&ps);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        if(LOWORD(wParam)&MK_CONTROL)
        {
            if(!ct.img)break;
            RECT rect;
            GetProperRect(rect);
            POINT mouse;
            GetCursorPos(&mouse);
            if(mouse.x>=rect.right||mouse.y>=rect.bottom)break;
            ScreenToClient(hwnd,&mouse);
            int mousex=ct.scale>=0?mouse.x>>ct.scale<<ct.scale:mouse.x,
                mousey=ct.scale>=0?mouse.y>>ct.scale<<ct.scale:mouse.y,
                lastoffx=ct.offx,
                lastoffy=ct.offy;
            ct.offx=ct.scale>=0?(ct.offx+mousex)>>ct.scale:
                (ct.offx+mousex)<<-ct.scale;
            ct.offy=ct.scale>=0?(ct.offy+mousey)>>ct.scale:
                (ct.offy+mousey)<<-ct.scale;
            int lastscale=ct.scale;
            if(short(HIWORD(wParam))>0)++ct.scale;
            else --ct.scale;
            if(!ScaleImage())
            {
                ct.offx=lastoffx;
                ct.offy=lastoffy;
                ct.scale=lastscale;
                break;
            }
            ct.offx=max((ct.scale>=0?ct.offx<<ct.scale:ct.offx>>-ct.scale)-mousex,0);
            ct.offy=max((ct.scale>=0?ct.offy<<ct.scale:ct.offy>>-ct.scale)-mousey,0);
            if(ct.width()-ct.offx<rect.right-rect.left)ct.offx=0;
            if(ct.height()-ct.offy<rect.bottom-rect.top)ct.offy=0;
            UpdateWindow(hwnd);
            SetWindowTextW(hwnd,(wstringstream()<<setprecision(3)<<fixed
                <<APPNAME<<L" - "<<ct.url<<L" ("<<ct.sc()*100.0f<<L"%) "<<
                ct.img->GetWidth()<<L"×"<<
                ct.img->GetHeight()).str()-cstr);
            RedrawScrollbar();
            RedrawWindow(hwnd,&rect,nullptr,RDW_UPDATENOW|RDW_INVALIDATE);
            UpdateTooltip();
        }
        else 
            if((short(HIWORD(wParam))>0))PostMessageW(hwnd,WM_VSCROLL,SB_LINEUP,
                (LPARAM)ct.hver);
            else PostMessageW(hwnd,WM_VSCROLL,SB_LINEDOWN,
                (LPARAM)ct.hver);
        break;
    }
    case WM_DROPFILES:
    {
        drophandle dh(wParam);
        ct.reset(dh.wstr(0));
        break;
    }
    case WM_MOUSEMOVE:
    {
        TRACKMOUSEEVENT te={sizeof(te)};
        te.cbSize=sizeof(te);
        te.dwFlags=TME_LEAVE;
        te.hwndTrack=hwnd;
        _TrackMouseEvent(&te);
        ShowWindow(ct.htip,SW_SHOWDEFAULT);
        UpdateTooltip();
        break;
    }
    case WM_MOUSELEAVE:
    {
        ShowWindow(ct.htip,SW_HIDE);
        break;
    }
    case WM_SIZE:
    {
        RedrawScrollbar();
        break;
    }
    case WM_HSCROLL:
    {
        RECT rect;
        GetProperRect(rect);
        if(rect.right-rect.left+ct.offx>=ct.width())break;
        int xnew;
        int xdelta;
        int linesc=ct.scale>0?ct.sc():1;
        switch(LOWORD(wParam))
        {
            #define HSC(f,a) case f:{xnew=ct.offx+(a);break;}
            HSC(SB_PAGEUP,-50)
            HSC(SB_PAGEDOWN,50)
            HSC(SB_LINEUP,0-linesc)
            HSC(SB_LINEDOWN,linesc)
            #undef HSC
            case SB_THUMBPOSITION:
                xnew=HIWORD(wParam);
                break;
            case SB_THUMBTRACK:
                xnew=HIWORD(wParam);
                break;
            default:
                xnew=ct.offx;
        }
        xnew=max(xnew,0);
        xnew=min(ct.width()-rect.right+rect.left,xnew);
        if(ct.scale>0)
        {
            int u=ct.sc();
            int dl=xnew&((1<<ct.scale)-1);
            if(dl<(u>>1))xnew=xnew>>ct.scale<<ct.scale;
            else xnew=((xnew>>ct.scale)+1)<<ct.scale;
        }
        xdelta=xnew-ct.offx;
        ct.offx=xnew;
        if(!ScrollWindowEx(hwnd,-xdelta,0,&rect,nullptr,nullptr,nullptr,
            SW_INVALIDATE|SW_ERASE))
            throw runtime_error(lasterror());
        RedrawScrollbar();
        GetWindowRect(ct.hhor,&rect);
        PostMessageW(ct.hver,WM_MOUSEMOVE,0,0);
        UpdateTooltip();
        break;
    }
    case WM_VSCROLL:
    {
        RECT rect;
        GetProperRect(rect);
        if(rect.bottom-rect.top+ct.offy>=ct.height())break;
        int ynew;
        int ydelta;
        int linesc=ct.scale>0?ct.sc():1;
        switch(LOWORD(wParam))
        {
            #define VSC(f,a) case f:{ynew=ct.offy+(a);break;}
            VSC(SB_PAGEUP,-50)
            VSC(SB_PAGEDOWN,50)
            VSC(SB_LINEUP,0-linesc)
            VSC(SB_LINEDOWN,linesc)
            #undef VSC
            case SB_THUMBPOSITION:
                ynew=HIWORD(wParam);
                break;
            case SB_THUMBTRACK:
                ynew=HIWORD(wParam);
                break;
            default:
                ynew=ct.offy;
        }
        ynew=max(ynew,0);
        ynew=min(ct.height()-rect.bottom+rect.top,ynew);
        if(ct.scale>0)
        {
            int u=ct.sc();
            int dl=ynew&((1<<ct.scale)-1);
            if(dl<(u>>1))ynew=ynew>>ct.scale<<ct.scale;
            else ynew=((ynew>>ct.scale)+1)<<ct.scale;
        }
        ydelta=ynew-ct.offy;
        ct.offy=ynew;
        if(!ScrollWindowEx(hwnd,0,-ydelta,&rect,nullptr,nullptr,nullptr,
            SW_INVALIDATE|SW_ERASE))
            throw runtime_error(lasterror());
        RedrawScrollbar();
        PostMessageW(ct.hhor,WM_MOUSEMOVE,0,0);
        UpdateTooltip();
        break;
    }
    case WM_KEYUP:
    {
        switch(wParam)
        {
            case VK_LEFT:
                PostMessageW(hwnd,WM_HSCROLL,SB_LINEUP,(LPARAM)ct.hhor);
                break;
            case VK_RIGHT:
                PostMessageW(hwnd,WM_HSCROLL,SB_LINEDOWN,(LPARAM)ct.hhor);
                break;
            case VK_UP:
                PostMessageW(hwnd,WM_VSCROLL,SB_LINEUP,(LPARAM)ct.hhor);
                break;
            case VK_DOWN:
                PostMessageW(hwnd,WM_VSCROLL,SB_LINEDOWN,(LPARAM)ct.hhor);
                break;
        }
        break;
    }
    YGP_END_MSG_MAP
    }
    catch(exception& e)
    {
        MessageBoxA(hwnd,e.what(),MBAPPNAME" - Error",MB_ICONERROR);
    }
    return 0;
}
LRESULT CALLBACK TooltipWndProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
    YGP_BEGIN_MSG_MAP
    case WM_CREATE:
    {
        ct.hsta=CreateWindowExW(0,L"static",L"",WS_CHILD|WS_VISIBLE,0,0,TIPSIZE,
            hwnd,nullptr,currentmodule(),nullptr);
        LOGFONTW font;
        zmem(font);
        font.lfHeight=20;
        wcscpy(font.lfFaceName,L"Comic Sans MS");
        SendMessageW(ct.hsta,WM_SETFONT,(WPARAM)CreateFontIndirectW(&font),0);
        break;
    }
    case WM_MOUSEMOVE:
    {
        SetWindowLongA(ct.htip,GWL_STYLE,
            GetWindowLongA(ct.htip,GWL_STYLE)|WS_VISIBLE);
        POINT pt;
        GetCursorPos(&pt);
        MoveWindow(ct.htip,pt.x+10,pt.y+10,TIPSIZE,TRUE);
        if(ct.strtip!=L"Drop an image here")SetWindowTextW(ct.hsta,L"");
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc=BeginPaint(hwnd,&ps);
        RECT rect;
        GetClientRect(hwnd,&rect);
        FillRect(hdc,&rect,CreateSolidBrush(TIPCOLOR));
        EndPaint(hwnd,&ps);
    }
    case WM_SETTIP:
    {
        SetWindowTextW(ct.hsta,ct.strtip-cstr);
        break;
    }
    case WM_KEYUP:
    {
        switch(wParam)
        {
            case VK_LEFT:
                PostMessageW(ct.hwnd,WM_HSCROLL,SB_LINEUP,(LPARAM)ct.hhor);
                break;
            case VK_RIGHT:
                PostMessageW(ct.hwnd,WM_HSCROLL,SB_LINEDOWN,(LPARAM)ct.hhor);
                break;
            case VK_UP:
                PostMessageW(ct.hwnd,WM_VSCROLL,SB_LINEUP,(LPARAM)ct.hhor);
                break;
            case VK_DOWN:
                PostMessageW(ct.hwnd,WM_VSCROLL,SB_LINEDOWN,(LPARAM)ct.hhor);
                break;
        }
        break;
    }
    YGP_END_MSG_MAP
    return 0;
}
YGP_WWINMAIN
{
    SetProcessDPIAware();
    if(*lpCmdLine)
    if(!wfile::exists(lpCmdLine))
        msgboxw()
            .title(APPNAME)
            .style(MB_ICONERROR)
            (lpCmdLine,L": No such file or directory.");
    else ct.url=lpCmdLine;
    using namespace Gdiplus;
    ULONG_PTR upGdiToken;
    GdiplusStartupInput gdiplusstartupinput;
    GdiplusStartup(&upGdiToken,&gdiplusstartupinput,nullptr);
    registerclass(MBAPPNAME,WndProc);
    registerclass("ZRY Tooltip Control",TooltipWndProc);
    ct.hwnd=CreateWindowExW(WS_EX_CLIENTEDGE|WS_EX_ACCEPTFILES,
        APPNAME,APPNAME,WS_OVERLAPPEDWINDOW|WS_VISIBLE,
        CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
        CW_USEDEFAULT,nullptr,nullptr,currentmodule(),nullptr);
    ct.hhor=CreateWindowExW(0,L"SCROLLBAR",nullptr,WS_CHILD|WS_VISIBLE|
        SBS_HORZ,0,0,0,0,ct.hwnd,nullptr,currentmodule(),nullptr);
    ct.hver=CreateWindowExW(0,L"SCROLLBAR",nullptr,WS_CHILD|WS_VISIBLE|
        SBS_VERT,0,0,0,0,ct.hwnd,nullptr,currentmodule(),nullptr);
    if(ct.url!=L"")RedrawScrollbar();
    POINT pt;
    GetCursorPos(&pt);
    ct.htip=CreateWindowExW(WS_EX_TOPMOST|WS_EX_NOACTIVATE|WS_EX_CLIENTEDGE,
        L"ZRY Tooltip Control",nullptr,
        WS_POPUP|WS_VISIBLE,pt.x,pt.y,TIPSIZE,
        nullptr,nullptr,currentmodule(),nullptr);
    return messageloop();
}