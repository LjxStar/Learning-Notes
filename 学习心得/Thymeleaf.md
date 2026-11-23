# 一、基础语法

@Controller  
public class IndexController {  
  
```java
@Controller  
public class IndexController {  
  
    @GetMapping("/")  
    public String index(Model model) {  
        model.addAttribute("title", "自定义的title");  
        model.addAttribute("description", "自定义的description");  
        model.addAttribute("keywords", "自定义的keywords");  
        return "index";  
    }  
}
```
```html
<!DOCTYPE html>  
<html lang="zh-CN" xmlns:th="http://www.thymeleaf.org">  
<head>  
    <meta charset="UTF-8">  
    <title th:text="${title}">默认的title</title>  
    <meta name="description" th:content="${description}" content="默认的description">  
    <meta name="keywords" th:content="${keywords}" content="默认的keywords">  
</head>  
<body>  
  
</body>  
</html>
```

 1. 
 `@Controller` **：标识一个类为 Spring MVC 控制器，方法默认返回视图名称，由视图解析器渲染页面。
`@RestController`：是 `@Controller` 和 `@ResponseBody` 的组合注解，方法返回值会直接写入 HTTP 响应体，通常序列化为 JSON 或 XML 数据发送给客户端。

2. **Model** 是 MVC 模式中的核心组件之一，用于在控制器和视图之间传递数据。它主要用于存储和管理应用程序的数据，并将这些数据传递给视图层进行展示。
Spring 提供了三种主要的模型表示方式：**Model**、**ModelMap** 和 **ModelAndView**。
// TODO

3. th:text 设置标签内文本
`th:xxx`：直接替换 html 属性，如 `th:content`、`th:href`、`th:src`

4. 拼接字符串
`th:text="'prefix' + ${title}"`  或者   `th:text="|prefix${title}|"`

对于一个对象
```
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


和一个与之关联的 controller
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
在 html 引用


```html
<div>  
<h2 th:text="${user.name}"></h2>  
    <p th:text="${user.age}"></p> </p>     
    <p th:text="${user.birthday}"></p>
    </div>
```
可简化为如下：

```
<div th:object="${user}">  
    <h2 th:text="*{name}"></h2>  
    <p th:text="*{age}"></p>  
    <p th:text="*{birthday}"></p>  
</div>
```

th：if

```
<div th:object="${user}">  
    <h2 th:text="*{name}"></h2>  
    <p th:text="*{age}"></p>  
    <p th:text="*{birthday}"></p>  
    <p th:if="*{isStudent}">是学生</p>  
</div>
```


th：each 循环
```
<div th:object="${user}">  
    <h2 th:text="*{name}"></h2>  
    <p th:text="*{age}"></p>  
    <p th:text="*{isStudent}"></p>  
    <p th:text="*{birthday}"></p>  
    <p th:if="*{isStudent}">是学生</p>  
    <p th:each="hobby : *{hobbies}" th:text="${hobby}"></p>  
</div>
```
关于 th:each="hobby，stat : *{hobbies}" 里 stat 常见用法

th：switch
```html
    <p th:text="*{age}"></p>  
    <p th:text="*{isStudent}"></p>  
    <p th:text="*{birthday}"></p>  
    <p th:if="*{isStudent}">是学生</p>  
    <p th:each="hobby : *{hobbies}" th:text="${hobby}"></p>  
    <p th:switch="*{sex}">  
        <span th:case="'男'">男</span>  
        <span th:case="'女'">女</span>  
        <span th:case="*">未知</span>  
    </p>  
</div>
```

还有 th：src
如果你在 resources /static 下新建了 app.css
你可以通过如下方式引入：
```html
<head>
<link rel="stylesheet" th:href="@{app.css}">
</head>

```

th:inline

渲染动态 js
```
<body>
<script th:inline="javascript">const user =/*[[${user}]]*/{};console.log(user)</script>
</body>

```

`th:inline="javascript"`：开启 Thymeleaf 的 JS 内联模式，让脚本里面可以解析 Thymeleaf 表达式。
`/*[[${user}]]*/`：Thymeleaf 标记。服务端渲染时，`${user}` 会被替换成后端 model 里 `user` 对象的 JSON；**如果模板直接在浏览器打开、不经过 Thymeleaf 渲染，这段就只是 JS 注释，不会报错**。
后面的 `{}` 是兜底：渲染失败时，`user` 默认为空对象。

或者渲染动态 css


th：classapend
解释并示例

# 二、组件