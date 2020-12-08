//EXTREME ATTENTION: Call ygp::com_uninit() AFTER YGP_CATCH!
//Because ygp::com_uninit() is a noexcept function
//and you cannot call Release method of any COM objects after it!
#ifndef _YGP_WIN_WEBBROWSER_HPP_
#define _YGP_WIN_WEBBROWSER_HPP_
#include"winerr.hpp"
#include<exdisp.h>
#include<mshtml.h>
BEGIN_NAMESPACE_YGP
#ifndef YGP_DISABLE_COM
struct htmlinputelement
{
    IHTMLInputElement* p;
    htmlinputelement():p(nullptr){}
    htmlinputelement(IHTMLInputElement* _p):p(_p){}
    htmlinputelement(IHTMLElement* pel)
    {
        throw_com_error(pel->QueryInterface(IID_IHTMLInputElement,
            (void**)&p),"htmlinputelement::htmlinputelement",
            "IHTMLElement::QueryInterface");
    }
    operator IHTMLInputElement*()const noexcept{return p;}
    htmlinputelement& value(const wchar_t* szval)
    {
        BSTR bsval=SysAllocString(szval);
        throw_com_error(p->put_value(bsval),"htmlinputelement::value",
            "IHTMLInputElement::put_value");
        SysFreeString(bsval);
        return *this;
    }
    std::wstring value()
    {
        BSTR ret;
        throw_com_error(p->get_value(&ret),"htmlinputelement::value",
            "IHTMLInputElement::get_value");
        std::wstring wstrt(ret);
        SysFreeString(ret);
        return wstrt;
    }
    ~htmlinputelement()
    {
        if(p)p->Release();
    }
};
struct htmlelement
{
    IHTMLElement* p;
    IHTMLElement2* p2;
    htmlelement():p(nullptr),p2(nullptr){}
    htmlelement(IHTMLElement* _p):p(_p),p2(nullptr){}
    htmlelement(const htmlelement& _el)
    {
        p=_el.p;
        p->AddRef();
        p2=_el.p2;
        if(p2)p2->AddRef();
    }
    operator IHTMLElement*()const noexcept{return p;}
    htmlelement& click()
    {
        throw_com_error(p->click(),"htmlelement::click",
            "IHTMLElement::click");
        return *this;
    }
    std::wstring id()
    {
        BSTR tmp;
        throw_com_error(p->get_id(&tmp),"htmlelement::id",
            "IHTMLElement::get_id");
        if(!tmp)throw std::runtime_error("ygp::htmlelement::id"
            " (function IHTMLElement::get_id): "
            "The element does not have an id.");
        std::wstring ret(tmp);
        SysFreeString(tmp);
        return ret;
    }
    std::wstring title()
    {
        BSTR tmp;
        throw_com_error(p->get_title(&tmp),"htmlelement::title",
            "IHTMLElement::get_title");
        if(!tmp)throw std::runtime_error("ygp::htmlelement::title"
            " (function IHTMLElement::get_title): "
            "The element does not have a title.");
        std::wstring ret(tmp);
        SysFreeString(tmp);
        return ret;
    }
    std::wstring classname()
    {
        BSTR tmp;
        throw_com_error(p->get_className(&tmp),"htmlelement::id",
            "IHTMLElement::get_className");
        if(!tmp)throw std::runtime_error("ygp::htmlelement::classname"
            " (function IHTMLElement::get_className): "
            "The element does not have a class name.");
        std::wstring ret(tmp);
        SysFreeString(tmp);
        return ret;
    }
    IHTMLElement* parent()
    {
        IHTMLElement* ret;
        throw_com_error(p->get_parentElement(&ret),
            "htmlelement::parent",
            "IHTMLElement::get_parentElement");
        return ret;
    }
    std::wstring outerhtml()
    {
        BSTR ret;
        throw_com_error(p->get_outerHTML(&ret),"htmlelement::outerhtml",
            "IHTMLElement::get_outerHTML");
        std::wstring wstrt(ret);
        SysFreeString(ret);
        return wstrt;
    }
    htmlelement& innerhtml(const wchar_t* szval)
    {
        BSTR bsval=SysAllocString(szval);
        throw_com_error(p->put_innerHTML(bsval),"htmlinputelement::innerhtml",
            "IHTMLInputElement::put_innerHTML");
        SysFreeString(bsval);
        return *this;
    }
    std::wstring innerhtml()
    {
        BSTR ret;
        throw_com_error(p->get_innerHTML(&ret),"htmlelement::innerhtml",
            "IHTMLElement::get_innerHTML");
        std::wstring wstrt(ret);
        SysFreeString(ret);
        return wstrt;
    }
    std::wstring innertext()
    {
        BSTR ret;
        throw_com_error(p->get_innerText(&ret),"htmlelement::innertext",
            "IHTMLElement::get_innerText");
        std::wstring wstrt(ret);
        SysFreeString(ret);
        return wstrt;
    }
    htmlelement& outerhtml(const wchar_t* szval)
    {
        BSTR bsval=SysAllocString(szval);
        throw_com_error(p->put_outerHTML(bsval),"htmlinputelement::outerhtml",
            "IHTMLInputElement::put_outerHTML");
        SysFreeString(bsval);
        return *this;
    }
    std::wstring tagname()
    {
        BSTR ret;
        throw_com_error(p->get_tagName(&ret),"htmlelement::tagname",
            "IHTMLElement::get_tagName");
        std::wstring wstrt(ret);
        SysFreeString(ret);
        return wstrt;
    }
    std::wstring tostring()
    {
        BSTR ret;
        throw_com_error(p->toString(&ret),"htmlelement::tostring",
            "IHTMLElement::toString");
        std::wstring wstrt(ret);
        SysFreeString(ret);
        return wstrt;
    }
    IHTMLElementCollection* children()
    {
        IDispatch* tmp;
        throw_com_error(p->get_children(&tmp),"htmlelement::children",
            "IHTMLElement::get_children");
        if(!tmp)throw std::runtime_error("ygp::htmlelement::children"
            " (function IHTMLElement::get_children): "
            "This element has no children.");
        IHTMLElementCollection* ret;
        throw_com_error(tmp->QueryInterface(IID_IHTMLElementCollection,
            (void**)&ret),"htmlelement::children",
            "IHTMLElement::children");
        tmp->Release();
        return ret;
    }
    long readystateval()
    {
        if(!p2)throw_com_error(p->QueryInterface(IID_IHTMLElement2,
            (void**)&p2),"htmlelement::readystateval",
            "IHTMLElement::QueryInterface");
        long ret;
        throw_com_error(p2->get_readyStateValue(&ret),
            "htmlelement::readystateval",
            "IHTMLElement2::get_readyStateValue");
        return ret;
    }
    htmlelement& operator=(IHTMLElement* _elem)
    {
        if(p)p->Release();
        if(p2)p2->Release();
        p=_elem;
        p2=nullptr;
        return *this;
    }
    ~htmlelement()
    {
        if(p)p->Release();
        if(p2)p2->Release();
    }
};
struct htmlelements//Element collection
{
    IHTMLElementCollection* p;
    htmlelements():p(nullptr){}
    htmlelements(IHTMLElementCollection* _p):p(_p){}
    operator IHTMLElementCollection*()const noexcept{return p;}
    std::size_t size()
    {
        long ret;
        throw_com_error(p->get_length(&ret),
            "htmlelements::get_length",
            "IHTMLElementCollection::size");
        return (std::size_t)ret;
    }
    IHTMLElement* operator()(std::size_t _index)//get element by index
    {
        VARIANT vari;
        VariantInit(&vari);
        vari.vt=VT_I4;
        vari.lVal=_index;
        IDispatch* tmp;
        throw_com_error(p->item(vari,vari,&tmp),
            "htmlelements::operator()","IHTMLElementCollection::item");
        VariantClear(&vari);
        if(!tmp)throw std::runtime_error("ygp::htmlelements::operator()"
            " (function IHTMLElementCollection::item): "
            "Failed to retrieve the specified item.");
        IHTMLElement* ret;
        throw_com_error(tmp->QueryInterface(IID_IHTMLElement,(void**)&ret),
            "htmlelements::operator()","IDispatch::QueryInterface");
        tmp->Release();
        return ret;
    }
    std::wstring tostring()
    {
        BSTR ret;
        throw_com_error(p->toString(&ret),"htmlelements::tostring",
            "IHTMLElementCollection::toString");
        std::wstring wstrt(ret);
        SysFreeString(ret);
        return wstrt;
    }
    ~htmlelements()
    {
        if(p)p->Release();
    }
};
struct htmlwindow
{
    IHTMLWindow2* p;
    htmlwindow():p(nullptr){}
    htmlwindow(IHTMLWindow2* _p):p(_p){}
    operator IHTMLWindow2*()const noexcept{return p;}
    VARIANT execscript(const wchar_t* content,const wchar_t* sctype
        =L"JavaScript")
    {
        VARIANT ret;
        BSTR bsctt=SysAllocString(content);
        BSTR bsctp=SysAllocString(sctype);
        throw_com_error(p->execScript(bsctt,bsctp,&ret),
            "htmlwindow::execscript","IHTMLWindow2::execScript");
        SysFreeString(bsctt);
        SysFreeString(bsctp);
        return ret;
    }
    std::wstring tostring()
    {
        BSTR ret;
        throw_com_error(p->toString(&ret),"htmlwindow::tostring",
            "IHTMLWindow2::toString");
        std::wstring wstrt(ret);
        SysFreeString(ret);
        return wstrt;
    }
    htmlwindow& scroll(long _x=0,long _y=0)
    {
        throw_com_error(p->scroll(_x,_y),"htmlwindow::scroll",
            "IHTMLWindow::scroll");
        return *this;
    }
    htmlwindow& scrollto(long _x=0,long _y=0)
    {
        throw_com_error(p->scrollTo(_x,_y),"htmlwindow::scrollto",
            "IHTMLWindow::scrollTo");
        return *this;
    }
    ~htmlwindow()
    {
        if(p)p->Release();
    }
};
struct htmldoc
{
    IHTMLDocument2* p;
    IHTMLDocument3* doc3;
    htmldoc():p(nullptr),doc3(nullptr){}
    htmldoc(IHTMLDocument2* _p):p(_p),doc3(nullptr){}
    operator IHTMLDocument2*()const noexcept{return p;}
    IHTMLWindow2* getwindow()
    {
        IHTMLWindow2* ret;
        throw_com_error(p->get_parentWindow(&ret),"htmldoc::getwindow",
            "IHTMLDocument2::get_parentWindow");
        return ret;
    }
    IHTMLElement* getelementbyid(const wchar_t* lpszid)
    {
        if(!doc3)throw_com_error(p->QueryInterface(IID_IHTMLDocument3,
            (void**)&doc3),"htmldoc::getelementbyid",
            "IHTMLDocument2::QueryInterface");
        IHTMLElement* ret;
        BSTR bstrid=SysAllocString(lpszid);
        throw_com_error(doc3->getElementById(bstrid,&ret),
            "htmldoc::getelementbyid",
            "IHTMLDocument3::getElementById");
        SysFreeString(bstrid);
        if(!ret)throw std::runtime_error(
            "ygp::htmldoc::getelementbyid "
            "(function IHTMLDocument3::getElementById): "
            "Could not find the element associated with the given ID.");
        return ret;
    }
    IHTMLElementCollection* getelementsbyname(const wchar_t* _name)
    {
        if(!doc3)throw_com_error(p->QueryInterface(IID_IHTMLDocument3,
            (void**)&doc3),"htmldoc::getelementbyid",
            "IHTMLDocument2::QueryInterface");
        IHTMLElementCollection* ret;
        BSTR bstrnm=SysAllocString(_name);
        throw_com_error(doc3->getElementsByName(bstrnm,&ret),
            "htmldoc::getelementsbyname",
            "IHTMLDocument3::getElementsByName");
        SysFreeString(bstrnm);
        if(!ret)throw std::runtime_error(
            "ygp::htmldoc::getelementsbyname "
            "(function IHTMLDocument3::getElementsByName): "
            "Could not find the element associated with the given name.");
        return ret;
    }
    IHTMLElementCollection* getelementsbytagname(const wchar_t* _name)
    {
        if(!doc3)throw_com_error(p->QueryInterface(IID_IHTMLDocument3,
            (void**)&doc3),"htmldoc::getelementbyid",
            "IHTMLDocument2::QueryInterface");
        IHTMLElementCollection* ret;
        BSTR bstrnm=SysAllocString(_name);
        throw_com_error(doc3->getElementsByTagName(bstrnm,&ret),
            "htmldoc::getelementsbytagname",
            "IHTMLDocument3::getElementsByTagName");
        SysFreeString(bstrnm);
        if(!ret)throw std::runtime_error(
            "ygp::htmldoc::getelementsbytagname "
            "(function IHTMLDocument3::getElementsByTagName): "
            "Could not find the element associated with the given tag name.");
        return ret;
    }
    IHTMLElement* elementfrompoint(long _x,long _y)
    {
        IHTMLElement* ret;
        throw_com_error(p->elementFromPoint(_x,_y,&ret),
            "htmldoc::elementfrompoint",
            "IHTMLDocument2::elementFromPoint");
        if(!ret)throw std::runtime_error("ygp::htmldoc::elementfrompoint"
            " (function IHTMLDocument2::elementFromPoint): "
            "Could not find the corresponding element at the given point.");
        return ret;
    }
    IHTMLElement* body()
    {
        IHTMLElement* ret;
        throw_com_error(p->get_body(&ret),
            "htmldoc::body",
            "IHTMLDocument2::get_body");
        if(!ret)throw std::runtime_error("ygp::htmldoc::body"
            " (function IHTMLDocument2::get_body): "
            "No <body> element could be found.");
        return ret;
    }
    std::wstring readystate()
    {
        BSTR tmp;
        throw_com_error(p->get_readyState(&tmp),
            "htmldoc::readystate",
            "IHTMLDocument2::get_readyState");
        std::wstring ret(tmp);
        SysFreeString(tmp);
        return ret;
    }
    std::wstring tostring()
    {
        BSTR ret;
        throw_com_error(p->toString(&ret),"htmldoc::tostring",
            "IHTMLDocument2::toString");
        std::wstring wstrt(ret);
        SysFreeString(ret);
        return wstrt;
    }
    ~htmldoc()
    {
        if(p)p->Release();
        if(doc3)doc3->Release();
    }
};
class webbrowser
{
    public:
        IWebBrowser2* p;
        webbrowser()
        {
            p=cocrtins<IWebBrowser2>(CLSID_InternetExplorer,
                IID_IWebBrowser2,CLSCTX_LOCAL_SERVER);
        }
        webbrowser& navigate(const wchar_t* url,const wchar_t*
            headers=nullptr)
        {
            BSTR burl=SysAllocString(url);
            VARIANT var;
            VARIANT varh;
            VariantInit(&var);
            VariantInit(&varh);
            if(headers)
            {
                varh.vt=VT_BSTR;
                varh.bstrVal=SysAllocString(headers);
            }
            throw_com_error(p->Navigate(burl,&var,&var,&var,&varh),
                "webbrowser::navigate","IWebBrowser2::Navigate");
            if(varh.bstrVal)SysFreeString(varh.bstrVal);
            VariantClear(&varh);
            VariantClear(&var);
            SysFreeString(burl);
            return *this;
        }
        bool visible()
        {
            VARIANT_BOOL varbl;
            throw_com_error(p->get_Visible(&varbl),"webbrowser::visible",
                "IWebBrowser2::get_Visible");
            return varbl==VARIANT_TRUE;
        }
        webbrowser& visible(bool bvs)
        {
            throw_com_error(p->put_Visible(bvs?VARIANT_TRUE:
                VARIANT_FALSE),"webbrowser::visible",
                "IWebBrowser2::put_Visible");
            return *this;
        }
        bool busy()
        {
            VARIANT_BOOL varbl;
            throw_com_error(p->get_Busy(&varbl),"webbrowser::busy",
                "IWebBrowser2::get_Busy");
            return varbl==VARIANT_TRUE;
        }
        HWND hwnd()
        {
            HWND ret;
            throw_com_error(p->get_HWND((SHANDLE_PTR*)&ret),
                "webbrowser::hwnd","IWebBrowser2::get_HWND");
            return ret;
        }
        IHTMLDocument2* doc()
        //Warning: You cannot retrieve the document until the document
        //         is completed.
        //         This is a way to ensure a document is
        //         completed:
        //             //C++ Code
        //             webbrowser brs;
        //             brs.navigate(L"https://www.bing.com/");
        //             while(brs.busy()&&brs.readystate()!=4)wsleep(100);
        //             htmldoc doc=brs.doc();
        {
            IDispatch* pdisp;
            IHTMLDocument2* ret;
            throw_com_error(p->get_Document(&pdisp),
                "webbrowser::doc","IWebBrowser2::get_Document");
            throw_com_error(pdisp->QueryInterface(IID_IHTMLDocument2,
                (void**)&ret),"webbrowser::doc",
                "IDispatch::QueryInterface");
            return ret;
        }
        READYSTATE readystate()
        {
            READYSTATE ret;
            throw_com_error(p->get_ReadyState(&ret),
                "webbrowser::readystate","IWebBrowser2::get_ReadyState");
            return ret;
        }
        webbrowser& refresh()
        {
            throw_com_error(p->Refresh(),"webbrowser::refresh",
                "IWebBrowser2::Refresh");
            return *this;
        }
        webbrowser& stop()
        {
            throw_com_error(p->Stop(),"webbrowser::stop",
                "IWebBrowser2::Stop");
            return *this;
        }
        webbrowser& quit()
        {
            throw_com_error(p->Quit(),"webbrowser::quit",
                "IWebBrowser2::Quit");
            return *this;
        }
        ~webbrowser()
        {
            if(p)p->Release();
        }
};
#endif
END_NAMESPACE_YGP
#endif