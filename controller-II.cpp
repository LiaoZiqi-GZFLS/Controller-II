#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include <cstring>
#include <tchar.h>
#include "color.h"
#include "admin.h"

int main(int argc, char* argv[]){
    using namespace std;
    colorDemo();
    if(!IsProcessRunAsAdmin()){
        SetConsoleColor(GREEN);
        printf("[User Mode]\n");
        SetConsoleColor(LIGHT_GREEN);
        printf("[User Mode]\n");
    }else{
        SetConsoleColor(RED);
        printf("[Administrator  Mode]\n");
        SetConsoleColor(DEFAULT);
    }
    if(argc==1){
        SetConsoleColor(DEFAULT);
        printf("Controller (II) [Version 1.2.0.0a]\n");
        SetConsoleColor(CYAN);
        printf("(c) Lazybones LZQ Corporation. ");
        SetConsoleColor(DEFAULT);
        printf("All rights reserved.\n");
    }else{
        SetConsoleColor(DEFAULT);
        
    }
    SetConsoleColor(GREEN);
    printf("Programe exited. ");
    SetConsoleColor(DEFAULT);
    printf("(Press any key to continue)\n");
    system("pause>nul");
    return 0;
}