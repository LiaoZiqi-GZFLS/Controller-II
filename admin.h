#include <windows.h>
#include <shellapi.h>
#include <iostream>
#include <tchar.h>
bool IsProcessRunAsAdmin()
{
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;
    BOOL b = AllocateAndInitializeSid(
        &NtAuthority,
        2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &AdministratorsGroup);
    if(b)
    {
        CheckTokenMembership(NULL, AdministratorsGroup, &b);
        FreeSid(AdministratorsGroup);
    }
    return b == TRUE;
}
short GetAdmin(LPCSTR Param, int Showcmd)
{
    if (IsProcessRunAsAdmin())
        return 0;
    TCHAR Path[MAX_PATH];
    ZeroMemory(Path, MAX_PATH);
    ::GetModuleFileName(NULL, Path, MAX_PATH);           //获取程序路径
    HINSTANCE res;
    res = ShellExecute(NULL, "runas", Path, Param, NULL, Showcmd);
    if((int)res > 32)
        return 1;
    else
        return 0;
}