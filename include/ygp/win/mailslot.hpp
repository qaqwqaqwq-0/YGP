#ifndef _YGP_WIN_MAILSLOT_HPP_
#define _YGP_WIN_MAILSLOT_HPP_
#include"winerr.hpp"
#include"sync.hpp"
#include<memory>
BEGIN_NAMESPACE_YGP
class mailslot
{
    private:
        HANDLE h;
    public:
        struct msinfo
        {
            DWORD rstr;//Size restriction
            DWORD cb;//Size of next message
            DWORD c;//Message count
            DWORD tmout;//Time out
        };
        mailslot(const char* name=nullptr,DWORD dwrestriction=0,DWORD dwtmout=MAILSLOT_WAIT_FOREVER)
        {
            h=CreateMailslotA(name,dwrestriction,dwtmout,nullptr);
            if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::mailslot::mailslot (function CreateMailslotA): "+lasterror<std::string>());
        }
        mailslot(HANDLE H)
        {
            if(H==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::mutex received an invalid handle value. Reason: "+lasterror<std::string>());
            h=H;
        }
        ~mailslot(){CloseHandle(h);}
        msinfo info()
        {
            msinfo m;
            if(!GetMailslotInfo(h,&m.rstr,&m.cb,&m.c,&m.tmout))throw std::runtime_error("ygp::mailslot::info (function GetMailslotInfo): "+lasterror<std::string>());
            return m;
        }
        DWORD nxtcb()//Next message size
        {
            DWORD ret=INFINITE;
            if(!GetMailslotInfo(h,nullptr,&ret,nullptr,nullptr))throw std::runtime_error("ygp::mailslot::info (function GetMailslotInfo): "+lasterror<std::string>());
            return ret;
        }
        DWORD count()//Count
        {
            DWORD ret=INFINITE;
            if(!GetMailslotInfo(h,nullptr,nullptr,&ret,nullptr))throw std::runtime_error("ygp::mailslot::info (function GetMailslotInfo): "+lasterror<std::string>());
            if(ret==MAILSLOT_NO_MESSAGE)throw std::runtime_error("The mailslot has no message.");
            return ret;
        }
        static mailslot open(const char* name)
        {
            HANDLE j=CreateFileA(name,GENERIC_WRITE,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);
            if(j==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::mailslot::(static method)open (function CreateFileA): "+lasterror<std::string>());
            return mailslot(j);
        }
        static mailslot* openp(const char* name)
        {
            HANDLE j=CreateFileA(name,GENERIC_ALL,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);
            if(j==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::mailslot::(static method)openp (function CreateFileA): "+lasterror<std::string>());
            return new mailslot(j);
        }
        DWORD write(void* buf,std::size_t sz)
        {
            DWORD dwret;
            if(!WriteFile(h,buf,sz,&dwret,nullptr))throw std::runtime_error("ygp::mailslot::write (function WriteFile): "+lasterror<std::string>());
            return dwret;
        }
        template<typename Tp>
        DWORD write(Tp);
        DWORD read(void* buf,std::size_t sz)
        {
            DWORD dwret;
            if(!ReadFile(h,buf,sz,&dwret,nullptr))throw std::runtime_error("ygp::mailslot::write (function ReadFile): "+lasterror<std::string>());
            return dwret;
        }
        DWORD read(void* buf,std::size_t sz,event& ev)
        {
            OVERLAPPED ov;
            ov.Offset=0;
            ov.OffsetHigh=0;
            ov.hEvent=ev.handle();
            DWORD dwret;
            if(!ReadFile(h,buf,sz,&dwret,&ov))throw std::runtime_error("ygp::mailslot::write (function ReadFile): "+lasterror<std::string>());
            return dwret;
        }
        std::unique_ptr<char,void(*)(char*)> read()
        {
            event ev(nullptr,false,false);
            DWORD nxtsz=this->nxtcb();
            std::unique_ptr<char,void(*)(char*)> unq((char*)GlobalAlloc(GPTR,nxtsz),[](char* ptr){GlobalFree(ptr);});
            read(unq.get(),nxtsz,ev);
            return unq;
        }
        HANDLE handle(){return h;}
        operator HANDLE(){return h;}
};
template<typename Tp>
inline DWORD mailslot::write(Tp dt)
{
    DWORD dwret;
    std::string wstr=std::to_string(dt);
    if(!WriteFile(h,wstr.c_str(),wstr.size()+1,&dwret,nullptr))throw std::runtime_error("ygp::mailslot::write (function WriteFile): "+lasterror<std::string>());
    return dwret;
}
template<>
inline DWORD mailslot::write<std::string>(std::string wstr)
{
    DWORD dwret;
    if(!WriteFile(h,wstr.c_str(),wstr.size()+1,&dwret,nullptr))throw std::runtime_error("ygp::mailslot::write (function WriteFile): "+lasterror<std::string>());
    return dwret; 
}
template<>
inline DWORD mailslot::write<const char*>(const char* wstr)
{
    DWORD dwret;
    if(!WriteFile(h,wstr,strlen(wstr)+1,&dwret,nullptr))throw std::runtime_error("ygp::mailslot::write (function WriteFile): "+lasterror<std::string>());
    return dwret;
}
END_NAMESPACE_YGP
#endif