#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <memory>

// 文件操作类型枚举
enum class FileAction {
    List,    // 列出文件
    Copy,    // 复制文件
    Move,    // 移动文件
    Rename,  // 重命名文件
    Delete   // 删除文件
};

// 文件树节点结构
struct FileTreeNode {
    std::wstring path;       // 完整路径
    bool is_directory;       // 是否为目录
    std::vector<std::unique_ptr<FileTreeNode>> children; // 子节点

    FileTreeNode(const std::wstring& p, bool is_dir)
        : path(p), is_directory(is_dir) {}
};

// 通配符转正则表达式（支持*和?）
std::wstring wildcard_to_regex(const std::wstring& pattern) {
    std::wstring regex_str = L"^";
    for (wchar_t c : pattern) {
        switch (c) {
            case L'*': regex_str += L".*"; break;  // 匹配任意字符
            case L'?': regex_str += L"."; break;   // 匹配单个字符
            case L'.': regex_str += L"\\."; break; // 转义点号
            case L'\\': regex_str += L"\\\\"; break;// 转义反斜杠
            default: regex_str += c; break;
        }
    }
    return regex_str + L"$"; // 完全匹配
}

// 递归创建目录（支持多级目录）
bool create_directories(const std::wstring& path) {
    size_t pos = 0;
    while (pos = path.find_first_of(L"\\/", pos + 1)) {
        std::wstring dir = path.substr(0, pos);
        // 创建目录并忽略已存在错误
        if (!CreateDirectoryW(dir.c_str(), nullptr) && 
            GetLastError() != ERROR_ALREADY_EXISTS) {
            return false;
        }
        if (pos == std::wstring::npos) break;
    }
    return true;
}

// 批量重命名核心功能
void batch_rename(
    std::vector<std::wstring>& files,    // 输入文件列表
    const std::wstring& pattern,        // 匹配模式
    const std::wstring& replacement,    // 替换模式
    bool use_regex                      // 使用正则表达式
) {
    std::wregex re;
    try {
        std::wstring regex_pattern = use_regex ? 
            pattern : wildcard_to_regex(pattern);
        re.assign(regex_pattern);
    } catch (const std::regex_error& e) {
        std::wcerr << L"正则表达式错误: " << e.what() << std::endl;
        return;
    }

    for (auto& file : files) {
        // 分离目录和文件名
        size_t pos = file.find_last_of(L"\\/");
        if (pos == std::wstring::npos) continue;

        std::wstring dir = file.substr(0, pos);
        std::wstring filename = file.substr(pos + 1);

        // 正则替换生成新文件名
        std::wstring new_name = std::regex_replace(filename, re, replacement);
        std::wstring new_path = dir + L"\\" + new_name;

        // 跳过同名文件
        if (new_path == file) continue;

        // 执行重命名并处理错误
        if (!MoveFileW(file.c_str(), new_path.c_str())) {
            DWORD err = GetLastError();
            if (err == ERROR_ALREADY_EXISTS) {
                std::wcerr << L"目标已存在: " << new_path << std::endl;
            } else {
                std::wcerr << L"重命名失败: " << file << L" => " << new_path 
                          << L" (错误码: " << err << L")" << std::endl;
            }
        } else {
            file = new_path; // 更新列表中的路径
        }
    }
}

// 文件处理核心功能
void process_files(
    const std::vector<std::wstring>& files, // 文件列表
    FileAction action,                     // 操作类型
    const std::wstring& target_dir = L""   // 目标目录（移动/复制时需要）
) {
    for (const auto& file : files) {
        switch (action) {
            case FileAction::List: // 列出文件
                std::wcout << L"[发现] " << file << std::endl;
                break;

            case FileAction::Copy: { // 复制文件
                size_t pos = file.find_last_of(L"\\/");
                std::wstring filename = (pos != std::wstring::npos) ? 
                    file.substr(pos + 1) : file;
                std::wstring target = target_dir + L"\\" + filename;

                if (!create_directories(target_dir)) {
                    std::wcerr << L"目录创建失败: " << target_dir << std::endl;
                    continue;
                }

                if (!CopyFileW(file.c_str(), target.c_str(), FALSE)) {
                    std::wcerr << L"复制失败: " << file << L" => " << target 
                              << L" (错误码: " << GetLastError() << L")" << std::endl;
                }
                break;
            }

            case FileAction::Move: { // 移动文件
                size_t pos = file.find_last_of(L"\\/");
                std::wstring filename = (pos != std::wstring::npos) ? 
                    file.substr(pos + 1) : file;
                std::wstring target = target_dir + L"\\" + filename;

                if (!create_directories(target_dir)) {
                    std::wcerr << L"目录创建失败: " << target_dir << std::endl;
                    continue;
                }

                DWORD flags = MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING;
                if (!MoveFileExW(file.c_str(), target.c_str(), flags)) {
                    std::wcerr << L"移动失败: " << file << L" => " << target 
                             << L" (错误码: " << GetLastError() << L")" << std::endl;
                }
                break;
            }

            case FileAction::Delete: { // 删除文件
                DWORD attrs = GetFileAttributesW(file.c_str());
                // 处理只读文件
                if (attrs != INVALID_FILE_ATTRIBUTES && 
                   (attrs & FILE_ATTRIBUTE_READONLY)) {
                    SetFileAttributesW(file.c_str(), attrs & ~FILE_ATTRIBUTE_READONLY);
                }

                if (!DeleteFileW(file.c_str())) {
                    std::wcerr << L"删除失败: " << file 
                              << L" (错误码: " << GetLastError() << L")" << std::endl;
                }
                break;
            }
        }
    }
}

