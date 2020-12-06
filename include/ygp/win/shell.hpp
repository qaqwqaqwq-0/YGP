#ifndef _YGP_WIN_SHELL_HPP_
#define _YGP_WIN_SHELL_HPP_
#include"winerr.hpp"
#include<shlobj.h>
#include<shobjidl.h>
#ifdef _MSC_VER
#pragma comment(lib,"shell32")
#endif
BEGIN_NAMESPACE_YGP
struct notifydata
{
    NOTIFYICONDATAA n;
    notifydata()
    {
        zmem(n);
        n.cbSize=sizeof(n);
    }
    void operator()(DWORD dwMessage=NIM_ADD)
    {
        if(!Shell_NotifyIconA(dwMessage,&n))
            throw std::runtime_error("ygp::notifydata::operator() "
                "(function Shell_NotifyIconA): "+lasterror<std::string>());
    }
    void icon(HICON hIcon)noexcept
    {
        n.uFlags|=NIF_ICON;
        n.hIcon=hIcon;
    }
    void tip(const char* tp)
    {
        n.uFlags|=NIF_TIP;
        int i=0;
        for(;i<128;++i)
            if(tp[i])n.szTip[i]=tp[i];
            else
            {
                n.szTip[i]='\0';
                break;
            }
        if(i==127)n.szTip[i]='\0';
    }
    void window(HWND hWnd)
    {
        n.hWnd=hWnd;
    }
    void msg(UINT Msg)
    {
        n.uFlags|=NIF_MESSAGE;
        n.uCallbackMessage=Msg;
    }
    void id(UINT uID)
    {
        n.uID=uID;
    }
};
#ifndef YGP_DISABLE_COM
static const UINT WM_TASKBARBUTTONCREATED=
    RegisterWindowMessageA("TaskbarButtonCreated");
    //After the window receives this WM_TASKBARBUTTONCREATED message,
    //you can use the methods related to HWND in structure taskbarlist.
