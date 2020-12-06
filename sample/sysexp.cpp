#include"../include/ygp.hpp"
#pragma comment(lib,"../lib/ntstr.lib")
YGP_INIT
YGP_MAIN
{
    if(argc==1)
    {
        cout<<"Convert NT status code to a string.";
        return 0;
    }
    DWORD d=strtoul(argv[1],nullptr,16);
    cout<<ntstatus2string(d);
}