#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

// 全局变量
std::vector<std::string> ascii_content;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO csbi;
bool running = true;

void enable_vt_mode() {
    // 启用虚拟终端支持
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void load_ascii(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    ascii_content.clear();
    while (std::getline(file, line)) {
        ascii_content.push_back(line);
    }
}

void update_console_size() {
    GetConsoleScreenBufferInfo(hConsole, &csbi);
}

void clear_screen() {
    // 使用VT100指令清屏
    std::cout << "\x1b[2J";
}

void set_cursor_pos(int x, int y) {
    // 使用VT100指令设置光标位置
    std::cout << "\x1b[" << y + 1 << ";" << x + 1 << "H";
}

void display_art() {
    clear_screen();
    
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int term_w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int term_h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int content_h = ascii_content.size();
    int content_w = content_h > 0 ? ascii_content[0].size() : 0;

    int display_h = std::min(term_h, content_h);
    int display_w = std::min(term_w, content_w);

    int start_y = std::max(0, (term_h - content_h) / 2);
    int start_x = std::max(0, (term_w - content_w) / 2);

    for (int y = 0; y < display_h; ++y) {
        if (y < content_h) {
            std::string line = ascii_content[y].substr(0, display_w);
            set_cursor_pos(start_x, start_y + y);
            std::cout << line;
        }
    }
    std::cout.flush();
}

BOOL WINAPI ctrl_handler(DWORD fdwCtrlType) {
    running = false;
    return TRUE;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ascii_file>" << std::endl;
        return 1;
    }

    // 设置控制台模式
    enable_vt_mode();
    SetConsoleCtrlHandler(ctrl_handler, TRUE);

    try {
        load_ascii(argv[1]);
    } catch (...) {
        std::cerr << "Failed to load file: " << argv[1] << std::endl;
        return 1;
    }

    while (running) {
        display_art();
        
        // 检查窗口大小变化
        static int last_w = 0, last_h = 0;
        update_console_size();
        int current_w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int current_h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        
        if (current_w != last_w || current_h != last_h) {
            last_w = current_w;
            last_h = current_h;
            display_art();
        }

        // 检查输入
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 'q' || ch == 'Q') break;
        }

        Sleep(50);
    }

    // 恢复光标位置
    clear_screen();
    set_cursor_pos(0, 0);
    return 0;
}