struct taskbarlist
{
    ITaskbarList4* p;
    taskbarlist()
    {
        p=cocrtins<ITaskbarList4>(CLSID_TaskbarList,IID_ITaskbarList4);
        throw_com_error(p->HrInit(),"taskbarlist::taskbarlist",
            "ITaskbarList4::HrInit");
    }
    void progressvalue(HWND _hwnd,ULONGLONG ullcur,ULONGLONG ulltot)
    {
        throw_com_error(p->SetProgressValue(_hwnd,ullcur,ulltot),
            "taskbarlist::progressvalue","ITaskbarList4::SetProgressValue");
    }
    void progressstate(HWND _hwnd,TBPFLAG _fl)
    {
        throw_com_error(p->SetProgressState(_hwnd,_fl),
            "taskbarlist::progressstate","ITaskbarList4::SetProgressState");
    }
    void settabactive(HWND _hwnd,HWND _hmain)
    {
        throw_com_error(p->SetTabActive(_hwnd,_hmain,0ul),
            "taskbarlist::settabactive","ITaskbarList4::SetTabActive");
    }
    void activetab(HWND _hwnd)
    {
        throw_com_error(p->ActivateTab(_hwnd),
            "taskbarlist::activetab","ITaskbarList4::ActivateTab");
    }
    void addtab(HWND _hwnd)
    {
        throw_com_error(p->AddTab(_hwnd),
            "taskbarlist::addtab","ITaskbarList4::AddTab");
    }
    void deletetab(HWND _hwnd)
    {
        throw_com_error(p->DeleteTab(_hwnd),
            "taskbarlist::deletetab","ITaskbarList4::DeleteTab");
    }
    void activealt(HWND _hwnd)
    {
        throw_com_error(p->SetActiveAlt(_hwnd),
            "taskbarlist::activealt","ITaskbarList4::SetActiveAlt");
    }
    void fullscreen(HWND _hwnd,bool bfl=true)
    {
        throw_com_error(p->MarkFullscreenWindow(_hwnd,(BOOL)bfl),
            "taskbarlist::fullscreen","ITaskbarList4::MarkFullscreenWindow");
    }
    void setoverlayicon(HWND _hwnd,HICON _hicon,LPCWSTR lpsz=nullptr)
    {
        throw_com_error(p->SetOverlayIcon(_hwnd,_hicon,lpsz),
            "taskbarlist::seticon","ITaskbarList4::SetOverlayIcon");
    }
    void removeoverlayicon(HWND _hwnd)
    {
        setoverlayicon(_hwnd,nullptr,nullptr);
    }
    void registertab(HWND _hwnd,HWND _hmain)
    {
        throw_com_error(p->RegisterTab(_hwnd,_hmain),
            "taskbarlist::registertab","ITaskbarList4::RegisterTab");
    }
    void settaborder(HWND _hwnd,HWND _hins)
    {
        throw_com_error(p->SetTabOrder(_hwnd,_hins),
            "taskbarlist::settaborder","ITaskbarList4::SetTabOrder");
    }
    void setthumbnailclip(HWND _hwnd,RECT& rect)
    {
        throw_com_error(p->SetThumbnailClip(_hwnd,&rect),
            "taskbarlist::setthumbnailclip","ITaskbarList4::SetThumbnailClip");
    }
    void resumethumbnailclip(HWND _hwnd)
    {
        throw_com_error(p->SetThumbnailClip(_hwnd,nullptr),
            "taskbarlist::setthumbnailclip","ITaskbarList4::SetThumbnailClip");
    }
    void thumbnailtooltip(HWND _hwnd,LPCWSTR lpszTip)
    {
        throw_com_error(p->SetThumbnailTooltip(_hwnd,lpszTip),
            "taskbarlist::thumbnailtooltip",
            "ITaskbarList4::SetThumbnailTooltip");
    }
    void thumbbaraddbuttons(HWND _hwnd,LPTHUMBBUTTON lpbtn,UINT cbtn)
    {
        throw_com_error(p->ThumbBarAddButtons(_hwnd,cbtn,lpbtn),
            "taskbarlist::thumbbaraddbuttons",
            "ITaskbarList4::ThumbBarAddButtons");
    }
    void setimglist(HWND _hwnd,HIMAGELIST himl)
    {
        throw_com_error(p->ThumbBarSetImageList(_hwnd,himl),
            "taskbarlist::setimglist",
            "ITaskbarList4::ThumbBarSetImageList");
    }
    void thumbbarupdatebuttons(HWND _hwnd,LPTHUMBBUTTON lpbtn,UINT cbtn)
    {
        throw_com_error(p->ThumbBarUpdateButtons(_hwnd,cbtn,lpbtn),
            "taskbarlist::thumbbarupdatebuttons",
            "ITaskbarList4::ThumbBarUpdateButtons");
    }
    void unregistertab(HWND _hwnd)
    {
        throw_com_error(p->UnregisterTab(_hwnd),
            "taskbarlist::unregistertab","ITaskbarList4::UnregisterTab");
    }
    void settabproperties(HWND _hwnd,STPFLAG _sfl)
    {
        throw_com_error(p->SetTabProperties(_hwnd,_sfl),
            "taskbarlist::settabproperties","ITaskbarList4::SetTabProperties");
    }
    ~taskbarlist()
    {
        if(p)p->Release();
        p=nullptr;
    }
};
struct basic_shellitem
{
    IShellItem* p;
    basic_shellitem():p(nullptr){}
    basic_shellitem(IShellItem* _p):p(_p){}
    basic_shellitem(LPCWSTR lpszPath)
    {
        PIDLIST_ABSOLUTE pidl;
        throw_com_error(
            SHParseDisplayName(lpszPath,nullptr,&pidl,SFGAO_FOLDER,nullptr),
            "basic_shellitem::basic_shellitem",
            "SHParseDisplayName");
        throw_com_error(SHCreateShellItem(nullptr,nullptr,pidl,&p),
            "basic_shellitem::basic_shellitem","SHCreateShellItem");
    }
    void parent(basic_shellitem& itm)
    {
        throw_com_error(p->GetParent(&itm.p),"basic_shellitem::parent",
            "IShellItem::GetParent");
    }
    #define YGP_BASIC_SHELLITEM_DECL_DISPLAYMETHOD(f,o) \
        std::wstring f() \
        { \
            LPWSTR tmp; \
            throw_com_error(p->GetDisplayName(o,&tmp), \
                "basic_shellitem::"#f,"IShellItem::GetDisplayName"); \
            std::wstring ret=tmp; \
            CoTaskMemFree(tmp); \
            return ret; \
        }
	YGP_BASIC_SHELLITEM_DECL_DISPLAYMETHOD(str,SIGDN_NORMALDISPLAY)
	YGP_BASIC_SHELLITEM_DECL_DISPLAYMETHOD(parentrelativestr,
        SIGDN_PARENTRELATIVEPARSING)
	YGP_BASIC_SHELLITEM_DECL_DISPLAYMETHOD(desktoprelativestr,
        SIGDN_DESKTOPABSOLUTEPARSING)
	YGP_BASIC_SHELLITEM_DECL_DISPLAYMETHOD(parentrelativeediting,
        SIGDN_PARENTRELATIVEEDITING)
	YGP_BASIC_SHELLITEM_DECL_DISPLAYMETHOD(desktoprelativeediting,
        SIGDN_DESKTOPABSOLUTEEDITING)
	YGP_BASIC_SHELLITEM_DECL_DISPLAYMETHOD(path,SIGDN_FILESYSPATH)
	YGP_BASIC_SHELLITEM_DECL_DISPLAYMETHOD(url,SIGDN_URL)
    #undef YGP_BASIC_SHELLITEM_DECL_DISPLAYMETHOD
};
struct shellitem:basic_shellitem
{
    shellitem(){}
    shellitem(IShellItem* _p):basic_shellitem(_p){}
    shellitem(LPCWSTR lpszPath):basic_shellitem(lpszPath){}
    shellitem(const basic_shellitem& itm){p=itm.p;}
    shellitem(const basic_shellitem&& itm){p=itm.p;}
    ~shellitem()
    {
        if(p)p->Release();
        p=nullptr;
    }
};
struct basic_shellitemarray
{
    IShellItemArray* p;
    basic_shellitemarray():p(nullptr){}
    basic_shellitemarray(IShellItemArray* _p):p(_p){}
    DWORD size()
    {
        DWORD ret;
        throw_com_error(p->GetCount(&ret),"basic_shellitemarray::size",
            "IShellItemArray::GetCount");
        return ret;
    }
    basic_shellitem operator[](DWORD dwindex)
    {
        basic_shellitem ret;
        throw_com_error(p->GetItemAt(dwindex,&ret.p),
            "basic_shellitemarray::operator[]",
            "IShellItemArray::GetItemAt");
        return ret;
    }
};
struct shellitemarray:public basic_shellitemarray
{
    shellitemarray(){}
    shellitemarray(IShellItemArray* _p):basic_shellitemarray(_p){}
    shellitemarray(const basic_shellitemarray& itm){p=itm.p;}
    shellitemarray(const basic_shellitemarray&& itm){p=itm.p;}
    ~shellitemarray()
    {
        if(p)p->Release();
        p=nullptr;
    }
};
template<typename _Tp>
struct filedlg
{
    _Tp* p;
    void close(HRESULT hr=0l)
    {
        throw_com_error(p->Close(hr),
            "filedlg::close","IFileOpenDialog::Close");
    }
    void title(LPCWSTR lpszTitle)
    {
        throw_com_error(p->SetTitle(lpszTitle),
            "filedlg::title","IFileOpenDialog::SetTitle");
    }
    bool show(HWND _hwnd=nullptr)
    {
        HRESULT hr=p->Show(_hwnd);
        if(hr==-2147023673)return false;
        else throw_com_error(hr,"filedlg::show","IFileOpenDialog::Show");
        return true;
    }
    FILEOPENDIALOGOPTIONS getoptions()
    {
        FILEOPENDIALOGOPTIONS ret;
        throw_com_error(p->GetOptions(&ret),
            "filedlg::getoptions","IFileOpenDialog::GetOptions");
        return ret;
    }
    void pickfolders()
    {
        throw_com_error(p->SetOptions(getoptions()|FOS_PICKFOLDERS),
            "filedlg::pickfolders","IFileOpenDialog::SetOptions");
    }
    void clearclientdata()
    {
        throw_com_error(p->ClearClientData(),
            "filedlg::clearclientdata","IFileOpenDialog::ClearClientData");
    }
    void editboxcontent(LPCWSTR _str)
    {
        throw_com_error(p->SetFileName(_str),
            "filedlg::editboxcontent","IFileOpenDialog::SetFileName");
    }
    std::wstring editboxcontent()
    {
        LPWSTR ret;
        throw_com_error(p->GetFileName(&ret),
            "filedlg::editboxcontent","IFileOpenDialog::GetFileName");
        std::wstring returnval=ret;
        CoTaskMemFree(ret);
        return returnval;
    }
    void filenamelabel(LPCWSTR lpsz)
    {
        throw_com_error(p->SetFileNameLabel(lpsz),
            "filedlg::filenamelabel","IFileOpenDialog::SetFileNameLabel");
    }
    UINT filetypeindex()
    {
        UINT ret;
        throw_com_error(p->GetFileTypeIndex(&ret),
            "filedlg::filetypeindex","IFileOpenDialog::GetFileTypeIndex");
        return ret;
    }
    void filetypeindex(UINT uind)
    {
        throw_com_error(p->SetFileTypeIndex(uind),
            "filedlg::filetypeindex","IFileOpenDialog::SetFileTypeIndex");
    }
    void filetypes(COMDLG_FILTERSPEC* cflst,std::size_t _size)
    {
        throw_com_error(p->SetFileTypes(_size,cflst),
            "filedlg::filetypes",
            "IFileOpenDialog::SetFileTypes");
    }
    void defaultfolder(LPCWSTR lpPath)
    {
        shellitem itm(lpPath);
        throw_com_error(p->SetDefaultFolder(itm.p),
            "filedlg::defaultfolder","IFileOpenDialog::SetDefaultFolder");
    }
    void defaultfolder(basic_shellitem& itm)
    {
        throw_com_error(p->SetDefaultFolder(itm.p),
            "filedlg::defaultfolder","IFileOpenDialog::SetDefaultFolder");
    }
    void defaultextension(LPCWSTR _str)//no leading period. e.g. L"jpg"
    {
        throw_com_error(p->SetDefaultExtension(_str),
            "filedlg::defaultextension","IFileOpenDialog::SetDefaultExtension");
    }
    void setoptions(FILEOPENDIALOGOPTIONS opts)
    {
        throw_com_error(p->SetOptions(opts),
            "filedlg::pickfolders","IFileOpenDialog::SetOptions");
    }
    std::wstring result()
    {
        IShellItem* itm;
        throw_com_error(p->GetResult(&itm),
            "filedlg::result","IFileOpenDialog::GetResult");
        LPWSTR tmp;
        throw_com_error(itm->GetDisplayName(SIGDN_FILESYSPATH,&tmp),
            "filedlg::result","IShellItem::GetDisplayName");
        std::wstring ret(tmp);
        CoTaskMemFree(tmp);
        itm->Release();
        return ret;
    }
    void result(basic_shellitem& itm)
    {
        throw_com_error(p->GetResult(&itm.p),
            "filedlg::result","IFileOpenDialog::GetResult");
    }
    void setokbuttonlabel(LPCWSTR _str)
    {
        throw_com_error(p->SetOkButtonLabel(_str),
            "filedlg::setokbuttonlabel","IFileOpenDialog::SetOkButtonLabel");
    }
    ~filedlg()
    {
        if(p)p->Release();
        p=nullptr;
    }
};
struct fileopendlg:filedlg<IFileOpenDialog>
{
    fileopendlg()
    {
        p=cocrtins<IFileOpenDialog>(CLSID_FileOpenDialog,IID_IFileOpenDialog);
    }
    basic_shellitemarray results()
    {
        basic_shellitemarray ret;
        throw_com_error(p->GetResults(&ret.p),"fileopendlg::results",
            "IFileOpenDialog::GetResults");
        return ret;
    }
    basic_shellitemarray selecteditems()
    {
        basic_shellitemarray ret;
        throw_com_error(p->GetSelectedItems(&ret.p),
            "fileopendlg::selecteditems",
            "IFileOpenDialog::GetSelectedItems");
        return ret;
    }
};
struct filesavedlg:filedlg<IFileSaveDialog>
{
    filesavedlg()
    {
        p=cocrtins<IFileSaveDialog>(CLSID_FileSaveDialog,IID_IFileSaveDialog);
    }
    void setsaveasitem(const basic_shellitem& itm)
    {
        throw_com_error(p->SetSaveAsItem(itm.p),"filesavedlg::setsaveasitem",
            "IFileSaveDialog::SetSaveAsItem");
    }
};
#endif
struct drophandle
{
    HDROP h;
    operator HDROP()const noexcept{return h;}
    drophandle(HDROP _hdr):h(_hdr){}
    drophandle(WPARAM _wParam):h((HDROP)_wParam){}
    POINT point()
    {
        POINT ret;
        if(!DragQueryPoint(h,&ret))
            throw std::runtime_error("ygp::drophandle::point "
                "(function DragQueryPoint): "+lasterror<std::string>());
        return ret;
    }
    std::string str(UINT uindex)
    {
        char* tmp=new char[1024];
        if(!DragQueryFileA(h,uindex,tmp,1024))
            throw std::runtime_error("ygp::drophandle::str "
                "(function DragQueryFileA): "+lasterror<std::string>());
        std::string ret=tmp;
        delete []tmp;
        return ret;
    }
    std::wstring wstr(UINT uindex)
    {
        wchar_t* tmp=new wchar_t[1024];
        if(!DragQueryFileW(h,uindex,tmp,1024))
            throw std::runtime_error("ygp::drophandle::wstr "
                "(function DragQueryFileW): "+lasterror<std::string>());
        std::wstring ret=tmp;
        delete []tmp;
        return ret;
    }
    ~drophandle()
    {
        if(h)DragFinish(h);
        h=nullptr;
    }
};
END_NAMESPACE_YGP
#endif