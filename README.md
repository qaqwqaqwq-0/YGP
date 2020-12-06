# YGP
A C++ Windows API wrapper library

## Introduction

YGP is a header-only C++ library for Windows. It provides an object-oriented wrapper on many of the Win32 and COM APIs. It is designed to be user-friendly and have a high performance. Unlike MFC and wxWidgets, this library suggests the use of pure C++ syntax and avoids the use of pointers. The focus of this library is GUI, but many other functions like networking and threading are also included.

## Features

### 1. Easy

How to implement a beautiful task dialog as shown below:

![Result](https://github.com/qaqwqaqwq-0/YGP/blob/main/sample/taskdlgresult.png)

```cpp
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
```

The library is designed to be user-friendly, so it is super easy to learn, even without a documentation.

### 2. Using C++17 Syntax

### 3. Compatible with MinGW and Microsoft Visual C++

## Quick Start

Since YGP is a header-only C++ library, it's quite simple to use. Just including the headers is OK.

## The Introduction to Each Part

### Cross-Platform Part

This part consists of the header files in include/ygp directory.

This part is designed to be available on all the platforms, since it only relies on standard C++. This part provides some algorithms, data structures, string manipulators, and so on.

### Windows-Only Part

This part consists of the header files in include/ygp/win directory.

This part is the wrapper of the non-GUI Win32 and COM APIs. Networking, encryption, encoding, threading, IPC, synchronization, media playback, web browser simulation and console text color management facilities are included. 

### GUI Part

This part consists of the header files in include/ygp/win/gui directory.

This part is more than a wrapper; it provides user-friendly access to GUI controls and manipulators. Common controls include static control, button control, edit control, tab control, etc. The Windows GDI objects are also included.

## Current Progress

The code part is in progress.

The document part is just started.