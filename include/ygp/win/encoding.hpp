#ifndef _YGP_WIN_ENCODING_HPP_
#define _YGP_WIN_ENCODING_HPP_
#include"winerr.hpp"
BEGIN_NAMESPACE_YGP
static int multibyte2widechar(const char* multibyte,std::wstring& widechar,UINT codepage,int cch=-1)
{
    int len=MultiByteToWideChar(codepage,0ul,multibyte,cch,nullptr,0ul);
    if(len<=0)throw std::runtime_error("ygp::multibyte2widechar (function MultiByteToWideChar): "+lasterror<std::string>());
    wchar_t* buf=new wchar_t[len];
    len=MultiByteToWideChar(codepage,0ul,multibyte,cch,buf,len);
    widechar.assign(buf);
    delete []buf;
    buf=nullptr;
    return len;
}
static int multibyte2widechar(const char* multibyte,std::shared_ptr<wchar_t>& widechar,UINT codepage,int cch=-1)
{
    int len=MultiByteToWideChar(codepage,0ul,multibyte,cch,nullptr,0ul);
    if(len<=0)throw std::runtime_error("ygp::multibyte2widechar (function MultiByteToWideChar): "+lasterror<std::string>());
    wchar_t* buf=new wchar_t[len];
    len=MultiByteToWideChar(codepage,0ul,multibyte,cch,buf,len);
    widechar.reset(buf);
    return len;
}
static int multibyte2widechar(const char* multibyte,std::unique_ptr<wchar_t>& widechar,UINT codepage,int cch=-1)
{
    int len=MultiByteToWideChar(codepage,0ul,multibyte,cch,nullptr,0ul);
    if(len<=0)throw std::runtime_error("ygp::multibyte2widechar (function MultiByteToWideChar): "+lasterror<std::string>());
    wchar_t* buf=new wchar_t[len];
    len=MultiByteToWideChar(codepage,0ul,multibyte,cch,buf,len);
    widechar.reset(buf);
    return len;
}
static int widechar2multibyte(const wchar_t* widechar,std::string& multibyte,UINT codepage,int cch=-1)
{
    int len=WideCharToMultiByte(codepage,0ul,widechar,cch,nullptr,0,nullptr,nullptr);
    if(len<=0)throw std::runtime_error("ygp::widechar2multibyte (function WideCharToMultiByte): "+lasterror<std::string>());
    char* buf=new char[len];
    len=WideCharToMultiByte(codepage,0ul,widechar,cch,buf,len,nullptr,nullptr);
    multibyte.assign(buf);
    delete []buf;
    buf=nullptr;
    return len;
}
static int widechar2multibyte(const wchar_t* widechar,std::shared_ptr<char>& multibyte,UINT codepage,int cch=-1)
{
    int len=WideCharToMultiByte(codepage,0ul,widechar,cch,nullptr,0,nullptr,nullptr);
    if(len<=0)throw std::runtime_error("ygp::widechar2multibyte (function WideCharToMultiByte): "+lasterror<std::string>());
    char* buf=new char[len];
    len=WideCharToMultiByte(codepage,0ul,widechar,cch,buf,len,nullptr,nullptr);
    multibyte.reset(buf);
    return len;
}
static int widechar2multibyte(const wchar_t* widechar,std::unique_ptr<char>& multibyte,UINT codepage,int cch=-1)
{
    int len=WideCharToMultiByte(codepage,0ul,widechar,cch,nullptr,0,nullptr,nullptr);
    if(len<=0)throw std::runtime_error("ygp::widechar2multibyte (function WideCharToMultiByte): "+lasterror<std::string>());
    char* buf=new char[len];
    len=WideCharToMultiByte(codepage,0ul,widechar,cch,buf,len,nullptr,nullptr);
    multibyte.reset(buf);
    return len;
}
/*Instruction:
    To convert a string to another encoding (e.g. UTF-8 -> GBK), use multibyte2widechar first (with codepage=CP_UTF8), then call widechar2multibyte (with codepage=CP_ACP).
*/
static std::string wstr2str(const std::wstring& _str,UINT codepage=CP_ACP)
{
    std::string ret;
    widechar2multibyte(_str.c_str(),ret,codepage);
    return ret;
}
static std::string wstr2str(const std::wstring&& _str,UINT codepage=CP_ACP)
{
    std::string ret;
    widechar2multibyte(_str.c_str(),ret,codepage);
    return ret;
}
static std::wstring str2wstr(const std::string& _str,UINT codepage=CP_ACP)
{
    std::wstring ret;
    multibyte2widechar(_str.c_str(),ret,codepage);
    return ret;
}
static std::wstring str2wstr(const std::string&& _str,UINT codepage=CP_ACP)
{
    std::wstring ret;
    multibyte2widechar(_str.c_str(),ret,codepage);
    return ret;
}
inline int hexchar2int(wchar_t hexchar)
{
    if(iswdigit(hexchar))return hexchar^L'0';
    hexchar=towlower(hexchar);
    return hexchar-L'a'+10;
}
static void unescape(const std::wstring& _str,std::wstring& unescaped)
{
    for(std::wstring::const_iterator ci=_str.cbegin();ci!=_str.cend();++ci)
    {
        if(*ci==L'\\')
        if(*(ci+1)!=L'\\')
        {
            if(*(++ci)==L'u')
            {
                wchar_t wc=L'\0';
                for(int i=0;i<4;++i)
                {
                    ++ci;
                    wc=(wc<<4)|(hexchar2int(*ci));
                }
                unescaped.push_back(wc);
            }
        }
        else unescaped.push_back(*ci),++ci;
        else unescaped.push_back(*ci);
    }
}
END_NAMESPACE_YGP
#endif