// 递归文件搜索实现
void find_files_recursive(
    const std::wstring& directory,   // 搜索目录
    const std::wstring& pattern,     // 匹配模式
    bool recursive,                  // 是否递归子目录
    bool use_regex,                  // 是否使用正则表达式
    std::vector<std::wstring>& results // 结果存储
) {
    WIN32_FIND_DATAW find_data;
    std::wstring search_path = directory + L"\\*";

    HANDLE hFind = FindFirstFileW(search_path.c_str(), &find_data);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        // 跳过.和..目录
        if (wcscmp(find_data.cFileName, L".") == 0 || 
           wcscmp(find_data.cFileName, L"..") == 0) continue;

        std::wstring full_path = directory + L"\\" + find_data.cFileName;

        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // 递归搜索子目录
            if (recursive) {
                find_files_recursive(full_path, pattern, recursive, use_regex, results);
            }
        } else {
            // 构建匹配模式
            std::wstring regex_pattern = use_regex ? 
                pattern : wildcard_to_regex(pattern);
            std::wregex re(regex_pattern, std::regex::icase);

            // 执行匹配
            if (std::regex_match(find_data.cFileName, re)) {
                results.push_back(full_path);
            }
        }
    } while (FindNextFileW(hFind, &find_data));

    FindClose(hFind);
}

// 构建文件树（智能指针自动管理内存）
std::unique_ptr<FileTreeNode> build_file_tree(
    const std::wstring& path,    // 根目录路径
    bool recursive               // 是否递归构建
) {
    DWORD attrs = GetFileAttributesW(path.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) return nullptr;

    // 创建节点
    auto node = std::make_unique<FileTreeNode>(path, (attrs & FILE_ATTRIBUTE_DIRECTORY));

    if (node->is_directory) {
        std::wstring search_path = path + L"\\*";
        WIN32_FIND_DATAW find_data;

        HANDLE hFind = FindFirstFileW(search_path.c_str(), &find_data);
        if (hFind == INVALID_HANDLE_VALUE) return node;

        do {
            if (wcscmp(find_data.cFileName, L".") == 0 || 
               wcscmp(find_data.cFileName, L"..") == 0) continue;

            std::wstring child_path = path + L"\\" + find_data.cFileName;
            // 递归构建子节点
            if (recursive || !(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                node->children.push_back(build_file_tree(child_path, recursive));
            }
        } while (FindNextFileW(hFind, &find_data));

        FindClose(hFind);
    }

    return node;
}

// 文件树搜索功能
void search_file_tree(
    const std::unique_ptr<FileTreeNode>& node,  // 当前节点
    const std::wstring& pattern,               // 匹配模式
    bool use_regex,                            // 是否使用正则
    std::vector<std::wstring>& results         // 搜索结果
) {
    if (!node) return;

    // 文件节点进行匹配
    if (!node->is_directory) {
        std::wstring filename = node->path.substr(node->path.find_last_of(L"\\/") + 1);
        std::wstring regex_pattern = use_regex ? pattern : wildcard_to_regex(pattern);
        std::wregex re(regex_pattern, std::regex::icase);

        if (std::regex_match(filename, re)) {
            results.push_back(node->path);
        }
    }

    // 递归搜索子节点
    for (const auto& child : node->children) {
        search_file_tree(child, pattern, use_regex, results);
    }
}

// ================= 使用示例 =================
int main() {
    // 示例1：查找并列出所有txt文件
    std::vector<std::wstring> txt_files;
    find_files_recursive(L"D:\\project\\Controller-II", L"*.txt", false, false, txt_files);
    process_files(txt_files, FileAction::List);

    return 0;

    // 示例2：复制图片到备份目录
    std::vector<std::wstring> images;
    find_files_recursive(L"D:\\Photos", L"*.jpg", true, false, images);
    process_files(images, FileAction::Copy, L"D:\\Backup\\Photos");

    // 示例3：构建项目文件树并搜索源代码
    auto project_tree = build_file_tree(L"C:\\Projects\\MyApp", true);
    std::vector<std::wstring> source_files;
    search_file_tree(project_tree, L".*\\.(cpp|h)", true, source_files);
    process_files(source_files, FileAction::List);

    // 示例4：批量重命名日志文件
    std::vector<std::wstring> log_files;
    find_files_recursive(L"C:\\Logs", L"log_*.txt", true, false, log_files);
    batch_rename(log_files, L"log_(\\d+).txt", L"archive_$1.log", true);

    return 0;
}