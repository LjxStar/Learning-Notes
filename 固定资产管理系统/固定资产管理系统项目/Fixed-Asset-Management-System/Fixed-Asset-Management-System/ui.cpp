#include <iostream>
#include <string>
#include <iomanip>
#include "config.h"
#include "ui.h"
#include "dao.h"
#include "util.h"
#include "service.h"

using namespace std;
extern Asset* head;

// 登录界面
void showLoginScreen() {
    cout << "===== 登录界面 =====" << endl;
    string username, password;
    cout << "请输入用户名：";
    cin >> username;
    clearInputBuffer();
    cout << "请输入密码：";
    cin >> password;
    clearInputBuffer();

    if (username != DEFAULT_USERNAME || password != DEFAULT_PASSWORD) {  // 验证失败
        cout << "用户名或密码错误，请重新输入。" << endl;
        showLoginScreen();
        return;
    }
    else {
        cout << "哈喽 " << DEFAULT_USERNAME << "～登录成功，系统已为您准备就绪啦！" << endl;
        showInitializeScreen();
    }
}

void showInitializeScreen() {
	askYesNo("是否加载已有资产数据？") ? head = dao_load() : head = nullptr;
    cout << "资产表初始化完成，正在进入主菜单..." << endl;
	showMainMenu();
}

// 主菜单界面
void showMainMenu() {
    cout << "\n===== 主菜单 =====" << endl;
    cout << "1 - 查询资产" << endl;
    cout << "2 - 添加资产" << endl;
    cout << "3 - 修改信息" << endl;
    cout << "4 - 删除资产" << endl;
    cout << "5 - 用户管理" << endl;
    cout << "6 - 保存数据到文件" << endl;
    cout << "7 - 退出系统" << endl;
    cout << "请选择操作（1-7）：";
    int choice;
    cin >> choice;
    clearInputBuffer();

    switch (choice) {
    case 1:
        showAssetQueryScreen();
        break;
    case 2:
        showAssetAddScreen();
        break;
    case 3:
        showAssetModifyScreen();
        break;
    case 4:
        showAssetDeleteScreen();
        break;
    case 5:
        showUserManageScreen();
        break;
    case 6:
        showSaveScreen();
        break;
    case 7:
        showExitScreen();
        break;
    default:
        cout << "无效的选择，请重新输入。" << endl;
        showMainMenu();
        break;
    }
}

// 退出系统确认
void showExitScreen() {
    cout << "\n===== 是否保存数据？=====" << endl;
    cout << "1 - 保存" << endl;
    cout << "0 - 不保存" << endl;
    cout << "-1 - 取消" << endl;
    int saveChoice;
    cin >> saveChoice;
    clearInputBuffer();

    if (saveChoice == 1) {
        if (dao_save(head)) {
            cout << "数据已成功保存。" << endl;
        }
        else {
            cout << "保存失败，请检查文件权限或文件路径是否正确。" << endl;
        }
    }
    else if (saveChoice == 0) {
        cout << "选择不保存。" << endl;
    }
    else {
        cout << "已取消退出。" << endl;
        showMainMenu();
        return;
    }
    cout << "再见，" << DEFAULT_USERNAME << "。感谢使用本系统！" << endl;
}

// 资产添加界面
void showAssetAddScreen() {
    cout << "\n===== 添加资产 =====" << endl;

	// 调用添加资产逻辑
    add_asset();

    if (askYesNo("是否继续添加资产？")) {
        showAssetAddScreen();
        return;
    }
    else {
        showMainMenu();
    }
}

// 资产删除界面
void showAssetDeleteScreen() {
    cout << "\n===== 删除资产 =====" << endl;
    cout << "请选择删除方式(1-5)：" << endl;
    cout << "1 - 按ID删除" << endl;
    cout << "2 - 按名称删除" << endl;
    cout << "3 - 按类别删除" << endl;
    cout << "4 - 按保管人删除" << endl;
    cout << "5 - 返回主菜单" << endl;

    int deleteChoice;
    cin >> deleteChoice;
    clearInputBuffer();


    switch (deleteChoice) {
    case 1:
        delete_by_id();
        break;
    case 2:
        delete_by_name();
        break;
    case 3:
        delete_by_category();
		break;
	case 4:
		delete_by_custodian();
		break;
    case 5:
        showMainMenu();
        return;
    default:
        cout << "无效的选择，请重新输入。" << endl;
        showAssetDeleteScreen();
        return;
    }

    if (askYesNo("是否继续删除资产？")) {
        showAssetDeleteScreen();
        return;
    } else {
		showMainMenu();
    }
}

// 资产修改界面
void showAssetModifyScreen() {
    cout << "\n===== 修改资产 =====" << endl;
    
    modify_asset();

    if (askYesNo("是否继续修改资产信息？")) {
        showAssetModifyScreen();
        return;
    } else {
		showMainMenu();
	}
}

// 资产查询界面
void showAssetQueryScreen() {
    cout << "\n===== 查询资产 =====" << endl;
    cout << "请选择查询方式：" << endl;
    cout << "1 - 查询所有资产" << endl;
    cout << "2 - 按ID查询" << endl;
    cout << "3 - 按名称查询" << endl;
    cout << "4 - 按类别查询" << endl;
    cout << "5 - 按保管人查询" << endl;
    cout << "6 - 返回主菜单" << endl;

    int queryChoice;
    cin >> queryChoice;
    clearInputBuffer();

    switch (queryChoice) {
    case 1:
        display_all_assets();
        pauseAndWaitEnter();
        break;
    case 2:
        search_by_id();
        pauseAndWaitEnter();
        break;
    case 3:
        search_by_name();
        pauseAndWaitEnter();
        break;
	case 4:
        search_by_category();
        pauseAndWaitEnter();
		break;
    case 5:
        search_by_custodian();
        pauseAndWaitEnter();
        break;
    case 6:
        showMainMenu();
        return;
    default:
        cout << "无效的选择，请重新输入。" << endl;
        pauseAndWaitEnter();
        showAssetQueryScreen();
        return;
    }

    showMainMenu();
}

// 显示表头
void display_header() {
    cout << "\n================================================================\n";
    cout << left
        << "| " << setw(10) << "ID"
        << "| " << setw(20) << "名称"
        << "| " << setw(15) << "类型"
        << "| " << setw(10) << "保管人"
        << "|" << endl;
    cout << "================================================================\n";
}

// 用户管理界面
void showUserManageScreen() {
    cout << "\n===== 用户管理 =====" << endl;
    cout << "该功能尚未实现。" << endl;
    showMainMenu();
}

// 保存数据界面
void showSaveScreen() {
    cout << "\n===== 保存数据到文件 =====" << endl;
    int ok = dao_save(head);
    if (ok) {
        cout << "数据已成功保存到文件。" << endl;
    }
    else {
        cout << "保存失败，请检查文件权限或文件路径是否正确。" << endl;
    }
    showMainMenu();
}
