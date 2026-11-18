
# 1.idea 创建 SpringBoot 项目时，官方脚手架与阿里云脚手架的区别？

⚙️ 编译参数 `-parameters` 的默认配置

`@RequestParam` 在简单类型上可以省略，但这依赖于编译时开启 `-parameters` 参数。

- **官方脚手架**：其 `spring-boot-starter-parent` 从 **Spring Boot 3.2** 开始已默认开启 `-parameters`，因此在 Maven 编译下可以省略。
- **阿里云脚手架**：其生成的 `pom.xml` 结构不同（通常不继承官方 parent），但**只要它使用的 Spring Boot 版本 ≥ 3.2，其依赖的编译插件配置也会遵循这一默认行为**。如果你选择了较旧的 Spring Boot 2.x 版本，则可能需要手动确保编译参数已开启。

# 2.常见快捷键
## 代码编辑

- `Ctrl + Alt + T`：用代码块包围选中代码（if、for、try-catch、synchronized、Runnable 等）
- `Ctrl + D`：复制当前行 / 复制选中的代码
- `Ctrl + Y`：删除当前行
- `Ctrl + W`：逐级扩大选中范围，连续按持续扩大
- `Shift + Enter`：光标下方新建一行
- `Ctrl + Enter`：拆分当前行
- `Ctrl + Alt + Enter`：光标上方新建一行
- `Ctrl + Shift + ↑ / ↓`：整块移动当前行 / 选中代码上下移动
- `Ctrl + [`：跳到当前代码块起始位置
- `Ctrl + ]`：跳到当前代码块结束位置

## 搜索与跳转

- `Ctrl + N`：查找类
- `Ctrl + Shift + N`：查找文件
- `双击 Shift`：全局搜索（类、文件、配置、符号）
- `Ctrl + F`：当前文件查找
- `Ctrl + R`：当前文件替换
- `Ctrl + B`：跳转到声明
- `Ctrl + Alt + B`：跳转到实现类 / 实现方法
- `Ctrl + U`：跳转到父类 / 父方法
- `Ctrl + G`：跳转到指定行号
- `Ctrl + E`：打开最近访问文件列表
- `Alt + ← / →`：光标浏览记录前进、后退
- `Alt + ↑ / ↓`：在方法之间快速跳转

## 重构、生成、修复

- `Alt + Enter`：快速修复、导入包、补全代码
- `Alt + Insert`：生成构造器、Getter/Setter、toString、equals、hashCode 等
- `Shift + F6`：批量重命名（变量、方法、类，安全重构）
- `Ctrl + O`：重写父类 / 接口方法

## 注释 & 代码折叠

- `Ctrl + /`：单行注释 //
- `Ctrl + Shift + /`：块注释 /* */
- `Ctrl + + / -`：展开 / 折叠当前代码块
- `Ctrl + Shift + + / -`：全部展开 / 全部折叠

## Live Template（代码模板）

- `sout`：`System.out.println()`
- `psvm`：public static void main(String[] args)
- `fori`：普通 for 循环
- `iter`：增强 for 循环 for-each
- `itar`：数组遍历 for 循环
- `.var`：根据表达式自动生成变量
- `main`：main 方法
- `.if`：生成 if 语句
- `.else`：生成 else
- `.while`：生成 while 循环

## Debug 调试

- `F7`：单步进入（进入方法内部）
- `F8`：单步跳过（执行下一行，不进方法）
- `Shift + F8`：单步跳出（从当前方法跳出）
- `F9`：恢复程序运行，直到下一个断点
- `F2 / Shift + F2`：快速跳到下一处 / 上一处错误警告

## 文档查看

- `Ctrl + Q`：查看快速文档
- `Ctrl + P`：查看方法参数提示