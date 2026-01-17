#include <iostream>
#include <fstream>
#include <sstream>
#include "dao.h"
#include "config.h"

using namespace std;

// 去除字符串首尾空白字符
static inline string trim(const string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

// 从 CSV 加载资产列表，返回链表头指针；失败返回 nullptr
Asset* dao_load() {
    ifstream fin(CSV_PATH);
    if (!fin.is_open()) {
        // 文件不存在或无法打开，返回空指针
        return nullptr;
    }

    Asset* new_head = nullptr;
    Asset* tail = nullptr;
    string line;

    // 逐行读取，并解析 CSV 字段；
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, category, custodian;
        if (!getline(ss, id, ',')) continue;
        getline(ss, name, ',');
        getline(ss, category, ',');
        getline(ss, custodian, ',');

        Asset* node = new Asset;
        node->asset_id = trim(id);
        node->asset_name = trim(name);
        node->category = trim(category);
        node->custodian = trim(custodian);
        node->next = nullptr;

        if (new_head == nullptr) {
            new_head = node;
            tail = node;
        }
        else {
            tail->next = node;
            tail = node;
        }
    }
    return new_head;
}

// 保存资产链表到 CSV，成功返回 1，失败返回 0
int dao_save(const Asset* head) {
    ofstream fout(CSV_PATH, ios::trunc);
    if (!fout.is_open()) {
        cerr << "无法写入文件: " << CSV_PATH << endl;
        return 0;
    }
    const Asset* current = head;
    while (current != nullptr) {
        fout << current->asset_id << ","
            << current->asset_name << ","
            << current->category << ","
            << current->custodian << "\n";
        current = current->next;
    }
    return 1;
}
