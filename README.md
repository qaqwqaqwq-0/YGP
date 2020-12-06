# YGP
A C++ Windows API wrapper library

## Introduction

YGP is a header-only C++ library for Windows. It provides an object-oriented wrapper on many of the Win32 and COM APIs. It is designed to be user-friendly and have a high performance. Unlike MFC and wxWidgets, this library suggests the use of pure C++ syntax and avoids the use of pointers. The focus of this library is GUI, but many other functions like networking and threading are also included.

## Quick Start

Since YGP is a header-only C++ library, it's quite simple to use. Just including the headers is OK.

## The Introduction to Each Part

### Cross-Platform Part

This part consists of the header files in include/ygp directory.

This part is designed to be available on all the platforms, since it only relies on standard C++.

### Windows-Only Part

This part consists of the header files in include/ygp/win directory.

This part is the wrapper of the non-GUI Win32 and COM APIs.

### GUI Part

This part consists of the header files in include/ygp/win/gui directory.

This part is more than a wrapper; it provides user-friendly access to GUI controls and manipulators.

## Current Progress

The code part is in progress.

The document part is just started.