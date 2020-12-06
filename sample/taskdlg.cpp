// Platform: Windows 10
// Compiled with: (using MSVC) cl taskdlg.cpp /std:c++17 /EHs /I ..\include
// Execute "mt -manifest taskdlg.exe.manifest -outputresource:taskdlg.exe;1" if you like.
#include<ygp/base.hpp>
#include<ygp/win/shell.hpp>
#include<ygp/win/gui/global.hpp>
#include<ygp/win/gui/dialog.hpp>
#pragma YGP_MANIFEST
YGP_INIT
YGP_MAIN
{
    YGP_INIT_GUI
    taskdlg td;
    td.enablehyperlinks();
    td.title(L"YGP Library Sample");
    td.setmainicon(TD_INFORMATION_ICON);
    td.instruction(L"YGP is an awesome and easy C++ library.");
    td.content(L"Please visit our <a href=\"https://github.com/qaqwqaqwq-0/YGP\">website</a>.");
    td.usecommandlinks();
    const TASKDIALOG_BUTTON buttons[]={{100,L"Download from website"},{101,L"Clone using Git"}};
    const TASKDIALOG_BUTTON radiobuttons[]={{103,L"This project is nice."},{104,L"This project is perfect."}};
    td.setbuttons(buttons,ARRAYSIZE(buttons));
    td.setradiobuttons(radiobuttons,ARRAYSIZE(radiobuttons));
    td.setfooter(L"Report issues <a href=\"https://github.com/qaqwqaqwq-0/YGP/issues\">here.</a>");
    td();
}