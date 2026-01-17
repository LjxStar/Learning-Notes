#include "ui.h"
#include "dao.h"
#include "util.h"

int main() {
    // 加载数据
    head = dao_load();

	// 显示登录界面
    showLoginScreen();

    // 退出前释放内存
    free_all_assets();
    return 0;
}
