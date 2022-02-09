#ifndef _YGP_TRIE_HPP_
#define _YGP_TRIE_HPP_
#include"configure.hpp"
#include<iostream>
#include<memory>
#include<string>
#include<functional>
BEGIN_NAMESPACE_YGP
template<typename ch_t=char,std::size_t sz=26>//sz: size of the character set; ch_t: character type
class trie//Trie dictionary tree
{
    public:
        typedef std::size_t(*hsh_t)(ch_t);
        typedef ch_t(*unhsh_t)(std::size_t);
        using triestr=std::basic_string<ch_t>;
        hsh_t hsh;
        unhsh_t unhsh;
        struct node
        {
            std::size_t cnt;
            std::shared_ptr<node> child[sz];
            node(){cnt=0;}
        }root;
        node* cur;
        trie(hsh_t _hsh=[](ch_t t)->std::size_t{return tolower(t)-'a';},unhsh_t _unhsh=[](std::size_t t)->ch_t{return t+'a';})
        {
            cur=&root;
            hsh=_hsh;
            unhsh=_unhsh;
        }
        void insertchar(ch_t ch)
        {
            std::size_t index=hsh(ch);
            if(!cur->child[index])cur->child[index].reset(new node());
            cur=cur->child[index].get();
        }
        void reset()
        {
            cur=&root;
        }
        void plus()
        {
            ++cur->cnt;
        }
        constexpr node* initial(){return &root;}
        void insert(const ch_t* str)
        {
            reset();
            for(;*str;++str)insertchar(*str);
            plus();
        }
        std::size_t query(const ch_t* str)
        {
            reset();
            for(;*str;++str)
            {
                std::size_t index=hsh(*str);
                if(!cur->child[index])return 0;
                cur=cur->child[index].get();
            }
            return cur->cnt;
        }
        void dfs(triestr& stk,node* nd,std::function<void(triestr&,std::size_t)> __f=
            [](triestr& _stk,std::size_t _cnt)
            {
                std::cout<<_stk,std::cout.put('\t');std::cout<<_cnt;std::cout.put('\n');
            })
        {
            if(nd->cnt)__f(stk,nd->cnt);
            for(std::size_t i=0;i<sz;++i)
            {
                if(nd->child[i])
                {
                    stk.push_back(unhsh(i));
                    dfs(stk,nd->child[i].get(),__f);
                    stk.pop_back();
                }
            }
        }
};
END_NAMESPACE_YGP
#endif