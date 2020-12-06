#ifndef _YGP_WIN_CNSLCL_HPP_
#define _YGP_WIN_CNSLCL_HPP_
#include"../configure.hpp"
#include<windows.h>
BEGIN_NAMESPACE_YGP
class cnslcl//Console text color manager
{
    CONSOLE_SCREEN_BUFFER_INFO info[3];
    HANDLE h[3];
    bool br;
    public:
        cnslcl()
        {
            h[0]=GetStdHandle(STD_INPUT_HANDLE);
            h[1]=GetStdHandle(STD_OUTPUT_HANDLE);
            h[2]=GetStdHandle(STD_ERROR_HANDLE);
            GetConsoleScreenBufferInfo(h[0],info+0);
            GetConsoleScreenBufferInfo(h[1],info+1);
            GetConsoleScreenBufferInfo(h[2],info+2);
            br=true;
        }
        cnslcl& donotresume()noexcept{br=false;return *this;}
        HANDLE hstdin()const noexcept{return h[0];}
        HANDLE hstdout()const noexcept{return h[1];}
        HANDLE hstderr()const noexcept{return h[2];}
        cnslcl& resume()
        {
            SetConsoleTextAttribute(h[0],info[0].wAttributes);
            SetConsoleTextAttribute(h[1],info[1].wAttributes);
            SetConsoleTextAttribute(h[2],info[2].wAttributes);
            return *this;
        }
        ~cnslcl(){if(br)resume();}
        static constexpr WORD black=0x0;
        static constexpr WORD red=0x4;
        static constexpr WORD blue=0x1;
        static constexpr WORD green=0x2;
        static constexpr WORD yellow=0x6;
        static constexpr WORD cyan=0x3;
        static constexpr WORD violet=0x5;
        static constexpr WORD white=0x7;
        static constexpr WORD bblack=0x0;
        static constexpr WORD bred=0x40;
        static constexpr WORD bblue=0x10;
        static constexpr WORD bgreen=0x20;
        static constexpr WORD byellow=0x60;
        static constexpr WORD bcyan=0x30;
        static constexpr WORD bviolet=0x50;
        cnslcl& in(WORD fr,WORD bk=bblack)
        {
            SetConsoleTextAttribute(h[0],fr|FOREGROUND_INTENSITY|bk);
            return *this;
        }
        cnslcl& out(WORD fr,WORD bk=bblack)
        {
            SetConsoleTextAttribute(h[1],fr|FOREGROUND_INTENSITY|bk);
            return *this;
        }
        cnslcl& err(WORD fr,WORD bk=bblack)
        {
            SetConsoleTextAttribute(h[2],fr|FOREGROUND_INTENSITY|bk);
            return *this;
        }
};
END_NAMESPACE_YGP
#endif