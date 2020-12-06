#ifndef _YGP_WIN_DEMENTOR_HPP_
#define _YGP_WIN_DEMENTOR_HPP_
//Encapsulation of IXMLHttpRequest interface
//Required libraries: libole32.a liboleaut32.a libuuid.a
#ifdef _MSC_VER
#pragma comment(lib,"ole32")
#pragma comment(lib,"oleaut32")
#pragma comment(lib,"uuid")
#endif
#include"winerr.hpp"
#include<msxml2.h>
#include<comdef.h>
#include<string_view>
BEGIN_NAMESPACE_YGP
#ifndef YGP_DISABLE_COM
struct dmt_bstr
{
    BSTR m_str;
    dmt_bstr(BSTR _BStr):m_str(_BStr){}
    dmt_bstr(){m_str=nullptr;}
    operator const wchar_t*()const noexcept{return m_str;}
    const wchar_t* c_str()const noexcept{return m_str;}
    operator std::wstring_view()const noexcept{return m_str;}
    void free()noexcept{if(m_str)SysFreeString(m_str),m_str=nullptr;}
    std::size_t size()const noexcept{return wcslen(m_str);}
    ~dmt_bstr(){free();}
};
enum dementortype
{
    string,
    stream
};
class basic_dementor
{
    protected:
        IXMLHttpRequest* xhr;
    public:
        virtual void initialize(wchar_t* meth,wchar_t* url)
        {
            throw_com_error(CoCreateInstance(CLSID_XMLHTTPRequest,nullptr,CLSCTX_INPROC,IID_IXMLHttpRequest,reinterpret_cast<void**>(&xhr)),"basic_dememtor::initialize","CoCreateInstance");
            _variant_t _async(false);
            throw_com_error(xhr->open(meth,url,_async,_variant_t(),_variant_t()),"basic_dementor::initialize","IXMLHttpRequest::open");
        }
        long status()
        {
            long ret;
            throw_com_error(xhr->get_status(&ret),"basic_dementor::status","IXMLHttpRequest::get_status");
            return ret;
        }
        dmt_bstr statustext()
        {
            BSTR ret;
            throw_com_error(xhr->get_statusText(&ret),"basic_dementor::statustext","IXMLHttpRequest::get_statusText");
            return ret;
        }
        void perform(_variant_t vbody=_variant_t())
        {
            throw_com_error(xhr->send(vbody),"basic_dementor::perform","IXMLHttpRequest::send");
        }
        dmt_bstr headers()
        {
            BSTR ret;
            throw_com_error(xhr->getAllResponseHeaders(&ret),"basic_dementor::headers","IXMLHttpRequest::getAllResponseHeaders");
            return ret;
        }
        LONG state()
        {
            long ret;
            throw_com_error(xhr->get_readyState(&ret),"basic_dementor::state","IXMLHttpRequest::state");
            return ret;
        }
        void header(wchar_t* item,wchar_t* value)
        {
            throw_com_error(xhr->setRequestHeader(item,value),"basic_dementor::header(const wchar_t*,const wchar_t*)","IXMLHttpRequest::setRequestHeader");
        }
        dmt_bstr header(wchar_t* item)
        {
            BSTR ret;
            throw_com_error(xhr->getResponseHeader(item,&ret),"basic_dementor::header","IXMLHttpRequest::getResponseHeader");
            return ret;
        }
        unsigned long long content_length()
        {
            return std::stoull(std::wstring(header(L"Content-Length")));
        }
        void set_content_range(unsigned long long _start,unsigned long long _end)
        {
            wchar_t buf[64];
            swprintf(buf,64,L"bytes=%llu-%llu",_start,_end);
            header(L"Range",buf);
        }
        virtual void cleanup()=0;
};
template<dementortype dmttype=dementortype::string>class dementor;
template<>
class dementor<dementortype::string>:public basic_dementor
{
    public:
        dementor(){}
        dementor(wchar_t* meth,wchar_t* url)
        {
            initialize(meth,url);
        }
        ~dementor(){cleanup();}
        void cleanup()override
        {
            if(xhr)xhr->Release();
            xhr=nullptr;
        }
        dmt_bstr text()
        {
            BSTR ret;
            throw_com_error(xhr->get_responseText(&ret),"dementor<ygp::dementortype::string>::text","IXMLHttpRequest::get_responseText");
            return ret;
        }
};
template<>
class dementor<dementortype::stream>:public basic_dementor
{
    private:
        IStream* m_is;
    public:
        dementor(){}
        dementor(wchar_t* meth,wchar_t* url)
        {
            initialize(meth,url);
            m_is=nullptr;
        }
        ~dementor(){cleanup();}
        void cleanup()noexcept override
        {
            if(m_is)m_is->Release();
            m_is=nullptr;
            if(xhr)xhr->Release();
            xhr=nullptr;
        }
        IStream*& get()//Must call this method before calling read
        {
            VARIANT varn;
            throw_com_error(xhr->get_responseStream(&varn),"dementor<ygp::dementortype::stream>::get","IXMLHttpRequest::get_responseStream");
            return m_is=(IStream*)varn.punkVal;
        }
        ULONG read(void* buf,ULONG len)
        {
            ULONG ret;
            throw_com_error(m_is->Read(buf,len,&ret),"dementor<ygp::dementortype::read","IStream::Read");
            return ret;
        }
};
static dmt_bstr dmt_fetch(wchar_t* url)
{
    dementor<dementortype::string> dmt(L"GET",url);
    dmt.perform();
    return dmt.text();
}
#endif
END_NAMESPACE_YGP
#endif