#include"../include/ygp.hpp"
YGP_INIT
YGP_MAIN
{
    if(argc==0)
    {
        print("Lookup a specific hex error code in a dll.");
        return 0;
    }
    if(argc==1)
    {
        prints(cerr,"Missing dll name.");
        return 1;
    }
    DWORD d=strtoul(argv[1],nullptr,16);
    HMODULE hm=LoadLibraryA(argv[2]);
    if(!hm)
    {
        cerr<<"Cannot load "<<argv[2]<<".\n";
        return 1;
    }
    cout<<ext_lasterror_without_system(argv[2],d);
    FreeLibrary(hm);
}