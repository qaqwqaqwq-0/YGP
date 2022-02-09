#ifndef _YGP_BASE_HPP_
#define _YGP_BASE_HPP_
#include"configure.hpp"
#include<iostream>
#include<memory>
BEGIN_NAMESPACE_YGP
static inline bool io_async(bool __async=true)
{
	return !std::ios::sync_with_stdio(!__async);
}
namespace detail
{
	template<typename _Tp>
	static std::ostream& __Prints(std::ostream& __os,char __delim,_Tp&& _TP)
	{
		return __os<<_TP;
	}
	template<typename _Tp,typename ...__args>
	static std::ostream& __Prints(std::ostream& __os,char __delim,_Tp&& _TP,__args&& ... __ARGS)
	{
		__os<<_TP<<__delim;return __Prints(__os,__delim,__ARGS...);
	}
}
template<typename ...__args>
static std::ostream& printde(std::ostream& __os,char __delim,char __end_of_line,__args&& ... __ARGS)
	//Print with specified std::ostream object, delimiter and end-of-line character.
{
	return detail::__Prints(__os,__delim,__ARGS...)<<__end_of_line;
}
template<typename ...__args>
static std::ostream& printswe(std::ostream& __os,char __delim,__args&& ... __ARGS)
	//Print with specified std::ostream object and delimiter without an end-of-line character.
{
	return detail::__Prints(__os,__delim,__ARGS...);
}
template<typename ...__args>
static std::ostream& prints(std::ostream& __os,__args&& ... __ARGS)
	//Print with specified std::ostream object, using ' ' and '\n' as delimiter 
	//and end-of-line character respectively.
{
	return printde(__os,' ','\n',__ARGS...);
}
template<typename ...__args>
static std::ostream& printwe(__args&& ... __ARGS)
	//Print with std::out, without an end-of-line character.
{
	return printswe(std::cout,' ',__ARGS...);
}
template<typename ...__args>
static std::ostream& print(__args&& ... __ARGS)
	//ygp::print has the same functions as the print function in Python.
	//i.e., print with std::out, and use ' ' and '\n' as delimiter 
	//and end-of-line character respectively.
	//e.g.
	//  std::cout<<std::boolalpha;
	//	ygp::print(3.14f,5201314,"string",false);
	//Output:
	//3.14 5201314 string false
	//
{
	return prints(std::cout,__ARGS...);
}
namespace detail
{
	template<typename _Tp>
	static std::ostream& __Print_without_space(std::ostream& __os,_Tp&& _TP)
	{
		return std::cout<<_TP;
	}
	template<typename _Tp,typename ...__args>
	static std::ostream& __Print_without_space(std::ostream& __os,_Tp&& _TP,__args&& ... __ARGS)
	{
		std::cout<<_TP;return __Print_without_space(__os,__ARGS...);
	}
}
template<typename ...__args>
static std::ostream& printwss(std::ostream& __os,__args&& ... __ARGS)
	//Print without space, with the output stream specified, and '\n' as the ending of a line.
{
	return detail::__Print_without_space(__os,__ARGS...)<<'\n';
}
template<typename ...__args>
static std::ostream& printwssd(std::ostream& __os,char __end_of_line,__args&& ... __ARGS)
	//Print without space, with the output stream and end-of-line character specified.
{
	return detail::__Print_without_space(__os,__ARGS...)<<__end_of_line;
}
template<typename ...__args>
static std::ostream& printws(__args&& ... __ARGS)
	//Print without space, with the character '\n' as the ending of a line.
{
	return printwss(std::cout,__ARGS...);
}
template<typename _Ch,typename _Tp>
static std::basic_ostream<_Ch,std::char_traits<_Ch>>& operator<<(std::basic_ostream<_Ch,std::char_traits<_Ch>>& __os,std::shared_ptr<_Tp>&& __ptr)
{
	return __os<<__ptr.get();
}
template<typename _Ch,typename _Tp>
static std::basic_ostream<_Ch,std::char_traits<_Ch>>& operator<<(std::basic_ostream<_Ch,std::char_traits<_Ch>>& __os,std::unique_ptr<_Tp>&& __ptr)
{
	return __os<<__ptr.get();
}
namespace detail
{
	template<typename _Ch,typename _Tuple,std::size_t _N>
	static std::basic_ostream<_Ch>& _Tuple_print(std::basic_ostream<_Ch>& __os,const _Tuple& __t,std::integral_constant<std::size_t,_N>)
	{
		_Tuple_print(__os,__t,std::integral_constant<std::size_t,_N-1>());
		return __os<<_Ch(',')<<std::get<_N-1u>(__t);
	}
	template<typename _Ch,typename _Tuple>
	static std::basic_ostream<_Ch>& _Tuple_print(std::basic_ostream<_Ch>& __os,const _Tuple& __t,std::integral_constant<std::size_t,1u>)
	{
		return __os<<std::get<0>(__t);
	}
}
template<typename _Ch,typename ...__Args>
static std::basic_ostream<_Ch>& operator<<(std::basic_ostream<_Ch>& __os,const std::tuple<__Args...>& __t)
{
	return detail::_Tuple_print(__os,__t,std::integral_constant<std::size_t,sizeof...(__Args)>());
}
#define YGP_MAIN int main(int argc,char** argv)
#define YGP_WMAIN int wmain(int argc,wchar_t** argv)
#define YGP_TRY try{
#define YGP_CATCH }catch(std::exception& __exp){std::cerr<<"[Error] "<<__exp.what()<<std::endl;}catch(...){std::cerr<<"[Error] Uncaught exception.\n";}
#define YGP_CATCH_MSGBOX }catch(std::exception& __exp){MessageBoxA(nullptr,(std::string("[Error] ")+__exp.what()).c_str(),"YGP Error",MB_ICONERROR);}catch(...){MessageBoxA(nullptr,"[Error] Uncaught exception.","YGP Error",MB_ICONERROR);}
#define YGP_CATCH_MSGBOX_(title) }catch(std::exception& __exp){MessageBoxA(nullptr,(std::string("[Error] ")+__exp.what()).c_str(),title,MB_ICONERROR);}catch(...){MessageBoxA(nullptr,"[Error] Uncaught exception.",title,MB_ICONERROR);}
#define YGP_CATCH_IGNORE }catch(...){}
#define YGP_CATCH_ALL YGP_CATCH_IGNORE
#define YGP_PRIT(x) {for(auto& i:x)std::cout<<i<<' ';std::cout<<'\n';}//Print itetarable object
#define YGP_WPRIT(x) {for(auto& i:x)std::wcout<<i<<' ';std::wcout<<L'\n';}
#define YGP_PUTVAR(k) {std::cout<<#k<<"="<<k<<'\n';}
#define YGP_INIT                                     \
    using namespace std;                             \
    using namespace std::literals::string_literals;  \
    using namespace ygp;
#ifdef _MSC_VER
#define YGP_LIB(l) comment(lib,#l)
#endif
#define YGP_ALLOC(var,type,...) type* var=new type(__VA_ARGS__);
#define YGP_ALLOCREF(var,type,...) type& var=*(new type(__VA_ARGS__));
#define YGP_FREE(var) delete var;
#define YGP_DELREF(var) delete &var;
END_NAMESPACE_YGP
#endif