#ifndef _YGP_RNG_HPP_
#define _YGP_RNG_HPP_
#include"configure.hpp"
#include<cstdint>
BEGIN_NAMESPACE_YGP
template<typename _Tp>
class rng/*range*/
{
    public:
        using t=_Tp;
        struct iterator
        {
            t tc,td;
            iterator(t B,t D):tc{B},td{D}{}
            t operator++(){return tc+=td;}
            t operator--(){return tc-=td;}
            const t& operator*()const{return tc;}
            bool operator==(const iterator& an)const{return tc==an.tc;}
            bool operator!=(const iterator& an)const{return tc!=an.tc;}
        };
        t b,e,d;
        explicit rng(t E)
        {
            b=0,e=E,d=1;
        }
        explicit rng(t B,t E,t D=1):b{B},e{E},d{D}{}
        iterator begin()const
        {
            return iterator{b,d};
        }
        iterator end()const
        {
            return iterator{e,d};
        }
};
/*Usage: for(auto i:rng(begin,end))//[begin,end)*/
END_NAMESPACE_YGP
#endif