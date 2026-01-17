#include <iostream>
#include <iomanip>  //setw
#include <string>
#include <vector>
#include "entity.h"
#include "service.h"
#include "ui.h"
#include "util.h"

using namespace std;
Asset* head = nullptr;

// 增加资产
int add_asset() {
    clearInputBuffer(); // 确保输入流初始干净
    Asset* new_asset = new Asset;
    new_asset->next = nullptr; // 初始化next指针，避免野指针

    // 1. 资产ID输入与处理
    int input_id;
    bool is_id_valid = false;
    while (!is_id_valid) {
        cout << "请输入资产ID（直接回车自动分配）：";

        if (cin.peek() == '\n') {
            clearInputBuffer(); 
            new_asset->asset_id = generateNextId(head);
            cout << "系统已自动分配资产ID: " << new_asset->asset_id << endl;
            is_id_valid = true;
        }

        else if (cin >> input_id) {
            clearInputBuffer();

            if (input_id < 0) {
                cout << "资产ID不能为负数，请重新输入！" << endl;
            }

            else {
                string manual_id = to_string(input_id);
                
                Asset* current = head;
                bool id_exist = false;
                while (current != nullptr) {
                    if (current->asset_id == manual_id) {
                        id_exist = true;
                        break;
                    }
                    current = current->next;
                }
                if (id_exist) {
                    cout << "资产ID [" << manual_id << "] 已存在，请重新输入！" << endl;
                }
                else {
                    new_asset->asset_id = manual_id;
                    cout << "资产ID [" << manual_id << "] 验证通过！" << endl;
                    is_id_valid = true;
                }
            }
        }

        else {
            clearInputBuffer();
            cout << "错误：资产ID必须为整数，请重新输入！" << endl;
        }
    }

    // 2. 资产名称输入与验证
    while (true) {
        cout << "请输入资产名称: ";
        getline(cin, new_asset->asset_name);
        if (isValidName(new_asset->asset_name)) {
            break;
        }
        cout << "名称长度不符合要求（需1-10个字符），请重新输入。" << endl;
    }

    // 3. 资产类别输入与验证
    // 资产类别选择（数字1-4）
    while (true) {
        cout << "\n请选择资产类别（输入对应数字）：" << endl;
        cout << "1 - 教学类" << endl;
        cout << "2 - 办公类" << endl;
        cout << "3 - 生活类" << endl;
        cout << "4 - 其他类" << endl;
        cout << "请输入数字（1-4）：";

        bool valid_category = false;
        int category_choice;
        // 读取数字并校验输入合法性
        if (cin >> category_choice) {
            clearInputBuffer(); // 清空输入缓冲区的换行符
            // 根据数字映射到对应类别
            switch (category_choice) {
            case 1:
                new_asset->category = "教学类";
                valid_category = true; // 类别有效，跳出循环
            case 2:
                new_asset->category = "办公类";
                valid_category = true;
            case 3:
                new_asset->category = "生活类";
                valid_category = true;
            case 4:
                new_asset->category = "其他类";
                valid_category = true;
            default:
                cout << "请输入 1-4 之间的数字！" << endl;
                break;
            }
        }
        else {
            // 处理非数字输入（字母/符号等）
            clearInputBuffer();
            cout << "请输入有效的数字（1-4）！" << endl;
        }

        if (valid_category) {
            break;
        } 

    }

    // 4. 保管人输入与验证
    while (true) {
        cout << "请输入保管人姓名: ";
        getline(cin, new_asset->custodian);
        if (isValidCustodian(new_asset->custodian)) {
            break;
        }
        cout << "错误：保管人姓名长度不符合要求（需1-10个字符），请重新输入。" << endl;
    }

    // 5. 插入链表
    new_asset->next = head;
    head = new_asset;

    cout << "\n资产 [" << new_asset->asset_name << "] (ID: " << new_asset->asset_id << ") 添加成功！" << endl;
    return 1;
}

// 按ID删除资产
int delete_by_id() {
    string id_to_delete;
    cout << "请输入要删除的资产代号: ";
    cin >> id_to_delete;
    clearInputBuffer();

    Asset* current = head;
    Asset* previous = nullptr;

    // 遍历查找
    while (current != nullptr && current->asset_id != id_to_delete) {
        previous = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "未找到资产代号为 [" << id_to_delete << "] 的资产。" << endl;
        return 0;
    }

    // 删除头节点
    if (previous == nullptr) {
        head = current->next;
    }
	// 删除中间或尾节点
    else {
        previous->next = current->next;
    }

    cout << "资产 [" << current->asset_name << "] (id: " << id_to_delete << ") 已删除。" << endl;
    delete current;
    return 1;
}

