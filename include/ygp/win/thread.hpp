#ifndef _YGP_WIN_THREAD_HPP_
#define _YGP_WIN_THREAD_hpp_
#include"winerr.hpp"
#include<functional>//std::invoke
#if defined(_MSC_VER)||defined(__GNUC__)||defined(__GNUG__)
#include<process.h>//_beginthreadex
#endif
BEGIN_NAMESPACE_YGP
class thread
{
	friend class threadex;
	typedef
#if defined(_MSC_VER)||defined(__GNUC__)||defined(__GNUG__)
	unsigned
#else
	DWORD
#endif
	thprm;
	template<typename _Tuple,std::size_t ..._Indices>
	static thprm WINAPI ThreadProc(LPVOID lpParam)noexcept
	{
		const std::unique_ptr<_Tuple> _FnVals(static_cast<_Tuple*>(lpParam));
		_Tuple& _tup=*_FnVals;
		std::invoke(std::move(std::get<_Indices>(_tup))...);
		return 0ul;
	}
	template<typename _Tuple,std::size_t ..._Indices>
	static constexpr auto GetInvoker(std::index_sequence<_Indices...>)noexcept
	{
		return &ThreadProc<_Tuple,_Indices...>;
	}
	template<typename _Callable,typename ...__Args>
	void begin(_Callable&& __f,__Args&&... __args)
	{
		//The exceptions thrown in __f must be caught within __f.
		static_assert(std::is_same<std::tuple<typename std::decay<__Args>::type...>,
			std::tuple<__Args...>>::value,
			"ygp::thread::begin: Static assertion failed:\n"
			"[YGP Error] The type of each argument must not be reference type.\n"
			"[YGP Error] Use std::ref to solve this problem.\n"
			"[YGP Error] For example,\n"
			"[YGP Error] \tint param=12345;\n"
			"[YGP Error] \tygp::thread([](int& i){param+=10;},param).join();\n"
			"[YGP Error] The code above is invalid, use:\n"
			"[YGP Error] \tygp::thread([](int& i){param+=10;},"
			"std::ref(param)).join();\n");
		//std::ref is in header <functional>.
		using _tuple_t=std::tuple<typename std::decay<_Callable>::type,
			typename std::decay<__Args>::type...>;
		auto _tuple=std::make_unique<_tuple_t>(std::forward<_Callable>(__f),
			std::forward<__Args>(__args)...);
		h=(HANDLE)
#if defined(_MSC_VER)||defined(__GNUC__)||defined(__GNUG__)
		_beginthreadex
#else
		CreateThread
#endif
		(nullptr,0,GetInvoker<_tuple_t>(std::make_index_sequence<1+
			sizeof...(__Args)>{}),_tuple.get(),0,(thprm*)&_id);
		if(!h)
			throw std::runtime_error("ygp::thread::begin (function "
#if defined(_MSC_VER)||defined(__GNUC__)||defined(__GNUG__)
			"_beginthreadex"
#else
			"CreateThread"
#endif
			"): "+lasterror<std::string>());
		_tuple.release();
	}
	public:
		struct threadtime
		{
			ULONGLONG createtime;
			ULONGLONG exittime;
			ULONGLONG kerneltime;
			ULONGLONG usertime;
		};
		HANDLE h;
		DWORD _id;
		thread():h(nullptr),_id(0ul){}
		template<typename _Callable,typename ...__Args>
		explicit thread(_Callable&& __f,__Args&&... __args)
		{
			begin(std::move(__f),std::move(__args)...);
		}
		explicit thread(HANDLE H)
		{
			if(H==INVALID_HANDLE_VALUE)
				throw std::runtime_error("ygp::thread::thread received an"
				"invalid handle value. Reason: "+lasterror<std::string>());
			h=H;
			_id=GetThreadId(H);
		}
		~thread()
		{
			if(h)CloseHandle(h);
		}
		void close()
		{
			if(h)CloseHandle(h);h=nullptr;
		}
		DWORD id()const noexcept{return _id;}
		operator HANDLE()const noexcept{return h;}
		HANDLE handle()const noexcept{return h;}
		DWORD join()
		{
			DWORD dwret=WaitForSingleObject(h,INFINITE);
			if(dwret==0xfffffffful)
				throw std::runtime_error("ygp::thread::join "
				"(function WaitForSingleObject): "+lasterror<std::string>());
			return dwret;
		}
		DWORD wait(DWORD dwml)noexcept{return WaitForSingleObject(h,dwml);}
		bool running()noexcept{return wait(0ul)==WAIT_TIMEOUT;}
		DWORD suspend()
		{
			DWORD dwret;
			if((dwret=SuspendThread(h))==(DWORD)-1)
				throw std::runtime_error("ygp::thread::suspend (function SuspendThread): "+
				lasterror<std::string>());
			return dwret;
		}
		DWORD resume()
		{
			DWORD dwret;
			if(dwret=ResumeThread(h)==(DWORD)-1)
				throw std::runtime_error("ygp::thread::resume (function ResumeThread): "+
				lasterror<std::string>());
			return dwret;
		}
		int priority()const noexcept{return GetThreadPriority(h);}
		DWORD procid()const
		{
			DWORD dwret;
			if(!(dwret=GetProcessIdOfThread(h)))
				throw std::runtime_error("ygp::thread::procid "
				"(function GetProcessIdOfThread): "+lasterror<std::string>());
			return dwret;
		}
		thread& priority(int nprio)
		{
			if(!SetThreadPriority(h,nprio))
				throw std::runtime_error("ygp::thread::priority "
				"(function SetThreadPriority): "+lasterror<std::string>());
			return *this;
		}
		DWORD exitcode()const
		{
			DWORD dwret;
			if(!GetExitCodeThread(h,&dwret))
				throw std::runtime_error("ygp::thread::exitcode "
				"(function GetExitCodeThread): "+lasterror<std::string>());
			return dwret;
		}
		YGP_DEPRECATED void terminate(DWORD dwext=1)
		{
			if(!TerminateThread(h,dwext))
				throw std::runtime_error("ygp::thread::terminate "
				"(function TerminateThread): "+lasterror<std::string>());
		}
		threadtime gettimes()const
		{
			FILETIME ct,et,kt,ut;
			if(!GetThreadTimes(h,&ct,&et,&kt,&ut))
				throw std::runtime_error("ygp::thread::gettimes "
				"(function GetThreadTimes): "+lasterror<std::string>());
			return {(ULONGLONG)ct.dwLowDateTime|((ULONGLONG)ct.dwLowDateTime<<32),
				(ULONGLONG)et.dwLowDateTime|((ULONGLONG)et.dwLowDateTime<<32),
				(ULONGLONG)kt.dwLowDateTime|((ULONGLONG)kt.dwLowDateTime<<32),
				(ULONGLONG)ut.dwLowDateTime|((ULONGLONG)ut.dwLowDateTime<<32)};
		}
};
class threadex:public thread
{
	struct param
	{
		DWORD dwStackSize;
		bool bInitialSuspended;
		bool bStackSizeParamIsReservation;
		param():dwStackSize(0ul),bInitialSuspended(false),
			bStackSizeParamIsReservation(false){}
		DWORD operator()()const noexcept
		{
			return (bInitialSuspended?CREATE_SUSPENDED:0)|
				(bStackSizeParamIsReservation?STACK_SIZE_PARAM_IS_A_RESERVATION:0);
		}
	}prm;
	template<typename _Callable,typename ...__Args>
	void beginex(_Callable&& __f,__Args&&... __args)
	{
		//The exceptions thrown in __f must be caught within __f.
		// static_assert(std::is_same<std::tuple<typename std::decay<__Args>::type...>,
		// 	std::tuple<__Args...>>::value,
		// 	"ygp::threadex::beginex: Static assertion failed:\n"
		// 	"[YGP Error] The type of each argument must not be reference type.\n"
		// 	"[YGP Error] Use std::ref to solve this problem.\n"
		// 	"[YGP Error] For example,\n"
		// 	"[YGP Error] \tint param=12345;\n"
		// 	"[YGP Error] \tygp::threadex th;\n"
		// 	"[YGP Error] \tth.create([](int& i){param+=10;},param).join();\n"
		// 	"[YGP Error] The code above is invalid, use:\n"
		// 	"[YGP Error] \tth.create([](int& i){param+=10;},"
		// 	"std::ref(param)).join();\n");
		using _tuple_t=std::tuple<typename std::decay<_Callable>::type,
			typename std::decay<__Args>::type...>;
		auto _tuple=std::make_unique<_tuple_t>(std::forward<_Callable>(__f),
			std::forward<__Args>(__args)...);
		h=(HANDLE)
#if defined(_MSC_VER)||defined(__GNUC__)||defined(__GNUG__)
		_beginthreadex
#else
		CreateThread
#endif
		(nullptr,prm.dwStackSize,GetInvoker<_tuple_t>(std::make_index_sequence<1+
			sizeof...(__Args)>{}),_tuple.get(),prm(),(thprm*)&_id);
		if(!h)
			throw std::runtime_error("ygp::threadex::beginex (function "
#if defined(_MSC_VER)||defined(__GNUC__)||defined(__GNUG__)
			"_beginthreadex"
#else
			"CreateThread"
#endif
			"): "+lasterror<std::string>());
		_tuple.release();
	}
	template<typename _Callable,typename ...__Args>
	void injectimpl(HANDLE hProcess,_Callable&& __f,__Args&&... __args)
	{
		//The exceptions thrown in __f must be caught within __f.
		static_assert(std::is_same<std::tuple<typename std::decay<__Args>::type...>,
			std::tuple<__Args...>>::value,
			"ygp::threadex::injectimpl: Static assertion failed:\n"
			"[YGP Error] The type of each argument must not be reference type.\n"
			"[YGP Error] Use std::ref to solve this problem.\n"
			"[YGP Error] For example,\n"
			"[YGP Error] \tint param=12345;\n"
			"[YGP Error] \tygp::threadex th;\n"
			"[YGP Error] \tth.inject(hProcess,[](int& i){param+=10;},param).join();\n"
			"[YGP Error] The code above is invalid, use:\n"
			"[YGP Error] \tth.inject(hProcess,[](int& i){param+=10;},"
			"std::ref(param)).join();\n");
		using _tuple_t=std::tuple<typename std::decay<_Callable>::type,
			typename std::decay<__Args>::type...>;
		auto _tuple=std::make_unique<_tuple_t>(std::forward<_Callable>(__f),
			std::forward<__Args>(__args)...);
		h=CreateRemoteThread(hProcess,nullptr,prm.dwStackSize,
			GetInvoker<_tuple_t>(std::make_index_sequence<1+
			sizeof...(__Args)>{}),_tuple.get(),prm(),(thprm*)&_id);
		if(!h)
			throw std::runtime_error("ygp::threadex::beginex "
				"(function CreateRemoteThread): "+lasterror<std::string>());
		_tuple.release();
	}
	public:
		using thread::thread;
		threadex& open(DWORD dwThreadId,DWORD dwDesiredAccess=THREAD_ALL_ACCESS)
		{
			_id=dwThreadId;
			if(!(h=OpenThread(dwDesiredAccess,FALSE,dwThreadId)))
				throw std::runtime_error("ygp::threadex::open "
					"(function OpenThread): "+lasterror<std::string>());
			return *this;
		}
		threadex& stacksize(DWORD dwBytes)noexcept
		{
			prm.dwStackSize=dwBytes;
			return *this;
		}
		threadex& createsuspended(bool _bsus)noexcept
			//This method does not create a thread; call create() after 
			//	configuring done.
			//Use resume() method to start the thread.
		{
			prm.bInitialSuspended=_bsus;
			return *this;
		}
		threadex& stacksizeparamisareservation(bool _bis)noexcept
		{
			prm.bStackSizeParamIsReservation=_bis;
			return *this;
		}
		template<typename _Callable,typename ...__Args>
		threadex& create(_Callable&& __f,__Args&&... __args)
		{
			beginex(std::move(__f),std::move(__args)...);
			return *this;
		}
		template<typename _Callable,typename ...__Args>
		threadex& inject(DWORD dwProcId,_Callable&& __f,__Args&&... __args)
		//inject a thread in another process
		{
			HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcId);
			if(!hProcess)
				throw std::runtime_error("ygp::threadex::inject"
					" (function OpenProcess): "+lasterror<std::string>());
			injectimpl(hProcess,std::move(__f),std::move(__args)...);
			CloseHandle(hProcess);
		}
		template<typename _Callable,typename ...__Args>
		threadex& inject(HANDLE hProcess,_Callable&& __f,__Args&&... __args)
		{
			injectimpl(hProcess,std::move(__f),std::move(__args)...);
			return *this;
		}
		DWORD startandjoin()
		{
			resume();
			return join();
		}
};
namespace thisthread
{
	static void DECLSPEC_NORETURN exit(DWORD _exc=0u)
	{
		ExitThread(_exc);
	}
	static DWORD id()
	{
		return GetCurrentThreadId();
	}
	static HANDLE handle()
	{
		return GetCurrentThread();
	}
	static void sleep(DWORD mil)
	{
		Sleep(mil);
	}
}
END_NAMESPACE_YGP
#endif