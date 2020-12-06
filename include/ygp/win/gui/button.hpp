//Windows Common Controls Wrapper - The Button Control
#ifndef _YGP_WIN_GUI_BUTTON_HPP_
#define _YGP_WIN_GUI_BUTTON_HPP_
#include"window.hpp"
#include<commctrl.h>
BEGIN_NAMESPACE_YGP
class basic_button:public window
{
    public:
        using window::create;
        basic_button(){}
        basic_button(HWND hParent,LPCSTR lpWindowName,int nID,
            DWORD dwStyle,DWORD dwExStyle,int X,int Y,
            int nWidth,int nHeight):window(hParent,
                WC_BUTTONA,lpWindowName,nID,dwStyle,dwExStyle,
                X,Y,nWidth,nHeight){}
        basic_button(HWND hParent,LPCWSTR lpWindowName,int nID,
            DWORD dwStyle,DWORD dwExStyle,int X,int Y,
            int nWidth,int nHeight):window(hParent,
                WC_BUTTONW,lpWindowName,nID,dwStyle,dwExStyle,
                X,Y,nWidth,nHeight){}
        basic_button& create(HWND hParent,LPCSTR lpWindowName,int nID,
            DWORD dwStyle,DWORD dwExStyle,int X,int Y,
            int nWidth,int nHeight)
        {
            this->create(hParent,
                WC_BUTTONA,lpWindowName,nID,dwStyle,dwExStyle,
                X,Y,nWidth,nHeight);
            return *this;
        }
        basic_button& create(HWND hParent,LPCWSTR lpWindowName,int nID,
            DWORD dwStyle,DWORD dwExStyle,int X,int Y,
            int nWidth,int nHeight)
        {
            this->create(hParent,
                WC_BUTTONW,lpWindowName,nID,dwStyle,dwExStyle,
                X,Y,nWidth,nHeight);
        }
        SIZE idealsize()const noexcept
        {
            SIZE ret;
            Button_GetIdealSize(hwnd,&ret);
            return ret;
        }
        rect textmargin()const noexcept
        {
            rect ret;
            Button_GetTextMargin(hwnd,&ret);
            return ret;
        }
        basic_button& textmargin(const rect& rct)
        {
            Button_SetTextMargin(hwnd,&rct);
            return *this;
        }
};
class basic_commandlink:public basic_button
{
    public:
        using window::create;
        basic_commandlink(){}
        basic_commandlink(HWND hParent,LPCSTR lpWindowName,int nID,
            int X,int Y,int nWidth,int nHeight,
            bool bIsDefCommandLink=false,DWORD dwStyle=0ul,
            DWORD dwExStyle=0ul):basic_button(hParent,lpWindowName,
            nID,WS_CHILD|WS_VISIBLE|(bIsDefCommandLink?
            BS_COMMANDLINK:BS_DEFCOMMANDLINK)|dwStyle,dwExStyle,
            X,Y,nWidth,nHeight){}
        basic_commandlink(HWND hParent,LPCWSTR lpWindowName,int nID,
            int X,int Y,int nWidth,int nHeight,
            bool bIsDefCommandLink=false,DWORD dwStyle=0ul,
            DWORD dwExStyle=0ul):basic_button(hParent,lpWindowName,
            nID,WS_CHILD|WS_VISIBLE|(bIsDefCommandLink?
            BS_COMMANDLINK:BS_DEFCOMMANDLINK)|dwStyle,dwExStyle,
            X,Y,nWidth,nHeight){}
        basic_commandlink& create(HWND hParent,LPCSTR lpWindowName,int nID,
            int X,int Y,int nWidth,int nHeight,
            bool bIsDefCommandLink=false,DWORD dwStyle=0ul,
            DWORD dwExStyle=0ul)
        {
            ((basic_button*)this)->create(hParent,lpWindowName,
                nID,WS_CHILD|WS_VISIBLE|(bIsDefCommandLink?
                BS_COMMANDLINK:BS_DEFCOMMANDLINK)|dwStyle,dwExStyle,
                X,Y,nWidth,nHeight);
            return *this;
        }
        basic_commandlink& create(HWND hParent,LPCWSTR lpWindowName,int nID,
            int X,int Y,int nWidth,int nHeight,
            bool bIsDefCommandLink=false,DWORD dwStyle=0ul,
            DWORD dwExStyle=0ul)
        {
            ((basic_button*)this)->create(hParent,lpWindowName,
                nID,WS_CHILD|WS_VISIBLE|(bIsDefCommandLink?
                BS_COMMANDLINK:BS_DEFCOMMANDLINK)|dwStyle,dwExStyle,
                X,Y,nWidth,nHeight);
        }
        basic_commandlink& setnote(LPCWSTR lpszNote)noexcept
        {
            sendmsgw(BCM_SETNOTE,0,(LPARAM)lpszNote);
                //only wide-char version is available
            return *this;
        }
        basic_commandlink& setnote(const std::wstring& strNote)
            noexcept
        {
            return setnote(strNote.c_str());
        }
        std::size_t notelength()noexcept
        {
            return (std::size_t)sendmsgw(BCM_GETNOTELENGTH);
        }
        std::wstring getnote()
        {
            std::size_t nlength=notelength()+1;
            wchar_t* wch=new wchar_t[nlength];
            if(!Button_GetNote(hwnd,wch,&nlength))
                throw std::runtime_error("ygp::basic_commandlink"
                    "::getnote (The reply to BCM_GETNOTE message "
                    "returned FALSE): "
                    +lasterror<std::string>());
            std::wstring ret(wch);
            delete []wch;
            return ret;
        }
};
class basic_checkbutton:public basic_button
{
    public:
        using basic_button::create;
        basic_checkbutton(){}
        basic_checkbutton(HWND hParent,LPCSTR lpWindowName,int nID,
            int X,int Y,int nWidth,int nHeight):basic_button(){}
};
END_NAMESPACE_YGP
#endif