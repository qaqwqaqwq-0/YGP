# YGP
A C++ Windows API wrapper library

## Brief Introduction

YGP is a header-only C++ library for Windows. It provides an object-oriented wrapper on many of the Win32 and COM APIs, designed to be user-friendly with a high performance. Unlike MFC or wxWidgets, this library suggests the use of pure C++ syntax and avoids the use of pointers. The focus of this library is GUI, but many other functions like networking and threading are also included. The YGP library can be used to develop simple GUI applications, but is mostly useful for complex user interfaces with powerful facilities.

## Purpose

1. To provide a C++ interface for Win32 and COM APIs
2. To reduce the amount of code and accelerate Windows software development
3. To support to develop high-performance software

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

### 2. Using the C++17 Syntax

The library adopts the modern C++ syntax, which can make the code more clear and readable.

### 3. Compatible with Both the MinGW and the Microsoft Visual C++ Compilers

The code of the library follows the common C++ syntax, so the compatibilities are not a problem. However, since g++ doesn't support the #pragma comment expression, MSVC is more recommended.

### 4. Header Only

No annoying linking issues. And, the library can be used in multi-source-file scenarios.

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

## Learn

1. The [Microsoft Docs](https://docs.microsoft.com/en-us/windows/win32/api/) is a great place to learn some design concepts and API usages.
2. The documentation is also a tutorial. By reading the docs, you can know not only what the functions and class methods do, but also how to use them.

## Comparisons Between YGP and MFC

| Aspects              | YGP            | MFC                 |
| :------------------- | :------------- | :------------------ |
| Compatibility        | MinGW and MSVC | Only MSVC           |
| Degree of Difficulty | Easy           | Extremely Difficult |
| Performance          | Efficient      | Slow                |
| Executable Size      | Tiny           | Giant               |
| UI Effect            | Beautiful      | Beautiful           |

## Current Progress

The code part is in progress.

The development of the document part has just started.