//Extensive string manipulator.
#ifndef _YGP_STRMANIP_HPP_
#define _YGP_STRMANIP_HPP_
#include"configure.hpp"
#include<string>
#include<sstream>
#ifndef YGP_STD_TO_STRING_EXTENSION_ALREADY_IMPLEMENTED
namespace std
{
    static string to_string(const char* _val)
    {
        return string(_val);
    }
    static string to_string(char _val)
    {
        string ret;
        ret.push_back(_val);
        return ret;
    }
    static wstring to_wstring(const wchar_t* _val)
    {
        return wstring(_val);
    }
    static wstring to_wstring(wchar_t _val)
    {
        wstring ret;
        ret.push_back(_val);
        return ret;
    }
}
#endif
BEGIN_NAMESPACE_YGP
namespace detail
{
    struct __CStr__t__{};
}
static detail::__CStr__t__ cstr;
template<typename _Tp>
static auto operator-(const _Tp&& __t,detail::__CStr__t__&)//("Vol"+"demort"s).c_str() is now "Vol"+"demort"s-cstr.
{
    return __t.c_str();
}
template<typename _Tp>
static auto operator-(const _Tp& __t,detail::__CStr__t__&)
{
    return __t.c_str();
}
#define YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(str,type)          \
    static std::str operator+(const std::str& _str,type _tp)    \
    {                                                             \
        return _str+std::to_##str(_tp);                           \
    }                                                             \
    static std::str operator+(type _tp,const std::str& _str)    \
    {                                                             \
        return std::to_##str(_tp)+_str;                           \
    }
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(string,char)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(string,int)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(string,long)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(string,long long)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(string,unsigned)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(string,unsigned long long)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(string,float)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(string,double)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(string,long double)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(wstring,wchar_t)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(wstring,int)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(wstring,long)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(wstring,long long)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(wstring,unsigned)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(wstring,unsigned long long)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(wstring,float)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(wstring,double)
YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS(wstring,long double)
#undef YGP_STRMANIP_DECL_STRING_OPERATOR_PLUS
template<typename _Ch>
static const _Ch* operator<<(std::basic_stringstream<_Ch>& _strm,detail::__CStr__t__&)
{
    return _strm.str().c_str();
}
template<typename _Ch>
static int kmp(const _Ch* _longstr,const _Ch* _shortstr)
{
    const int slen=std::char_traits<_Ch>::length(_shortstr);
    int* nxt=new int[slen];
    nxt[0]=-1;
    int i=0,j=-1;
    while(i<slen)
        if(j==-1||_shortstr[i]==_shortstr[j])++i,++j,nxt[i]=j;
        else j=nxt[j];
    i=j=0;
    while(_longstr[i]&&j<slen)
        if(j==-1||_longstr[i]==_shortstr[j])++i,++j;
        else j=nxt[j];
    delete []nxt;
    if(j==slen)return i-j;
    return -1;
}
namespace detail
{
    template<typename _Tp>
    struct __Strmaip__impl__format__t__;
    template<>
    struct __Strmaip__impl__format__t__<char>
    {
        using _Tp=char;
        const _Tp* a;
        std::basic_string<_Tp> ret;
        __Strmaip__impl__format__t__(const _Tp* __a){a=__a;}
        template<typename _Ty>
        void _impl(_Ty&& __t)
        {
            for(;*a;++a)
            {
                if(*a=='%')
                {
                    if(*(++a)=='%')
                    {
                        ret.push_back('%');
                    }
                    else
                    {
                        ret.append(std::to_string(__t));
                        ret.append(a);
                        return;
                    }
                }
                else
                {
                    ret.push_back(*a);
                }
            }
        }
        template<typename _Ty,typename ...__Args>
        void _impl(_Ty&& __t,__Args&&... __args)
        {
            for(;*a;++a)
            {
                if(*a=='%')
                {
                    if(*(++a)=='%')
                    {
                        ret.push_back('%');
                    }
                    else
                    {
                        ret.append(std::to_string(__t));
                        _impl(__args...);
                        return;
                    }
                }
                else
                {
                    ret.push_back(*a);
                }
            }
        }
        template<typename ...__Args>
        std::basic_string<_Tp> operator()(__Args&&... __args)
        {
            _impl(__args...);
            return ret;
        }
    };//struct __Strmaip__impl__format__t__<char>
    template<>
    struct __Strmaip__impl__format__t__<wchar_t>
    {
        using _Tp=wchar_t;
        const _Tp* a;
        std::basic_string<_Tp> ret;
        __Strmaip__impl__format__t__(const _Tp* __a){a=__a;}
        template<typename _Ty>
        void _impl(_Ty&& __t)
        {
            for(;*a;++a)
            {
                if(*a=='%')
                {
                    if(*(++a)=='%')
                    {
                        ret.push_back('%');
                    }
                    else
                    {
                        ret.append(std::to_wstring(__t));
                        ret.append(a);
                        return;
                    }
                }
                else
                {
                    ret.push_back(*a);
                }
            }
        }
        template<typename _Ty,typename ...__Args>
        void _impl(_Ty&& __t,__Args&&... __args)
        {
            for(;*a;++a)
            {
                if(*a=='%')
                {
                    if(*(++a)=='%')
                    {
                        ret.push_back('%');
                    }
                    else
                    {
                        ret.append(std::to_wstring(__t));
                        _impl(__args...);
                        return;
                    }
                }
                else
                {
                    ret.push_back(*a);
                }
            }
        }
        template<typename ...__Args>
        std::basic_string<_Tp> operator()(__Args&&... __args)
        {
            _impl(__args...);
            return ret;
        }
    };//struct __Strmaip__impl__format__t__<wchar_t>
}
static detail::__Strmaip__impl__format__t__<char> operator""_f(const char* __format,std::size_t len)
{
    return detail::__Strmaip__impl__format__t__<char>(__format);
}
static detail::__Strmaip__impl__format__t__<wchar_t> operator""_f(const wchar_t* __format,std::size_t len)
{
    return detail::__Strmaip__impl__format__t__<wchar_t>(__format);
}
END_NAMESPACE_YGP
#endif