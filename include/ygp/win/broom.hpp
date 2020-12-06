//Encapsulation of the WinINet library.
#ifndef _YGP_WIN_BROOM_HPP_
#define _YGP_WIN_BROOM_HPP_
#ifdef _MSC_VER                 //Use Microsoft Visual C++ compiler
#pragma comment(lib,"wininet")  //For Internet* and Http* functions
#pragma comment(lib,"advapi32") //For Reg* functions
#endif
#include"winerr.hpp"  //lasterror<>, ext_lasterror<> and "../configure.hpp"
#include<string.h>    //strncpy, strcmpi, etc.
#include<wininet.h>   //Internet* and Http* functions
#include<memory>      //std::shared_ptr
BEGIN_NAMESPACE_YGP
static bool check_internet_connection()//Check if the current device is connected to the Internet.
{
    DWORD flag;
    return (bool)InternetGetConnectedState(&flag,0ul);
}
static void disable_certificate_revocation_check_via_registry(std::string&& strval="Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings")
{
    HKEY hkey;
    LONG errcode;
    if(errcode=RegOpenKeyExA(HKEY_CURRENT_USER,strval.c_str(),0ul,KEY_ALL_ACCESS,&hkey))
        throw std::runtime_error("ygp::disable_certificate_revocation_check_via_registry (function RegOpenKeyExA): "+lasterror<std::string>((DWORD)errcode));
    DWORD dwval=0x0ul;
    if(errcode=RegSetValueExA(hkey,"CertificateRevocation",0ul,REG_DWORD,(LPBYTE)&dwval,sizeof(DWORD)))
        throw std::runtime_error("ygp::disable_certificate_revocation_check_via_registry (function RegSetValueExA): "+lasterror<std::string>((DWORD)errcode));
    RegCloseKey(hkey);
}
namespace detail
{
    struct hmc
    {
        LONG dwref;    //Reference count
                       //If dwref==1 then load wininet.dll
                       //If dwref==0 then free wininet.dll
                       //Increase and decrease it when constructing and destructing a broom/nimbus class
        HMODULE hm;    //Module handle
    };
    hmc hc{0,nullptr};
}
class broom
{
    private:
        HINTERNET hSession;
        HINTERNET hConnect;
    public:
        explicit broom(const char* url,const char* hds=nullptr,DWORD opt=0)
        {
            InterlockedIncrement(&detail::hc.dwref);
            if(detail::hc.dwref==1)
            {
                detail::hc.hm=LoadLibraryA("wininet.dll");
                if(!detail::hc.hm)//Failed to load wininet.dll
                {
                    throw std::runtime_error("ygp::broom::broom: Unable to load wininet.dll");
                }
            }
            hSession=hConnect=nullptr;
            hSession=InternetOpenA("Microsoft Internet Explorer",INTERNET_OPEN_TYPE_PRECONFIG,nullptr,nullptr,0ul);
            if(!hSession)
                throw std::runtime_error("ygp::broom::broom (function InternetOpenA): "+ext_lasterror<std::string>("wininet.dll"));
            if(strlen(url)<5)throw std::invalid_argument("ygp::broom::broom: Invalid URL.");
            const char* i=url;
            for(;!isalpha(*i);++i);
            char prtbuf[10];    //Buffer that contains the protocal name (http, https, ftp (not supported), etc.)
            strncpy(prtbuf,i,5);//Use safer functions
            prtbuf[5]='\0';
            if(!strcmpi(prtbuf,"https"))
            {
                hConnect=InternetOpenUrlA(hSession,url,hds,(DWORD)-1,opt|0x843200ul,0ul);
                    //0x843200=INTERNET_FLAG_SECURE|INTERNET_FLAG_IGNORE_CERT_CN_INVALID|INTERNET_FLAG_IGNORE_CERT_DATE_INVALID|INTERNET_FLAG_NO_AUTH|INTERNET_FLAG_NO_UI
            }
            else
            {
                hConnect=InternetOpenUrlA(hSession,url,hds,(DWORD)-1,opt,0);
            }
            if(!hConnect)
                throw std::runtime_error("ygp::broom::broom (function InternetOpenUrlA): "+ext_lasterror<std::string>("wininet.dll"));
        }
        DWORD read(void* buf,DWORD len)
        {
            DWORD dwret;
            if(!InternetReadFile(hConnect,buf,len,&dwret))
                throw std::runtime_error("ygp::broom::read (function InternetReadFile): "+ext_lasterror<std::string>("wininet.dll"));
            return dwret;
        }
        std::shared_ptr<char> headers()//Raw headers
        {
            DWORD dwerr;
            DWORD dwsize;
            HttpQueryInfoA(hConnect,HTTP_QUERY_RAW_HEADERS_CRLF,nullptr,&dwsize,nullptr);
            if((dwerr=GetLastError())!=ERROR_INSUFFICIENT_BUFFER)
                throw std::runtime_error("ygp::broom::headers (function HttpQueryInfoA): "+ext_lasterror<std::string>("wininet.dll"));
            std::shared_ptr<char> ret(new char[dwsize]);
            HttpQueryInfoA(hConnect,HTTP_QUERY_RAW_HEADERS_CRLF,ret.get(),&dwsize,nullptr);
            return ret;
        }
        ~broom()
        {
            --detail::hc.dwref;
            if(!detail::hc.dwref&&detail::hc.hm)FreeLibrary(detail::hc.hm);
            if(hConnect)InternetCloseHandle(hConnect);
            if(hSession)InternetCloseHandle(hSession);
        }
};
class nimbus//Advanced implementation of 'broom' (name from "Nimbus 2000")
{
    HINTERNET hSession;
    HINTERNET hConnect;
    HINTERNET hRequest;
    INTERNET_PORT m_port;
    public:
        explicit nimbus(const char* meth,const char* url,DWORD opt=0ul)//meth: request method ("GET","POST","PUT",etc.) url: URL opt: extra options for HttpSendRequestA
        {
            InterlockedIncrement(&detail::hc.dwref);
            if(detail::hc.dwref==1ul)
            {
                detail::hc.hm=LoadLibraryA("wininet.dll");
                if(detail::hc.hm==INVALID_HANDLE_VALUE)
                {
                    throw std::runtime_error("ygp::nimbus::nimbus: Unable to load wininet.dll");
                }
            }
            hSession=hConnect=hRequest=nullptr;
            hSession=InternetOpenA("Microsoft Internet Explorer",INTERNET_OPEN_TYPE_PRECONFIG,nullptr,nullptr,0ul);
            if(!hSession)
                throw std::runtime_error("ygp::nimbus::nimbus (function InternetOpenA): "+ext_lasterror<std::string>("wininet.dll"));
            DWORD dwmaxconn=0xffffffff;//No max connection restriction
            if(!InternetSetOptionA(nullptr,INTERNET_OPTION_MAX_CONNS_PER_1_0_SERVER,&dwmaxconn,sizeof(DWORD)))
                throw std::runtime_error("ygp::nimbus::nimbus (function InternetSetOptionA): "+ext_lasterror<std::string>("wininet.dll"));
            ULONG ultimeout=0xffffffff;//Disable time out
            if(!InternetSetOptionA(hSession,INTERNET_OPTION_SEND_TIMEOUT,&ultimeout,sizeof(ULONG)))
                throw std::runtime_error("ygp::nimbus::nimbus (function InternetSetOptionA): "+ext_lasterror<std::string>("wininet.dll"));
            if(!InternetSetOptionA(hSession,INTERNET_OPTION_CONNECT_TIMEOUT,&ultimeout,sizeof(ULONG)))
                throw std::runtime_error("ygp::nimbus::nimbus (function InternetSetOptionA): "+ext_lasterror<std::string>("wininet.dll"));
            DWORD urllen=strlen(url)+1;
            char* szhostname=new char[urllen]{'\0'};
            char* szusername=new char[128]{'\0'};
            char* szpassword=new char[128]{'\0'};
            char* szurlpath=new char[urllen]{'\0'};
            URL_COMPONENTSA compo={sizeof(URL_COMPONENTSA),nullptr,0ul,INTERNET_SCHEME_DEFAULT,szhostname,urllen,0,szusername,128,szpassword,128,szurlpath,urllen,nullptr,0ul};
            if(!InternetCrackUrlA(url,0ul,0ul,&compo))
                throw std::runtime_error("ygp::nimbus::nimbus (function InternetCrackUrlA): "+ext_lasterror<std::string>("wininet.dll"));
            m_port=compo.nPort;
            hConnect=InternetConnectA(hSession,szhostname,compo.nPort,szusername,szpassword,INTERNET_SERVICE_HTTP,0ul,0ul);
            if(!hConnect)
                throw std::runtime_error("ygp::nimbus::nimbus (function InternetConnectA): "+ext_lasterror<std::string>("wininet.dll"));
            if(!compo.dwUrlPathLength)strncpy(szurlpath,"/",2);
            hRequest=HttpOpenRequestA(hConnect,meth,szurlpath,"HTTP/1.1","",nullptr,compo.nScheme==INTERNET_SCHEME::INTERNET_SCHEME_HTTPS?opt|0x843200ul:opt,0ul);
            if(!hRequest)
                throw std::runtime_error("ygp::nimbus::nimbus (function HttpOpenRequestA): "+ext_lasterror<std::string>("wininet.dll"));
            delete []szhostname;
            delete []szusername;
            delete []szpassword;
            delete []szurlpath;
            header("User-Agent","Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36");
                //Disguise itself as an explorer
        }
        nimbus& header(const char* k1,const char* k2)
        {
            DWORD dwlen=strlen(k1)+strlen(k2)+6;
            char* szhd=new char[dwlen]{'\0'};
            strcpy(szhd,k1);
            strcat(szhd,": ");
            strcat(szhd,k2);
            strcat(szhd,"\r\n");
            if(!HttpAddRequestHeadersA(hRequest,szhd,0xffffffff,0ul))
                throw std::runtime_error("ygp::nimbus::header (function HttpAddRequestHeadersA): "+ext_lasterror<std::string>("wininet.dll"));
            delete []szhd;
            return *this;
        }
        nimbus& header(const char* szhd)
        {
            if(!HttpAddRequestHeadersA(hRequest,szhd,0xffffffff,0ul))
                throw std::runtime_error("ygp::nimbus::header (function HttpAddRequestHeadersA): "+ext_lasterror<std::string>("wininet.dll"));
            return *this;
        }
        std::shared_ptr<char> headers()
        {
            DWORD dwerr;
            DWORD dwsize;
            HttpQueryInfoA(hRequest,HTTP_QUERY_RAW_HEADERS_CRLF,nullptr,&dwsize,nullptr);
            if((dwerr=GetLastError())!=ERROR_INSUFFICIENT_BUFFER)
                throw std::runtime_error("ygp::nimbus::headers (function HttpQueryInfoA): "+ext_lasterror<std::string>("wininet.dll"));
            std::shared_ptr<char> ret(new char[dwsize]);
            HttpQueryInfoA(hRequest,HTTP_QUERY_RAW_HEADERS_CRLF,ret.get(),&dwsize,nullptr);
            return ret;
        }
        nimbus& send(const char* hds=nullptr,void* data=nullptr,DWORD dwdatalen=0ul)//hds: headers
        {
            if(!HttpSendRequestA(hRequest,hds,hds?0xfffffffful:0ul,data,dwdatalen))
                throw std::runtime_error("ygp::nimbus::send (function HttpSendRequestA): "+ext_lasterror<std::string>("wininet.dll"));
            return *this;
        }
        DWORD read(void* buf,DWORD len)
        {
            DWORD dwret;
            if(!InternetReadFile(hRequest,buf,len,&dwret))
                throw std::runtime_error("ygp::nimbus::read (function InternetReadFile): "+ext_lasterror<std::string>("wininet.dll"));
            return dwret;
        }
        DWORD write(const void* buf,DWORD len)
        {
            DWORD dwret;
            if(!InternetWriteFile(hRequest,buf,len,&dwret))
                throw std::runtime_error("ygp::nimbus::write (function InternetWriteFile): "+ext_lasterror<std::string>("wininet.dll"));
            return dwret;
        }
        DWORD postdata(const void* buf,DWORD len)
        {
            INTERNET_BUFFERSA inbuf;
            zmem(inbuf);
            inbuf.dwStructSize=sizeof(inbuf);
            inbuf.dwBufferTotal=len;
            if(!HttpSendRequestExA(hRequest,&inbuf,nullptr,HSR_INITIATE,0ul))
                throw std::runtime_error("ygp::nimbus::postdata (function HttpSendRequestExA): "+ext_lasterror<std::string>("wininet.dll"));
            return write(buf,len);
        }
        nimbus& endrequest()
        {
            if(!HttpEndRequestA(hRequest,nullptr,0ul,0ul))
                throw std::runtime_error("ygp::nimbus::endrequest (function HttpEndRequestA): "+ext_lasterror<std::string>("wininet.dll"));
            return *this;
        }
        DWORD status()//Status code
        {
            DWORD dwdw=sizeof(DWORD);
            DWORD ret;
            if(!HttpQueryInfoA(hRequest,HTTP_QUERY_FLAG_NUMBER|HTTP_QUERY_STATUS_CODE,&ret,&dwdw,nullptr))
                throw std::runtime_error("ygp::nimbus::status (function HttpQueryInfoA): "+ext_lasterror<std::string>("wininet.dll"));
            return ret;
        }
        std::uint64_t content_length()//Content-Length item in response header
        {
            DWORD dwsize;
            DWORD dwerr;
            HttpQueryInfoA(hRequest,HTTP_QUERY_CONTENT_LENGTH,nullptr,&dwsize,nullptr);
            if((dwerr=GetLastError())!=ERROR_INSUFFICIENT_BUFFER)
                throw std::runtime_error("ygp::nimbus::headers (function HttpQueryInfoA): "+ext_lasterror<std::string>("wininet.dll"));
            char* ret=new char[dwsize];
            HttpQueryInfoA(hRequest,HTTP_QUERY_CONTENT_LENGTH,ret,&dwsize,nullptr);
            return strtoull(ret,nullptr,10);
        }
        INTERNET_PORT port()const noexcept
        {
            return m_port;
        }
        ~nimbus()
        {
            InterlockedDecrement(&detail::hc.dwref);
            if(!detail::hc.dwref&&detail::hc.hm)FreeLibrary(detail::hc.hm);
            if(hRequest)InternetCloseHandle(hRequest);
            if(hConnect)InternetCloseHandle(hConnect);
            if(hSession)InternetCloseHandle(hSession);
        }
};
END_NAMESPACE_YGP
#endif