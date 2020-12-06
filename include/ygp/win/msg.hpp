#ifndef _YGP_WIN_MSG_HPP_
#define _YGP_WIN_MSG_HPP_
#include"mailslot.hpp"
BEGIN_NAMESPACE_YGP
enum ygpmsgtype
{
    sender,
    receiver
};
template<ygpmsgtype msgtype=ygpmsgtype::sender>class msg;
template<>
class msg<ygpmsgtype::receiver>
{
    mailslot* mms;
    mailslot* tms;
    event* mev;
    event* tev;
    public:
        explicit msg(const char* name)
        {
            mms=new mailslot((std::string("\\\\.\\mailslot\\_")+name).c_str());
            tms=mailslot::openp((std::string("\\\\.\\mailslot\\")+name).c_str());
            mev=event::openp((std::string(name).append(1,'_')).c_str());
            tev=event::openp((std::string(name)).c_str());
            mev->set();
        }
        ~msg()
        {
            delete mms;
            delete tms;
            delete mev;
            delete tev;
        }
        std::unique_ptr<char,void(*)(char*)> get()
        {
            mev->wait();
            tev->block();
            return mms->read();
        }
        std::unique_ptr<char,void(*)(char*)> getw()//Get the message and the sender do not wait
        {
            if(!mms->count())mev->wait();
            mev->block();
            return mms->read();
        }
        void yield(const char* content)
        {
            tms->write(content);
            mev->block();
            tev->set();
        }
};
template<>
class msg<ygpmsgtype::sender>
{
    mailslot* mms;
    mailslot* tms;
    event* mev;
    event* tev;
    PROCESS_INFORMATION procinfo;
    public:
        explicit msg(const char* name,char* cmd)
        {
            tms=new mailslot((std::string("\\\\.\\mailslot\\")+name).c_str());
            mev=new event((std::string(name).append(1,'_')).c_str(),false);
            tev=new event((std::string(name)).c_str(),false);
            {
                STARTUPINFOA si={sizeof(si)};
                si.dwFlags=STARTF_USESHOWWINDOW;
                si.wShowWindow=TRUE;
                if(!CreateProcessA(nullptr,cmd,nullptr,nullptr,FALSE,CREATE_NEW_CONSOLE,nullptr,nullptr,&si,&procinfo))
                    throw std::runtime_error("ygp::msg::msg (function CreateProcessA): "+lasterror<std::string>());
            }
            mev->wait();
            mms=mailslot::openp((std::string("\\\\.\\mailslot\\_")+name).c_str());
        }
        ~msg()
        {
            delete mms;
            delete tms;
            delete mev;
            delete tev;
            CloseHandle(procinfo.hThread);
            CloseHandle(procinfo.hProcess);
        }
        std::unique_ptr<char,void(*)(char*)> send(const char* content)
        {
            mms->write(content);
            mev->set();
            tev->wait();
            return tms->read();
        }
        void sendw(const char* content)//Send the message and return immediately
        {
            mms->write(content);
            mev->set();
        }
        UINT wait(DWORD dwml=INFINITE)
        {
            return WaitForSingleObject(procinfo.hProcess,dwml);
        }
};
END_NAMESPACE_YGP
#endif