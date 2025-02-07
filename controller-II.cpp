#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include <cstring>
#include <tchar.h>
#include "color.h"
#include "admin.h"
#include "encoding.h"
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void setCmdEnv(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    system("@echo off");
    system("@chcp 65001>nul");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);  
    system("@title Controller-II");
    return;
}

std::string executeCommand(const std::string& cmd) {
    FILE* pipe = _popen(cmd.c_str(), "r");
    if (!pipe) {
        return "Error opening pipe";
    }

    char buffer[2048];
    std::string result;

    while (!feof(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            result += buffer;
        }
    }

    _pclose(pipe);
    return result;
}

int main(int argc, char* argv[]){
    using namespace std;
    if(argc==1){
        system("cls");
        setCmdEnv();
        if(!IsProcessRunAsAdmin()){
            SetConsoleColor(GREEN);
            printf("[User Mode]\n");
        }else{
            SetConsoleColor(RED);
            printf("[Administrator  Mode]\n");
            SetConsoleColor(DEFAULT);
        }
        SetConsoleColor(DEFAULT);
        printf("Controller (II) [Version 1.2.0.0a]\n");
        SetConsoleColor(CYAN);
        printf("All rights reserved by (c) Lazybones LZQ Corporation. \n");
        SetConsoleColor(DEFAULT);
        printf("All rights reserved by author LiaoZiqi.\n\n");
        /*------------------------------*/
        string line;
        string promts = "Controller-II> ";
        string echo;
        cout<<ANSI_RESET<<promts<<ANSI_RESET;
        while(getline(cin,line)){
            //cout<<ANSI_RESET<<line<<ANSI_RESET<<endl;
            //line = "cmd /c \"" + line + "\"";
            //system(line.c_str());
            echo = executeCommand(line);
            cout<<echo<<endl;
            cout<<detectEncoding(line)<<endl;
            cout<<detectEncoding(echo)<<endl;
            cout<<ANSI_RESET<<promts<<ANSI_RESET;
        }
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