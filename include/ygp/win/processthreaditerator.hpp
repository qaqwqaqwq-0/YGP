#ifndef _YGP_WIN_PROCESSTHREADITERATOR_HPP_
#define _YGP_WIN_PROCESSTHREADITERATOR_HPP_
#include"winerr.hpp"
#include<tlhelp32.h>
BEGIN_NAMESPACE_YGP
class processiterator
{
    HANDLE h;
    PROCESSENTRY32 entry;
    public:
        processiterator()
        {
            h=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0ul);
            if(h==INVALID_HANDLE_VALUE)
                throw std::runtime_error("ygp::processiterator::processiterator (function CreateToolhelp32Snapshot): "+lasterror<std::string>());
            entry.dwSize=sizeof(entry);
        }
        operator PROCESSENTRY32&()noexcept{return entry;}
        operator HANDLE()const noexcept{return h;}
        HANDLE snapshothandle()const noexcept{return h;}
        struct iterator
        {
            PROCESSENTRY32* etr;
            HANDLE hsnap;
            iterator(PROCESSENTRY32* _entry,HANDLE _hs):etr(_entry),hsnap(_hs){}
            iterator& operator++()
            {
                if(!Process32Next(hsnap,etr))
                    etr=nullptr,CloseHandle(hsnap),hsnap=nullptr;
                return *this;
            }
            bool operator!=(const iterator& another)const noexcept
            {
                return bool(etr)!=bool(another.etr);
            }
            PROCESSENTRY32& operator*()
            {
                return *etr;
            }
        };
        iterator begin()
        {
            if(!Process32First(h,&entry))
                throw std::runtime_error("ygp::processiterator::begin (function Process32First): "+lasterror<std::string>());
            return iterator(&entry,h);
        }
        iterator end()
        {
            return iterator(nullptr,nullptr);
        }
};
class threaditerator
{
    HANDLE h;
    THREADENTRY32 entry;
    public:
        threaditerator()
        {
            h=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0ul);
            if(h==INVALID_HANDLE_VALUE)
                throw std::runtime_error("ygp::threaditerator::threaditerator (function CreateToolhelp32Snapshot): "+lasterror<std::string>());
            entry.dwSize=sizeof(entry);
        }
        operator THREADENTRY32&()noexcept{return entry;}
        operator HANDLE()const noexcept{return h;}
        HANDLE snapshothandle()const noexcept{return h;}
        struct iterator
        {
            THREADENTRY32* etr;
            HANDLE hsnap;
            iterator(THREADENTRY32* _entry,HANDLE _hs):etr(_entry),hsnap(_hs){}
            iterator& operator++()
            {
                if(!Thread32Next(hsnap,etr))
                    etr=nullptr,CloseHandle(hsnap),hsnap=nullptr;
                return *this;
            }
            bool operator!=(const iterator& another)const noexcept
            {
                return bool(etr)!=bool(another.etr);
            }
            THREADENTRY32& operator*()
            {
                return *etr;
            }
        };
        iterator begin()
        {
            if(!Thread32First(h,&entry))
                throw std::runtime_error("ygp::threaditerator::begin (function Thread32First): "+lasterror<std::string>());
            return iterator(&entry,h);
        }
        iterator end()
        {
            return iterator(nullptr,nullptr);
        }
        static DWORD procthreadcount(DWORD dwProcId=GetCurrentProcessId())
        {
            threaditerator ti;
            DWORD dwRet=0ul;
            for(const auto& i:ti)if(i.th32OwnerProcessID==dwProcId)++dwRet;
            return dwRet;
        }
};
END_NAMESPACE_YGP
#endif