// 按名称删除资产
int delete_by_name() {
    string name_to_delete;
    cout << "请输入要删除的资产名称(支持模糊): ";
    clearInputBuffer();
    getline(cin, name_to_delete);

    bool continue_deleting = true;
    int total_deleted = 0; // 统计删除的资产总数

    while (continue_deleting) {
        // 查找所有匹配名称的资产
        vector<pair<Asset*, Asset*>> matched_assets = findAssetsByName(name_to_delete);
        int asset_count = displayMatchedAssets(matched_assets);

        // 无匹配资产的处理
        if (asset_count == 0) {
            cout << "未找到名称包含 [" << name_to_delete << "] 的资产。" << endl;
            break;
        }

        // 输入要删除的序号
        int selected_idx = -1;
        bool valid_input = false;
        while (!valid_input) {
            cout << "请输入要删除的资产序号（0-" << asset_count - 1 << "）: ";
            if (cin >> selected_idx) {
                clearInputBuffer(); // 清空换行符
                if (selected_idx >= 0 && selected_idx < asset_count) {
                    valid_input = true;
                }
                else {
                    cout << "序号超出范围，请输入 0 到 " << asset_count - 1 << " 之间的数字！" << endl;
                }
            }
            else {
                // 处理非数字输入
                clearInputBuffer();
                cout << "请输入有效的整数序号！" << endl;
            }
        }

        // 执行删除操作
        Asset* prev_node = matched_assets[selected_idx].first;
        Asset* target_node = matched_assets[selected_idx].second;

        // 删除头节点
        if (prev_node == nullptr) {
            head = target_node->next;
        }
        // 删除中间/尾节点
        else {
            prev_node->next = target_node->next;
        }

        // 释放内存并统计
        cout << "已删除资产：名称=>" << target_node->asset_name << " | ID=>" << target_node->asset_id << endl;
        delete target_node;
        total_deleted++;

        // 询问是否继续删除
        if (askYesNo("\n是否继续删除名称包含 [" + name_to_delete + "] 的资产？")) {
            continue_deleting = true;
        }
        else {
            continue_deleting = false;
        }
    }

    // 最终结果提示
    if (total_deleted > 0) {
        cout << "\n本次操作共删除 " << total_deleted << " 个资产，删除完成！" << endl;
        return 1;
    }
    else {
        return 0;
    }
}

// 按类别删除资产
int delete_by_category() {
    cout << "\n--- 按资产类别删除（精确匹配） ---" << endl;
    // 选择资产类别
    int category_choice = 0;
    string target_category;
    bool valid_choice = false;
    while (!valid_choice) {
        cout << "请选择要删除的资产类别（1-4）：" << endl;
        cout << "1 - 教学类" << endl;
        cout << "2 - 办公类" << endl;
        cout << "3 - 生活类" << endl;
        cout << "4 - 其他类" << endl;

        if (cin >> category_choice) {
            clearInputBuffer();
            switch (category_choice) {
            case 1: target_category = "教学类"; valid_choice = true; break;
            case 2: target_category = "办公类"; valid_choice = true; break;
            case 3: target_category = "生活类"; valid_choice = true; break;
            case 4: target_category = "其他类"; valid_choice = true; break;
            default: cout << "序号超出范围，请输入 1-4 之间的数字！" << endl;
            }
        }
        else {
            clearInputBuffer();
            cout << "请输入有效的整数序号！" << endl;
        }
    }

    // 循环删除逻辑
    bool continue_deleting = true;
    int total_deleted = 0;
    while (continue_deleting) {
        // 查找该类别下的所有资产
        vector<pair<Asset*, Asset*>> matched_assets = findAssetsByCategory(target_category);
        int asset_count = displayMatchedAssets(matched_assets);

        // 无匹配资产
        if (asset_count == 0) {
            cout << "未找到类别为 [" << target_category << "] 的资产。" << endl;
            break;
        }

        // 输入要删除的资产序号
        int selected_idx = -1;
        bool valid_input = false;
        while (!valid_input) {
            cout << "请输入要删除的资产序号（0-" << asset_count - 1 << "）: ";
            if (cin >> selected_idx) {
                clearInputBuffer();
                if (selected_idx >= 0 && selected_idx < asset_count) {
                    valid_input = true;
                }
                else {
                    cout << "序号超出范围，请输入 0 到 " << asset_count - 1 << " 之间的数字！" << endl;
                }
            }
            else {
                clearInputBuffer();
                cout << "请输入有效的整数序号！" << endl;
            }
        }

        // 执行删除操作
        Asset* prev_node = matched_assets[selected_idx].first;
        Asset* target_node = matched_assets[selected_idx].second;

        // 删除头节点
        if (prev_node == nullptr) {
            head = target_node->next; 
        }
        // 删除中间/尾节点
        else {
            prev_node->next = target_node->next; 
        }

        // 释放内存并统计
        cout << "已删除资产：名称=>" << target_node->asset_name
            << " | 类别=>" << target_category << endl;
        delete target_node;
        total_deleted++;

        // 询问是否继续删除该类别资产
        continue_deleting = askYesNo("\n是否继续删除 [" + target_category + "] 类的资产？");
    }

    // 最终结果提示
    if (total_deleted > 0) {
        cout << "\n本次操作共删除 " << total_deleted << " 个 [" << target_category << "] 类资产，删除完成！" << endl;
        return 1;
    }
    else {
        return 0;
    }
}

