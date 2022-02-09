#ifndef _YGP_WIN_WINERR_HPP_
#define _YGP_WIN_WINERR_HPP_
#if defined(__GNUC__)||defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#endif
#include"../configure.hpp"
#include<string>
#include<stdexcept>
#include<cstdio>
#include<windows.h>
#include<windowsx.h>
BEGIN_NAMESPACE_YGP
template<typename _Tp=std::string>
static _Tp lasterror(DWORD lsterr=GetLastError())
{
	LPSTR lpbuf=nullptr;
	_Tp __h;
	if(!FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,nullptr,lsterr,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPSTR)&lpbuf,0,nullptr))
	{
		char __buf[64];
		std::snprintf(__buf,64,"%u (0x%x): Unknown error.",lsterr,lsterr);
		return __h=__buf;
	}
	else
	{
		__h=lpbuf;
		LocalFree(lpbuf);
	}
	return __h;
}
template<typename _Tp=std::string>
static _Tp thr_lasterror(DWORD lsterr=GetLastError())
{
	LPSTR lpbuf=nullptr;
	_Tp __h;
	if(!FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,nullptr,lsterr,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPSTR)&lpbuf,0,nullptr))
	{
		throw std::invalid_argument("ygp::thr_lasterror failed to format the error code.");
	}
	else
	{
		__h=lpbuf;
		LocalFree(lpbuf);
	}
	return __h;
}
template<typename _Tp>
static void zmem(_Tp& _x)
{
	memset(&_x,0,sizeof(_x));
}
static UINT wexec(LPCSTR __cmd)
{
	return WinExec(__cmd,SW_NORMAL);
}
static void wsleep(DWORD dwml)
{
	Sleep(dwml);
}
static void wpause(const char* msg=nullptr)
{
	if(msg)std::fputs(msg,stderr);
	std::system("pause>nul");
}
#ifndef YGP_DISABLE_COM
#ifdef _MSC_VER
#pragma comment(lib,"ole32")
#pragma comment(lib,"oleaut32")
#pragma comment(lib,"strmiids")
#endif
static void throw_com_error(HRESULT _hr,const std::string& extra1,const char* extra2)
{
	if(FAILED(_hr))
		throw std::runtime_error("ygp::"+extra1+" (function "+extra2+"): "+lasterror<std::string>(_hr));
}
static std::string com_callback_err_str(HRESULT _hr,const std::string& extra1)
{
	return "ygp::"+extra1+" (callback method): "+lasterror<std::string>(_hr);
}
static void com_callback_thr(HRESULT _hr,const std::string& extra1)
{
	if(FAILED(_hr))
		throw std::runtime_error(com_callback_err_str(_hr,extra1));
}
static void com_init()
{
	throw_com_error(CoInitialize(nullptr),"com_init","CoInitialize");
}
template<typename _Tp>
static _Tp* cocrtins(REFIID _clsid,REFIID _riid
#ifdef _MSC_VER
=__uuidof(_Tp)
#endif
,DWORD dwcls=CLSCTX_INPROC_SERVER)
{
	_Tp* ret;
	throw_com_error(CoCreateInstance(_clsid,nullptr,dwcls,_riid,(void**)&ret),"cocrtins","CoCreateInstance");
	return ret;
}
static inline void com_uninit()noexcept
{
	CoUninitialize();
}
#endif
template<typename _Tp=std::string>
static _Tp ext_lasterror(const char* modulename,DWORD dwerror=GetLastError())
{
	LPSTR lpbuf;
	_Tp __h;
	if(!FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_HMODULE|FORMAT_MESSAGE_FROM_SYSTEM,GetModuleHandleA(modulename),dwerror,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),LPSTR(&lpbuf),0,nullptr))
	{
		char __buf[64];
		std::snprintf(__buf,64,"%u (0x%x): Unknown error.",dwerror,dwerror);
		return __h=__buf;
	}
	else
	{
		__h=lpbuf;
		LocalFree(lpbuf);
	}
	return __h;
}
template<typename _Tp=std::string>
static _Tp ext_lasterror_without_system(const char* modulename,DWORD dwerror=GetLastError())
{
	LPSTR lpbuf;
	_Tp __h;
	if(!FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_HMODULE,GetModuleHandleA(modulename),dwerror,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),LPSTR(&lpbuf),0,nullptr))
	{
		char __buf[64];
		std::snprintf(__buf,64,"%u (0x%x): Unknown error.",dwerror,dwerror);
		return __h=__buf;
	}
	else
	{
		__h=lpbuf;
		LocalFree(lpbuf);
	}
	return __h;
}
template<typename _Tp=std::string>
static _Tp thr_ext_lasterror(const char* modulename,DWORD dwerror=GetLastError())
{
	LPSTR lpbuf;
	_Tp __h;
	if(!FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_HMODULE|FORMAT_MESSAGE_FROM_SYSTEM,GetModuleHandleA(modulename),dwerror,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),LPSTR(&lpbuf),0,nullptr))
	{
		throw std::invalid_argument("ygp::thr_ext_lasterror failed to format the error code.");
	}
	else
	{
		__h=lpbuf;
		LocalFree(lpbuf);
	}
	return __h;
}
template<typename _Tp=std::string>
static _Tp thr_ext_lasterror_modulew(const wchar_t* modulename,DWORD dwerror=GetLastError())
{
	LPSTR lpbuf;
	_Tp __h;
	if(!FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_HMODULE|FORMAT_MESSAGE_FROM_SYSTEM,GetModuleHandleW(modulename),dwerror,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),LPSTR(&lpbuf),0,nullptr))
	{
		throw std::invalid_argument("ygp::thr_ext_lasterror failed to format the error code.");
	}
	else
	{
		__h=lpbuf;
		LocalFree(lpbuf);
	}
	return __h;
}
template<typename _Tp=std::string>
static _Tp thr_ext_lasterror_modulew_without_system(const wchar_t* modulename,DWORD dwerror=GetLastError())
{
	LPSTR lpbuf;
	_Tp __h;
	if(!FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_HMODULE,GetModuleHandleW(modulename),dwerror,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),LPSTR(&lpbuf),0,nullptr))
	{
		throw std::invalid_argument("ygp::thr_ext_lasterror failed to format the error code.");
	}
	else
	{
		__h=lpbuf;
		LocalFree(lpbuf);
	}
	return __h;
}
extern std::string ntstatus2string(DWORD);//Export library: ntstatus.lib (see ygp/lib)
static std::string crtlasterror()
{
	char* buf=new char[1024];
	strerror_s(buf,1024,errno);
	std::string ret=buf;
	delete []buf;
	return ret;
}
static std::string __cdecl strfmt(const char* __format,...)
{
	va_list arglist;
	va_start(arglist,__format);
	LPSTR buf=nullptr;
	int nlength=_vscprintf(__format,arglist);
	if(nlength<0)throw std::runtime_error("ygp::strfmt (function _vscprintf): "+crtlasterror());
	buf=new char[nlength+1];
	vsnprintf(buf,1024,__format,arglist);
	va_end(arglist);
	std::string ret=buf;
	delete []buf; 
	return ret;
}
static std::wstring __cdecl strfmt(const wchar_t* __format,...)
{
	va_list arglist;
	va_start(arglist,__format);
	LPWSTR buf=nullptr;
	int nlength=_vscwprintf(__format,arglist);
	if(nlength<0)throw std::runtime_error("ygp::strfmt (function _vscwprintf): "+crtlasterror());
	buf=new wchar_t[nlength+1];
	_vsnwprintf(buf,1024,__format,arglist);
	va_end(arglist);
	std::wstring ret=buf;
	delete []buf; 
	return ret;
}
static int __cdecl cstrfmt(char*& buf,const char* __format,...)
{
	va_list arglist;
	va_start(arglist,__format);
	int nlength=_vscprintf(__format,arglist);
	if(nlength<0)throw std::runtime_error("ygp::strfmt (function _vscprintf): "+crtlasterror());
	buf=new char[nlength+1];
	vsnprintf(buf,1024,__format,arglist);
	va_end(arglist);
	std::string ret=buf;
	return nlength;
}
static int __cdecl cstrfmt(wchar_t*& buf,const wchar_t* __format,...)
{
	va_list arglist;
	va_start(arglist,__format);
	int nlength=_vscwprintf(__format,arglist);
	if(nlength<0)throw std::runtime_error("ygp::strfmt (function _vscwprintf): "+crtlasterror());
	buf=new wchar_t[nlength+1];
	_vsnwprintf(buf,1024,__format,arglist);
	va_end(arglist);
	std::wstring ret=buf;
	return nlength;
}
#define YGP_DEPRECATED
END_NAMESPACE_YGP
#endif