#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class EnvironmentConfig {
private:
    std::vector<std::string> validVars;
    bool DETECT_ENCODING, IOmod, DEBUG;

    bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }

    void readConfig(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;

        while (std::getline(file, line)) {
            // 跳过空行和注释
            if (line.empty() || line[0] == '#') {
                continue;
            }

            // 解析行
            std::istringstream iss(line);
            std::string varName, symbol, value;
            if (!(iss >> varName >> symbol >> value) && symbol != "=") {
                std::cout << "warning::error &jumping invalid line - " << line << std::endl;
                continue;
            }

            // 检查变量名是否有效
            bool isValidVar = false;
            for (const std::string& validVar : validVars) {
                if (varName == validVar) {
                    isValidVar = true;
                    break;
                }
            }

            if (!isValidVar) {
                std::cout << "warning::error &jumping invalid variable - " << varName << std::endl;
                continue;
            }

            // 赋值
            if (varName == "DETECT_ENCODING") {
                if (value == "True") {
                    DETECT_ENCODING = true;
                } else if (value == "False") {
                    DETECT_ENCODING = false;
                } else {
                    std::cout << "warring::error &jumping invalid value::[" << value << "] - " << std::endl;
                }
            }
            if (varName == "IOmod") {
                if (value == "True") {
                    IOmod = true;
                } else if (value == "False") {
                    IOmod = false;
                } else {
                    std::cout << "warring::error &jumping invalid value::[" << value << "] - " << std::endl;
                }
            }
        }
    }

public:
    EnvironmentConfig(const std::vector<std::string>& validVars)
        : validVars(validVars), DETECT_ENCODING(false) {
        std::string filename = "con2.ini"; // 默认文件名
        if (fileExists(filename)) {
            readConfig(filename);
        } else {
            std::cout << "warring::file [" << filename << "] doesn't exist! - " << std::endl;
        }
    }

    // 提供可选的文件名参数
    EnvironmentConfig(const std::vector<std::string>& validVars, const std::string& filename)
        : validVars(validVars), DETECT_ENCODING(false) {
        if (fileExists(filename)) {
            readConfig(filename);
        } else {
            std::cout << "warring::file [" << filename << "] doesn't exist! - " << std::endl;
        }
    }

    // 修改变量值
    void setVariable(const std::string& varName, const std::string& newValue, const std::string& filename) {
        std::ifstream file(filename);
        std::vector<std::string> lines;
        bool found = false;

        std::string line;
        while (std::getline(file, line)) {
            // 保留注释和空行
            if (!line.empty() && line[0] != '#') {
                std::istringstream iss(line);
                std::string name, symbol, value;
                if (iss >> name >> symbol >> value && symbol == "=") {
                    if (name == varName) {
                        line = name + " " + symbol + " " + newValue;
                        found = true;
                    }
                }
            }
            lines.push_back(line);
        }

        file.close();

        if (!found) {
            std::cout << "warning::error &variable not found::[" << varName << "] $can't be changed!" << std::endl;
            return;
        }

        // 更新内存中的值
        if (varName == "DETECT_ENCODING") {
            if (newValue == "True") {
                DETECT_ENCODING = true;
            } else if (newValue == "False") {
                DETECT_ENCODING = false;
            } else {
                std::cout << "警告：无效的值 - " << newValue << "，内存中值未更新" << std::endl;
                return;
            }
        }
        if (varName == "IOmod") {
            if (newValue == "True") {
                IOmod = true;
            } else if (newValue == "False") {
                IOmod = false;
            } else {
                std::cout << "警告：无效的值 - " << newValue << "，内存中值未更新" << std::endl;
                return;
            }
        }

        // 将内容写回文件
        std::ofstream outFile(filename);
        for (const std::string& l : lines) {
            outFile << l << std::endl;
        }
        outFile.close();
    }

    // 获取变量值
    bool getDETECT_ENCODING() const {
        return DETECT_ENCODING;
    }
    bool getIOmod() const {
        return IOmod;
    }
};

#endif // ENVIRONMENT_HPP