// 按保管人删除资产
int delete_by_custodian() {
    string custodian_to_delete;
    cout << "\n--- 按保管人删除（支持模糊匹配） ---" << endl;
    cout << "请输入要删除的资产保管人姓名: ";
    clearInputBuffer();
    getline(cin, custodian_to_delete);

    bool continue_deleting = true;
    int total_deleted = 0;

    while (continue_deleting) {
        // 查找匹配保管人的资产
        vector<pair<Asset*, Asset*>> matched_assets = findAssetsByCustodian(custodian_to_delete);
        int asset_count = displayMatchedAssets(matched_assets);

        // 无匹配资产
        if (asset_count == 0) {
            cout << "未找到保管人包含 [" << custodian_to_delete << "] 的资产。" << endl;
            break;
        }

        // 输入删除序号
        int selected_idx = -1;
        bool valid_input = false;
        while (!valid_input) {
            cout << "请输入要删除的资产序号（0-" << asset_count - 1 << "）: ";
            if (cin >> selected_idx) {
                clearInputBuffer();
                if (selected_idx >= 0 && selected_idx < asset_count) {
                    valid_input = true;
                }
                else {
                    cout << "序号超出范围，请输入 0 到 " << asset_count - 1 << " 之间的数字！" << endl;
                }
            }
            else {
                clearInputBuffer();
                cout << "请输入有效的整数序号！" << endl;
            }
        }

        // 执行删除
        Asset* prev_node = matched_assets[selected_idx].first;
        Asset* target_node = matched_assets[selected_idx].second;

        if (prev_node == nullptr) {
            head = target_node->next;
        }
        else {
            prev_node->next = target_node->next;
        }

        cout << "已删除资产：名称=>" << target_node->asset_name << " | 保管人=>" << target_node->custodian << endl;
        delete target_node;
        total_deleted++;

        // 询问是否继续删除
        continue_deleting = askYesNo("\n是否继续删除保管人包含 [" + custodian_to_delete + "] 的资产？");
    }

    // 结果提示
    if (total_deleted > 0) {
        cout << "\n本次操作共删除 " << total_deleted << " 个资产，删除完成！" << endl;
        return 1;
    }
    else {
        return 0;
    }
}

