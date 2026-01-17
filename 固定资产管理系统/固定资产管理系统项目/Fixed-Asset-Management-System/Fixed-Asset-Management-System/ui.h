#ifndef UI_H
#define UI_H

 /** 显示登录界面并处理登录流程 */
void showLoginScreen();

/** 初始化资产表界面 */
void showInitializeScreen();

/** 显示主菜单 */
void showMainMenu();

/** 显示添加资产界面（调用 add_asset 并显示结果） */
void showAssetAddScreen();

/** 显示删除资产界面（按 id / 按名称） */
void showAssetDeleteScreen();

/** 显示查询资产界面（按 id/name/custodian/全部） */
void showAssetQueryScreen();

/** 显示修改资产界面 */
void showAssetModifyScreen();

/** 显示用户管理界面（未实现） */
void showUserManageScreen();

/** 显示保存数据到文件的界面 */
void showSaveScreen();

/** 显示退出确认界面（是否保存？） */
void showExitScreen();

/** 打印资产列表的表头（格式化输出） */
void display_header();

#endif
