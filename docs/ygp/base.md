# base.hpp Document

## Brief Introduction

This header provides wrappers for basic I/O functions, some overload operators for smart pointers and tuples, as well as some basic macros.

## Functions

1. ###### ygp::io_async

```cpp
bool io_async(bool async=true);
```

Since `iostream` operators has a better performance when they are asynchronous with `stdio` functions, we provide a function to turn off (or turn on) the synchronization switch.

__Parameters__:

`async ` Turn on or off the asynchronous mode.

__Return value__:

Whether the asynchronous mode is on before this operation.

------

2. ###### ygp::print

```cpp
template<typename ...__args>
std::ostream& print(__args&& ... __ARGS);
```

The function imitates the `print` function in the Python language.

e.g.,

```cpp
ygp::print("string", 1, 9.085, std::make_tuple(7, 'c', 30.0f));
```

Output:

```c++
string 1 9.085 7,c,30

```

__Parameters__:

`__ARGS...` The arguments of the output.

__Return value__:

A reference to `std::cout`.

------

3. ygp::prints

```cpp

```

***

## Macros

1. ###### YGP_MAIN

```cpp
#define YGP_MAIN int main(int argc,char** argv)
```

The declaration of the `main` entry point.

***

2. ###### YGP_WMAIN

```cpp
#define YGP_WMAIN int wmain(int argc,wchar_t** argv)
```

The declaration of the `wmain` entry point. The arguments are in wide-characters.

***

3. ###### YGP_TRY

```cpp
#define YGP_TRY try{
```

The beginning of a `try` block.

Sample:

```cpp
YGP_TRY
    std::wstring widechar;
    ygp::multibyte2widechar("Multi-byte string", widechar, CP_ACP)
YGP_CATCH_MSGBOX
```

***