# IntelliJ IDEA 常用快捷键

## 高频编辑

### 行操作与撤销

| 快捷键                | 功能                |
| ------------------ | ----------------- |
| `Ctrl + D`         | 复制当前行；若有选区则复制选中内容 |
| `Ctrl + Y`         | 删除当前行             |
| `Ctrl + Z`         | 撤销上一步操作           |
| `Ctrl + Shift + Z` | 重做已撤销的操作          |

### 插入与换行

| 快捷键                    | 功能                               |
| ---------------------- | -------------------------------- |
| `Shift + Enter`        | 在当前行下方插入新行，并将光标移至新行              |
| `Ctrl + Alt + Enter`   | 在当前行上方插入新行，并将光标移至新行              |
| `Ctrl + Enter`         | 在光标处拆分当前行（光标后的内容移至下一行）           |
| `Ctrl + Shift + Enter` | 补全当前语句（自动补充分号、括号等），并将光标移至下一行合适位置 |

### 移动与选择

| 快捷键                    | 功能                              |
| ---------------------- | ------------------------------- |
| `Alt + Shift + ↑ / ↓`  | 上移 / 下移当前行（Move Line）           |
| `Ctrl + Shift + ↑ / ↓` | 上移 / 下移当前语句或代码块（Move Statement） |
| `Ctrl + W`             | 逐级扩大选区（连续按可持续扩大）                |
| `Ctrl + Shift + W`     | 逐级缩小选区                          |
| `Ctrl + [`             | 跳转到当前代码块起始位置                    |
| `Ctrl + ]`             | 跳转到当前代码块结束位置                    |

### 编辑辅助

| 快捷键                      | 功能                                                     |
| ------------------------ | ------------------------------------------------------ |
| `Ctrl + Alt + T`         | 使用代码块包围选中代码（如 `if`、`for`、`try-catch`、`synchronized` 等） |
| `Ctrl + Shift + U`       | 切换选中内容的大小写                                             |
| `Ctrl + Q`               | 查看快速文档（Quick Documentation）                            |
| `Ctrl + P`               | 查看方法参数提示（Parameter Info）                               |
| `Ctrl + Alt + Shift + T` | 弹出重构菜单                                                 |
| `Ctrl + Shift + V`       | 剪切板历史                                                  |

## 搜索与导航

| 快捷键                | 功能                                  |
| ------------------ | ----------------------------------- |
| `Double Shift`     | 随处搜索（Search Everywhere：类、文件、符号、配置等） |
| `Ctrl + N`         | 查找类（Class）                          |
| `Ctrl + Shift + N` | 查找文件（File）                          |
| `Ctrl + F`         | 在当前文件中查找                            |
| `Ctrl + R`         | 在当前文件中替换                            |
| `Ctrl + E`         | 打开最近访问文件列表（Recent Files）            |
| `Ctrl + G`         | 跳转到指定行号                             |
| `Ctrl + B`         | 跳转到声明或用法（Go to Declaration）         |
| `Ctrl + Alt + B`   | 跳转到实现类或实现方法（Go to Implementation）   |
| `Ctrl + U`         | 跳转到父类或父方法（Go to Super）              |
| `Alt + ← / →`      | 快速切换代码视图                            |
| `Alt + ↑ / ↓`      | 在方法之间快速跳转                           |
| `Ctrl + Shift + F` | 全局项目内查找                             |
| `Ctrl + Shift + R` | 全局项目内替换                             |

## 代码生成与重构

| 快捷键            | 功能                                                       |
| -------------- | -------------------------------------------------------- |
| `Alt + Enter`  | 显示意图操作与快速修复（如导入类、修正错误、生成代码等）                             |
| `Alt + Insert` | 生成代码（构造器、Getter/Setter、`toString`、`equals`、`hashCode` 等） |
| `Ctrl + O`     | 重写父类或接口方法（Override Methods）                              |
| `Shift + F6`   | 重命名符号（变量、方法、类、文件等，安全重构）                                  |

## 注释与代码折叠

| 快捷键                                     | 功能                 |
| --------------------------------------- | ------------------ |
| `Ctrl + /`                              | 切换行注释（`//`）        |
| `Ctrl + Shift + /`                      | 切换块注释（`/* ... */`） |
| `Ctrl + +` / `Ctrl + -`                 | 展开 / 折叠当前代码块       |
| `Ctrl + Shift + +` / `Ctrl + Shift + -` | 展开 / 折叠全部代码块       |

## Live Template 与后缀补全

### Live Template（输入缩写后按 `Tab`）

#### `sout`：输出到控制台

```java
System.out.println();
```

#### `psvm` / `main`：生成 `main` 方法

```java
public static void main(String[] args) {
    
}
```

#### `fori`：普通索引 `for` 循环

```java
for (int i = 0; i < ; i++) {
    
}
```

#### `iter`：增强 `for-each` 循环

```java
for (ElementType element : collection) {
    
}
```

#### `itar`：遍历数组的 `for` 循环

```java
for (int i = 0; i < array.length; i++) {
    ElementType element = array[i];
    
}
```

### 后缀补全（在表达式后输入，如 `expr.var` 再按 `Tab`）

#### `.var`：根据表达式生成变量声明

```java
// 示例：new ArrayList<String>().var
ArrayList<String> list = new ArrayList<String>();
```

#### `.if`：生成 `if` 判断

```java
// 示例：condition.if
if (condition) {
    
}
```

#### `.else`：生成取反条件的 `if` 判断

```java
// 示例：condition.else
if (!condition) {
    
}
```

#### `.while`：生成 `while` 循环

```java
// 示例：condition.while
while (condition) {
    
}
```

## 调试

| 快捷键          | 功能                              |
| ------------ | ------------------------------- |
| `F7`         | 单步进入（Step Into）：进入当前方法内部        |
| `F8`         | 单步跳过（Step Over）：执行当前行，不进入被调用方法  |
| `Shift + F8` | 单步跳出（Step Out）：跳出当前方法           |
| `F9`         | 恢复运行（Resume Program）：继续执行至下一个断点 |
| `F2`         | 跳转到下一处错误或警告                     |
| `Shift + F2` | 跳转到上一处错误或警告                     |

## 其他

| 快捷键                      | 功能                           |
| ------------------------ | ---------------------------- |
| `Alt + 左键拖动`             | 框选一竖列                        |
| `Alt + Shift + Insert`   | 切换列选择模式                      |
|                          | `Alt + ↑ / ↓` 纵向增加光标，批量编辑同一列 |
| `Alt + Shift + 左键点击`     | 在任意位置添加单个光标                  |
| `Alt + J`                | 选中下一个相同文本                    |
| `Ctrl + Alt + Shift + J` | 一次性选中文件里全部相同文本（批量改变量名）       |

