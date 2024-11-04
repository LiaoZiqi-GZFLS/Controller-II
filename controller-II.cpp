#include<iostream>
#include <windows.h>
#include <shellapi.h>
void RequestAdministrator() {
    SHELLEXECUTEINFO sei = { sizeof(sei) };
    sei.lpVerb = L"runas";
    sei.lpFile = L"controller-II.exe"; // 替换为你的程序路径
    sei.hwnd = NULL;
    sei.nShow = SW_NORMAL;

    // 尝试以管理员身份运行程序
    if (!ShellExecuteEx(&sei)) {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_CANCELLED) {
            // 用户取消操作
            printf("Operation canceled.");
            //printf("Please run this programe with administrator privileges.");
        } else {
            // 其他错误
            printf("Runtime error.");
            //printf("Please run this programe with administrator privileges.");
        }
    }
}

BOOL IsAdministrator() {
    BOOL bIsElevated = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION te;
        DWORD dwReturnLength = 0;
        if (GetTokenInformation(hToken, TokenElevation, &te, sizeof(te), &dwReturnLength)) {
            bIsElevated = te.TokenIsElevated;
        }
        CloseHandle(hToken);
    }
    return bIsElevated;
}

int main(int argc, char* argv[]){
    using namespace std;
    RequestAdministrator();
    if(!IsAdministrator){
        printf("Please run this programe with administrator privileges.");
    }
    if(argc==1){

    }else{

    }
    return 0;
}