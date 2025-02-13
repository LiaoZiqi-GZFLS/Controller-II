#include <iostream>
#include <windows.h>
#include <shellapi.h>
#include <cstring>
#include <tchar.h>
#include "color.h"
#include "admin.h"
#include "encoding.h"
#include "environment.hpp"
#include "findfile.hpp"
#include "cmdline/cmdline.h"
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#include <stdio.h>

void setCmdEnv(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    system("@echo off");
    system("chcp");
    if(GetACP()==65001){
        system("@chcp 65001>nul");
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);  
        system("echo UTF-8 Mode");
    }else if(GetACP()==936){
        system("@chcp 936>nul");
        SetConsoleOutputCP(CP_ACP);
        SetConsoleCP(CP_ACP);
        system("echo GBK Mode");
    }else{
        system("@chcp 65001>nul");
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);  
        system("echo UTF-8 Mode (Default)");
    }
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

void getConfig(bool &DETECT_ENCODING, bool &IOmod){
    // 定义有效的配置变量
    std::vector<std::string> validVars = {"DETECT_ENCODING", "IOmod","DEBUG"};
    // 创建配置管理对象，使用默认文件名 "con2.ini"
    EnvironmentConfig config(validVars);
    //EnvironmentConfig config2(validVars, "custom.ini");
    // 获取值
    DETECT_ENCODING = config.getDETECT_ENCODING();
    IOmod = config.getIOmod();
}

int main(int argc, char* argv[]){
    using namespace std;
    bool IOmod = true;
    bool DETECT_ENCODING = false;
    auto file_tree = NULL
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
        /*---------Loading file---------*/
        getConfig(DETECT_ENCODING, IOmod);
        /*------------------------------*/
        string line;
        string promts = "Controller-II> ";
        string echo;
        cout<<ANSI_RESET<<promts<<ANSI_RESET;
        while(getline(cin,line)){
            if(line=="admin"){
                if(!IsProcessRunAsAdmin()){
                    SetConsoleColor(RED);
                    printf("Requesting for administrator privilege...\n");
                    SetConsoleColor(DEFAULT);
                    GetAdmin(NULL, SW_SHOWNORMAL);
                }else{
                    SetConsoleColor(GREEN);
                    printf("Already in administrator mode.\n");
                    SetConsoleColor(DEFAULT);
                }
                cout<<ANSI_RESET<<promts<<ANSI_RESET;
                continue;
            }
            if(line=="exit"){
                cout<<ANSI_RESET<<promts<<ANSI_RESET;
                break;
            }
            if(line=="help"){
                cout<<"exit: Exit the program"<<endl;
                cout<<"help: Show this help message"<<endl;
                cout<<"echo: Echo the input"<<endl;
                cout<<"cls: Clear the screen"<<endl;
                cout<<"color: Change the color of the console"<<endl;
                cout<<ANSI_RESET<<promts<<ANSI_RESET;
                continue;
            }
            if(line=="play"){
                string a_arg = "cmd /c preDecoding.exe ";
                string line2;
                cout<<ANSI_RESET<<"Please input the video path: "<<ANSI_RESET;
                getline(cin,line2);
                if(line2!=""){
                    a_arg += "-v " + line2;
                }
                cout<<ANSI_RESET<<"Please input the audio path: "<<ANSI_RESET;
                getline(cin,line2);
                if(line2!=""){
                    a_arg += "-a " + line2;
                }
                system(a_arg.c_str());
                cout<<ANSI_RESET<<promts<<ANSI_RESET;
                continue;
            }
            if(IOmod){
                cout<<ANSI_RESET<<line<<ANSI_RESET<<endl;
                line = "cmd /c \"" + line + "\"";
                system(line.c_str());
                cout<<ANSI_RESET<<promts<<ANSI_RESET;
            }else{
                echo = executeCommand(line);
                cout<<echo<<endl;
                if(DETECT_ENCODING){
                    cout<<"Input: "<<endl<<detectEncoding(line);
                    cout<<"Output: "<<endl<<detectEncoding(echo);
                }
                cout<<ANSI_RESET<<promts<<ANSI_RESET;
            }
            
        }
        /*------------------------------*/
        SetConsoleColor(GREEN);
        printf("Programe exited. ");
        SetConsoleColor(DEFAULT);
        printf("(Press any key to continue)\n");
        system("pause>nul");
    }else{
        SetConsoleColor(DEFAULT);
        cmdline::parser a;
        //a.add<string>("host", 'h', "host name", true, "");
        //a.add<int>("port", 'p', "port number", false, 80, cmdline::range(1, 65535));
        //a.add<string>("type", 't', "protocol type", false, "http", cmdline::oneof<string>("http", "https", "ssh", "ftp"));
        //a.add("gzip", '\0', "gzip when transfer");
        a.add("admin", 'a', "request for administrator privilege");
        a.add("help", 0, "print this message");
        a.add("play", '\0', "play the ascii art");
        a.add<string>("video", '\0', "video path", false, "");
        a.add<string>("audio", '\0', "audio path", false, "");
        a.add("version", 'v', "show version");
        a.add<wstring>("findfile", 'f', "find file");
        a.add<wstring>("pattern", 'p', "pattern of file");
        a.add("recursive", 'r', "enable recursive search");
        a.add("regex", 'e', "use regex pattern");
        a.add<wstring>("build", 'b', "build file tree");
        a.parse_check(argc, argv);
        if(a.exist("play")){
            system("cls");
            string a_arg = "cmd /c preDecoding.exe ";
            if(a.exist("video")){
                a_arg += "-v " + a.get<string>("video");
            }
            if(a.exist("audio")){
                a_arg += "-a" + a.get<string>("audio");
            }
            system(a_arg.c_str());
        }
        if(a.exist("admin")){
            if(!IsProcessRunAsAdmin()){
                SetConsoleColor(RED);
                printf("Requesting for administrator privilege...\n");
                SetConsoleColor(DEFAULT);
                GetAdmin(NULL, SW_SHOWNORMAL);
            }else{
                SetConsoleColor(GREEN);
                printf("Already in administrator mode.\n");
                SetConsoleColor(DEFAULT);
            }
        }
        if(a.exist("help")){
            cout<<a.usage();
        }
        if(a.exist("version")){
            cout<<"Controller-II [Version 1.2.0.0a]"<<endl;
            cout<<"All rights reserved by (c) Lazybones LZQ Corporation."<<endl;
            cout<<"All rights reserved by author LiaoZiqi."<<endl;
        }
        if(a.exist("findfile")){
            if(!a.exist("pattern")){
                cout<<"Loss: [-p]."<<endl<<"Please input the pattern of file."<<endl;
                return 0;
            }
            std::vector<std::wstring> files;
            find_files_recursive(a.get<wstring>("findfile"), a.get<wstring>("pattern"), a.exist("recursive"), a.exist("regex"), files);
            process_files(files, FileAction::List, L"");
        }
    }
    return 0;
}