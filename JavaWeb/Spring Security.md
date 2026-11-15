## 0. 简介

**Spring Security** 是 Spring 家族中的安全管理框架。相比于另一个安全框架 **Shiro**，它提供的功能更丰富，社区资源也更丰富。一般来说，中大型项目多使用 **Spring Security** 作为安全框架；小项目使用 Shiro 的比较多，因为相比之下 Shiro 上手更简单。

一般 Web 应用需要进行**认证**和**授权**：

- **认证**：验证当前访问系统的是不是本系统的用户，并且要确认具体是哪个用户；
- **授权**：经过认证后，判断当前用户是否有权限进行某个操作。

认证与授权，正是 Spring Security 作为安全框架的两大核心功能。

## 1. 快速入门

### 1.1 准备工作

先搭建一个简单的 Spring Boot 工程。

**① 设置父工程并添加依赖**

![[Pasted image 20261115204649.png]]
「新建 SpringBoot 项目所需依赖」
**② 创建 Controller**

```java
@RestController  
public class Hello {  
    @RequestMapping("/hello")  
    public String sayHello() {  
        return "Hello, World!";  
    }  
}
```

**③ 访问接口**

引入 `Spring Security` 依赖后再访问之前的接口，会自动跳转到默认登录页面：默认用户名是 `user`，密码会输出在控制台。**必须登录之后才能访问接口**。

![[Pasted image 20261115205330.png]]
「自动生成临时密码」
> **小结**：一行依赖就带来了整套过滤器链，所有请求默认被拦截。后续所有改造，本质上都是「替换链条中的某些环节」：把默认登录页换成自定义登录接口、把默认用户源换成数据库、把 Session 换成 JWT + Redis、把默认错误响应换成统一 JSON。