// 修改资产信息
int modify_asset() {
    std::string id_to_modify;
    std::cout << "请输入要修改的资产代号: ";
    std::cin >> id_to_modify;
    clearInputBuffer();

    // 1. 查找目标资产并验证资产是否存在
    Asset* current = head;
    while (current != nullptr && current->asset_id != id_to_modify) {
        current = current->next;
    }

    if (current == nullptr) {
        std::cout << "未找到资产代号为 [" << id_to_modify << "] 的资产。" << std::endl;
        pauseAndWaitEnter();
        return 0;
    }

	// 2. 展示当前资产信息并选择修改字段
    std::cout << "\n当前资产信息如下：" << std::endl;
    display_header();
    display_single_asset(current);

    int choice = -1;
    while (true) {
        std::cout << "\n请选择要修改的字段(0-3)：\n";
        std::cout << "1. 资产名称" << std::endl;
        std::cout << "2. 资产类别" << std::endl;
        std::cout << "3. 保管人" << std::endl;
        std::cout << "0. 取消" << std::endl;

        // 验证输入是否为数字
        if (!(std::cin >> choice)) {
            std::cout << "请输入有效的数字（0-3）！" << std::endl;
            clearInputBuffer(); // 清空非法输入
            continue;
        }
        clearInputBuffer(); 

        // 验证选择范围
        if (choice >= 0 && choice <= 3) {
            break; // 输入合法，退出循环
        }
        else {
            std::cout << "请输入0-3之间的数字！" << std::endl;
        }
    }

    // 3. 处理字段修改
    bool modify_success = false;
    switch (choice) {
    case 1: { // 修改资产名称
        std::string new_name;
        while (true) {
            std::cout << "\n请输入新的资产名称（1-10个汉字/字符，不能为空）：";
            std::getline(std::cin, new_name);

            // 验证名称合法性
            if (isValidName(new_name)) {
                current->asset_name = new_name;
                modify_success = true;
                break;
            }
            else {
                std::cout << "资产名称格式无效！。" << std::endl;
                pauseAndWaitEnter();
            }
        }
        break;
    }
    case 2: { // 修改资产类别
        std::string new_cat;
        while (true) {
            std::cout << "\n请输入新的资产类别（仅限：教学类, 办公类, 生活类, 其他类）：";
            std::getline(std::cin, new_cat);

            // 验证类别合法性
            if (isValidCategory(new_cat)) {
                current->category = new_cat;
                modify_success = true;
                break;
            }
            else {
                std::cout << "资产类别无效！" << std::endl;
                pauseAndWaitEnter();
            }
        }
        break;
    }
    case 3: { // 修改保管人
        std::string new_cus;
        while (true) {
            std::cout << "\n请输入新的保管人姓名（1-10个汉字/字符，不能为空）：";
            std::getline(std::cin, new_cus);

            // 验证保管人合法性
            if (isValidCustodian(new_cus)) {
                current->custodian = new_cus;
                modify_success = true;
                break;
            }
            else {
                std::cout << "保管人格式无效！" << std::endl;
                pauseAndWaitEnter();
            }
        }
        break;
    }
    case 0: { // 取消修改
        std::cout << "已取消修改操作。" << std::endl;
        return 0;
    }
    default: { 
        std::cout << "无效的选择！" << std::endl;
        pauseAndWaitEnter();
        return 0;
    }
    }

    // 4. 修改成功后的反馈
    if (modify_success) {
        std::cout << "\n资产信息修改成功！修改后信息如下：" << std::endl;
        display_header();
        display_single_asset(current);
        pauseAndWaitEnter();
        return 1;
    }
    else {
        std::cout << "\n资产信息修改失败！" << std::endl;
        pauseAndWaitEnter();
        return 0;
    }
}

// 显示表头
void display_single_asset(const Asset* asset) {
    cout << left
        << "| " << setw(10) << asset->asset_id
        << "| " << setw(20) << asset->asset_name
        << "| " << setw(15) << asset->category
        << "| " << setw(10) << asset->custodian
        << "|" << endl;
}

// 显示所有资产
void display_all_assets() {
    cout << "\n--- 当前所有固定资产 ---\n";
    if (head == nullptr) {
        cout << "资产表为空。" << endl;
        return;
    }

    display_header();
    Asset* current = head;
    while (current != nullptr) {
        display_single_asset(current);
        current = current->next;
    }
    cout << "================================================================\n";
}

// 按ID查询资产
void search_by_id() {
    string id_to_find;
    cout << "\n--- 以资产代号查资产明细 ---\n";
    cout << "请输入资产代号: ";
    cin >> id_to_find;
    clearInputBuffer();

    Asset* current = head;
    bool found = false;

    while (current != nullptr) {
        if (current->asset_id == id_to_find) {
            display_header();
            display_single_asset(current);
            found = true;
            break;
        }
        current = current->next;
    }
	

    if (!found) {
        cout << "未找到资产代号为 [" << id_to_find << "] 的资产。" << endl;
    }
    else {
        cout << "================================================================\n";
    }
}

// 按名称查询资产
void search_by_name() {
    string name_to_find;
    cout << "\n--- 以资产名称查资产明细 ---\n";
    cout << "请输入资产名称: ";
    clearInputBuffer();
    getline(cin, name_to_find);

    Asset* current = head;
    bool found = false;
    display_header();

    while (current != nullptr) {
        if (current->asset_name.find(name_to_find) != string::npos) {
            display_single_asset(current);
            found = true;
        }
        current = current->next;
    }

    if (!found) {
        cout << "未找到包含名称 [" << name_to_find << "] 的资产。" << endl;
    }
    else {
        cout << "================================================================\n";
    }
}

