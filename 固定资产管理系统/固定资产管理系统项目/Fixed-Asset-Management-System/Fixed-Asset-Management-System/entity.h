#ifndef ENTITY_H
#define ENTITY_H

#include <string>

// 资产实体结构体
struct Asset {
    std::string asset_id;    // 资产编号（唯一标识）
    std::string asset_name;  // 资产名称
    std::string category;    // 资产类别
    std::string custodian;   // 资产保管人
    Asset* next = nullptr;   // 指向下一个节点
};

// 资产表头指针
extern Asset* head;

#endif
