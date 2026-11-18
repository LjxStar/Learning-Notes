
# 1.idea 创建 SpringBoot 项目时，官方脚手架与阿里云脚手架的区别？

⚙️ 编译参数 `-parameters` 的默认配置

`@RequestParam` 在简单类型上可以省略，但这依赖于编译时开启 `-parameters` 参数。

- **官方脚手架**：其 `spring-boot-starter-parent` 从 **Spring Boot 3.2** 开始已默认开启 `-parameters`，因此在 Maven 编译下可以省略。
- **阿里云脚手架**：其生成的 `pom.xml` 结构不同（通常不继承官方 parent），但**只要它使用的 Spring Boot 版本 ≥ 3.2，其依赖的编译插件配置也会遵循这一默认行为**。如果你选择了较旧的 Spring Boot 2.x 版本，则可能需要手动确保编译参数已开启。

# 2.常见快捷键
① Ctrl + Alt + T 包围方式 例如条件、循环、异常、同步、Runnable
② Ctrl + P 查询方法参数
③ Crtil + D 复制当前行
 - Crtl + Y 删除当前行
** `Ctrl + N` 查找类**
Crtil + Shift + N 查找文件
 ** `Alt + Enter` ** 快速修复
 双击 Shift 全局搜索
 - **Ctrl + Z / Ctrl + Shift + Z**：撤销 / 重做
 - **Shift + Enter**：在下一行插入新行
 - crtl + enter 
 - Ctrl + Alt + Enter 在上一行插入新行
Ctrl + " `+ / -`" 将当前方法块代码展开 / 折叠
- Crtl + /
- Crtl Shift + / 块注释
- **Ctrl + F / Ctrl + R**：查找 / 替换
- **Ctrl + E**：最近打开的文件
- 
- **Ctrl + B**：跳转到声明
- **Ctrl + Alt + B**：跳转到实现
- **Ctrl + U**：跳转到父类
- **Ctrl + G**：跳转到指定行

- **Alt + ← / →**：后退 / 前进
- 在方法间快速移动定位（即光标以方法为单位移动） Alt + ⬆️/⬇️
- **Ctrl + Q**：快速文档
- **sout / psvm / fori / iter**：常用 Live Template 模板（自动生成打印语句、主函数、循环等）
- .if .else .while

- **F 8**：单步跳过
- **F 7**：单步进入
- **Shift + F 8**：跳出
- **F 9**：继续运行
crtl + w 选中文本，连续按可以扩大选中范围
Ctrl + O 快速重写基类或者接口中的方法
快速定位到代码块 `开始位置` Ctrl + [
快速定位到代码块 `结束位置` Ctrl + ]
快速生成含有任意形参的构造器和私有属性的 `Getter/Setter` 等方法 Alt + Insert
