#define YGP_DISABLE_DEMENTOR
#include"../include/ygp.hpp"
YGP_INIT
YGP_MAIN
{
    YGP_TRY
        WCHAR w[]=L"安安";//UTF-8 string
        ptws(w);
        crlf();
        string s;
        widechar2multibyte(w,s,CP_ACP);
        cout<<s<<endl;
        wstring q;
        multibyte2widechar(s.c_str(),q,CP_UTF8);
        ptws(q-cstr);//ANSI string
    YGP_CATCH
}
/*Output:
瀹夊畨
瀹夊畨
安安
*/
//Current console codepage: 936