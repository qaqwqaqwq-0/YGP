#ifndef _YGP_WIN_SHARED_MEMORY_HPP_
#define _YGP_WIN_SHARED_MEMORY_HPP_
#include"winerr.hpp"
BEGIN_NAMESPACE_YGP
class shared_memory//share memory between multiple processes
{
    HANDLE h;
    public:
        operator HANDLE()const noexcept{return h;}
        HANDLE handle()const noexcept{return h;}
        shared_memory(std::uint64_t size,const char* name=nullptr,HANDLE hfile=INVALID_HANDLE_VALUE)
        {
            if(!(h=CreateFileMappingA(hfile,nullptr,PAGE_READWRITE,size>>32,size&0xffffffff,name)))
                throw std::runtime_error("ygp::shared_memory::shared_memory (function CreateFileMappingA): "+lasterror<std::string>());
        }
        shared_memory(HANDLE H)
        {
            if(!H)
                throw std::runtime_error("ygp::shared_memory::shared_memory received an null handle value. Reason: "+lasterror<std::string>());
            h=H;
        }
        void* alloc(DWORD dwsize)
        {
            void* ret;
            if(!(ret=MapViewOfFile(h,FILE_MAP_ALL_ACCESS,0ul,0ul,dwsize)))
                throw std::runtime_error("ygp::shared_memory::alloc (function MapViewOfFile): "+lasterror<std::string>());
            return ret;
        }
        static shared_memory open(const char* name)
        {
            return shared_memory(OpenFileMappingA(FILE_MAP_ALL_ACCESS,TRUE,name));
        }
        void free(void* _ptr)
        {
            if(!UnmapViewOfFile(_ptr))
                throw std::runtime_error("ygp::shared_memory::free (function UnmapViewOfFile): "+lasterror<std::string>());
        }
        ~shared_memory()
        {
            CloseHandle(h);
        }
};
END_NAMESPACE_YGP
#endif