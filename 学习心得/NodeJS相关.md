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
```

3. D 盘新建文件夹：

可将
```
D:\npm
├─ global   ← 全局包
└─ cache    ← npm缓存
```

4. 修改 npm 配置，指向 D 盘路径
```bash
# 设置全局包目录
npm config set prefix "D:\nodejs\node_global"
# 设置缓存目录
npm config set cache "D:\nodejs\node_cache"
```

5. 校验是否设置成功
```bash
npm config get prefix
npm config get cache
```

6. 修改系统环境变量
	删除旧 C 盘条目：`C:\Users\userName\AppData\Roaming\npm`
	新增条目：`D:\node\node_global`

7. 重装全局包到 D 盘
```bash
npm install -g @anthropic-ai/claude-code
```

