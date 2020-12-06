#ifndef _YGP_GENETIC_ALGORITHM_HPP_
#define _YGP_GENETIC_ALGORITHM_HPP_
#include"configure.hpp"
#include<cstring>
BEGIN_NAMESPACE_YGP
template<typename _Tp>
struct basic_gene
{
    typedef unsigned char data_type;
    std::size_t size;
    std::size_t mutateamount;
    protected:
        data_type* data;
};
template<typename _Tp>
class binary_gene:public basic_gene<_Tp>
{
    public:
        data_type* getdata()const noexcept
        {
            return data;
        }
        binary_gene():data(nullptr),size(0u);
        binary_gene(const _Tp& _val)
        {
            size=sizeof(_Tp);
            data=new data_type[size];
            std::memcpy(data,&_val,size);
        }
        ~binary_gene()
        {
            if(data)delete []data;
        }
};
END_NAMESPACE_YGP
#endif