// 按类别查询资产
void search_by_category() {
    cout << "\n--- 以资产类别查资产明细 ---" << endl;
    // 显示数字对应的类别选项
    cout << "请选择资产类别（输入对应数字）：" << endl;
    cout << "1 - 教学类" << endl;
    cout << "2 - 办公类" << endl;
    cout << "3 - 生活类" << endl;
    cout << "4 - 其他类" << endl;
    cout << "请输入数字（1-4）：";

    // 处理数字输入并校验
    int choice = 0;
    bool valid_choice = false;
    string target_category; // 最终要查询的类别名称

    while (!valid_choice) {
        // 读取数字输入
        if (cin >> choice) {
            clearInputBuffer(); // 清空换行符
            // 校验数字范围
            switch (choice) {
            case 1:
                target_category = "教学类";
                valid_choice = true;
                break;
            case 2:
                target_category = "办公类";
                valid_choice = true;
                break;
            case 3:
                target_category = "生活类";
                valid_choice = true;
                break;
            case 4:
                target_category = "其他类";
                valid_choice = true;
                break;
            default:
                cout << "错误：请输入 1-4 之间的数字！重新输入：";
                break;
            }
        }
        else {
            // 处理非数字输入（如字母、符号）
            clearInputBuffer();
            cout << "错误：请输入有效的数字！重新输入：";
        }
    }

    // 遍历链表查找匹配类别的资产
    Asset* current = head;
    bool found = false;
    display_header();

    while (current != nullptr) {
        // 类别精确匹配
        if (current->category == target_category) {
            display_single_asset(current);
            found = true;
        }
        current = current->next;
    }

    // 未找到匹配资产的提示
    if (!found) {
        cout << "未找到类别为 [" << target_category << "] 的资产。" << endl;
    } 
    else {
        cout << "================================================================\n";
    }
}

// 按保管人查询资产
void search_by_custodian() {
    string custodian_to_find;
    cout << "\n--- 以资产保管人查资产明细 ---\n";
    cout << "请输入保管人姓名: ";
    clearInputBuffer();
    getline(cin, custodian_to_find);

    Asset* current = head;
    bool found = false;
    display_header();

    while (current != nullptr) {
        if (current->custodian.find(custodian_to_find) != string::npos) {
            display_single_asset(current);
            found = true;
        }
        current = current->next;
    }


    if (!found) {
        cout << "未找到保管人为 [" << custodian_to_find << "] 的资产。" << endl;
    }
    else {
        cout << "================================================================\n";
    }
}

// 显示匹配的资产列表，返回匹配数量
int displayMatchedAssets(const vector<pair<Asset*, Asset*>>& assets) {
    if (assets.empty()) {
        return 0;
    }
    cout << "\n===== 找到以下匹配的资产 =====" << endl;
    for (int i = 0; i < assets.size(); ++i) {
        Asset* asset = assets[i].second;
        cout << "序号 [" << i << "] - 名称: " << asset->asset_name
            << " | ID: " << asset->asset_id
            << " | 类别: " << asset->category
            << " | 保管人: " << asset->custodian << endl;
    }
    cout << "==============================\n" << endl;
    return (int)assets.size();
}

// 根据名称模糊筛选资产
vector<pair<Asset*, Asset*>> findAssetsByName(const string& name) {
    vector<pair<Asset*, Asset*>> matched_assets;
    Asset* prev = nullptr;
    Asset* curr = head;

    while (curr != nullptr) {
        if (curr->asset_name.find(name) != string::npos) {
            matched_assets.emplace_back(prev, curr);
        }
        prev = curr;
        curr = curr->next;
    }
    return matched_assets;
}

// 根据保管人模糊筛选资产
vector<pair<Asset*, Asset*>> findAssetsByCustodian(const string& custodian) {
    vector<pair<Asset*, Asset*>> matched_assets;
    Asset* prev = nullptr;
    Asset* curr = head;

    while (curr != nullptr) {
        if (curr->custodian.find(custodian) != string::npos) {
            matched_assets.emplace_back(prev, curr);
        }
        prev = curr;
        curr = curr->next;
    }
    return matched_assets;
}

// 按类别筛选资产
vector<pair<Asset*, Asset*>> findAssetsByCategory(const string& category) {
    vector<pair<Asset*, Asset*>> matched_assets;
    Asset* prev = nullptr;
    Asset* curr = head;
    while (curr != nullptr) {
        // 类别为精确匹配（教学类/办公类/生活类/其他类）
        if (curr->category == category)
            matched_assets.emplace_back(prev, curr);
        prev = curr;
        curr = curr->next;
    }
    return matched_assets;
}