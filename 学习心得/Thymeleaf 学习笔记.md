Thymeleaf 是 Java 生态中常用的模板引擎，用于在服务端把数据填充到 HTML 页面，也是 Spring Boot 官方推荐的视图技术之一。

## 一、快速入门

### 1.1 认识 Thymeleaf

Thymeleaf 崇尚**自然模板**：模板本身是纯正的 HTML，脱离模板引擎也能在浏览器中直接打开预览。

这依赖于 `th:*` 指令——表达式写在自定义属性里。静态环境下浏览器会直接忽略这些属性，因此不会报错；服务端渲染时再由 Thymeleaf 解析并替换页面内容。例如：

```html
<title th:text="${title}">默认的 title</title>
```

直接用浏览器打开时显示「默认的 title」；由 Thymeleaf 渲染后，`th:text` 的值会覆盖标签内的文字。

### 1.2 环境准备

Spring Boot 项目中引入 starter：

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-thymeleaf</artifactId>
</dependency>
```

目录约定：

- `resources/templates/`：存放模板页面（如 `index.html`）。
- `resources/static/`：存放 CSS、JS、图片等静态资源。

Thymeleaf 默认开启页面缓存，以提高并发能力，但会导致修改页面后不能立即生效。开发阶段建议关闭：

```yaml
spring:
  thymeleaf:
    cache: false
```

> 生产环境可以保持缓存开启，以获得更好的性能。

### 1.3 第一个页面

控制器准备数据并返回视图名：

```java
@Controller
public class IndexController {

    @GetMapping("/")
    public String index(Model model) {
        model.addAttribute("title", "自定义的 title");
        model.addAttribute("description", "自定义的 description");
        model.addAttribute("keywords", "自定义的 keywords");
        return "index";
    }
}
```

模板 `index.html`：

```html
<!DOCTYPE html>
<html lang="zh-CN" xmlns:th="http://www.thymeleaf.org">
<head>
    <meta charset="UTF-8">
    <title th:text="${title}">默认的 title</title>
    <meta name="description" th:content="${description}" content="默认的 description">
    <meta name="keywords" th:content="${keywords}" content="默认的 keywords">
</head>
<body>

</body>
</html>
```

注意标签里同时写了 `th:content` 和 `content`：**静态打开时显示 `content` 的默认值，服务端渲染后被 `th:content` 覆盖**，这正是自然模板的实现方式。

### 1.4 用到的 Spring MVC 知识

- `@Controller`：标识 Spring MVC 控制器，方法默认返回**视图名称**，由视图解析器渲染页面。
- `@RestController`：`@Controller` 与 `@ResponseBody` 的组合注解，返回值直接写入响应体，通常序列化为 JSON。

`Model` 用于在控制器和视图之间传递数据。Spring 主要提供三种方式：

- `Model`、`ModelMap`：通过 `addAttribute("key", value)` 存放数据。
- `ModelAndView`：同时封装数据和视图名，例如 `return new ModelAndView("index", "title", "标题")`。

## 二、基本语法：`th` 属性与表达式

### 2.1 常用 `th` 属性速览

| 属性                                         | 作用                                             |
| ------------------------------------------ | ---------------------------------------------- |
| `th:text` / `th:utext`                     | 设置标签内文本（转义 / 不转义）                              |
| `th:xxx`                                   | 替换同名 HTML 属性，如 `th:content`、`th:href`、`th:src` |
| `th:object` 与 `*{...}`                     | 选定对象并读取其属性                                     |
| `th:if` / `th:unless`                      | 条件判断                                           |
| `th:each`                                  | 遍历集合                                           |
| `th:switch` / `th:case`                    | 多分支选择                                          |
| `th:inline`                                | 在 JS / CSS 中使用表达式                              |
| `th:classappend`                           | 追加 CSS 类                                       |
| `th:fragment` / `th:insert` / `th:replace` | 片段复用                                           |

所有表达式都写在 `th:*` 属性中，这是 Thymeleaf 自然模板的基础。

### 2.2 变量输出：`th:text`、`th:utext` 与 `th:xxx`

- `th:text`：设置标签内的文本。
- `th:xxx`：直接替换对应的 HTML 属性。

`th:text` 出于安全考虑会对内容做转义，防止 HTML 注入。例如 `<p>你好</p>` 会输出为 `&lt;p&gt;你好&lt;/p&gt;`。如果想输出原始 HTML，改用 `th:utext`。

### 2.3 OGNL 表达式：取值、属性与方法

获取变量通常用 `对象.属性名`。如果属性名本身也是变量，可以用类似 JS 的语法糖：

```html
<span th:text="${user.name}"></span>
<!-- 等价于 -->
<span th:text="${user['name']}"></span>
```

OGNL 也支持方法调用，例如拆分姓名（假设 `user.name = "John Smith"`）：

```html
<p th:text="${user.name.split(' ')[0]}">John</p>
<p th:text="${user.name.split(' ')[1]}">Smith</p>
```

### 2.4 选择变量：`th:object` 与 `*{...}`

定义一个对象：

```java
@Data
@Builder
public class User {
    private String name;
    private int age;
    private String sex;
    private boolean isStudent;
    private List<String> hobbies;
    private LocalDate birthday;
}
```

对应的控制器：

```java
@Controller
public class ShowController {

