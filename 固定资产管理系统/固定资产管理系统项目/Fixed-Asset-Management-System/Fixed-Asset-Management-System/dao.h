#ifndef DAO_H
#define DAO_H

#include "entity.h"

// 从 CSV 文件加载资产链表
Asset* dao_load();

// 将资产链表保存到 CSV 文件
int dao_save(const Asset* head);

#endif // DAO_H
