#define YGP_DISABLE_DEMENTOR
#define YGP_DISABLE_BROOM
#define YGP_DISABLE_PLAYER
#define APPNAME "YGP Taskbar Manager"
#include"../include/ygp.hpp"
#pragma YGP_LIB(comctl32)
#pragma YGP_MANIFEST
YGP_INIT
HWND GetLastVisibleActivePopUpOfWindow(HWND hwin)
{
    HWND lastpopup=GetLastActivePopup(hwin);
    if(IsWindowVisible(lastpopup))return lastpopup;
    else if(lastpopup==hwin)return nullptr;
    return GetLastVisibleActivePopUpOfWindow(hwin);
}
bool KeepWindowHandleInAltTabList(HWND& hwin)
{
    if(hwin==GetShellWindow())return false;
    HWND root=GetAncestor(hwin,GA_ROOTOWNER);
    if(GetLastVisibleActivePopUpOfWindow(root)==hwin)
    {
        char* classname=new char[10240];
        GetClassNameA(hwin,classname,10240);
        string cls(classname);
        if(cls=="Shell_TrayWnd"
            ||cls=="DV2ControlHost"
            ||((!strcmpi(classname,"Button"))
                &&(!strcmp(getwindowtext<char>(hwin).get(),"Start")))
            ||cls=="MsgrIMEWindowClass"
            ||cls=="SysShadow"
            ||cls._Starts_with("WMP9MediaBarFlyout")
            ||GetWindowLongA(hwin,GWL_STYLE)&WS_POPUP)
        {
            delete []classname;
            return true;
        }
        delete []classname;
        return false;
    }
    return true;
}
HWND hwnd;
HWND hlst;
list<HWND> lst;
taskbarlist* tbl;
HMENU hm;
bool bsm=false;
void UpdateListViewContents()
{
    ListView_DeleteAllItems(hlst);
    lst.clear();
    LVCOLUMNA clm;
    zmem(clm);
    clm.mask=LVCF_WIDTH;
    clm.cx=10000;
    ListView_InsertColumn(hlst,0,&clm);
    enumwindows(lst);
    lst.remove_if(KeepWindowHandleInAltTabList);
    int idx=0;
    for(auto iter=lst.begin();iter!=lst.end();++iter)
    {
        auto& i=*iter;
        auto t=getwindowtext<char>(i);
        char g[1024];
        GetClassNameA(i,g,1024);
        if(!strcmp(g,"Progman"))
        {
            lst.erase(iter);
            continue;
        }
        LVITEMA lvi;
        zmem(lvi);
        lvi.pszText=t.get();
        lvi.mask=LVIF_TEXT|LVIF_STATE;
        lvi.iItem=idx;
        ListView_InsertItem(hlst,&lvi);
        ListView_SetCheckState(hlst,idx,TRUE);
        ++idx;
    }
    SetWindowTextA(hwnd,(APPNAME+" - "s+lst.size()+" Windows")-cstr);
}
LRESULT NotifyHandler(HWND hwnd,int id,NMHDR* pnm)
{
    switch(pnm->code)
    {
        case LVN_ITEMCHANGING:
        {
            LPNMLISTVIEW pnmlv=CONTAINING_RECORD(pnm,NMLISTVIEW,hdr);
            if(pnmlv->uChanged&LVIF_STATE)
            {
                auto it=lst.begin();
                for(int i=0;i<pnmlv->iItem;++i)++it;
                if(pnmlv->uOldState==4096&&pnmlv->uNewState==8192)
                {
                    tbl->addtab(*it);
                    if(bsm)ShowWindow(*it,SW_RESTORE);
                }
                else if(pnmlv->uOldState==8192&&pnmlv->uNewState==4096)
                {
                    tbl->deletetab(*it);
                    if(bsm)ShowWindow(*it,SW_MINIMIZE);
                }
            }
            break;
        }
    }
    return 0;
}
YGP_WNDPROC
{
    YGP_BEGIN_MSG_MAP
    case WM_CREATE:
    {
        ::hwnd=hwnd;
        SendMessageA(hwnd,WM_SETICON,ICON_SMALL,
            (LPARAM)LoadIconA(currentmodule(),MAKEINTRESOURCEA(1)));
        RECT rect;
        GetClientRect(hwnd,&rect);
        hlst=CreateWindowA(WC_LISTVIEWA,
            nullptr,WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT,0,0,
            rect.right-rect.left,rect.bottom-rect.top,hwnd,(HMENU)1,
            currentmodule(),nullptr);
        ListView_SetExtendedListViewStyle(hlst,
            LVS_EX_CHECKBOXES|LVS_EX_GRIDLINES);
        UpdateListViewContents();
        hm=CreateMenu();
        AppendMenuA(hm,MF_STRING,2,"Update");
        AppendMenuA(hm,MF_STRING,3,"Minimize or Restore Window Simultaneously");
        SetMenu(hwnd,hm);
        break;
    }
    case WM_SIZE:
    {
        RECT rect;
        GetClientRect(hwnd,&rect);
        MoveWindow(hlst,0,0,rect.right-rect.left,
            rect.bottom-rect.top,TRUE);
        break;
    }
    case WM_COMMAND:
    {
        switch(wParam)
        {
            case 2:
            {
                UpdateListViewContents();
                break;
            }
            case 3:
            {
                if(!bsm)ModifyMenuA(hm,3,MF_BYCOMMAND,3,
                    "Do Not Minimize or Restore Window Simultaneously"),
                    bsm=true;
                else ModifyMenuA(hm,3,MF_BYCOMMAND,3,
                    "Minimize or Restore Window Simultaneously"),
                    bsm=false;
                DrawMenuBar(hwnd);
                break;
            }
        }
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        delete tbl;
        com_uninit();
        break;
    }
    HANDLE_MSG(hwnd,WM_NOTIFY,NotifyHandler);
    YGP_END_MSG_MAP
    return 0;
}
YGP_WINMAIN
{
    YGP_TRY
    SetProcessDPIAware();
    InitCommonControls();
    com_init();
    tbl=new taskbarlist;
    registerclass(APPNAME,WndProc);
    hwnd=CreateWindowExA(0,APPNAME,APPNAME,
        WS_VISIBLE|WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,
        CW_USEDEFAULT,CW_USEDEFAULT,nullptr,nullptr,currentmodule(),
        nullptr);
    return messageloop();
    YGP_CATCH_MSGBOX
    return -1;
}