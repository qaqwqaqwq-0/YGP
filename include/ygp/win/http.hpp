/**
 * Wrapper classes of WINHTTP client-side APIs.
 */
#ifndef _YGP_WIN_HTTP_HPP_
#define _YGP_WIN_HTTP_HPP_
#include"winerr.hpp"
#include<memory>
#include<winhttp.h>
#ifndef YGP_DISABLE_COM
#include<shlwapi.h>
#endif
#ifdef _MSC_VER
#pragma comment(lib,"winhttp")
#ifndef YGP_DISABLE_COM
#pragma comment(lib,"shlwapi")
#endif
#endif
BEGIN_NAMESPACE_YGP
namespace detail
{
    static HMODULE hm_winhttp_dll=nullptr;//module handle of winhttp.dll
    static std::string winhttplasterror()
    {
        return ext_lasterror<std::string>("winhttp.dll");
    }
};
static void initializewinhttp()
{
    detail::hm_winhttp_dll=LoadLibraryA("winhttp.dll");
    if(!detail::hm_winhttp_dll)throw std::runtime_error(
        "ygp::initializewinhttp (function LoadLibraryA): "
        +lasterror<std::string>());
}
static void uninitializewinhttp()noexcept
{
    if(detail::hm_winhttp_dll)
        FreeLibrary(detail::hm_winhttp_dll);
}
#ifndef YGP_DISABLE_COM
static DWORD urlescape(LPCWSTR lpszUrl,std::wstring& escaped,
    DWORD dwFlags=URL_ESCAPE_AS_UTF8|
    URL_ESCAPE_ASCII_URI_COMPONENT,int cch=-1)
{
    DWORD dwSize=(cch==-1?wcslen(lpszUrl):cch)*9+1;
    wchar_t* wch=new wchar_t[dwSize];
    throw_com_error(UrlEscapeW(lpszUrl,wch,&dwSize,dwFlags),
        "urlescape","UrlEscapeW");
    escaped.assign(wch);
    delete []wch;
    return dwSize;//not including the terminating-'\0'
}
static DWORD urlunescape(LPWSTR lpszUrl/*both in and out*/,
    DWORD dwFlags=URL_UNESCAPE_AS_UTF8|URL_UNESCAPE_INPLACE|URL_UNESCAPE_URI_COMPONENT,
    int cch=-1)
{
    DWORD dwSize=(cch==-1?wcslen(lpszUrl):cch)+1;
    throw_com_error(UrlUnescapeW(lpszUrl,nullptr,&dwSize,dwFlags),
        "urlunescape","UrlUnescapeW");
    return dwSize;//not including the terminating-'\0'
}
#endif
class urlcomponents
{
    void initialize()
    {
        zmem(components);
        components.dwStructSize=sizeof(components);
        components.dwSchemeLength=
        components.dwExtraInfoLength=
        components.dwUserNameLength=
        components.dwUrlPathLength=
        components.dwPasswordLength=
        components.dwHostNameLength=
            0xfffffffful;
    }
    public:
        enum flags:DWORD
        {
            decodechars=ICU_DECODE,
            escapechars=ICU_ESCAPE
        };
        URL_COMPONENTSW components;
        void create(LPCWSTR lpszUrl,DWORD dwFlags=0ul)
        {
            if(!WinHttpCrackUrl(lpszUrl,0ul,dwFlags,&components))
                throw std::runtime_error("ygp::urlcomponents::"
                    "create (function WinHttpCrackUrl): "+
                    detail::winhttplasterror());
        }
        urlcomponents()
        {
            initialize();
        }
        explicit urlcomponents(LPCWSTR lpszUrl,DWORD dwFlags=0ul)
        {
            initialize();
            create(lpszUrl,dwFlags);
        }
        void geturl(std::shared_ptr<wchar_t> _ptr,
            DWORD dwFlags=0ul)
        {
            DWORD dwLen=0ul;
            WinHttpCreateUrl(&components,dwFlags,nullptr,&dwLen);
            if(GetLastError()!=ERROR_INSUFFICIENT_BUFFER)
                throw std::runtime_error("ygp::urlcomponents::"
                    "geturl (function WinHttpCreateUrl): "+
                    detail::winhttplasterror());
            LPWSTR lpUrl=new wchar_t[dwLen];
            WinHttpCreateUrl(&components,dwFlags,lpUrl,&dwLen);
            _ptr.reset(lpUrl);
        }
        void geturl(std::unique_ptr<wchar_t> _ptr,
            DWORD dwFlags=0ul)
        {
            DWORD dwLen=0ul;
            WinHttpCreateUrl(&components,dwFlags,nullptr,&dwLen);
            if(GetLastError()!=ERROR_INSUFFICIENT_BUFFER)
                throw std::runtime_error("ygp::urlcomponents::"
                    "geturl (function WinHttpCreateUrl): "+
                    detail::winhttplasterror());
            LPWSTR lpUrl=new wchar_t[dwLen];
            WinHttpCreateUrl(&components,dwFlags,lpUrl,&dwLen);
            _ptr.reset(lpUrl);
        }
        std::wstring geturl(DWORD dwFlags=0ul)
        {
            DWORD dwLen=0ul;
            WinHttpCreateUrl(&components,dwFlags,nullptr,&dwLen);
            if(GetLastError()!=ERROR_INSUFFICIENT_BUFFER)
                throw std::runtime_error("ygp::urlcomponents::"
                    "geturl (function WinHttpCreateUrl): "+
                    detail::winhttplasterror());
            LPWSTR lpUrl=new wchar_t[dwLen];
            WinHttpCreateUrl(&components,dwFlags,lpUrl,&dwLen);
            std::wstring ret(lpUrl);
            delete []lpUrl;
            return ret;
        }
        std::wstring getscheme()const noexcept
        {
            return std::wstring(components.lpszScheme,
                components.dwSchemeLength);
        }
        std::wstring gethostname()const noexcept
        {
            return std::wstring(components.lpszHostName,
                components.dwHostNameLength);
        }
        std::wstring getusername()const noexcept
        {
            return std::wstring(components.lpszUserName,
                components.dwUserNameLength);
        }
        std::wstring getpassword()const noexcept
        {
            return std::wstring(components.lpszPassword,
                components.dwPasswordLength);
        }
        std::wstring geturlpath()const noexcept
        {
            return std::wstring(components.lpszUrlPath,
                components.dwUrlPathLength);
        }
        std::wstring getextrainfo()const noexcept
        {
            return std::wstring(components.lpszExtraInfo,
                components.dwExtraInfoLength);
        }
        INTERNET_PORT getport()const noexcept
        {
            return components.nPort;
        }
};
class httpsession
{
    public:
        HINTERNET h;
        operator HINTERNET()const noexcept
        {
            return h;
        }
        void close()
        {
            if(h)CloseHandle(h);h=nullptr;
        }
        httpsession():h(nullptr){}
        explicit httpsession(LPCWSTR lpszUserAgent,bool bAsync=false)
        {
            h=WinHttpOpen(lpszUserAgent,
                WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
                WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,
                bAsync?WINHTTP_FLAG_ASYNC:0ul);
            if(!h)
                throw std::runtime_error("ygp::httpsession::"
                    "httpsession (function WinHttpOpen): "+
                    detail::winhttplasterror());
        }
        explicit httpsession(LPCWSTR lpszUserAgent,LPCWSTR lpszProxy,
            LPCWSTR lpszProxyBypass=WINHTTP_NO_PROXY_BYPASS,
            bool bAsync=false)
        {
            h=WinHttpOpen(lpszUserAgent,
                WINHTTP_ACCESS_TYPE_NAMED_PROXY,
                lpszProxy,lpszProxyBypass,bAsync?WINHTTP_FLAG_ASYNC:0ul);
            if(!h)
                throw std::runtime_error("ygp::httpsession::"
                    "httpsession (function WinHttpOpen): "+
                    detail::winhttplasterror());
        }
        HINTERNET createproxyresolver()const
        {
            HINTERNET ret;
            DWORD dwError=WinHttpCreateProxyResolver(h,&ret);
            if(dwError)
                throw std::runtime_error("ygp::httpsession::"
                    "httpsession (function WinHttpOpen): "+
                    ext_lasterror<std::string>("winhttp.dll",dwError));
            return ret;
        }
        void getproxyforurl(LPCWSTR lpszUrl,
            WINHTTP_AUTOPROXY_OPTIONS& opts,
            WINHTTP_PROXY_INFO& ppinfo)
        {
            if(!WinHttpGetProxyForUrl(h,lpszUrl,&opts,&ppinfo))
                throw std::runtime_error("ygp::httpsession::"
                    "getproxyforurl (function WinHttpGetProxyForUrl): "+
                    detail::winhttplasterror());
        }
        void resetautoproxy(DWORD dwFlags=WINHTTP_RESET_ALL|
            WINHTTP_RESET_OUT_OF_PROC)
        {
            if(!WinHttpResetAutoProxy(h,dwFlags))
                throw std::runtime_error("ygp::httpsession::"
                    "resetautoproxy (function WinHttpResetAutoProxy): "+
                    detail::winhttplasterror());
        }
        httpsession& setoption(DWORD dwOption,LPVOID lpBuffer,DWORD
            dwBufferLength)
        {
            if(!WinHttpSetOption(h,dwOption,lpBuffer,dwBufferLength))
                throw std::runtime_error("ygp::httpsession::"
                    "setoption (function WinHttpSetOption): "+
                    detail::winhttplasterror());
        }
        template<typename _Tp>
        httpsession& setoption(DWORD dwOption,_Tp& _data)
        {
            if(!WinHttpSetOption(h,dwOption,&_data,sizeof(_data)))
                throw std::runtime_error("ygp::httpsession::"
                    "setoption (function WinHttpSetOption): "+
                    detail::winhttplasterror());
        }
        template<typename _Tp>
        httpsession& setoption(DWORD dwOption,_Tp&& _data)
        {
            if(!WinHttpSetOption(h,dwOption,&_data,sizeof(_data)))
                throw std::runtime_error("ygp::httpsession::"
                    "setoption (function WinHttpSetOption): "+
                    detail::winhttplasterror());
        }
        void settimeouts(int resolvetimeout=0,int connecttimeout=0,
            int sendtimeout=0,int receivetimeout=0)
            //0,-1=infinite
            //default values:
            //resolvetimeout=infinite,connecttimeout=60s,
            //sendtimeout=30s,receivetimeout=30s
        {
            if(!WinHttpSetTimeouts(h,resolvetimeout,connecttimeout,
                sendtimeout,receivetimeout))
                throw std::runtime_error("ygp::httpsession::"
                    "settimeouts (function WinHttpSetTimeouts): "+
                    detail::winhttplasterror());
        }
        ~httpsession()
        {
            if(h)CloseHandle(h);
        }
        //TBD:
        //WinHttpQueryOption
        //WinHttpSetStatusCallback
};
class httpconnection
{
    public:
        HINTERNET h;
        operator HINTERNET()const noexcept
        {
            return h;
        }
        void close()
        {
            if(h)CloseHandle(h);h=nullptr;
        }
        httpconnection():h(nullptr){}
        void create(HINTERNET hSession,LPCWSTR lpszServerName,
            INTERNET_PORT port)
        {
            if(!(h=WinHttpConnect(hSession,lpszServerName,port,0ul)))
                throw std::runtime_error("ygp::httpconnection::"
                    "create (function WinHttpConnect): "+
                    detail::winhttplasterror());
        }
        void create(HINTERNET hSession,urlcomponents cmp)
        {
            create(hSession,cmp.gethostname().c_str(),
                cmp.getport());
        }
        httpconnection(HINTERNET hSession,LPCWSTR lpszServerName,
            INTERNET_PORT port)
        {
            create(hSession,lpszServerName,port);
        }
        httpconnection(HINTERNET hSession,LPCWSTR lpszUrl)
        {
            urlcomponents cmp(lpszUrl);
            create(hSession,cmp.gethostname().c_str(),
                cmp.getport());
        }
        httpconnection(HINTERNET hSession,const urlcomponents& cmp)
        {
            create(hSession,cmp.gethostname().c_str(),
                cmp.getport());
        }
        ~httpconnection()
        {
            if(h)CloseHandle(h);
        }
        //TBD:
        //WinHttpSetStatusCallback
};
class httprequest
{
    public:
        HINTERNET h;
        operator HINTERNET()const noexcept
        {
            return h;
        }
        void close()
        {
            if(h)CloseHandle(h);h=nullptr;
        }
        httprequest():h(nullptr){}
        void create(HINTERNET hConnect,LPCWSTR lpszVerb,
            LPCWSTR lpszObjectName,LPCWSTR lpszVersion=nullptr
            /*The WinHttpOpenRequest API uses HTTP/1.1 by default*/,
            LPCWSTR lpszReferer=WINHTTP_NO_REFERER,
            LPCWSTR* ppAcceptTypes=WINHTTP_DEFAULT_ACCEPT_TYPES,
            DWORD dwFlags=0ul)
        {
            if(!(h=WinHttpOpenRequest(hConnect,lpszVerb,lpszObjectName,
                lpszVersion,lpszReferer,ppAcceptTypes,dwFlags)))
                throw std::runtime_error("ygp::httprequest::"
                    "create (function WinHttpOpenRequest: "+
                    detail::winhttplasterror());
        }
        httprequest(HINTERNET hConnect,LPCWSTR lpszVerb,
            LPCWSTR lpszObjectName,LPCWSTR lpszVersion,
            LPCWSTR lpszReferer=WINHTTP_NO_REFERER,
            LPCWSTR* ppAcceptTypes=WINHTTP_DEFAULT_ACCEPT_TYPES,
            DWORD dwFlags=0ul)
        {
            create(hConnect,lpszVerb,lpszObjectName,
                lpszVersion,lpszReferer,ppAcceptTypes,dwFlags);
        }
        httprequest(HINTERNET hConnect,LPCWSTR lpszUrl,LPCWSTR lpszVerb,
            DWORD dwFlags=0ul,LPCWSTR lpszReferer=WINHTTP_NO_REFERER,
            LPCWSTR* ppAcceptTypes=WINHTTP_DEFAULT_ACCEPT_TYPES)
        {
            urlcomponents cmp(lpszUrl);
            DWORD flags=dwFlags;
            if(cmp.components.nScheme==INTERNET_SCHEME_HTTPS)
                flags|=WINHTTP_FLAG_SECURE;
            create(hConnect,lpszVerb,cmp.geturlpath().c_str(),nullptr,
                lpszReferer,ppAcceptTypes,flags);
        }
        httprequest(HINTERNET hConnect,const urlcomponents& cmp,LPCWSTR lpszVerb,
            DWORD dwFlags=0ul,LPCWSTR lpszReferer=WINHTTP_NO_REFERER,
            LPCWSTR* ppAcceptTypes=WINHTTP_DEFAULT_ACCEPT_TYPES)
        {
            DWORD flags=dwFlags;
            if(cmp.components.nScheme==INTERNET_SCHEME_HTTPS)
                flags|=WINHTTP_FLAG_SECURE;
            create(hConnect,lpszVerb,cmp.geturlpath().c_str(),nullptr,
                lpszReferer,ppAcceptTypes,flags);
        }
        void send()
        {
            if(!WinHttpSendRequest(h,WINHTTP_NO_ADDITIONAL_HEADERS,
                0ul,nullptr,0ul,0ul,0ull))
                throw std::runtime_error("ygp::httprequest::send "
                    "(function WinHttpSendRequest): "+
                    detail::winhttplasterror());
        }
        struct sender
        {
            LPCWSTR headers;
            LPVOID data;//send immediately after the request headers
            DWORD datalength;
            DWORD totallength;
            sender():headers(WINHTTP_NO_ADDITIONAL_HEADERS),
                data(nullptr),datalength(0ul),totallength(0ul){}
        };
        void send(const sender& sd)
        {
            if(!WinHttpSendRequest(h,sd.headers,0xfffffffful,
                sd.data,sd.datalength,sd.totallength,0ull))
                throw std::runtime_error("ygp::httprequest::send "
                    "(function WinHttpSendRequest): "+
                    detail::winhttplasterror());
        }
        //TBD:
        //send with callback function params
        void receiveresponse()
        {
            if(!WinHttpReceiveResponse(h,nullptr))
                throw std::runtime_error("ygp::httprequest::"
                    "receiveresponse (function"
                    " WinHttpReceiveResponse): "+
                    detail::winhttplasterror());
        }
        DWORD statuscode()
        {
            DWORD dwSize=0ul;
            WinHttpQueryHeaders(h,WINHTTP_QUERY_STATUS_CODE,
                nullptr,nullptr,&dwSize,WINHTTP_NO_HEADER_INDEX);
            if(GetLastError()!=ERROR_INSUFFICIENT_BUFFER)
                throw std::runtime_error("ygp::httprequest::"
                    "statuscode (function"
                    " WinHttpQueryHeaders): "+
                    detail::winhttplasterror());
            BYTE* ret=new BYTE[dwSize];
            WinHttpQueryHeaders(h,WINHTTP_QUERY_STATUS_CODE,
                nullptr,ret,&dwSize,WINHTTP_NO_HEADER_INDEX);
            const wchar_t* wch=(const wchar_t*)ret+(dwSize>>1);
            DWORD dwRet=0ul;
            for(const wchar_t* c=(const wchar_t*)ret;c!=wch;++c)
                //Convert wide character string to interger
                dwRet=(dwRet<<1)+(dwRet<<3)+(*c^L'0');
            delete []ret;
            return dwRet;
        }
        //HTTP status codes:
        //100 Continue
        //101 Switching Protocols
        //103 Early Hints
        //200 OK
        //201 Created
        //202 Accepted
        //203 Non-Authoritative Information
        //204 No Content
        //205 Reset Content
        //206 Partial Content
        //300 Multiple Choices
        //301 Moved Permanently
        //302 Found
        //303 See Other
        //304 Not Modified
        //307 Temporary Redirect
        //308 Permanent Redirect
        //400 Bad Request
        //401 Unauthorized
        //402 Payment Required
        //403 Forbidden
        //404 Not Found
        //405 Method Not Allowed
        //406 Not Acceptable
        //407 Proxy Authentication Required
        //408 Request Timeout
        //409 Conflict
        //410 Gone
        //411 Length Required
        //412 Precondition Failed
        //413 Payload Too Large
        //414 URI Too Long
        //415 Unsupported Media Type
        //416 Range Not Satisfiable
        //417 Expectation Failed
        //418 I'm a teapot
        //422 Unprocessable Entity
        //425 Too Early
        //426 Upgrade Required
        //428 Precondition Required
        //429 Too Many Requests
        //431 Request Header Fields Too Large
        //451 Unavailable For Legal Reasons
        //500 Internal Server Error
        //501 Not Implemented
        //502 Bad Gateway
        //503 Service Unavailable
        //504 Gateway Timeout
        //505 HTTP Version Not Supported
        //506 Variant Also Negotiates
        //507 Insufficient Storage
        //508 Loop Detected
        //510 Not Extended
        //511 Network Authentication Required
        std::wstring getheaders()
        {
            DWORD dwSize=0ul;
            WinHttpQueryHeaders(h,WINHTTP_QUERY_RAW_HEADERS_CRLF,
                nullptr,nullptr,&dwSize,WINHTTP_NO_HEADER_INDEX);
            if(GetLastError()!=ERROR_INSUFFICIENT_BUFFER)
                throw std::runtime_error("ygp::httprequest::"
                    "statuscode (function"
                    " WinHttpQueryHeaders): "+
                    detail::winhttplasterror());
            wchar_t* ret=new wchar_t[dwSize/sizeof(wchar_t)];
            WinHttpQueryHeaders(h,WINHTTP_QUERY_RAW_HEADERS_CRLF,
                nullptr,ret,&dwSize,WINHTTP_NO_HEADER_INDEX);
            std::wstring wstr(ret,dwSize/sizeof(wchar_t));
            delete []ret;
            return wstr;
        }
        void getheaders(std::shared_ptr<wchar_t>& _str)
        {
            DWORD dwSize=0ul;
            WinHttpQueryHeaders(h,WINHTTP_QUERY_RAW_HEADERS_CRLF,
                nullptr,nullptr,&dwSize,WINHTTP_NO_HEADER_INDEX);
            if(GetLastError()!=ERROR_INSUFFICIENT_BUFFER)
                throw std::runtime_error("ygp::httprequest::"
                    "statuscode (function"
                    " WinHttpQueryHeaders): "+
                    detail::winhttplasterror());
            wchar_t* ret=new wchar_t[dwSize/sizeof(wchar_t)];
            WinHttpQueryHeaders(h,WINHTTP_QUERY_RAW_HEADERS_CRLF,
                nullptr,ret,&dwSize,WINHTTP_NO_HEADER_INDEX);
            _str.reset(ret);
        }
        void getheaders(std::unique_ptr<wchar_t>& _str)
        {
            DWORD dwSize=0ul;
            WinHttpQueryHeaders(h,WINHTTP_QUERY_RAW_HEADERS_CRLF,
                nullptr,nullptr,&dwSize,WINHTTP_NO_HEADER_INDEX);
            if(GetLastError()!=ERROR_INSUFFICIENT_BUFFER)
                throw std::runtime_error("ygp::httprequest::"
                    "statuscode (function"
                    " WinHttpQueryHeaders): "+
                    detail::winhttplasterror());
            wchar_t* ret=new wchar_t[dwSize/sizeof(wchar_t)];
            WinHttpQueryHeaders(h,WINHTTP_QUERY_RAW_HEADERS_CRLF,
                nullptr,ret,&dwSize,WINHTTP_NO_HEADER_INDEX);
            _str.reset(ret);
        }
        std::wstring getheader(LPCWSTR lpszHeader)
        {
            DWORD dwSize=0ul;
            WinHttpQueryHeaders(h,WINHTTP_QUERY_CUSTOM,
                lpszHeader,nullptr,&dwSize,WINHTTP_NO_HEADER_INDEX);
            if(GetLastError()!=ERROR_INSUFFICIENT_BUFFER)
                throw std::runtime_error("ygp::httprequest::"
                    "statuscode (function"
                    " WinHttpQueryHeaders): "+
                    detail::winhttplasterror());
            wchar_t* ret=new wchar_t[dwSize/sizeof(wchar_t)];
            WinHttpQueryHeaders(h,WINHTTP_QUERY_CUSTOM,
                lpszHeader,ret,&dwSize,WINHTTP_NO_HEADER_INDEX);
            std::wstring wstr(ret,dwSize/sizeof(wchar_t));
            delete []ret;
            return wstr;
        }
        std::wstring getheader(DWORD dwInfo)
        {
            DWORD dwSize=0ul;
            WinHttpQueryHeaders(h,dwInfo,
                nullptr,nullptr,&dwSize,WINHTTP_NO_HEADER_INDEX);
            if(GetLastError()!=ERROR_INSUFFICIENT_BUFFER)
                throw std::runtime_error("ygp::httprequest::"
                    "statuscode (function"
                    " WinHttpQueryHeaders): "+
                    detail::winhttplasterror());
            wchar_t* ret=new wchar_t[dwSize/sizeof(wchar_t)];
            WinHttpQueryHeaders(h,dwInfo,
                nullptr,ret,&dwSize,WINHTTP_NO_HEADER_INDEX);
            std::wstring wstr(ret,dwSize/sizeof(wchar_t));
            delete []ret;
            return wstr;
        }
        DWORD read(LPVOID lpBuffer,DWORD dwBytesToRead)
        {
            DWORD dwRet;
            if(!WinHttpReadData(h,lpBuffer,dwBytesToRead,&dwRet))
                throw std::runtime_error("ygp::httprequest::"
                    "read (function WinHttpReadData): "+
                    detail::winhttplasterror());
            return dwRet;
        }
        DWORD write(LPCVOID lpBuffer,DWORD dwBytesToWrite)
        {
            DWORD dwRet;
            if(!WinHttpWriteData(h,lpBuffer,dwBytesToWrite,&dwRet))
                throw std::runtime_error("ygp::httprequest::"
                    "write (function WinHttpWriteData): "+
                    detail::winhttplasterror());
            return dwRet;
        }
        httprequest& addheaders(LPCWSTR lpszHeaders,DWORD dwFlags=
            WINHTTP_ADDREQ_FLAG_ADD)
        {
            if(!WinHttpAddRequestHeaders(h,lpszHeaders,0xfffffffful,
                dwFlags))
                throw std::runtime_error("ygp::httprequest::"
                    "addheaders (function"
                    " WinHttpAddRequestHeaders): "+
                    detail::winhttplasterror());
            return *this;
        }
        httprequest& addheaders(std::wstring& strHeaders,DWORD dwFlags=
            WINHTTP_ADDREQ_FLAG_ADD)
        {
            return addheaders(strHeaders.c_str(),dwFlags);
        }
        void settimeouts(int resolvetimeout=0,int connecttimeout=0,
            int sendtimeout=0,int receivetimeout=0)
            //0,-1=infinite
        {
            if(!WinHttpSetTimeouts(h,resolvetimeout,connecttimeout,
                sendtimeout,receivetimeout))
                throw std::runtime_error("ygp::httprequest::"
                    "settimeouts (function WinHttpSetTimeouts): "+
                    detail::winhttplasterror());
        }
        ~httprequest()
        {
            if(h)CloseHandle(h);
        }
};
END_NAMESPACE_YGP
#endif