@Controller  
public class IndexController {  
  
    @GetMapping("/")  
    public String index(Model model) {  
        model.addAttribute("title", "自定义的title");  
        return "index";  
    }  
}
```
<!DOCTYPE html>  
<html lang="zh-CN" xmlns:th="http://www.thymeleaf.org">  
<head>  
    <meta charset="UTF-8">  
    <title th:text="${title}">默认的title</title>  
</head>  
<body>  
  
</body>  
</html>
```

 `@Controller` **：标识一个类为 Spring MVC 控制器，方法默认返回视图名称，由视图解析器渲染页面。

`@RestController`：是 `@Controller` 和 `@ResponseBody` 的组合注解，方法返回值会直接写入 HTTP 响应体，通常序列化为 JSON 或 XML 数据发送给客户端。

**Model** 是 MVC 模式中的核心组件之一，用于在控制器和视图之间传递数据。它主要用于存储和管理应用程序的数据，并将这些数据传递给视图层进行展示。
Spring 提供了三种主要的模型表示方式：**Model**、**ModelMap** 和 **ModelAndView**。
// TODO

