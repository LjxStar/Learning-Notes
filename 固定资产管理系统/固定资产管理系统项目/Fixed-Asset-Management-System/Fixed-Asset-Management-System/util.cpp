#include "util.h"
#include <iostream>
#include <limits>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

// 询问是/否
bool askYesNo(const string& message) {
    char choice;
    while (true) {
        cout << message << " (y/n): ";
        cin >> choice;
        clearInputBuffer();

        if (choice == 'y' || choice == 'Y') {
            return true;
        }
        else if (choice == 'n' || choice == 'N') {
            return false;
        }
        else {
            cout << "无效输入，请输入 y 或 n。" << endl;
        }
    }
}

// 暂停并等待用户按回车键
void pauseAndWaitEnter() {
    cout << "\n操作完成，按回车键继续..." << endl;
    clearInputBuffer();
    cin.get();
}

// 清空输入缓冲区
void clearInputBuffer() {
    cin.clear();
    // 检查缓冲区是否已经为空，避免阻塞
    if (cin.rdbuf()->in_avail() > 0) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// 验证名称：1-10个字符
bool isValidName(const string& name) {
    // 过滤全空白字符的情况
    if (name.empty()) return false;
    bool allWhitespace = all_of(name.begin(), name.end(), [](char c) {
        return isspace(static_cast<unsigned char>(c));
        });
    if (allWhitespace) return false;

    size_t len = name.length();
    return len >= 1 && len <= 10;
}

// 验证保管人：1-10个字符
bool isValidCustodian(const string& name) {
    // 过滤空字符串
    if (name.empty()) return false;
    bool allWhitespace = all_of(name.begin(), name.end(), [](char c) {
        return isspace(static_cast<unsigned char>(c));
        });
    if (allWhitespace) return false;

    size_t len = name.length();
    return len >= 1 && len <= 10;
}

// 验证类别：必须是指定几类
bool isValidCategory(const string& category) {
    return (category == "教学类" || category == "办公类" ||
        category == "生活类" || category == "其他类");
}

// 自动生成ID：遍历链表寻找最小未用ID
std::string generateNextId(const Asset* head) {
    std::set<int> existingIds;
    const Asset* current = head;

    while (current != nullptr) {
        try {
            if (!current->asset_id.empty()) {
                int id = std::stoi(current->asset_id);
                // 仅收集非负ID
                if (id >= 0) {
                    existingIds.insert(id);
                }
            }
        }
        catch (...) {
            // 非数字ID、空ID或转换异常
        }
        current = current->next;
    }

    int nextId = 0;
    for (int id : existingIds) {
        if (id == nextId) {
            // 当前ID已存在，检查下一个
            nextId++;
        }
        else {
            // 找到空缺ID，直接返回
            break;
        }
    }

    // 转换为字符串返回
    return std::to_string(nextId);
}

// 释放所有资产链表的内存
void free_all_assets() {
    Asset* current = head;
    while (current != nullptr) {
        Asset* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}