#ifndef _YGP_WIN_IO_HPP_
#define _YGP_WIN_IO_HPP_
#include"winerr.hpp"
#include"sync.hpp"
#include<memory>
BEGIN_NAMESPACE_YGP
static int pts(const char* buf)
{
    return fputs(buf,stdout);
}
static DWORD ptws(const wchar_t* wcs,std::size_t sz=0,HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE))//put wide character string
{
    if(!sz)sz=wcslen(wcs);
    if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::ptws (function GetStdHandle): "+lasterror<std::string>());
    DWORD dwret;
    WriteConsoleW(h,wcs,sz,&dwret,nullptr);
    return dwret;
}
static DWORD ptwse(const wchar_t* wcs,std::size_t sz=0,HANDLE h=GetStdHandle(STD_ERROR_HANDLE))
{
    if(!sz)sz=wcslen(wcs);
    if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::ptwse (function GetStdHandle): "+lasterror<std::string>());
    DWORD dwret;
    WriteConsoleW(h,wcs,sz,&dwret,nullptr);
    return dwret;
}
static DWORD ptws(std::shared_ptr<wchar_t>& wcs,std::size_t sz=0,HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE))//put wide character string
{
    if(!sz)sz=wcslen(wcs.get());
    if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::ptws (function GetStdHandle): "+lasterror<std::string>());
    DWORD dwret;
    WriteConsoleW(h,wcs.get(),sz,&dwret,nullptr);
    return dwret;
}
static DWORD ptwse(std::shared_ptr<wchar_t>&& wcs,std::size_t sz=0,HANDLE h=GetStdHandle(STD_ERROR_HANDLE))
{
    if(!sz)sz=wcslen(wcs.get());
    if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::ptwse (function GetStdHandle): "+lasterror<std::string>());
    DWORD dwret;
    WriteConsoleW(h,wcs.get(),sz,&dwret,nullptr);
    return dwret;
}
static DWORD ptws(std::unique_ptr<wchar_t>& wcs,std::size_t sz=0,HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE))//put wide character string
{
    if(!sz)sz=wcslen(wcs.get());
    if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::ptws (function GetStdHandle): "+lasterror<std::string>());
    DWORD dwret;
    WriteConsoleW(h,wcs.get(),sz,&dwret,nullptr);
    return dwret;
}
static DWORD ptwse(std::unique_ptr<wchar_t>&& wcs,std::size_t sz=0,HANDLE h=GetStdHandle(STD_ERROR_HANDLE))
{
    if(!sz)sz=wcslen(wcs.get());
    if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::ptwse (function GetStdHandle): "+lasterror<std::string>());
    DWORD dwret;
    WriteConsoleW(h,wcs.get(),sz,&dwret,nullptr);
    return dwret;
}
static void ptwc(wchar_t w,HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE))//put a wide character
{
    if(ptws(&w,1,h)!=1)throw std::runtime_error("ygp::ptwc (function ygp::ptws): "+lasterror<std::string>());
}
static void ptwce(wchar_t w,HANDLE h=GetStdHandle(STD_ERROR_HANDLE))
{
    if(ptwse(&w,1,h)!=1)throw std::runtime_error("ygp::ptwce (function ygp::ptwse): "+lasterror<std::string>());
}
static void crlf(HANDLE h=GetStdHandle(STD_ERROR_HANDLE)){ptws(L"\r\n",2,h);}
static DWORD gtws(wchar_t* wcs,std::size_t len,HANDLE h=GetStdHandle(STD_INPUT_HANDLE))//get a wide character string
{
    if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::gtws (function GetStdHandle): "+lasterror<std::string>());
    DWORD dwret;
    ReadConsoleW(h,wcs,len,&dwret,nullptr);
    return dwret;
}
static wchar_t gtwc(HANDLE h=GetStdHandle(STD_INPUT_HANDLE))//get a wide character
{
    wchar_t wcret;
    if(gtws(&wcret,1,h)!=1)throw std::runtime_error("ygp::gtwc (function GetStdHandle): "+lasterror<std::string>());
    return wcret;
}
class wfile//Windows file
{
    HANDLE h;
    public:
        operator HANDLE()const{return h;}
        HANDLE handle()const{return h;}
        std::uint64_t size()
        {
            ULONG ulow,uhigh;
            ulow=GetFileSize(h,&uhigh);
            return std::uint64_t(ulow)|(std::uint64_t(uhigh)<<32);
            
        }
        //<dw1>
        static constexpr DWORD in=0x80000000L;
        static constexpr DWORD out=0x40000000L;
        //</dw1>
        //<dw2>
		static constexpr DWORD share_read=0x00000001;
		static constexpr DWORD share_write=0x00000002;
		static constexpr DWORD share_delete=0x00000004;
        //</dw2>
        //<dw3>
		static constexpr DWORD create_new=1;
		static constexpr DWORD create_always=2;
		static constexpr DWORD open_existing=3;
		static constexpr DWORD open_always=4;
		static constexpr DWORD truncate=5;
        //</dw3>
        //<seek>
        static constexpr DWORD seek_begin=0;
        static constexpr DWORD seek_current=1;
        static constexpr DWORD seek_end=2;
        //</seek>
        wfile():h(nullptr){}
        wfile& open(const char* filename,DWORD dw1=in|out,DWORD dw2=share_read|share_write,DWORD dw3=open_always)
        {
            h=CreateFileA(filename,dw1,dw2,nullptr,dw3,0l,0);
            if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::wfile::open (function CreateFileA): "+lasterror<std::string>());
            return *this;
        }
        wfile& open(const wchar_t* filename,DWORD dw1=in|out,DWORD dw2=share_read|share_write,DWORD dw3=open_always)
        {
            h=CreateFileW(filename,dw1,dw2,nullptr,dw3,0l,0);
            if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::wfile::open (function CreateFileW): "+lasterror<std::string>());
            return *this;
        }
        wfile(const char* filename,DWORD dw1=in|out,DWORD dw2=share_read|share_write,DWORD dw3=open_always)
        {
            open(filename,dw1,dw2,dw3);
        }
        wfile(const wchar_t* filename,DWORD dw1=in|out,DWORD dw2=share_read|share_write,DWORD dw3=open_always)
        {
            open(filename,dw1,dw2,dw3);
        }
        wfile(HANDLE H){h=H;}
        ~wfile(){if(h)CloseHandle(h);}
        static DWORD attr(const char* filename)
        {
            return GetFileAttributesA(filename);
        }
        static DWORD attr(const wchar_t* filename)
        {
            return GetFileAttributesW(filename);
        }
        DWORD type(){return GetFileType(h);}
        static bool exists(const char* filename)
        {
            WIN32_FIND_DATAA finddata;
            HANDLE hfile=FindFirstFileA(filename,&finddata);
            bool ret=hfile!=INVALID_HANDLE_VALUE;
            if(!ret)FindClose(hfile);
            return ret;
        }
        static bool exists(const wchar_t* filename)
        {
            WIN32_FIND_DATAW finddata;
            HANDLE hfile=FindFirstFileW(filename,&finddata);
            bool ret=hfile!=INVALID_HANDLE_VALUE;
            if(!ret)FindClose(hfile);
            return ret;
        }
        static void del(const char* filename)//delete a file
        {
            if(!DeleteFileA(filename))throw std::runtime_error("ygp::wfile::del (function DeleteFileA): "+lasterror<std::string>());
        }
        static void del(const wchar_t* filename)
        {
            if(!DeleteFileW(filename))throw std::runtime_error("ygp::wfile::del (function DeleteFileW): "+lasterror<std::string>());
        }
        DWORD read(void* buf,DWORD toread)
        {
            DWORD dwret;
            if(!ReadFile(h,buf,toread,&dwret,nullptr))throw std::runtime_error("ygp::wfile::read (function ReadFile): "+ygp::lasterror<std::string>());
            return dwret;
        }
        DWORD read(void* buf,DWORD toread,event&& ev)
        {
            DWORD dwret;
            OVERLAPPED ol={0};
            ol.hEvent=ev.handle();
            if(!ReadFile(h,buf,toread,&dwret,&ol))throw std::runtime_error("ygp::wfile::read (function ReadFile): "+lasterror<std::string>());
            return dwret;
        }
        template<typename _Tp>
        _Tp readt()
        {
            _Tp _t;
            read(_t,sizeof(_Tp));
            return _t;
        }
        DWORD write(const void* buf,DWORD towrite)
        {
            DWORD dwret;
            if(!WriteFile(h,buf,towrite,&dwret,nullptr))throw std::runtime_error("ygp::wfile::write (function WriteFile): "+lasterror<std::string>());
            return dwret;
        }
        DWORD write(const void* buf,DWORD towrite,event&& ev)
        {
            DWORD dwret;
            OVERLAPPED ol={0};
            ol.hEvent=ev.handle();
            if(!WriteFile(h,buf,towrite,&dwret,&ol))throw std::runtime_error("ygp::wfile::write (function WriteFile): "+lasterror<std::string>());
            return dwret;
        }
        template<typename _Tp>
        void writet()
        {
            _Tp _t;
            write(_t,sizeof(_Tp));
        }
        void writestr(const char* _str)
        {
            write(_str,strlen(_str));
        }
        void writewcs(const wchar_t* _wcs)
        {
            write(_wcs,sizeof(wchar_t)*wcslen(_wcs));
        }
        std::uint64_t tell()//current file pointer position
        {
            return this->seek(0,seek_current);
        }
        std::uint64_t seek(std::int64_t skpnt,DWORD mvtp=seek_begin)
        {
            LARGE_INTEGER li={0};
            li.QuadPart=skpnt;
            li.LowPart=SetFilePointer(h,li.LowPart,&li.HighPart,mvtp);
            if(li.LowPart==INVALID_SET_FILE_POINTER)throw std::runtime_error("ygp::wfile::seek (function SetFilePointer): "+lasterror<std::string>());
            return li.QuadPart;
        }
        std::string readline()
        {
            char* _s=new char[1024];
            std::string ret;
            DWORD dwRead;
            while(dwRead=this->read(_s,1024))
            {
                char* _tmp=(char*)memchr(_s,'\n',dwRead);
                if(_tmp)*_tmp='\0';
                ret+=_s;
                if(_tmp)
                {
                    seek(_tmp-_s-(LONG)dwRead+1l,seek_current);
                    break;
                }
            }
            delete []_s;
            return ret;
        }
        std::uint64_t toend()
        {
            return seek(0,seek_end);
        }
        void close()
        {
            if(h)CloseHandle(h);
            h=nullptr;
        }
};
END_NAMESPACE_YGP
#endif