#include"../include/ygp.hpp"
#include<filesystem>
YGP_INIT
using namespace filesystem;
YGP_WMAIN
{
    if(argc==1)
    {
        wcerr<<L"Look up an error code from all DLLs and EXEs in a specific directory.\n";
        return 0;
    }
    if(argc==2)
    {
        wcerr<<L"Please specify a directory.\n";
        return 1;
    }
    DWORD dwerr=wcstoul(argv[1],nullptr,16);
    wcout<<hex<<L"0x"<<dwerr<<L": \n";
    if(!exists(argv[2]))
    {
        wcerr<<argv[2]<<L": No such file or directory.\n";
        return 1;
    }
    bool success=false;
    YGP_TRY
        auto ret=thr_lasterror(dwerr);
        print("System: ",ret);
        success=true;
    YGP_CATCH_IGNORE
    for(auto& file:directory_iterator(argv[2]))
    {
        auto name=file.path();
        if(name.has_extension()&&(name.extension()==L".dll"||name.extension()==L".exe"))
        {
            HMODULE hm=LoadLibraryW(name.c_str());
            if(!hm)continue;
            try
            {
                auto ret=thr_ext_lasterror_modulew_without_system(name.c_str(),dwerr);
                ptws(name.c_str());
                wcout<<L": ";
                cout<<ret;
                wcout<<endl;
                success=true;
            }
            catch(...){}
            if(hm)FreeLibrary(hm);
        }
    }
    if(!success)
    {
        wcout<<L"Not found.";
        return 1;
    }
}