#define YGP_DISABLE_DEMENTOR
#define YGP_DISABLE_BROOM
#define YGP_DISABLE_PLAYER
#define YGP_DISABLE_WEBBROWSER
#include"../include/ygp.hpp"
#include"../include/ygp/win/gui/button.hpp"
#pragma YGP_MANIFEST
YGP_INIT
window e[10][10];
int a[10][10];
frame win;
thread* th=nullptr;
button* pbtn;
font ft;
bool dfs(int num)
{
    if(num>81)return true;
    int x=num%9;
    if(!x)x=9;
    int y=(num-x)/9+1;
    if(!a[x][y])
    {
        bool f[10];
        zmem(f);
        for(int i=1;i<10;++i)
        {
            f[a[i][y]]=true;
        }
        for(int j=1;j<10;++j)
        {
            f[a[x][j]]=true;
        }
        int u=((x-1)/3)*3+1,v=((y-1)/3)*3+1;
        for(int i=0;i<3;++i)
            for(int j=0;j<3;++j)
                f[a[i+u][j+v]]=true;
        for(int i=1;i<10;++i)
        {
            if(!f[i])
            {
                a[x][y]=i;
                if(dfs(num+1))return true;
            }
        }
        a[x][y]=0;
        return false;
    }
    return dfs(num+1);
}
void threadproc()
{
    if(dfs(1))
    for(int i=1;i<10;++i)
        for(int j=1;j<10;++j)
            e[i][j].text(to_string(a[i][j]));
    else msgboxa().title("YGP Sudoku Solver").style(MB_ICONINFORMATION)
        .hwnd(win)("Failed.");
    win.win.text("YGP Sudoku Solver");
    pbtn->enable();
}
void solve()
{
    for(int i=1;i<10;++i)
        for(int j=1;j<10;++j)
        {
            char str[2];
            GetWindowTextA(e[i][j],str,2);
            if(str[0]=='0')
            {
                msgboxa().title("YGP Sudoku Solver").style(MB_ICONERROR)
                    .hwnd(win)("Invalid input value: 0.");
                e[i][j].text("");
                return;
            }
            a[i][j]=atoi(str);
        }
    for(int i=1;i<10;++i)
    {
        bool f[10];
        zmem(f);
        for(int j=1;j<10;++j)
        {
            if(!a[i][j])continue;
            if(f[a[i][j]])
            {
                msgboxa().title("YGP Sudoku Solver").style(MB_ICONERROR)
                    .hwnd(win)(strfmt("Repitition detected at row %d"
                    " (value: %d).",i,a[i][j]));
                return;
            }
            else f[a[i][j]]=true;
        }
    }
    for(int i=1;i<10;++i)
    {
        bool f[10];
        zmem(f);
        for(int j=1;j<10;++j)
        {
            if(!a[j][i])continue;
            if(f[a[j][i]])
            {
                msgboxa().title("YGP Sudoku Solver").style(MB_ICONERROR)
                    .hwnd(win)(strfmt("Repitition detected at column %d"
                    " (value: %d).",i,a[j][i]));
                return;
            }
            else f[a[j][i]]=true;
        }
    }
    for(int x=0;x<3;++x)
        for(int y=0;y<3;++y)
        {
            bool f[10];
            zmem(f);
            for(int i=1;i<4;++i)
                for(int j=1;j<4;++j)
                {
                    if(!a[x*3+i][y*3+j])continue;
                    if(f[a[x*3+i][y*3+j]])
                    {
                        msgboxa().title("YGP Sudoku Solver").style(MB_ICONERROR)
                            .hwnd(win)(strfmt("Repitition detected at tile"
                            " (row %d, column %d) (value: %d).",y+1,x+1,a[x*3+i][y*3+j]));
                        return;
                    }
                    else f[a[x*3+i][y*3+j]]=true;
                }
        }
    pbtn->disable();
    win.win.text("YGP Sudoku Solver - Calculating...");
    if(th)delete th;
    th=new thread(threadproc);
}
YGP_WINMAIN
{
    YGP_TRY
    SetProcessDPIAware();
    ft.name("Segoe Script").height(55).create();
    frame::initializer().name("YGP Sudoku Solver").create(win);
    for(int i=1;i<10;++i)
        for(int j=1;j<10;++j)
            e[i][j].create(win,"edit","",(i<<4)|j,WS_CHILD|WS_VISIBLE|WS_BORDER|
                ES_CENTER|ES_NUMBER,0,i*60+(i-1)/3*10,
                j*60+(j-1)/3*10,60,60),
            e[i][j].sendmsga(EM_SETLIMITTEXT,1),
            e[i][j].setfont(ft);
    button btn(win,"Solve",250,640,180,40);
    ft.del();
    ft.name("ËÎÌו").height(0).create();
    btn.setfont(ft);
    pbtn=&btn;
    btn.onclick(solve);
    win.mhd.pwmcommandh->map[++win.idlist]=
        detail::WM_COMMAND_handler::func([&]()
        {
            for(int i=1;i<10;++i)
                for(int j=1;j<10;++j)
                    e[i][j].text("");
        });
    HMENU hm=CreateMenu();
    AppendMenuA(hm,MF_STRING,win.idlist,"Clear");
    win.win.setmenu(hm);
    messageloop([&](HWND,UINT Msg,WPARAM wParam,LPARAM)
    {
        if(Msg==WM_KEYUP)
        {
            window hFocus=window::getfocus();
            LONG_PTR id=hFocus.id();
            int x=(id&0xf0)>>4;
            int y=id&0xf;
            if(!x||x>9||!y||y>9)return;
            switch(wParam)
            {
                case VK_LEFT:
                    x=max(1,x-1);break;
                case VK_RIGHT:
                    x=min(9,x+1);break;
                case VK_UP:
                    y=max(1,y-1);break;
                case VK_DOWN:
                    y=min(9,y+1);break;
                case VK_RETURN:
                    solve();return;
                default:return;
            }
            e[x][y].focus();
        }
    });
    YGP_CATCH_MSGBOX
    return 0;
}