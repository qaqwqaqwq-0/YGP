/*
    Encapsulation for Windows sychronization APIs.
*/
#ifndef _YGP_WIN_SYNC_HPP_
#define _YGP_WIN_SYNC_HPP_
#include"winerr.hpp"
BEGIN_NAMESPACE_YGP
struct ilchar//atomic char (all change operations are interlocked)
{
    volatile char v;
    ilchar(char _v):v(_v){}
    operator char()const noexcept
    {
        return v;
    }
#ifdef _MSC_VER
    char operator=(char _val)noexcept//returns the previous value
    {
        return InterlockedExchange8(&v,_val);
    }
#endif
    ilchar& operator&=(char _val)noexcept
    {
        InterlockedAnd8(&v,_val);
        return *this;
    }
    ilchar& operator|=(char _val)noexcept
    {
        InterlockedOr8(&v,_val);
        return *this;
    }
    ilchar& operator^=(char _val)noexcept
    {
        InterlockedXor8(&v,_val);
        return *this;
    }
};
struct ilshort//atomic short (all change operations are interlocked)
{
    volatile SHORT v;
    ilshort(SHORT _v):v(_v){}
    operator SHORT()const noexcept
    {
        return v;
    }
    SHORT operator++()noexcept
    {
        return InterlockedIncrement16(&v);
    }
    SHORT operator--()noexcept
    {
        return InterlockedDecrement16(&v);
    }
    SHORT operator++(int)noexcept
    {
        SHORT _tmp=v;
        InterlockedIncrement16(&v);
        return _tmp;
    }
    SHORT operator--(int)noexcept
    {
        SHORT _tmp=v;
        InterlockedDecrement16(&v);
        return _tmp;
    }
#ifdef _MSC_VER
    SHORT operator=(SHORT _val)noexcept//returns the previous value
    {
        return InterlockedExchange16(&v,_val);
    }
#endif
    ilshort& operator&=(SHORT _val)noexcept
    {
        InterlockedAnd16(&v,_val);
        return *this;
    }
    ilshort& operator|=(SHORT _val)noexcept
    {
        InterlockedOr16(&v,_val);
        return *this;
    }
    ilshort& operator^=(SHORT _val)noexcept
    {
        InterlockedXor16(&v,_val);
        return *this;
    }
};
struct illong//atomic long (all change operations are interlocked)
{
    volatile LONG v;
    illong(LONG _v):v(_v){}
    operator LONG()const noexcept
    {
        return v;
    }
    LONG operator++()noexcept
    {
        return InterlockedIncrement(&v);
    }
    LONG operator--()noexcept
    {
        return InterlockedDecrement(&v);
    }
    LONG operator++(int)noexcept
    {
        LONG _tmp=v;
        InterlockedIncrement(&v);
        return _tmp;
    }
    LONG operator--(int)noexcept
    {
        LONG _tmp=v;
        InterlockedDecrement(&v);
        return _tmp;
    }
    illong& operator+=(LONG _val)noexcept
    {
        InterlockedAdd(&v,_val);
        return *this;
    }
    illong& operator-=(LONG _val)noexcept
    {
        return this->operator-=(-_val);
    }
    LONG operator=(LONG _val)noexcept//returns the previous value
    {
        return InterlockedExchange(&v,_val);
    }
    illong& operator&=(LONG _val)noexcept
    {
        InterlockedAnd(&v,_val);
        return *this;
    }
    illong& operator|=(LONG _val)noexcept
    {
        InterlockedOr(&v,_val);
        return *this;
    }
    illong& operator^=(LONG _val)noexcept
    {
        InterlockedXor(&v,_val);
        return *this;
    }
    bool testandset(LONG _Offset)
    {
        return (bool)_interlockedbittestandset(&v,_Offset);
        //Example:
        //  std::cout<<hex;
        //  ygp::illong i=0x3;//0011
        //  ygp::print(i.testandset(3));
        //  ygp::print(i);
        //Output:
        //  0
        //  b                 //0111
    }
    bool testandreset(LONG _Offset)
    {
        return (bool)_interlockedbittestandreset(&v,_Offset);
        //Example:
        //  std::cout<<hex;
        //  ygp::illong i=0xb;//0111
        //  ygp::print(i.testandreset(3));
        //  ygp::print(i);
        //Output:
        //  1
        //  3                 //0011
    }
};
struct illong64//atomic long long (all change operations are interlocked)
{
    volatile LONG64 v;
    illong64(LONG64 _v):v(_v){}
    operator LONG64()const noexcept
    {
        return v;
    }
    LONG64 operator++()noexcept
    {
        return InterlockedIncrement64(&v);
    }
    LONG64 operator--()noexcept
    {
        return InterlockedDecrement64(&v);
    }
    LONG64 operator++(int)noexcept
    {
        LONG _tmp=v;
        InterlockedIncrement64(&v);
        return _tmp;
    }
    LONG64 operator--(int)noexcept
    {
        LONG _tmp=v;
        InterlockedDecrement64(&v);
        return _tmp;
    }
    illong64& operator+=(LONG64 _val)noexcept
    {
        InterlockedAdd64(&v,_val);
        return *this;
    }
    illong64& operator-=(LONG64 _val)noexcept
    {
        return this->operator-=(-_val);
    }
    LONG64 operator=(LONG64 _val)noexcept//returns the previous value
    {
        return InterlockedExchange64(&v,_val);
    }
    illong64& operator&=(LONG64 _val)noexcept
    {
        InterlockedAnd64(&v,_val);
        return *this;
    }
    illong64& operator|=(LONG64 _val)noexcept
    {
        InterlockedOr64(&v,_val);
        return *this;
    }
    illong64& operator^=(LONG64 _val)noexcept
    {
        InterlockedXor64(&v,_val);
        return *this;
    }
#if defined(_M_AMD64)||defined(_M_IA64)
    bool testandset(LONG64 _Offset)
    {
        return (bool)_interlockedbittestandset64(&v,_Offset);
        //Example:
        //  std::cout<<hex;
        //  ygp::illong64 i=0x200000000ll;
        //  ygp::print(i.testandset(33));
        //  ygp::print(i);
        //Output:
        //  0
        //  300000000
    }
    bool testandreset(LONG64 _Offset)
    {
        return (bool)_interlockedbittestandreset64(&v,_Offset);
        //Example:
        //  std::cout<<hex;
        //  ygp::illong64 i=0x300000000ll;
        //  ygp::print(i.testandreset(33));
        //  ygp::print(i);
        //Output:
        //  1
        //  200000000
    }
#endif
};
class mutex
{
    private:
        HANDLE h;
    public:
        mutex()
        {
            h=CreateMutexA(nullptr,false,nullptr);
            if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::mutex::mutex (function CreateMutexA): "+lasterror<std::string>());
        }
        mutex(HANDLE H)
        {
            if(H==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::mutex received an invalid handle value.Reason: "+lasterror<std::string>());
            h=H;
        }
        explicit mutex(const char* name)
        {
            h=CreateMutexA(nullptr,false,name);
            if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::mutex received an invalid handle value.Reason: "+lasterror<std::string>());
        }
        explicit mutex(bool binitial)
        {
            h=CreateMutexA(nullptr,(BOOL)binitial,nullptr);
            if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::mutex received an invalid handle value.Reason: "+lasterror<std::string>());
        }
        explicit mutex(bool binitial,const char* name)
        {
            h=CreateMutexA(nullptr,(BOOL)binitial,name);
            if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::mutex received an invalid handle value.Reason: "+lasterror<std::string>());
        }
        ~mutex(){CloseHandle(h);}
        static mutex open(const char* name)
        {
            return mutex(OpenMutexA(MUTEX_ALL_ACCESS,TRUE,name));
        }
        static mutex* openp(const char* name)
        {
            return new mutex(OpenMutexA(MUTEX_ALL_ACCESS,TRUE,name));
        }
        DWORD wait(DWORD _time=INFINITE)
        {
            return WaitForSingleObject(h,_time);
        }
        void release()
        {
            if(!ReleaseMutex(h))
                throw std::runtime_error("ygp::mutex::release (function ReleaseMutex): "+lasterror<std::string>());
        }
        HANDLE handle(){return h;}
        operator HANDLE(){return h;}
};
class event
{
    private:
        HANDLE h;
    public:
        event(const char* name=nullptr,bool initialset=true/*not blocked at first*/,bool manual=false)
        {
            h=CreateEventA(nullptr,(BOOL)manual,(BOOL)initialset,name);
            if(h==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::event::event (function CreateEventA): "+lasterror<std::string>());
        }
        event(HANDLE H)
        {
            if(H==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::event received an invalid handle value.Reason: "+lasterror<std::string>());
            h=H;
        }
        ~event(){CloseHandle(h);}
        static event open(const char* name)
        {
            return event(OpenEventA(EVENT_ALL_ACCESS,TRUE,name));
        }
        static event* openp(const char* name)
        {
            return new event(OpenEventA(EVENT_ALL_ACCESS,TRUE,name));
        }
        void block()
        {
            if(!ResetEvent(h))
                throw std::runtime_error("ygp::event::block (function ResetEvent): "+lasterror<std::string>());
        }
        void set()
        {
            if(!SetEvent(h))
                throw std::runtime_error("ygp::event::set (function SetEvent): "+lasterror<std::string>());
        }
        void pulse()
        {
            if(!PulseEvent(h))
                throw std::runtime_error("ygp::event::pulse (function PulseEvent): "+lasterror<std::string>());
        }
        DWORD wait(DWORD tm=INFINITE)
        {
            return WaitForSingleObject(h,tm);
        }
        HANDLE handle(){return h;}
        operator HANDLE(){return h;}
};
class events
{
    HANDLE* h;
    std::size_t n;
    void allocate(void)
    {
        h=new HANDLE[n];
    }
    public:
        explicit events(std::size_t N,bool initial=true,bool manual=false)
        {
            n=N;
            allocate();
            for(std::size_t i=0;i<n;++i)
            {
                h[i]=CreateEventA(nullptr,(BOOL)manual,(BOOL)initial,nullptr);
                if(h[i]==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::events::events (function CreateEventA): "+lasterror<std::string>());
            }
        }
        explicit events(const char* name,std::size_t N,bool initial=true,bool manual=false)
        {
            n=N;
            allocate();
            for(std::size_t i=0;i<n;++i)
            {
                h[i]=CreateEventA(nullptr,(BOOL)manual,(BOOL)initial,(std::string(name)+std::to_string(i)).c_str());
                if(h[i]==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::events::events (function CreateEventA): "+lasterror<std::string>());
            }
        }
        explicit events(HANDLE* H,std::size_t N){h=H,n=N;}
        ~events(){for(std::size_t i=0;i<n;++i)CloseHandle(h[i]);delete []h;}
        static events open(const char* name,std::size_t N)
        {
            HANDLE* k=new HANDLE[N];
            for(std::size_t i=0;i<N;++i)
            {
                k[i]=OpenEventA(EVENT_ALL_ACCESS,TRUE,name);
                if(k[i]==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::events::(static method)open (function OpenEventA): "+lasterror<std::string>());
            }
            return events(name,N);
        }
        static events* openp(const char* name,std::size_t N)
        {
            HANDLE* k=new HANDLE[N];
            for(std::size_t i=0;i<N;++i)
            {
                k[i]=OpenEventA(EVENT_ALL_ACCESS,TRUE,name);
                if(k[i]==INVALID_HANDLE_VALUE)throw std::runtime_error("ygp::events::(static method)openp (function OpenEventA): "+lasterror<std::string>());
            }
            return new events(name,N);
        }
    private:
        struct _M_event
        {
            HANDLE hd;
            explicit _M_event(HANDLE HD){hd=HD;}
            void block()
            {
                if(!ResetEvent(hd))throw std::runtime_error("ygp::events::_M_event::block (function ResetEvent): "+lasterror<std::string>());
            }
            void set()
            {
                if(!SetEvent(hd))throw std::runtime_error("ygp::events::_M_event::set (function SetEvent): "+lasterror<std::string>());
            }
            void pulse()
            {
                if(!PulseEvent(hd))throw std::runtime_error("ygp::events::_M_event::pulse (function PulseEvent): "+lasterror<std::string>());
            }
            DWORD wait(DWORD dwml=INFINITE)
            {
                return WaitForSingleObject(hd,dwml);
            }
            operator HANDLE()const{return hd;}
            HANDLE handle()const{return hd;}
        };
    public:
        _M_event operator[](std::size_t _index)
        {
            if(_index>=n)throw std::out_of_range("ygp::events::_M_event::operator[]: Index out of range.");
            return _M_event(h[_index]);
        }
        void blockall()
        {
            for(std::size_t i=0;i<n;++i)
            {
                if(!ResetEvent(h[i]))throw std::runtime_error("ygp::events::blockall (function ReserEvent): "+lasterror<std::string>());
            }
        }
        void setall()
        {
            for(std::size_t i=0;i<n;++i)
            {
                if(!SetEvent(h[i]))throw std::runtime_error("ygp::events::setall (function SetEvent): "+lasterror<std::string>());
            }
        }
        DWORD wait(bool waitall=true,DWORD dwml=INFINITE)
        {
            if(n>64)throw std::out_of_range("ygp::events::wait: Event amount is more than MAXIMUM_WAIT_OBJECTS(64).");
            return WaitForMultipleObjects(n,h,(BOOL)waitall,dwml);
        }
        /*
            Interpretation of parameter 'waitall':
                If waitall is true, then the function would not return until all events are set;
                If it is false, the function would return as soon as one event is set.
        */
};
class critical_section//has better performance than mutex but cannot be used in interprocess situations
{
    CRITICAL_SECTION h;
    public:
        operator CRITICAL_SECTION&()noexcept{return h;}
        critical_section()noexcept
        {
            InitializeCriticalSection(&h);
        }
        ~critical_section()noexcept
        {
            DeleteCriticalSection(&h);
        }
        void enter()noexcept
        {
            EnterCriticalSection(&h);
        }
        void leave()noexcept
        {
            LeaveCriticalSection(&h);
        }
        bool tryenter()noexcept
        {
            return (bool)TryEnterCriticalSection(&h);
        }
};
#if _WIN32_WINNT>=0x0600//Windows version requirement
class srwlock//Slim Read/Write Lock
{
    SRWLOCK h;
    public:	    
        operator SRWLOCK&()noexcept{return h;}
	    srwlock()noexcept{InitializeSRWLock(&h);}
	    srwlock& acquire()noexcept{AcquireSRWLockExclusive(&h);return *this;}          //Exclusive
	    srwlock& release()noexcept{ReleaseSRWLockExclusive(&h);return *this;}          //Exclusive
	    srwlock& acquire_shared()noexcept{AcquireSRWLockShared(&h);return *this;}      //Shared
	    srwlock& release_shared()noexcept{ReleaseSRWLockShared(&h);return *this;}      //Shared
	    ~srwlock()noexcept{}//There is no need to delete the object explicitly.
};
#endif
class waitgroup
{
    public:
        event ev;
        illong cnt;
        waitgroup(const char* name=nullptr):cnt(0l),ev(name,false){}
        DWORD wait(DWORD dwml=INFINITE){return ev.wait(dwml);}
        LONG inc()//increase
        {
            return ++cnt;
        }
        LONG dec()//decrease
        {
            if(!--cnt)ev.set();//When the cnt variable is decreased to 0l, set the event.
            return cnt;
        }
};
class signer
{
    public:
        event ev;
        illong cnt;
        LONG tot;
        signer(LONG _tot,const char* name=nullptr):tot(_tot),cnt(0l),ev(name,false){}
        DWORD wait(DWORD dwml=INFINITE){return ev.wait(dwml);}
        LONG inc()//increase
        {
            if(++cnt==tot)ev.set();
            return cnt;
        }
};
END_NAMESPACE_YGP
#endif