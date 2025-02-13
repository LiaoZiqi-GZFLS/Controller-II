#include "findfile.hpp"
// 示例用法
int main() {
    // 1. 文件搜索示例
    std::vector<std::wstring> files;
    find_files_recursive(L"D:\\project\\Controller-II", L".*\\.txt", true, true, files);
    //process_files(files, FileAction::List, L"");

    // 2. 文件树示例
    std::vector<std::wstring> results;
    auto file_tree = build_file_tree(L"D:\\project\\Controller-II", true);
    search_file_tree(file_tree, L".*\\.(cpp|hpp)", true, results);
    process_files(results, FileAction::List, L"");
    return 0;
}