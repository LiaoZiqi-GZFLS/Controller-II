#include <iostream>
#include <opencv2/opencv.hpp> 
#include <windows.h> // 用于获取窗口大小

// 字符集，从暗到亮
const char* charSet = "@W#$OEXC[(/?=^~_. ";

// 获取命令行窗口大小
void getConsoleSize(int& width, int& height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

// 将灰度值映射到字符
char getCharFromGray(int gray) {
    int index = gray * (strlen(charSet) - 1) / 255;
    return charSet[index];
}

// 主程序
int main() {
    // 打开视频文件
    cv::VideoCapture cap("input.mp4");
    if (!cap.isOpened()) {
        std::cerr << "Error opening video file" << std::endl;
        return -1;
    }

    // 获取初始窗口大小
    int consoleWidth, consoleHeight;
    getConsoleSize(consoleWidth, consoleHeight);

    // 循环读取视频帧
    cv::Mat frame, grayFrame;
    while (cap.read(frame)) {
        // 转换为灰度图像
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);

        // 根据窗口大小调整图像分辨率
        double aspectRatio = grayFrame.cols / (double)grayFrame.rows;
        int newWidth = consoleWidth - 2; // 减去边框
        int newHeight = static_cast<int>(newWidth / aspectRatio);

        if (newHeight > consoleHeight - 2) { // 确保高度不超过窗口
            newHeight = consoleHeight - 2;
            newWidth = static_cast<int>(newHeight * aspectRatio);
        }

        cv::resize(grayFrame, grayFrame, {newWidth, newHeight});

        // 转换为字符画
        std::string charArt;
        for (int y = 0; y < grayFrame.rows; y++) {
            for (int x = 0; x < grayFrame.cols; x++) {
                int gray = grayFrame.at<uchar>(y, x);
                charArt += getCharFromGray(gray);
            }
            charArt += "\n"; // 换行
        }

        // 清屏并输出字符画
        system("cls");
        std::cout << charArt;

        // 监听窗口大小变化
        //GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleWidth, &consoleHeight);

        // 延迟
        cv::waitKey(30);
    }

    cap.release();
    return 0;
}