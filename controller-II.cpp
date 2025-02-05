#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include <cstring>
#include <tchar.h>
#include "color.h"
#include "admin.h"

int main(int argc, char* argv[]){
    using namespace std;
    if(argc==1){
        if(!IsProcessRunAsAdmin()){
            SetConsoleColor(GREEN);
            printf("[User Mode]\n");
        }else{
            SetConsoleColor(RED);
            printf("[Administrator  Mode]\n");
            SetConsoleColor(DEFAULT);
        }
        system("cls");
        SetConsoleColor(DEFAULT);
        printf("Controller (II) [Version 1.2.0.0a]\n");
        SetConsoleColor(CYAN);
        printf("(c) Lazybones LZQ Corporation. ");
        SetConsoleColor(DEFAULT);
        printf("All rights reserved.\n");
        /*------------------------------*/

        /*------------------------------*/
        SetConsoleColor(GREEN);
        printf("Programe exited. ");
        SetConsoleColor(DEFAULT);
        printf("(Press any key to continue)\n");
        system("pause>nul");
    }else{
        SetConsoleColor(DEFAULT);
        
    }
    return 0;
}