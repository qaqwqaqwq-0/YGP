#ifndef _YGP_LST_HPP_
#define _YGP_LST_HPP_
#include"configure.hpp"
#if __cplusplus>=201103L
#include<type_traits>
#endif
#include<memory>
#include<utility>
BEGIN_NAMESPACE_YGP
template<typename value_type>
class lst
{
	#if __cplusplus>201103L
	static_assert(std::is_same<typename std::remove_cv<value_type>::type,value_type>::value,"ygp::lst must have a non-const, non-volatile value_type!");
	#endif
	public:
		struct iterator
		{
			node* m;
			value_type& operator*()
			{
				return m->value;
			}
			iterator operator++()
			{
				return m=m->nxt;
			}
			value_type* operator->()
			{
				return &(m->value);
			}
			bool operator==(iterator another)
			{
				return m==another.m;
			}
			bool operator!=(iterator another)
			{
				return m!=another.m;
			}
		};
		typedef std::allocator<value_type> alloc;
		struct node
		{
			value_type value;
			node* nxt;
			node(const value_type& __x):value(__x){}
			#if __cplusplus>=201103L
			node(const value_type&& ___x):value(__x){}
			#endif
		};
	private:
		node* first;
		node** last;
	public:
		iterator begin()
		{
			return first;
		}
		iterator end()
		{
			return ygpnull;
		}
		void append(const value_type& __x)
		{
			*last=new node(__x);
			last=&((*last)->nxt);
		}
		#if __cplusplus>=201103L
		void append(const value_type&& __x)
		{
			*last=new node(__x);
			last=&((*last)->nxt);
		}
		#endif
		lst()
		{
			first=ygpnull;
			last=&first;
		}
		~lst()
		{
			node* __i1,*__i2;
			__i2=first;
			while(__i2)
			{
				__i1=__i2;
				__i2=__i2->nxt;
				delete __i1;
			}
		}
		/*#if __cplusplus>=201103L
		explicit lst(std::size_t __n,const value_type&& value)
		{
			for(std::size_t i=0;i<__n;++i)
		}
		#endif*/
};
END_NAMESPACE_YGP
#endif
