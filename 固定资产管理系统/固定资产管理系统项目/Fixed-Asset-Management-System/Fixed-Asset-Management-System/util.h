#ifndef UTIL_H
#define UTIL_H
#include "entity.h"
#include <string>

/** 询问是否继续 */
bool askYesNo(const std::string& message);

// 暂停并等待用户按回车键
void pauseAndWaitEnter();

// 清空输入缓冲区
void clearInputBuffer(); 

// 计算UTF-8字符串的字符个数（混合中英文时，一个汉字算1个，一个字母算1个）(已停用)
//size_t getUtf8Length(const std::string& str);

// 验证名称：1-10个字符
bool isValidName(const std::string& name);

// 验证保管人：1-10个字符
bool isValidCustodian(const std::string& name);

// 验证类别：必须是指定几类
bool isValidCategory(const std::string& category);

// 自动生成ID：遍历链表寻找最大ID + 1
std::string generateNextId(const Asset* head);

// 释放所有资产链表的内存
void free_all_assets();

#endif