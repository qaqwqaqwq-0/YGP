/*
    Extended type traits.
*/
#ifndef _YGP_EXTENDED_TYPE_TRAITS_HPP_
#define _YGP_EXTENDED_TYPE_TRAITS_HPP_
#include"configure.hpp"
#include<tuple>
#include<cstdint>
BEGIN_NAMESPACE_YGP
template<std::size_t _S,std::size_t ..._N>
struct _Seq_gen;
template<std::size_t _S,std::size_t _I,std::size_t ..._N>
struct _Seq_gen<_S,_I,_N...>
{
    using type=typename _Seq_gen<_S,_I-1,_I-1,_N...>::type;
};
template<std::size_t _S,std::size_t ..._N>
struct _Seq_gen<_S,_S,_N...>
{
    using type=std::index_sequence<_N...>;
};
template<std::size_t _S,std::size_t _N>
using start_position_specified_index_sequence=typename _Seq_gen<_S,_N>::type;
template<typename _Callable,typename _Tuple,std::size_t ..._Ind>
void ygp_apply(_Callable&& __f,_Tuple&& _tp,std::index_sequence<_Ind...>)
{
    __f(std::get<_Ind>(std::move(_tp))...);
}
template<std::size_t __N>struct integral_type_with_specific_size;
template<>
struct integral_type_with_specific_size<1u>
{
    typedef uint8_t type;
};
template<>
struct integral_type_with_specific_size<2u>
{
    typedef uint16_t type;
};
template<>
struct integral_type_with_specific_size<4u>
{
    typedef uint32_t type;
};
template<>
struct integral_type_with_specific_size<8u>
{
    typedef uint64_t type;
};
template<std::size_t _N,typename _Tp,typename ... _Types>
struct _Same_type_gen
{
    using type=typename _Same_type_gen<_N-1,_Tp,_Tp,_Types...>::type;
};
template<typename _Tp,typename ... _Types>
struct _Same_type_gen<0,_Tp,_Types...>
{
    using type=std::tuple<_Types...>;
};
template<std::size_t _N,typename _Tp>
using same_type_tuple=typename _Same_type_gen<_N,_Tp>::type;
END_NAMESPACE_YGP
#endif