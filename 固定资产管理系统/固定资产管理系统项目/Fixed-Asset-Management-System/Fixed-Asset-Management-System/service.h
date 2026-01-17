#ifndef SERVICE_H
#define SERVICE_H

#include "entity.h"
#include <vector>
#include <string>


// 添加新资产到链表，完成ID自动分配/手动指定、各类信息校验
int add_asset();

// 按资产ID删除（ID唯一，直接删除匹配项）
int delete_by_id();

// 按资产名称删除（支持模糊匹配，多结果时按序号选择删除，可循环删除）
int delete_by_name();

// 按类别删除资产（数字选择类别，多匹配项按序号删除）
int delete_by_category();

// 按保管人删除（支持模糊匹配，多结果时按序号选择删除，可循环删除）
int delete_by_custodian();

// 修改指定资产的信息（需先定位资产，再更新字段）
int modify_asset();

// 按资产ID精确查询并展示资产明细
void search_by_id();

// 按资产名称模糊查询并展示所有匹配资产明细
void search_by_name();

// 按资产类别精确查询并展示所有匹配资产明细
void search_by_category();

// 按保管人模糊查询并展示所有匹配资产明细
void search_by_custodian();

// 展示单条资产的详细信息
void display_single_asset(const Asset* asset);

// 展示链表中所有资产的信息
void display_all_assets();

// ========== 辅助函数（删除功能专用） ==========
// 展示匹配的资产列表（带序号），返回列表长度；空列表返回0
int displayMatchedAssets(const std::vector<std::pair<Asset*, Asset*>>& assets);

// 根据资产名称模糊筛选资产，返回<前驱节点, 目标节点>的配对列表
std::vector<std::pair<Asset*, Asset*>> findAssetsByName(const std::string& name);

// 根据保管人模糊筛选资产，返回<前驱节点, 目标节点>的配对列表
std::vector<std::pair<Asset*, Asset*>> findAssetsByCustodian(const std::string& custodian);

// 按类别筛选资产，返回<前驱节点, 目标节点>配对列表
std::vector<std::pair<Asset*, Asset*>> findAssetsByCategory(const std::string& category);

#endif
