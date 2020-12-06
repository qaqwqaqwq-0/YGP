/*
    More secure access of std::array.
*/
#ifndef _YGP_ARR_HPP_
#define _YGP_ARR_HPP_
#include"configure.hpp"
#include<array>
BEGIN_NAMESPACE_YGP
template<typename _Tp,std::size_t _N>
class arr:public std::array<_Tp,_N>
{
    public:
        using std::array<_Tp,_N>::array;
        typename std::array<_Tp,_N>::reference operator[](typename std::array<_Tp,_N>::size_type _Pos)
        {
            return this->at(_Pos);
        }
        typename std::array<_Tp,_N>::reference operator()(int _Pos)
        {
            return _Pos<0?this->at((int)_N+_Pos):this->at(_Pos);
        }
};
END_NAMESPACE_YGP
#endif