#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <memory>

// 操作类型枚举
enum class FileAction {
    List,    // 仅列出文件
    Copy,    // 复制文件
    Move,    // 移动文件
    Rename,  // 重命名文件
    Delete   // 删除文件
};

// 文件树节点结构
struct FileTreeNode {
    std::wstring path;
    bool is_directory;
    std::vector<std::unique_ptr<FileTreeNode>> children;

    FileTreeNode(const std::wstring& p, bool is_dir)
        : path(p), is_directory(is_dir) {}
};

// 通配符转正则表达式
std::wstring wildcard_to_regex(const std::wstring& pattern) {
    std::wstring regex_str = L"^";
    for (wchar_t c : pattern) {
        switch (c) {
            case L'*': regex_str += L".*"; break;
            case L'?': regex_str += L"."; break;
            case L'.': regex_str += L"\\."; break;
            case L'\\': regex_str += L"\\\\"; break;
            default: regex_str += c; break;
        }
    }
    return regex_str + L"$";
}

// 递归创建目录
bool create_directories(const std::wstring& path) {
    size_t pos = 0;
    while ((pos = path.find_first_of(L"\\/", pos + 1)) != std::wstring::npos) {
        std::wstring dir = path.substr(0, pos);
        if (!CreateDirectoryW(dir.c_str(), nullptr) && GetLastError() != ERROR_ALREADY_EXISTS) {
            return false;
        }
        if (pos == std::wstring::npos) break;
    }
    return true;
}

// 批量重命名文件
void batch_rename(
    std::vector<std::wstring>& files,
    const std::wstring& pattern,
    const std::wstring& replacement,
    bool use_regex
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
        // 提取目录和原文件名
        size_t pos = file.find_last_of(L"\\/");
        if (pos == std::wstring::npos) continue;

        std::wstring dir = file.substr(0, pos);
        std::wstring filename = file.substr(pos + 1);

        // 执行替换操作
        std::wstring new_name = std::regex_replace(filename, re, replacement);
        std::wstring new_path = dir + L"\\" + new_name;

        // 跳过同名文件
        if (new_path == file) continue;

        // 执行重命名
        if (!MoveFileW(file.c_str(), new_path.c_str())) {
            DWORD err = GetLastError();
            if (err == ERROR_ALREADY_EXISTS) {
                std::wcerr << L"目标文件已存在: " << new_path << std::endl;
            } else {
                std::wcerr << L"重命名失败: " << file << L" => " << new_path 
                          << L" (错误码: " << err << L")" << std::endl;
            }
        } else {
            file = new_path; // 更新文件列表
        }
    }
}

// 文件处理核心函数
void process_files(
    const std::vector<std::wstring>& files,
    FileAction action,
    const std::wstring& target_dir = L"",
    const std::wstring& pattern = L"",
    bool use_regex = false
) {
    for (const auto& file : files) {
        switch (action) {
            case FileAction::List:
            std::cout<<"Find file: ";
                std::wcout << file << std::endl;
                break;

            case FileAction::Copy: {
                size_t pos = file.find_last_of(L"\\/");
                std::wstring filename = (pos != std::wstring::npos) ? file.substr(pos + 1) : file;
                std::wstring target = target_dir + L"\\" + filename;

                if (!create_directories(target_dir)) {
                    std::cout<<"Create directory failed: ";
                    std::wcerr << target_dir << std::endl;
                    continue;
                }

                if (!CopyFileW(file.c_str(), target.c_str(), FALSE)) {
                    std::cerr<<"Copy failed: ";
                    std::wcerr << file ;
                    std::cerr<<" => ";
                    std::wcerr << target << std::endl;

                }
                break;
            }

            // 在process_files函数中添加移动处理
            case FileAction::Move: {
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

            case FileAction::Rename: {
                batch_rename(const_cast<std::vector<std::wstring>&>(files),  target_dir, pattern, use_regex);
                break;
            }

            case FileAction::Delete: {
                DWORD attrs = GetFileAttributesW(file.c_str());
                if (attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_READONLY)) {
                    SetFileAttributesW(file.c_str(), attrs & ~FILE_ATTRIBUTE_READONLY);
                }

                if (!DeleteFileW(file.c_str())) {
                    std::wcerr << L"删除失败: " << file << L" (错误码: " << GetLastError() << L")" << std::endl;
                    std::cerr << "Delete failed: " << std::endl;
                    std::wcerr << file;
                    std::cerr << "Error code: " << GetLastError() << std::endl;
                }
                break;
            }
        }
    }
}

// 递归文件搜索实现
void find_files_recursive(
    const std::wstring& directory,
    const std::wstring& pattern,
    bool recursive,
    bool use_regex,
    std::vector<std::wstring>& results
) {
    WIN32_FIND_DATAW find_data;
    std::wstring search_path = directory + L"\\*";
    //std::wcout << search_path << std::endl;

    HANDLE hFind = FindFirstFileW(search_path.c_str(), &find_data);
    if (hFind == INVALID_HANDLE_VALUE) return;

    do {
        if (wcscmp(find_data.cFileName, L".") == 0 || 
           wcscmp(find_data.cFileName, L"..") == 0) continue;

        std::wstring full_path = directory + L"\\" + find_data.cFileName;
        //std::wcout << full_path << std::endl;

        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            //std::wcout << full_path << std::endl;
            if (recursive) {
                find_files_recursive(full_path, pattern, recursive, use_regex, results);
            }
        } else {
            //std::wcout << full_path << std::endl;
            std::wstring regex_pattern = use_regex ? pattern : wildcard_to_regex(pattern);
            std::wregex re(regex_pattern, std::regex::icase);
            
            bool match = std::regex_match(find_data.cFileName, re);
            if (match) {
                //std::wcout << full_path << std::endl;
                results.push_back(full_path);
            }
        }
    } while (FindNextFileW(hFind, &find_data));

    FindClose(hFind);
}

// 构建文件树（智能指针版）
std::unique_ptr<FileTreeNode> build_file_tree(
    const std::wstring& path,
    bool recursive
) {
    DWORD attrs = GetFileAttributesW(path.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) return nullptr;

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
            if (recursive || !(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                node->children.push_back(build_file_tree(child_path, recursive));
            }
        } while (FindNextFileW(hFind, &find_data));

        FindClose(hFind);
    }

    return node;
}

void search_file_tree(
    const std::unique_ptr<FileTreeNode>& node,  // 当前节点
    const std::wstring& pattern,               // 匹配模式
    bool use_regex,                            // 是否使用正则表达式
    std::vector<std::wstring>& results         // 搜索结果
) {
    if (!node) return;

    // 如果是文件则进行匹配
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

