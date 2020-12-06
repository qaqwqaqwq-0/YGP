#include<windows.h>
#include<random>
#include<list>
#include"../include/ygp.hpp"
YGP_INIT
#define ms(a) MessageBoxA(0,a,0,0)
#define mse(a) MessageBoxA(0,a.c_str(),0,0)
constexpr int per=2;
struct obj:RECT
{
	HDC hdc;
	HBRUSH blc,wht,rdb;
	bool b;
	bool f;
	bool lft;
	bool rgh;
	obj& m(LONG l1,LONG l2,LONG l3,LONG l4)
	{
		left=l1,top=l2,right=l3,bottom=l4;
		return *this;
	}
	obj& v(LONG l)
	{
		c();
		left+=l*20;
		right=left+40;
		if(left>=580){left=580,right=620;d();return *this;}
		if(left<=0){left=0,right=40;d();return *this;}
		d();
		return *this;
	}
	RECT r(){return (RECT)*this;}
	obj& d()
	{
		RECT rect=(RECT)*this;
		FillRect(hdc,&rect,rdb);
		return *this;
	}
	obj& c()
	{
		RECT rect=(RECT)*this;
		FillRect(hdc,&rect,wht);
		return *this;
	}
}o;
mt19937 gen;
list<array<RECT,per>> q;
LONG w[per];//coordinate
ULONGLONG u;
LONG dth=0;
HWND win;
bool intersect(const RECT& r1,const RECT r2)
{
	RECT rect;
	return (bool)IntersectRect(&rect,&r1,&r2);
}
void check()
{
	auto it=q.begin();
	if(q.size()<7)return;
	for(auto& i:rng(q.size()-7))++it;
	for(auto& i:*it)
	{
		if(intersect(i,o.r()))
		{
			++dth;
			char buf[100];
			snprintf(buf,100,"%d (total 10) %.2f seconds",dth,((long double)u)/200);
			SetWindowTextA(win,buf);
			if(dth==10)
			{
				MessageBeep(MB_ICONERROR);
				MessageBoxA(win,"Game Over!","Game",MB_OK);
				ExitProcess(1u);
			}
			MessageBeep(MB_ICONEXCLAMATION);
			break;
		}
	}
}
void proc(HWND hwnd)
{
	gen.seed(time(nullptr));
	HDC hdc;
	PAINTSTRUCT ps;
	hdc=BeginPaint(hwnd,&ps);
	o.hdc=hdc;
	o.blc=CreateSolidBrush(0x000000);
	o.wht=CreateSolidBrush(0xffffff);
	o.rdb=CreateSolidBrush(0x00ff00);
	o.b=true;
	o.m(280,160,320,200);
	o.d();
	while(!o.f)
	{
		if(o.lft)
		{
			o.lft=false;
			o.v(-1);
			check();
		}
		if(o.rgh)
		{
			o.rgh=false;
			o.v(1);
			check();
		}
		if(!(u%100))
		{
			if(q.size()>=11)
			{
				auto& h=q.front();
				for(auto& j:h)
				{
					FillRect(hdc,&j,o.wht);
				}
				q.pop_front();
			}
			for(auto& i:q)
			{
				for(auto& j:i)
				{
					FillRect(hdc,&j,o.wht);
					j.top-=40;
					j.bottom-=40;
					FillRect(hdc,&j,o.blc);
				}
			}
			for(auto& i:w)i=gen()%600;
			q.push_back(array<RECT,per>());
			for(const auto& i:rng(0,per))q.back()[i]={w[i],400,w[i]+40,440},FillRect(hdc,&q.back()[i],o.blc);
			o.d();
			check();
		}
		Sleep(5);
		char buf[100];
		snprintf(buf,100,"%d (total 10) %.2f seconds",dth,((long double)u)/200);
		SetWindowTextA(win,buf);
		++u;
	}
	EndPaint(hwnd,&ps);
}
LRESULT CALLBACK WndProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
	static HWND hbutton;
	switch(Message)
	{
		case WM_CREATE:
		{
			hbutton=CreateWindowA("button","Start",WS_CHILD|WS_VISIBLE|WS_BORDER,0,0,640,480,hwnd,(HMENU)1,((LPCREATESTRUCT)lParam)->hInstance,nullptr);
			o.b=o.f=o.lft=o.rgh=false;
			break;
		}
		case WM_DESTROY:
		{
			o.f=true;
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND:
		{
			switch(wParam)
			{
				case 1:
				{
					DestroyWindow(hbutton);
					thread(proc,hwnd);
					break;
				}
			}
			break;
		}
		case WM_KEYDOWN:
		{
			if(!o.b)break;
			switch(wParam)
			{
				case VK_LEFT:
				{
					o.lft=true;
					break;
				}
				case VK_RIGHT:
				{
					o.rgh=true;
					break;
				}
			}
			break;
		}
		default:
			return DefWindowProc(hwnd,Message,wParam,lParam);
	}
	return 0;
}
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;
	memset(&wc,0,sizeof(wc));
	wc.cbSize=sizeof(WNDCLASSEX);
	wc.lpfnWndProc=WndProc;
	wc.hInstance=hInstance;
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName="WindowClass";
	wc.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wc.hIconSm=LoadIcon(NULL,IDI_APPLICATION);
	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL,"Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	hwnd=CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Game",WS_VISIBLE|WS_OVERLAPPEDWINDOW&~(WS_SIZEBOX),CW_USEDEFAULT,CW_USEDEFAULT,640,480,NULL,NULL,hInstance,NULL);
	if(hwnd==NULL)
	{
		MessageBox(NULL,"Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	win=hwnd;
	while(GetMessage(&msg,NULL,0,0)>0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}