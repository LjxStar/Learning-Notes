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

** `@Controller` **：传统页面控制器。Spring 会交给视图解析器