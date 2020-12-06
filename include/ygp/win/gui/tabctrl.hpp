//Windows Common Controls Wrapper - The Tab Control
#ifndef _YGP_WIN_GUI_TABCTRL_HPP_
#define _YGP_WIN_GUI_TABCTRL_HPP_
#include"window.hpp"
#include<commctrl.h>
BEGIN_NAMESPACE_YGP
class basic_tabctrl:public window
{
    public:
        using window::create;
        basic_tabctrl(){}
        basic_tabctrl(HWND hParent,LPCSTR lpWindowName,int nID,int X,int Y,
            int nWidth,int nHeight,DWORD dwStyle=0ul,DWORD dwExStyle=0ul):
            window(hParent,WC_TABCONTROLA,lpWindowName,nID,
            dwStyle|WS_CHILD|WS_VISIBLE,dwExStyle,X,Y,nWidth,nHeight){}
        basic_tabctrl(HWND hParent,LPCWSTR lpWindowName,int nID,int X,int Y,
            int nWidth,int nHeight,DWORD dwStyle=0ul,DWORD dwExStyle=0ul):
            window(hParent,WC_TABCONTROLW,lpWindowName,nID,
            dwStyle|WS_CHILD|WS_VISIBLE,dwExStyle,X,Y,nWidth,nHeight){}
        basic_tabctrl& create(HWND hParent,LPCSTR lpWindowName,int nID,
            int X,int Y,int nWidth,int nHeight,DWORD dwStyle=0ul,
            DWORD dwExStyle=0ul)
        {
            this->create(hParent,WC_TABCONTROLA,lpWindowName,nID,
                dwStyle|WS_CHILD|WS_VISIBLE,dwExStyle,X,Y,nWidth,nHeight);
            return *this;
        }
        basic_tabctrl& create(HWND hParent,LPCWSTR lpWindowName,int nID,
            int X,int Y,int nWidth,int nHeight,DWORD dwStyle=0ul,
            DWORD dwExStyle=0ul)
        {
            this->create(hParent,WC_TABCONTROLW,lpWindowName,nID,
                dwStyle|WS_CHILD|WS_VISIBLE,dwExStyle,X,Y,nWidth,nHeight);
            return *this;
        }
        int getcursel()const noexcept//returns -1 if no tab is selected
        {
            return TabCtrl_GetCurSel(hwnd);
        }
        int setcursel(int nSel)const noexcept//returns the previous selected tab
        {
            return TabCtrl_SetCurSel(hwnd,nSel);
        }
        struct itema:TCITEMA//ASCII version
        {
            itema():TCITEMA{0ul}{}
            itema& settext(LPSTR _pszText)noexcept
            {
                mask|=TCIF_TEXT;
                pszText=_pszText;
                return *this;
            }
            LPSTR gettext()const noexcept
            {
                return pszText;
            }
        };
        struct itemw:TCITEMW//Unicode version
        {
            itemw():TCITEMW{0ul}{}
            itemw& settext(LPWSTR _pszText)noexcept
            {
                mask|=TCIF_TEXT;
                pszText=_pszText;
                return *this;
            }
            LPWSTR gettext()const noexcept
            {
                return pszText;
            }
        };
        int insert(int iIndex,const itema& itm)
        {
            int res=sendmsga(TCM_INSERTITEMA,iIndex,(LPARAM)&itm);
            if(res==-1)
                throw std::runtime_error("ygp::basic_tabctrl::insert: "
                    "The reply to the TCM_INSERTITEMA message returned -1, "
                    "indicating the control failed to insert the item.");
            return res;
        }
        int insert(int iIndex,const itemw& itm)
        {
            int res=sendmsgw(TCM_INSERTITEMW,iIndex,(LPARAM)&itm);
            if(res==-1)
                throw std::runtime_error("ygp::basic_tabctrl::insert: "
                    "The reply to the TCM_INSERTITEMW message returned -1, "
                    "indicating the control failed to insert the item.");
            return res;
        }
        int insert(int iIndex,const char* _text)
        {
            itema tmp;
            tmp.mask|=TCIF_TEXT;
            tmp.pszText=new char[strlen(_text)+1];
            strcpy(tmp.pszText,_text);
            int res=sendmsga(TCM_INSERTITEMA,iIndex,(LPARAM)&tmp);
            delete []tmp.pszText;
            if(res==-1)
                throw std::runtime_error("ygp::basic_tabctrl::insert: "
                    "The reply to the TCM_INSERTITEMA message returned -1, "
                    "indicating the control failed to insert the item.");
            return res;
        }
        int insert(int iIndex,const wchar_t* _text)
        {
            itemw tmp;
            tmp.mask|=TCIF_TEXT;
            tmp.pszText=new wchar_t[wcslen(_text)+1];
            wcscpy(tmp.pszText,_text);
            int res=sendmsgw(TCM_INSERTITEMW,iIndex,(LPARAM)&tmp);
            delete []tmp.pszText;
            if(res==-1)
                throw std::runtime_error("ygp::basic_tabctrl::insert: "
                    "The reply to the TCM_INSERTITEMW message returned -1, "
                    "indicating the control failed to insert the item.");
            return res;
        }
};
END_NAMESPACE_YGP
#endif