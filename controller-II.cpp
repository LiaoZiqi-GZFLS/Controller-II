#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include <cstring>
#include <tchar.h>
#include "color.h"
#include "admin.h"

// 设置控制台颜色的函数
void SetConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 获取控制台句柄
    SetConsoleTextAttribute(hConsole, color); // 设置颜色
}

int main(int argc, char* argv[]){
    using namespace std;
    if(!IsProcessRunAsAdmin()){
        SetConsoleColor(DEFAULT);
        printf("Please run this programe with administrator privileges.\n");
    }
    if(argc==1){
        SetConsoleColor(DEFAULT);
        printf("Controller (II) Version");
        SetConsoleColor(CYAN);
        printf("(c) Lazybones LZQ Corporation. ");
        SetConsoleColor(DEFAULT);
        printf("All rights reserved.\n");
    }else{
        SetConsoleColor(DEFAULT);
        printf("Controller (II) running.");
    }
    system("pause");
    return 0;
}