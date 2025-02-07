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

// 前景颜色
#define ANSI_COLOR_BLACK "\x1B[30m"
#define ANSI_COLOR_RED "\x1B[31m"
#define ANSI_COLOR_GREEN "\x1B[32m"
#define ANSI_COLOR_YELLOW "\x1B[33m"
#define ANSI_COLOR_BLUE "\x1B[34m"
#define ANSI_COLOR_MAGENTA "\x1B[35m"
#define ANSI_COLOR_CYAN "\x1B[36m"
#define ANSI_COLOR_WHITE "\x1B[37m"

// 背景颜色
#define ANSI_BACKGROUND_BLACK "\x1B[40m"
#define ANSI_BACKGROUND_RED "\x1B[41m"
#define ANSI_BACKGROUND_GREEN "\x1B[42m"
#define ANSI_BACKGROUND_YELLOW "\x1B[43m"
#define ANSI_BACKGROUND_BLUE "\x1B[44m"
#define ANSI_BACKGROUND_MAGENTA "\x1B[45m"
#define ANSI_BACKGROUND_CYAN "\x1B[46m"
#define ANSI_BACKGROUND_WHITE "\x1B[47m"

// 高亮前景颜色
#define ANSI_COLOR_BRIGHT_BLACK "\x1B[90m"
#define ANSI_COLOR_BRIGHT_RED "\x1B[91m"
#define ANSI_COLOR_BRIGHT_GREEN "\x1B[92m"
#define ANSI_COLOR_BRIGHT_YELLOW "\x1B[93m"
#define ANSI_COLOR_BRIGHT_BLUE "\x1B[94m"
#define ANSI_COLOR_BRIGHT_MAGENTA "\x1B[95m"
#define ANSI_COLOR_BRIGHT_CYAN "\x1B[96m"
#define ANSI_COLOR_BRIGHT_WHITE "\x1B[97m"

// 高亮背景颜色
#define ANSI_BACKGROUND_BRIGHT_BLACK "\x1B[100m"
#define ANSI_BACKGROUND_BRIGHT_RED "\x1B[101m"
#define ANSI_BACKGROUND_BRIGHT_GREEN "\x1B[102m"
#define ANSI_BACKGROUND_BRIGHT_YELLOW "\x1B[103m"
#define ANSI_BACKGROUND_BRIGHT_BLUE "\x1B[104m"
#define ANSI_BACKGROUND_BRIGHT_MAGENTA "\x1B[105m"
#define ANSI_BACKGROUND_BRIGHT_CYAN "\x1B[106m"
#define ANSI_BACKGROUND_BRIGHT_WHITE "\x1B[107m"

// 其他属性
#define ANSI_RESET "\x1B[0m"       // 重置所有属性
#define ANSI_BOLD "\x1B[1m"        // 加粗（高亮）
#define ANSI_UNDERLINE "\x1B[4m"   // 下划线
#define ANSI_REVERSE "\x1B[7m"     // 反显（前景色和背景色互换）
#define ANSI_HIDDEN "\x1B[8m"      // 隐藏文本

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