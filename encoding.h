#include <string>
#include <cmath>

std::string detectEncoding(const std::string& str) {
    const size_t length = str.size();
    int asciiCount = 0;
    int utf8_2byteCount = 0;
    int utf8_3byteCount = 0;
    int utf8_4byteCount = 0;
    int gbkCount = 0;
    int isoCount = 0;

    for (size_t i = 0; i < length; ++i) {
        unsigned char c = static_cast<unsigned char>(str[i]);

        // ASCII
        if (c <= 0x7F) {
            asciiCount++;
        }
        // UTF-8 2-byte
        else if (c >= 0xC2 && c <= 0xDF) {
            if (i + 1 < length) {
                unsigned char next = static_cast<unsigned char>(str[i + 1]);
                if (next >= 0x80 && next <= 0xBF) {
                    utf8_2byteCount++;
                    i++; // Skip next byte
                }
            }
        }
        // UTF-8 3-byte
        else if (c >= 0xE0 && c <= 0xEF) {
            if (i + 2 < length) {
                unsigned char next1 = static_cast<unsigned char>(str[i + 1]);
                unsigned char next2 = static_cast<unsigned char>(str[i + 2]);
                if ((next1 >= 0x80 && next1 <= 0xBF) &&
                    (next2 >= 0x80 && next2 <= 0xBF)) {
                    utf8_3byteCount++;
                    i += 2; // Skip next two bytes
                }
            }
        }
        // UTF-8 4-byte
        else if (c >= 0xF0 && c <= 0xF4) {
            if (i + 3 < length) {
                unsigned char next1 = static_cast<unsigned char>(str[i + 1]);
                unsigned char next2 = static_cast<unsigned char>(str[i + 2]);
                unsigned char next3 = static_cast<unsigned char>(str[i + 3]);
                if ((next1 >= 0x80 && next1 <= 0xBF) &&
                    (next2 >= 0x80 && next2 <= 0xBF) &&
                    (next3 >= 0x80 && next3 <= 0xBF)) {
                    utf8_4byteCount++;
                    i += 3; // Skip next three bytes
                }
            }
        }
        // GBK/GB2312 (lead byte in 0x81-0xFE)
        else if (c >= 0x81 && c <= 0xFE) {
            if (i + 1 < length) {
                unsigned char next = static_cast<unsigned char>(str[i + 1]);
                if (next >= 0x40 && next <= 0xFE) {
                    gbkCount++;
                    i++; // Skip next byte
                }
            }
        }
        // ISO-8859-1 (extended ASCII)
        else {
            isoCount++;
        }
    }

    // Calculate percentages
    const double total = static_cast<double>(length);
    double asciiPercentage = static_cast<double>(asciiCount) / total * 100;
    double utf8Percentage = (utf8_2byteCount + utf8_3byteCount + utf8_4byteCount) * 200 / total;
    double gbkPercentage = gbkCount * 200 / total;
    double isoPercentage = static_cast<double>(isoCount) / total * 100;

    // Determine encoding based on heuristics
    if (utf8Percentage > 5) {
        return "UTF-8";
    } else if (gbkPercentage > 5) {
        return "GBK/GB2312";
    } else if (isoPercentage > 5) {
        return "ISO-8859-1";
    } else {
        return "ACSLL/Unknown";
    }
}

std::string GBKToUTF8(const std::string& gbkStr) {
    if (gbkStr.empty()) {
        return "";
    }

    // 将 GBK 转换为 WideChar (Unicode)
    int wideCharLength = MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, nullptr, 0);
    if (wideCharLength == 0) {
        return "";
    }

    std::wstring wideStr(wideCharLength, L'\0');
    if (MultiByteToWideChar(CP_ACP, 0, gbkStr.c_str(), -1, &wideStr[0], wideCharLength) == 0) {
        return "";
    }

    // 将 WideChar 转换为 UTF-8
    int utf8Length = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (utf8Length == 0) {
        return "";
    }

    std::string utf8Str(utf8Length, '\0');
    if (WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Length, nullptr, nullptr) == 0) {
        return "";
    }

    return utf8Str;
}