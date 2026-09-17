# 1.把 `npm install -g` 全局安装的包，从默认 C 盘用户目录，迁移到 D 盘自定义目录。
> 默认全局路径：`C:\Users\81518\AppData\Roaming\npm`

1. 查看全局已安装包：
```bash
npm list -g --depth=0
```

2. 卸载 C 盘全部全局包：
```bash
# 删除单个包示例
npm uninstall -g @anthropic-ai/claude-code
npm uninstall -g @musistudio/claude-code-router
```

3. D 盘新建文件夹：

在 `D:\nodeJS` 下面新建 2 个文件夹（基于安装位置）
- `node_global`：全局包存放目录（prefix）
- `node_cache`：npm 缓存目录

4. 修改 npm 配置，指向 D 盘路径
```bash
# 设置全局包目录
npm config set prefix "D:\nodeJS\node_global"
# 设置缓存目录
npm config set cache "D:\nodeJS\node_cache"
```

5. 校验是否设置成功
```bash
npm config get prefix
npm config get cache
```

6. 修改系统环境变量