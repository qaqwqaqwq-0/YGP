#ifndef _YGP_WIN_PIPE_HPP_
#define _YGP_WIN_PIPE_HPP_
#include"winerr.hpp"
BEGIN_NAMESPACE_YGP
class pipe
{
    HANDLE hpipein;
    HANDLE hpipeout;
    HANDLE hpp1;
    HANDLE hpp2;
    PROCESS_INFORMATION procinfo;
    public:
        pipe(char* cmd)
        {
            SECURITY_ATTRIBUTES sa;
            sa.nLength=sizeof(sa);
            sa.lpSecurityDescriptor=nullptr;
            sa.bInheritHandle=TRUE;
            if(!CreatePipe(&hpipein,&hpp1,&sa,0))throw std::runtime_error("ygp::pipe::pipe (function CreatePipe): "+lasterror<std::string>());
            if(!CreatePipe(&hpp2,&hpipeout,&sa,0))throw std::runtime_error("ygp::pipe::pipe (function CreatePipe): "+lasterror<std::string>());
            STARTUPINFO si;
            zmem(si);
            si.cb=sizeof(si);
            si.dwFlags=STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES;
            si.wShowWindow=SW_HIDE;
            si.hStdInput=hpp2;
            si.hStdOutput=si.hStdError=hpp1;
            if(!CreateProcessA(nullptr,cmd,nullptr,nullptr,TRUE,0,nullptr,nullptr,&si,&procinfo))throw std::runtime_error("ygp::pipe::pipe (function CreateProcessA): "+lasterror<std::string>());
        }
        DWORD size()
        {
            DWORD dwret;
            if(!PeekNamedPipe(hpipein,nullptr,0,&dwret,nullptr,nullptr))throw std::runtime_error(lasterror<std::string>());
            return dwret;
        }
        DWORD read(void* buf,DWORD dwsize)
        {
            DWORD dwret;
            if(!ReadFile(hpipein,buf,dwsize,&dwret,nullptr))throw std::runtime_error(lasterror<std::string>());
            return dwret;
        }
        DWORD write(void* buf,DWORD dwsize)
        {
            DWORD dwret;
            if(!WriteFile(hpipeout,buf,dwsize,&dwret,nullptr))throw std::runtime_error(lasterror<std::string>());
            return dwret;
        }
        DWORD wait(DWORD dwml=INFINITE)
        {
            return WaitForSingleObject(procinfo.hProcess,dwml);
        }
        void terminate(UINT ex=1)
        {
            if(!TerminateProcess(procinfo.hProcess,1))throw std::runtime_error(lasterror<std::string>());
        }
        ~pipe()
        {
            CloseHandle(hpipein);
            CloseHandle(hpipeout);
            CloseHandle(hpp1);
            CloseHandle(hpp2);
            CloseHandle(procinfo.hThread);
            CloseHandle(procinfo.hProcess);
        }
};
END_NAMESPACE_YGP
#endif