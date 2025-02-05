#ifdef _WIN32
#include <windows.h>
const WORD BLACK = FOREGROUND_INTENSITY; // 黑色
const WORD BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY; // 蓝色
const WORD GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY; // 绿色
const WORD CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // 青色
const WORD RED = FOREGROUND_RED | FOREGROUND_INTENSITY; // 红色
const WORD MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // 紫红色
const WORD YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY; // 黄色
const WORD WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; // 白色

const WORD DARK_GRAY = FOREGROUND_INTENSITY; // 深灰色
const WORD LIGHT_BLUE = FOREGROUND_BLUE; // 浅蓝色
const WORD LIGHT_GREEN = FOREGROUND_GREEN; // 浅绿色
const WORD LIGHT_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE; // 浅青色
const WORD LIGHT_RED = FOREGROUND_RED; // 浅红色
const WORD LIGHT_MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE; // 浅紫红色
const WORD LIGHT_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN; // 浅黄色
const WORD LIGHT_GRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // 浅灰色
const WORD DEFAULT = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; // 默认颜色
#endif

// 设置控制台颜色的函数
void SetConsoleColor(WORD color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 获取控制台句柄
    SetConsoleTextAttribute(hConsole, color); // 设置颜色
}

void colorDemo(){
    for (int fg = 0; fg < 16; ++fg) {
        for (int bg = 0; bg < 16; ++bg) {
            SetConsoleColor(fg | (bg << 4)); // 设置前景色和背景色
            std::cout << "Color combination: FG=" << fg << ", BG=" << bg << std::endl;
        }
    }
}