    @GetMapping("/show")
    public String show(Model model) {
        User user = User.builder()
                .name("张三")
                .age(18)
                .sex("男")
                .isStudent(true)
                .hobbies(List.of("reading", "traveling"))
                .birthday(LocalDate.of(2005, 1, 5))
                .build();
        model.addAttribute("user", user);
        return "profile";
    }
}
```

页面中引用：

```html
<div>
    <h2 th:text="${user.name}">姓名</h2>
    <p th:text="${user.age}">年龄</p>
    <p th:text="${user.birthday}">生日</p>
</div>
```

重复写 `${user.` 比较繁琐，可以用 `th:object` 配合 `*{...}` 选择表达式简化：

```html
<div th:object="${user}">
    <h2 th:text="*{name}">姓名</h2>
    <p th:text="*{age}">年龄</p>
    <p th:text="*{birthday}">生日</p>
</div>
```

> [!note] `${...}` 与 `*{...}`
> `${...}` 从上下文（Model）取值，`*{...}` 从最近一层 `th:object` 选定的对象中取值。

### 2.5 字符串拼接与运算符

字符串拼接有两种写法：

```html
<p th:text="'前缀' + ${title}"></p>
<p th:text="|前缀${title}|"></p>
```

运算符：

> [!warning] 注意
> `${}` 内部由 OGNL 引擎解析，外部才由 Thymeleaf 解析，因此运算符尽量写在 `${}` 外面，例如 `${user.age} + 2`。

- **算术运算**：`+`、`-`、`*`、`/`、`%`
- **比较运算**：`gt`、`lt`、`ge`、`le`、`eq`、`neq` / `ne`、`not`。`==` 和 `!=` 类似 `equals`，可用于比较非数值类型。
- **逻辑运算**：`and`、`or`、`not`
- **三元运算**：`condition ? then : else`

```html
<span th:text="${user.sex} ? '男' : '女'"></span>
```

取值可能为空时，可以用 `?:` 指定默认值（注意 `?:` 之间没有空格）：

```html
<span th:text="${user.name} ?: '小红'"></span>
```

## 三、流程控制

### 3.1 条件判断：`th:if` / `th:unless`

两者含义相反：`th:if` 条件成立时显示，`th:unless` 条件不成立时显示。

```html
<div th:object="${user}">
    <h2 th:text="*{name}">姓名</h2>
    <p th:if="*{isStudent}">是学生</p>
    <p th:unless="*{isStudent}">不是学生</p>
</div>
```

### 3.2 循环：`th:each`

```html
<div th:object="${user}">
    <p th:each="hobby : *{hobbies}" th:text="${hobby}">爱好</p>
</div>
```

也可以加一个状态变量：`th:each="hobby, stat : *{hobbies}"`，`stat` 常用属性有：

| 属性 | 含义 |
| --- | --- |
| `index` | 从 0 开始的索引 |
| `count` | 从 1 开始的元素序号 |
| `size` | 元素总数 |
| `current` | 当前遍历到的元素 |
| `even` / `odd` | 是否为偶数 / 奇数位 |
| `first` / `last` | 是否为第一个 / 最后一个元素 |

### 3.3 分支：`th:switch`

`th:case="*"` 表示默认分支。

```html
<p th:switch="*{sex}">
    <span th:case="'男'">男</span>
    <span th:case="'女'">女</span>
    <span th:case="*">未知</span>
</p>
```

## 四、页面常用属性

### 4.1 引入静态资源

放在 `resources/static` 下的文件，可以用 `@{...}` 引入：

```html
<head>
    <link rel="stylesheet" th:href="@{app.css}">
</head>
<body>
    <img th:src="@{img/logo.png}" alt="logo">
</body>
```

### 4.2 `th:inline`：渲染动态 JS / CSS

```html
<script th:inline="javascript">
    const user = /*[[${user}]]*/ {};
    console.log(user);
</script>

<style th:inline="css">
    body { color: /*[[${color}]]*/ #333; }
</style>
```

`th:inline="javascript"` 开启 JS 内联模式，`/*[[${user}]]*/` 是 Thymeleaf 的标记：服务端渲染时 `${user}` 会被替换为对应对象的 JSON；如果直接在浏览器打开，这只是一个 JS 注释，不会报错。后面的 `{}` 是静态预览时的兜底值。CSS 内联的用法类似。

### 4.3 `th:classappend`：追加样式类

`th:classappend` 不会覆盖已有的 `class`，而是在后面追加：

```html
<p class="info" th:classappend="${user.isStudent} ? ' active'">学生信息</p>
```

## 五、片段复用

在 `templates` 下新建 `component.html`：

```html
<!DOCTYPE html>
<html lang="zh-CN" xmlns:th="http://www.thymeleaf.org">
<body>
<div th:fragment="com1">com1</div>
<div id="com2">com2</div>

<footer th:fragment="com3(message)">
    <p th:text="${message}">默认消息</p>
</footer>

<div th:fragment="com4(content)">
    <div th:replace="${content}"></div>
</div>
</body>
</html>
```

用 `th:replace` 或 `th:insert` 引入：

```html
<div th:replace="~{component :: com1}"></div>
<div th:insert="~{component :: com1}"></div>
```

区别是：`th:replace` 会用片段**替换**整个宿主标签；`th:insert` 则**保留**宿主标签，把片段插入到它内部。

按 `id` 引入：

```html
<div th:insert="~{component :: #com2}"></div>
```

传递参数：

```html
<div th:insert="~{component :: com3('传递的数据')}"></div>
```

局部替换（把当前模板中 `id="message"` 的元素传给片段）：

```html
<div th:insert="~{component :: com4(~{::#message})}">
    <p id="message">替换的模块</p>
</div>
```

给片段中的变量声明类型，方便 IDE 提示（该注释不会被渲染到页面）：

```html
<footer th:fragment="com1">
    <!--/*@thymesVar id="user" type="cn.lookroot.loop.thymeleafdemo.vo.UserVo"*/-->
    <h2 th:text="${user.name}">用户名</h2>
</footer>
```

## 六、内置对象

环境相关对象：

| 对象 | 说明 |
| --- | --- |
| `#ctx` | Thymeleaf 的 Context 对象 |
| `#request` | HttpServletRequest（Web 环境） |
| `#response` | HttpServletResponse（Web 环境） |
| `#session` | HttpSession（Web 环境） |
| `#servletContext` | ServletContext（Web 环境） |

Thymeleaf 提供的全局工具对象：

| 对象 | 作用 |
| --- | --- |
| `#dates` | 处理 `java.util.Date` |
| `#calendars` | 处理 `java.util.Calendar` |
| `#numbers` | 数字格式化 |
| `#strings` | 字符串处理 |
| `#bools` | 布尔值判断 |
| `#arrays` | 数组处理 |
| `#lists` | List 集合处理 |
| `#sets` | Set 集合处理 |
| `#maps` | Map 集合处理 |

例如，控制器传入当前日期：

```java
@GetMapping("/date")
public String date(Model model) {
    model.addAttribute("today", new Date());
    return "date";
}
```

页面中格式化：

```html
<p>今天是：<span th:text="${#dates.format(today, 'yyyy-MM-dd')}">2020-03-30</span></p>
```

## 七、参考

> 版权声明：本文为 CSDN 博主「龙源 lll」的原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接及本声明。
> 原文链接：https://blog.csdn.net/Lzy410992/article/details/115371017
