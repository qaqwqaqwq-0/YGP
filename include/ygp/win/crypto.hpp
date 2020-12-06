#ifndef _YGP_WIN_CRYPTO_HPP_
#define _YGP_WIN_CRYPTO_HPP_
//Encapsulation of Wincrypt API.
#include"winerr.hpp"
#include<memory>
BEGIN_NAMESPACE_YGP
class hashprov//Hash provider
{
    private:
        HCRYPTPROV pr;
    public:
        hashprov()
        {
            if(!CryptAcquireContextA(&pr,nullptr,nullptr,PROV_RSA_AES,CRYPT_VERIFYCONTEXT))
                throw std::runtime_error("ygp::hashprov::hashprov (function CryptAcquireContextA): "+lasterror<std::string>());
        }
        ~hashprov()
        {
            CryptReleaseContext(pr,0ul);
        }
        std::pair<std::shared_ptr<BYTE>,DWORD> perform(const void* buf,DWORD len,DWORD hashtype=CALG_MD5)
        {
            HCRYPTHASH hhash;
            if(!CryptCreateHash(pr,hashtype,0,0,&hhash))
                throw std::runtime_error("ygp::hashprov::perform (function CryptCreateHash): "+lasterror<std::string>());
            if(!CryptHashData(hhash,(const BYTE*)buf,len,0ul))
            {
                CryptDestroyHash(hhash);
                throw std::runtime_error("ygp::hashprov::perform (function CryptHashData): "+lasterror<std::string>());
            }
            DWORD dwsize;
            DWORD dwlen=sizeof(dwsize);
            if(!CryptGetHashParam(hhash,HP_HASHSIZE,(BYTE*)&dwsize,&dwlen,0ul))
            {
                CryptDestroyHash(hhash);
                throw std::runtime_error("ygp::hashprov::perform (function CryptGetHashParam): "+lasterror<std::string>());
            }
            std::shared_ptr<BYTE> ret(new BYTE[dwsize]);
            dwlen=dwsize;
            if(!CryptGetHashParam(hhash,HP_HASHVAL,ret.get(),&dwlen,0ul))
            {
                CryptDestroyHash(hhash);
                throw std::runtime_error("ygp::hashprov::perform (function CryptGetHashParam): "+lasterror<std::string>());
            }
            CryptDestroyHash(hhash);
            return std::pair<std::shared_ptr<BYTE>,DWORD>(ret,dwlen);
        }
        std::shared_ptr<char> hashstring(std::pair<std::shared_ptr<BYTE>,DWORD>&& pr)
        {
            std::shared_ptr<char> ret(new char[pr.second*2+1]);
            char buf[3];
            for(std::size_t i=0;i<pr.second;++i)
            {
                snprintf(buf,3,"%02x",UINT(pr.first.get()[i]));
                memcpy(ret.get()+2*i,buf,2);
            }
            ret.get()[2*pr.second]='\0';
            return ret;
        }
};
END_NAMESPACE_YGP
#endif