# 一、项目结构与开发约定

## 1.1 IOC 与 DI

### 1.1.1 分层解耦

软件开发中，一个功能通常要经过**数据校验 → 业务处理 → 数据存取**这几个环节，如果把它们全部写在一个方法里，代码会又长又难维护，也会因为「改一处动全身」而频繁出 bug。

**分层解耦**就是把每一类职责单独抽出来，交给专门的类去做：

- **Controller 层**：只负责接收请求、校验参数、返回响应，不写业务逻辑；
- **Service 层**：只负责业务处理，需要操作数据库时再往下调用；
- **Mapper 层**：只负责与数据库打交道，把 SQL 执行结果返回上去。

> [!IMPORTANT]
> 分层的意义不只是「代码好看」，更重要的是**各层可以独立替换、独立测试**。比如业务逻辑变了，SQL 一行都不用改；表结构变了，也不需要动 Controller。

有了分层还不够：Controller 里直接 `new DeptServiceImpl()`，Service 就和具体实现类**死死绑定**在一起，Service 内部同样会 `new deptMapper()`。要让上层「只依赖抽象、不依赖实现」，就需要容器把依赖对象主动送上来——这就是下面要讲的 IOC 与 DI。

### 1.1.2 IOC 与 DI

- **控制反转**：Inversion Of Control，简称 **IOC**。对象的创建控制权由程序自身转移到外部（容器），这种思想称为控制反转。

    - 对象的创建权由程序员主动创建转移到容器（由容器创建、管理对象）。这个容器称为：**IOC 容器**，也就是 Spring 容器。

- **依赖注入**：Dependency Injection，简称 **DI**。容器为应用程序提供运行时所依赖的资源，称之为依赖注入。

    - 程序运行时需要某个资源，此时容器就为其提供这个资源。
    - 例：`EmpController` 程序运行时需要 `EmpService` 对象，Spring 容器就为其提供并注入 `EmpService` 对象。

- **bean 对象**：IOC 容器中创建、管理的对象，称之为 **bean 对象**。

> [!TIP]
> 记忆：IOC 是**思想**（控制权反转了），DI 是**实现手段**（用「注入」的方式把依赖送进来），Spring 容器就是这套机制的具体落地。同理，bean 是容器里的对象，bean 之间的关系由依赖注入维护。

### 1.1.3 三层架构

SSM + Spring Boot 项目的代码按 Controller → Service → Mapper 三层组织，每一层由一个固定注解标记，注解位置错了 Spring 就无法完成依赖注入与对象管理。

| 注解                | 标注位置                      | 作用                                                   |
| ----------------- | ------------------------- | ---------------------------------------------------- |
| `@RestController` | Controller 层**类**上        | 等价于 `@Controller` + `@ResponseBody`，方法返回值自动序列化为 JSON |
| `@Service`        | Service 层**实现类**上，不要标在接口上 | 标记为业务层组件，参与事务管理                                      |
| `@Mapper`         | Mapper 层**接口**上           | 标记为 MyBatis Mapper 接口，自动生成代理实现类                      |

1. **Mapper 层**
```java
@Mapper
public interface DeptMapper {

    @Select("select * from dept")
    List<Dept> findAll();
}
```

2. **Service 层**
```java
@Service
public class DeptServiceImpl implements DeptService {

    @Autowired
    private DeptMapper deptMapper;   // 由 Spring 注入 Mapper 代理对象

    @Override
    public List<Dept> findAll() {
        return deptMapper.findAll();
    }
}
```

3. **Controller 层**
```java
@RestController
@RequestMapping("/depts")
public class DeptController {

    @Autowired
    private DeptService deptService; // 由 Spring 注入 Service 代理对象

    @GetMapping
    public Result list() {
        return Result.success(deptService.findAll());
    }
}
```

`@RestController`、`@Service`、`@Mapper` 这类注解统称为**构造型（stereotype）注解**，作用都一样：把类标记为组件，交给 Spring IoC 容器管理、实例化成 bean。它们的区别只是「标在哪一层」：

| 构造型注解 | 特点 |
| --- | --- |
| `@Component` | **通用构造型**，任何由 Spring 管理的组件都可以用它标注 |
| `@Service` | `@Component` 在业务层的特殊化，额外具备事务管理的语义 |
| `@Repository` | `@Component` 在数据访问层的特殊化，额外会把持久层异常统一转换为 Spring 的数据访问异常 |
| `@Controller` | `@Component` 在控制层的特殊化，`@RestController` = `@Controller` + `@ResponseBody` |
| `@Mapper` | **不是** Spring 自带的构造型注解，是 MyBatis 提供的，用于把 Mapper 接口交给 MyBatis 生成代理实现类 |

> [!TIP]
> 四个 Spring 自带的构造型注解**功能上几乎等价**，真正决定 bean 命名的是类名首字母小写（`DeptServiceImpl` → `deptServiceImpl`）。因此选哪个主要是为了表达语义：Service 层就写 `@Service`，别图省事全写 `@Component`。

## 1.2 统一响应结果 Result

后端所有接口统一返回 `Result` 对象，把「业务状态码 + 提示信息 + 数据」三部分封装在一起，前端只需判断 `code` 即可。

| 字段 | 类型 | 说明 |
| --- | --- | --- |
| `code` | `Integer` | 业务状态码，本项目约定 `1` = 成功、`0` = 失败 |
| `msg` | `String` | 提示信息，成功时为 `success`，失败时是给用户看的错误原因 |
| `data` | `Object` | 业务数据，可以是单个对象，也可以是 `List` |

```java
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class Result {

    private Integer code;   // 1 = 成功，0 = 失败
    private String msg;     // 错误提示信息
    private Object data;    // 业务数据

    /** 成功，不返回数据 */
    public static Result success() {
        return new Result(1, "success", null);
    }

    /** 成功，返回数据 */
    public static Result success(Object data) {
        return new Result(1, "success", data);
    }

    /** 失败 */
    public static Result error(String msg) {
        return new Result(0, msg, null);
    }
}
```

> [!TIP]
> - 三个静态工厂方法让 Controller 写起来很干净：`return Result.success(data);` 一行搞定，不用每次 `new`。
> - Lombok 的 `@Data` 会自动生成 `getCode()` / `setCode()` 等方法，Jackson 序列化 JSON 时正是靠 getter 取值。
> - 状态码可以进一步扩展为 `401` 未登录、`403` 无权限等，配合 [[#七、全局异常处理器|全局异常处理器]] 统一返回。

## 1.3 路径抽取

### 1.3.1 类级 + 方法级 @RequestMapping 组合

一个完整的请求路径 = **类上 `@RequestMapping` 的 value** + **方法上 `@RequestMapping` 的 value**。把公共前缀（资源名）统一放在类上，方法上只写子路径，既避免重复，也便于维护。

```java
@RestController
@RequestMapping("/depts")   // 公共前缀，只写一次
public class DeptController {

    // 完整路径：GET /depts
    @GetMapping
    public Result list() {
        return Result.success();
    }

    // 完整路径：GET /depts/{id}
    @GetMapping("/{id}")
    public Result getById(@PathVariable Integer id) {
        return Result.success();
    }

    // 完整路径：POST /depts
    @PostMapping
    public Result save(@RequestBody Dept dept) {
        return Result.success();
    }
}
```

> [!NOTE]
> 类上有 `@RequestMapping("/depts")` 时，方法上的路径**只写子路径**：写 `@GetMapping`（相当于 `""`）、`@GetMapping("/{id}")`。如果方法上再写一遍 `@GetMapping("/depts")`，完整路径就变成了 `/depts/depts`，前端怎么调都 404。

### 1.3.2 @RequestMapping 及其派生注解

`@RequestMapping` 用于声明请求路径（`value`）与请求方式（`method`）。实际开发中常用它的四个派生注解，可以省略 `method` 属性，写法更简洁：

| 派生注解             | 等价写法                                                             | 请求方式   |
| ---------------- | ---------------------------------------------------------------- | ------ |
| `@GetMapping`    | `@RequestMapping(value = "/xxx", method = RequestMethod.GET)`    | GET    |
| `@PostMapping`   | `@RequestMapping(value = "/xxx", method = RequestMethod.POST)`   | POST   |
| `@PutMapping`    | `@RequestMapping(value = "/xxx", method = RequestMethod.PUT)`    | PUT    |
| `@DeleteMapping` | `@RequestMapping(value = "/xxx", method = RequestMethod.DELETE)` | DELETE |

---

# 二、参数传递

Spring MVC 会把 HTTP 请求中的各种数据（查询参数、路径变量、请求体、请求头、Cookie）**自动绑定**到 Controller 方法的形参上。理解这一章的关键只有一句话：**参数从哪个「位置」来，就用哪个「注解」去接**。

| 参数所在位置           | 常用注解                | 示例请求                                              |
| ---------------- | ------------------- | ------------------------------------------------- |
| 查询参数（URL 后面 `?`） | 无 / `@RequestParam` | `GET /depts?id=1`                                 |
| 路径变量（URL 中 `{}`） | `@PathVariable`     | `GET /depts/1`                                    |
| JSON 请求体         | `@RequestBody`      | `POST /depts`，`Content-Type: application/json`    |
| 普通表单请求体          | 无（用 setter 绑定）      | `POST /login`，`application/x-www-form-urlencoded` |
| 文件表单             | `MultipartFile`     | `POST /upload`，`multipart/form-data`              |
| 请求头              | `@RequestHeader`    | `GET /emps`，`token: abc`                          |
| Cookie           | `@CookieValue`      | `GET /emps`，`Cookie: token=abc`                   |

## 2.1 简单参数

### 2.1.1 单个参数或多个不同名参数

以删除部门为例，前端请求 `DELETE /depts?id=1`，有三种接收方式。

#### （1）HttpServletRequest 原生对象

直接注入 Servlet 原始对象，从请求参数表里按名取值。类型转换需要自己完成，一般不推荐。

```java
@DeleteMapping
public Result delete(HttpServletRequest request) {
    // 从请求参数表中取出字符串，再手动转成 Integer
    Integer id = Integer.parseInt(request.getParameter("id"));
    deptService.deleteById(id);
    return Result.success();
}
```

#### （2）@RequestParam 注解

`@RequestParam` 显式指定请求参数的名称。当形参名与请求参数名不一致时（或未开启 `-parameters` 编译参数）**必须**使用。

```java
@DeleteMapping
public Result delete(@RequestParam("id") Integer id) {
    deptService.deleteById(id);
    return Result.success();
}
```

当形参与请求参数名一致时，`@RequestParam("id")` 中的 `"id"` 也可以省略，写成 `@RequestParam Integer id`。

#### （3）同名参数自动绑定

当**形参名与请求参数名完全一致**时，可以省略 `@RequestParam`，Spring MVC 会自动完成类型转换与绑定。

```java
@DeleteMapping
public Result delete(Integer id) {
    deptService.deleteById(id);
    return Result.success();
}
```

> [!WARNING]
> 方式（3）依赖 **形参名**。IDEA 编译时若没有勾选 `Add parameters`（等价于 javac 的 `-parameters`），class 文件里就丢掉了形参名，此时 Spring MVC 拿不到 `id`，会直接报 `Name for argument of type [java.lang.Integer] not specified`。
>
> 三个示例里类上已有 `@RequestMapping("/depts")`，所以方法上的 `@DeleteMapping` 后面什么都不写，完整路径就是 `DELETE /depts`。

### 2.1.2 多个同名参数

前端一次传多个同名的参数时，例如 `DELETE /depts?ids=1&ids=2&ids=3` 或者 `DELETE /depts?ids=1,2,3`，可以用**数组**或**集合**接收。

#### （1）通过数组来接收

```java
@DeleteMapping
public Result delete(Integer[] ids) {
    deptService.deleteById(ids);
    return Result.success();
}
```

#### （2）通过集合来接收

```java
@DeleteMapping
public Result delete(@RequestParam List<Integer> ids) {
    deptService.deleteById(ids);
    return Result.success();
}
```

> [!TIP]
> - 两种写法 Spring MVC 都能自动转换，但**推荐用集合**：`List` 语义更清晰，后面可以直接 `.stream().forEach(...)`，不必再写循环。
> - 集合接收**必须加 `@RequestParam`**，否则 Spring MVC 会认为你要接收的是一个复杂对象，直接尝试去创建 `List` 实例并报转换异常。

### 2.1.3 用实体类封装多个查询条件

分页条件查询的请求参数很多（页码、姓名、性别、入职时间区间……），直接堆在方法形参上既冗长又难维护。正确做法是**定义一个实体类统一封装**，并保证**前端传递的请求参数名与实体类属性名完全一致**，Spring MVC 会自动完成绑定。

```java
package com.itheima.pojo;

import lombok.Data;
import org.springframework.format.annotation.DateTimeFormat;

import java.time.LocalDate;

@Data
public class EmpQueryParam {

    private Integer page = 1;        // 页码，给默认值，防止前端不传
    private Integer pageSize = 10;   // 每页展示记录数
    private String name;             // 姓名
    private Integer gender;          // 性别
    private Short job;               // 职位
    @DateTimeFormat(pattern = "yyyy-MM-dd")
    private LocalDate begin;         // 入职开始时间
    @DateTimeFormat(pattern = "yyyy-MM-dd")
    private LocalDate end;           // 入职结束时间
}
```

```java
@GetMapping
public Result page(EmpQueryParam empQueryParam) {
    // 直接把整个对象当形参接收，参数自动按属性名绑定
    log.info("查询请求参数：{}", empQueryParam);
    PageResult<Emp> pageResult = empService.page(empQueryParam);
    return Result.success(pageResult);
}
```

> [!TIP]
> - 属性名必须和查询参数名**完全一致**（`pageSize` 对应 `?pageSize=10`），Spring MVC 底层是靠 setter 逐个赋值的，名字对不上就绑定不上。
> - `LocalDate` / `LocalDateTime` 是 Java 8 的日期类型，Spring MVC **不会**自动解析前端传来的字符串，必须加 `@DateTimeFormat` 指定格式；`LocalDateTime` 则写 `@DateTimeFormat(pattern = "yyyy-MM-dd HH:mm:ss")`。

---

## 2.2 路径参数

### 2.2.1 @PathVariable 接收路径变量

把参数直接写进 URL 路径，用 `/{id}` 占位，再通过 `@PathVariable` 取出。适合资源定位的场景。

```java
// 完整路径：GET /depts/1
@GetMapping("/{id}")                  // 路径中用 {id} 占位
public Result getById(@PathVariable Integer id) {
    Dept dept = deptService.getById(id);
    return Result.success(dept);
}
```

> [!TIP]
> 当形参名与路径变量名不一致时，必须写明名称，例如 `@PathVariable("id") Integer deptId`。
> 路径参数是 URL 的一部分，不适合放敏感信息，也不要放太多—— `/depts/1/2/3` 这样的路径可读性差，用查询参数更合适。

## 2.3 JSON 请求体

### 2.3.1 @RequestBody 接收 JSON

当请求体是 JSON 格式（`Content-Type: application/json`）时，用 `@RequestBody` 把 JSON 反序列化成 Java 对象。

```java
@PostMapping
public Result save(@RequestBody Dept dept) {
    deptService.save(dept);
    return Result.success();
}
```

## 2.4 请求头与 Cookie 参数

查询参数、路径变量、JSON 都拿到了，接下来两类参数藏在请求的「信封」里：请求头和 Cookie。用 `@RequestHeader`、`@CookieValue` 接收。

### 2.4.1 @RequestHeader 接收请求头

`@RequestHeader` 用于获取 HTTP 请求头中的数据，例如 token、Cookie、User-Agent 等。

#### （1）指定请求头名称

显式指定要读取的请求头 key，**请求头名称不区分大小写**。

```java
@GetMapping
public Result list(@RequestHeader("token") String token) {
    log.info("请求头中的 token：{}", token);
    return Result.success();
}
```

> 说明：前端请求头不管是 `Token` / `TOKEN` / `token`，都可以正常拿到值。

#### （2）省略请求头名称

如果方法形参名和请求头的 key 完全相同，可以省略括号内的名字。

```java
@GetMapping
public Result list(@RequestHeader String token) {
    return Result.success();
}
```

> 实际开发**不推荐省略写法**，依赖编译参数容易出现线上故障，建议都写清楚 `@RequestHeader("token")`。

#### （3）使用原生 HttpServletRequest 获取

不使用注解，通过 `HttpServletRequest` 对象手动读取请求头，效果和 `@RequestHeader` 完全一致。

```java
@GetMapping
public Result list(HttpServletRequest request) {
    String token = request.getHeader("token");
    return Result.success();
}
```

### 2.4.2 @CookieValue 接收 Cookie

```java
@GetMapping
public Result list(@CookieValue("token") String token) {
    log.info("cookie 中的 token：{}", token);
    return Result.success();
}
```

> [!WARNING]
> Cookie **可能不存在**（用户没登录、没勾选「记住我」），此时直接取会抛异常。登录状态一律加上默认值并判空：

```java
@GetMapping
public Result list(@CookieValue(value = "token", required = false) String token) {
    if (token == null) {
        return Result.error("未登录");
    }
    return Result.success();
}
```

> [!TIP]
> `@RequestHeader` 与 `@CookieValue` 的属性和 `@RequestParam` 一样：`required`（是否必填）、`defaultValue`（默认值）。**只要写了 `defaultValue`，`required` 就自动变成 `false`**。

## 2.5 普通表单提交

`Content-Type: application/x-www-form-urlencoded` 是浏览器表单的默认提交方式，只能传**文本键值对**。因为它本质还是「一堆 name-value」，所以**不需要任何注解**，Spring MVC 会把参数按名字塞进形参或实体类：

```java
@PostMapping("/login")
public Result login(String username, String password) {
    log.info("用户名：{}，密码：{}", username, password);
    return Result.success();
}
```

```java
// 也可以直接封装成实体类，属性名与表单的 name 对应即可
@PostMapping("/login")
public Result login(Emp emp) {
    log.info("登录表单：{}", emp);
    return Result.success();
}
```

> [!IMPORTANT]
> **两种表单提交方式的区别**：
> - `application/x-www-form-urlencoded`：只传文本键值对 → 直接绑定形参 / 实体类（本章 2.5）
> - `multipart/form-data`：带 `<input type="file">`，以二进制传文件 → 用 `MultipartFile` 接收
>
> 可以根据请求头里的 `Content-Type` 来判断是哪种表单提交方式。

## 2.6 `multipart/form-data` 表单提交

文件上传用的是这种表单提交：前端以 `multipart/form-data` 的表单提交二进制文件，Spring MVC 用 `MultipartFile` 这个类来接收，Controller 形参直接写 `MultipartFile file` 即可。

### 2.6.1 `MultipartFile` 表单介绍

#### （1）表单三要素

原始 `form` 表单要完成文件上传，必须**同时**满足以下三点，缺一不可：

| 要素         | 写法                                | 为什么                                                        |
| ---------- | --------------------------------- | ---------------------------------------------------------- |
| `file` 域   | `<input type="file" name="file">` | 提供文件选择入口，让用户选到要上传的文件                                       |
| 提交方式为 POST | `method="post"`                   | 文件通常较大，GET 只能把数据拼在 URL 后面，承载不了二进制数据                        |
| 编码类型       | `enctype="multipart/form-data"`   | 默认的 `application/x-www-form-urlencoded` 只支持文本键值对，无法传输二进制内容 |

```html
<form action="/upload" method="post" enctype="multipart/form-data">
    <input type="file" name="file" />
    <button type="submit">上传</button>
</form>
```

> [!TIP]
> 实际开发很少手写 `form`，前端多用 Element Plus 的 `<el-upload :auto-upload="false" :http-request="customUpload">`（详见 [[Web开发之前端]]）。但它底层提交的依然是 `multipart/form-data`，**后端的接收写法完全一样**。

#### （2）MultipartFile 常用方法

`MultipartFile` 是 Spring 封装的上传文件对象，常用方法如下：

| 方法                             | 说明             |
| ------------------------------ | -------------- |
| `String getOriginalFilename()` | 获取原始文件名        |
| `void transferTo(File dest)`   | 将接收的文件转存到磁盘文件中 |
| `byte[] getBytes()`            | 获取文件内容的字节数组    |
| `InputStream getInputStream()` | 获取接收到的文件内容的输入流 |
| `long getSize()`               | 获取文件的大小        |
| `boolean isEmpty()`            | 判断是否为空文件       |

> [!TIP]
> `transferTo(File)` 与 `getBytes()` 都能拿到文件内容，区别在于：文件较大时 `getBytes()` 会把整个文件一次性读进内存，容易 Out Of Memory；`transferTo` 是**边写边读**，更省内存，大文件优先用它。

#### （3）上传大小限制

Spring Boot 默认单文件上限只有 1 MB，超出会抛 `MaxUploadSizeExceededException`（表现为接口直接返回 400）。需要放宽时在 `application.yml` 中配置：

```yaml
spring:
  servlet:
    multipart:
      max-file-size: 10MB        # 单个文件最大大小
      max-request-size: 100MB    # 一次请求携带的所有文件的总大小
```

两者的区别：`max-file-size` 管**单个文件**，`max-request-size` 管**整个请求**。一次上传多个文件时，所有文件的总大小不能超过 `max-request-size`。

### 2.6.2 本地存储

本地存储的思路就四步：**接收文件 → 生成新文件名 → 转存到磁盘 → 返回结果**（若需要前端回显图片，再把文件路径一起返回）。

> [!WARNING]
> 落盘前一定要用 UUID 之类的随机名重命名，**不要直接拿原始文件名存盘**：
> - 同名文件会互相覆盖，先传的被后传顶掉；
> - 扩展名必须保留（用 `substring(lastIndexOf("."))` 截取），否则浏览器无法识别文件类型。

```java
package com.itheima.controller;

import com.itheima.pojo.Result;
import lombok.extern.slf4j.Slf4j;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.multipart.MultipartFile;

import java.io.File;
import java.util.UUID;

@Slf4j
@RestController
public class UploadController {

    private static final String UPLOAD_DIR = "uploads/";

    /**
     * 文件上传，本地存储
     */
    @PostMapping("/upload")
    public Result uploadFile(MultipartFile file) {
        log.info("上传文件：{}", file.getOriginalFilename());

        if (file.isEmpty()) {
            return Result.error("上传文件为空");
        }

        try {
            // 1. 截取原始文件名的扩展名
            String originalFilename = file.getOriginalFilename();
            String fileExtension = originalFilename.substring(originalFilename.lastIndexOf("."));

            // 2. 用 UUID 生成新文件名，避免同名覆盖
            String newFileName = UUID.randomUUID().toString() + fileExtension;

            // 3. 创建上传目录；目录不存在时递归创建（单级用 mkdir，多级用 mkdirs）
            File newFile = new File(UPLOAD_DIR + newFileName);
            if (!newFile.getParentFile().exists()) {
                newFile.getParentFile().mkdirs();
            }

            // 4. 转存到磁盘
            file.transferTo(newFile);
            log.info("文件上传成功：{}", newFile.getAbsolutePath());

            // 5. 返回文件路径，方便前端回显
            return Result.success(newFile.getAbsolutePath());
        } catch (Exception e) {
            log.error("文件上传失败", e);
            return Result.error("文件上传失败");
        }
    }
}
```

> [!IMPORTANT]
> 本地存储只适合**开发练习与单机小流量**场景，它有两个绕不开的问题：
> - **多实例不共享**：以后部署两台服务器，A 传的图片在 B 上访问不到；
> - **服务器磁盘会满**：数据库里存的路径还依赖应用目录，容器一重新部署就全没了。
>
> 所以生产环境一律用对象存储。

### 2.6.3 阿里云 OSS

#### （1）开通与准备

本地存储的局限很明显：文件和服务绑死、不能多台服务器共享、占服务器磁盘。生产环境通常改用**对象存储**（阿里云 OSS、腾讯云 COS 等），文件传到云上，数据库只存返回的 URL。

开通流程：**开通 OSS 云服务 → 创建 Bucket**。


> [!NOTE]
> 为什么创建 Bucket 时，权限选择 **公共读** ？
>
> - **私有**：读文件也要带签名（AccessKey 签名）才能访问。浏览器用 `<img src="...">` 加载图片时不会带签名，直接就白板了。
> - **公共访问**：Bucket 里的任何人都可以列出、删除、上传文件，等于把整个存储桶敞开，风险极大。
> - **公共读**：只有「读取对象」是公开的，图片可以直接展示；而**列出文件、删除文件等管理操作仍然需要 AccessKey**。正好满足「图片要能直接显示，但不允许陌生人管理文件」的需求。
>

#### （2）引入依赖与配置凭证

以 Java SDK v2 为例，在 `pom.xml` 中添加如下依赖，并将 `<version>` 替换为在 [Maven Repository](https://mvnrepository.com/artifact/com.aliyun/alibabacloud-oss-v2) 查询到的最新版本号：

```xml
<dependency>
    <groupId>com.aliyun</groupId>
    <artifactId>alibabacloud-oss-v2</artifactId>
    <version><!-- 填写最新版本号 --></version>
</dependency>
```

**创建 AccessKey** → **配置 AccessKey**：在 [RAM 控制台](https://ram.console.aliyun.com/users/create) 创建「使用永久 AccessKey 访问」的 RAM 用户，保存 AccessKey，然后为该用户授予 `AliyunOSSFullAccess` 权限。

将 RAM 用户的 AccessKey 写入环境变量作为凭证：

```bash
setx OSS_ACCESS_KEY_ID "YOUR_ACCESS_KEY_ID"
setx OSS_ACCESS_KEY_SECRET "YOUR_ACCESS_KEY_SECRET"
```

```bash
echo %OSS_ACCESS_KEY_ID%
echo %OSS_ACCESS_KEY_SECRET%
```

> [!WARNING]
> - `setx` 只对**新打开**的命令行窗口生效，执行后需要重开终端（IDEA 中则需重启项目）才能读到值。
> - AccessKey 的权限等同于账号密码，一旦泄露要立刻在 RAM 控制台禁用该密钥。更稳妥的做法是用「临时 STS 令牌」，但配置更复杂，个人项目用环境变量即可。

#### （3）集成代码

建议把上传逻辑封装成一个 `@Component` 组件，对外只暴露一个「传字节数组 + 原始文件名 → 返回 URL」的方法，Controller 调用起来很干净。

```java
package com.itheima.utils;

import com.aliyun.sdk.service.oss2.OSSClient;
import com.aliyun.sdk.service.oss2.credentials.CredentialsProvider;
import com.aliyun.sdk.service.oss2.credentials.EnvironmentVariableCredentialsProvider;
import com.aliyun.sdk.service.oss2.models.PutObjectRequest;
import com.aliyun.sdk.service.oss2.models.PutObjectResult;
import com.aliyun.sdk.service.oss2.transport.BinaryData;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Component;

import java.net.URI;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.UUID;

@Slf4j
@Component
public class AliyunOSSOperator {

    // 配置信息，根据自己的 bucket 修改；用常量命名，表示运行期不变
    private static final String ENDPOINT = "https://oss-cn-hangzhou.aliyuncs.com";
    private static final String REGION = "cn-hangzhou";
    private static final String BUCKET_NAME = "tlias-system-javaweb";

    /**
     * 上传文件到 OSS
     * @param content 文件字节数组
     * @param originalFilename 原始文件名
     * @return 文件的外网访问 url
     * @throws Exception 上传异常
     */
    public String upload(byte[] content, String originalFilename) throws Exception {
        // 1. 从环境变量中读取 AccessKey，避免密钥硬编码
        CredentialsProvider provider = new EnvironmentVariableCredentialsProvider();

        // 2. 构建 OSS 客户端
        OSSClient client = OSSClient.newBuilder()
                .credentialsProvider(provider)
                .endpoint(URI.create(ENDPOINT))
                .region(REGION)
                .build();

        // 3. 拼接对象路径：yyyy/MM/UUID.后缀，按日期分层避免单目录文件过多
        String suffix = originalFilename.substring(originalFilename.lastIndexOf("."));
        String datePath = LocalDate.now().format(DateTimeFormatter.ofPattern("yyyy/MM"));
        String objectKey = datePath + "/" + UUID.randomUUID() + suffix;

        try {
            PutObjectRequest request = PutObjectRequest.newBuilder()
                    .bucket(BUCKET_NAME)
                    .key(objectKey)
                    .body(BinaryData.fromBytes(content))
                    .build();

            PutObjectResult result = client.putObject(request);
            log.info("上传成功，status code：{}，request id：{}，eTag：{}",
                    result.statusCode(), result.requestId(), result.eTag());
        } finally {
            client.close();
        }

        // 4. 拼接外网访问 URL
        return "https://" + BUCKET_NAME + "." + ENDPOINT.replace("https://", "") + "/" + objectKey;
    }
}
```

Controller 侧调用同样干净：

```java
@Slf4j
@RestController
public class UploadController {

    @Autowired
    private AliyunOSSOperator aliyunOSSOperator;

    /**
     * 文件上传到阿里云 OSS
     */
    @PostMapping("/upload")
    public Result upload(MultipartFile file) throws Exception {
        String url = aliyunOSSOperator.upload(file.getBytes(), file.getOriginalFilename());
        return Result.success(url);
    }
}
```

| 配置项 | 说明 |
| --- | --- |
| `ENDPOINT` | 阿里云 OSS 中 Bucket 对应的域名，如 `https://oss-cn-hangzhou.aliyuncs.com` |
| `REGION` | Bucket 所属区域，如 `cn-hangzhou`，需与 Bucket 实际所在区域一致 |
| `BUCKET_NAME` | Bucket 名称（控制台上的名字，不是域名） |
| `objectKey` | 文件在 Bucket 中的**对象路径**（含文件名），即上面拼的 `yyyy/MM/UUID.后缀` |
| `EnvironmentVariableCredentialsProvider` | 凭证提供者，从环境变量里读取 AccessKey，避免密钥写进代码 |

> [!TIP]
> 三个概念分清楚，上传代码就写对了：
> - **Bucket**：存储空间，相当于一个顶层文件夹；
> - **Object**：Bucket 里的一个文件，`objectKey` 就是它的完整「路径 + 文件名」；
> - **Endpoint + Region**：访问域名和所在区域，**换区域就必须同步改这两处**，否则会报 403 / `AccessDenied`。
>
> 另外别忘了：`<img src="...">` 能直接显示，靠的是 Bucket 的**公共读**权限；返回给前端的 URL 要存进数据库字段（如 `emp.image`），下次查询直接返回。

---

## 2.7 常用参数注解补充

| 注解                                        | 作用                                       |
| ----------------------------------------- | ---------------------------------------- |
| `@RequestParam(defaultValue = "1")`       | 设置请求参数的默认值，请求参数缺失时生效                     |
| `@RequestParam(required = false)`         | 参数非必填，`null` 也能通过校验                      |
| `@RequestBody`                            | 接收 JSON 请求体                              |
| `@PathVariable`                           | 接收 URL 路径变量                              |
| `@RequestHeader`                          | 接收请求头                                    |
| `@CookieValue`                            | 接收 Cookie                                |
| `@DateTimeFormat(pattern = "yyyy-MM-dd")` | Spring MVC 接收前端提交的字符串日期，自动转为 `LocalDate` |

`@RequestParam(defaultValue = "1")` 的典型用法——前端不传页码时兜底：

```java
@GetMapping
public Result page(@RequestParam(defaultValue = "1") Integer page,
                   @RequestParam(defaultValue = "10") Integer pageSize) {
    // page 缺省为 1，pageSize 缺省为 10
    return Result.success();
}
```

---

# 三、MyBatis

## 3.1 基础注解

### 3.1.1 四种 CRUD 注解

MyBatis 支持用注解直接在接口方法上编写 SQL，无需 XML。四种 CRUD 注解与 SQL 类型的对应关系如下：

| 注解        | 对应 SQL   | 标注位置         | 作用与常见用法                     |
| --------- | -------- | ------------ | --------------------------- |
| `@Select` | `select` | Mapper 接口方法上 | 查询数据。返回单个对象或 `List`；        |
| `@Insert` | `insert` | Mapper 接口方法上 | 新增数据。可搭配 @Options 实现自增主键回填。 |
| `@Update` | `update` | Mapper 接口方法上 | 修改数据。执行更新操作。                |
| `@Delete` | `delete` | Mapper 接口方法上 | 删除数据。执行删除操作。                |

```java
@Mapper
public interface DeptMapper {

    /** 查询所有部门 */
    @Select("select * from dept")
    List<Dept> findAll();

    /** 根据 id 查询部门 */
    @Select("select * from dept where id = #{id}")
    Dept getById(Integer id);

    /** 保存部门 */
    @Insert("insert into dept (name, create_time, update_time) values (#{name}, #{createTime}, #{updateTime})")
    void save(Dept dept);

    /** 更新部门 */
    @Update("update dept set name = #{name}, update_time = #{updateTime} where id = #{id}")
    void update(Dept dept);

    /** 根据 id 删除部门 */
    @Delete("delete from dept where id = #{id}")
    void deleteById(Integer id);
}
```

> [!WARNING]
> 注解名必须与 SQL 类型严格对应。写 `@Select("delete ...")` 会在运行时报 `BadSqlGrammarException`。
>
> 另：`@Update` / `@Delete` 漏写 `where` 条件是**删库级别的事故**，且事务也救不回来（没有异常就不会回滚），所以这两个注解的 SQL 一定要逐条检查。

**注解里也能写动态 SQL**：注解中的 SQL 本质是一个字符串，解析器默认当纯文本处理。把 SQL 包在 `<script>…</script>` 里，MyBatis 就会按 XML 的规则解析，`<if>`、`<foreach>` 都能用：

```java
@Select("""
    <script>
        select * from emp
        <where>
            <if test="name != null and name != ''">
                and name like concat('%', #{name}, '%')
            </if>
            <if test="gender != null">
                and gender = #{gender}
            </if>
        </where>
        order by id desc
    </script>
    """)
List<Emp> list(String name, Integer gender);
```

> [!TIP]
> **注解和 XML 可以在同一个 Mapper 接口中混用**：方法名与 XML 里的 `id` 对应时，XML 中的 SQL 生效，方法上的注解被忽略。**简单 SQL 用注解，复杂的动态 SQL 写 XML**。

> [!WARNING]
> 注解中的 `#{…}` 是**预编译占位符**，能防 SQL 注入；而 `${…}` 是**字符串直接拼接**，只允许出现在排序字段（`order by ${orderBy}`）这类无法参数化的位置，拼接用户输入会导致注入风险。

## 3.2 参数传递

### 3.2.1 单个或多个简单类型参数

Mapper 方法的形参是单个普通类型时，`#{…}` 里的属性名可以任意写（`#{id}`、`#{value}` 都可以），MyBatis 会把唯一的那一个实参传进去。

```java
/** 根据 id 删除部门 */
@Delete("delete from dept where id = #{id}")
void deleteById(Integer id);
```

### 3.2.2 对象类型参数

需要传多个参数时，把它们封装到一个对象中。此时 `#{…}` 里写的是**对象的属性名**，注意是 Java 属性名，不是数据库表的字段名。

```java
@Mapper
public interface EmpMapper {

    /** 保存员工 */
    @Insert("insert into emp (name, gender, entry_date) values (#{name}, #{gender}, #{entryDate})")
    void save(Emp emp);
}
```

上例中 `#{name}`、`#{gender}`、`#{entryDate}` 取的都是 `Emp` 对象的属性；注意 `entryDate` 对应表字段 `entry_date`，靠的是驼峰命名映射（见 3.3.1）。

## 3.3 结果映射

### 3.3.1 @Options 主键回填

依赖数据库的**自增主键**时，插入数据后数据库会生成主键。`@Options(useGeneratedKeys = true, keyProperty = "id")` 能让 MyBatis 把生成的主键**回填到传入对象上**。

典型场景：保存员工基本信息之后，还要用这个员工的 ID 去保存他的工作经历。

```java
@Mapper
public interface EmpMapper {

    /**
     * 保存员工基本信息
     * 插入成功后，自增主键会自动回填到 emp.id 上
     */
    @Options(useGeneratedKeys = true, keyProperty = "id")
    @Insert("insert into emp (name, gender, entry_date) values (#{name}, #{gender}, #{entryDate})")
    void save(Emp emp);
}
```

插入成功后，`emp.getId()` 已经有值，可以直接拿去关联工作经历：

```java
@Override
public void save(Emp emp) {
    // 1. 保存基本信息，执行完毕后 emp.id 已被回填
    empMapper.save(emp);

    // 2. 直接用回填的 id 关联工作经历
    List<EmpExpr> exprList = emp.getExprList();
    if (exprList != null && !exprList.isEmpty()) {
        exprList.forEach(expr -> expr.setEmpId(emp.getId()));
        empExprMapper.saveBatch(exprList);
    }
}
```

当 SQL 定义在 XML 上时，也可以直接在 `<insert>` 标签上写：

```xml
<!-- useGeneratedKeys、keyProperty 写在 insert 标签上，实现主键回填 -->
<insert id="save" useGeneratedKeys="true" keyProperty="id">
    insert into emp (name, gender, entry_date) values (#{name}, #{gender}, #{entryDate})
</insert>
```


### 3.3.2 属性名与字段名不一致

实体类属性名与数据库字段名不一致时，MyBatis 无法自动封装。字段名与属性名**符合驼峰命名规则**时（`create_time` ↔ `createTime`），MyBatis 会自动完成映射。三种解决方式如下。

#### （1）全局配置开启驼峰映射

在 `application.yml` 中开启，一行配置全局生效，无需改动任何 SQL。

```yaml
mybatis:
  configuration:
    # 开启下划线命名到驼峰命名的自动映射，默认 false
    map-underscore-to-camel-case: true
```

开启后，SQL 可以直接写 `select *`：

```java
@Select("select * from dept")
List<Dept> findAll();
```

#### （2）SQL 中使用 as 起别名

在 SQL 里为不一致的列名起别名，别名与实体类属性名保持一致。

```java
@Select("select id, name, create_time as createTime, update_time as updateTime from dept")
List<Dept> findAll();
```

#### （3）@Results + @Result 手动映射

在方法上用注解声明「属性名 ↔ 列名」的对应关系，适合列名与属性名毫无规律的情况。

```java
@Results({
    @Result(property = "createTime", column = "create_time"),
    @Result(property = "updateTime", column = "update_time")
})
@Select("select * from dept")
List<Dept> findAll();
```

> [!TIP]
> 全局驼峰映射只能解决「下划线 ↔ 驼峰」这一种情况，**列名与属性名毫无规律时**（如 `u_n` ↔ `userName`）只能靠 `as` 别名或 `@Results` 手动映射。三种方式可以叠加使用，建议把（1）常开，作为兜底。

### 3.3.3 resultType

`resultType` 用于指定 `<select>` 语句返回值的类型，MyBatis 会按「列名 ↔ 属性名」自动把每行记录封装成该类型的对象。**最常见的用法是直接写实体类（POJO）的全限定名**：

```xml
<select id="findAll" resultType="com.itheima.pojo.Dept">
    select * from dept
</select>
```

此时 Mapper 方法的返回值写成 `List<Dept>` 即可，查询返回几行，List 里就有几个 `Dept` 对象。

除了 POJO，`resultType` 还可以放 `java.util.Map` 等类型，例如：
```xml
<select id="countEmpJobData" resultType="java.util.Map">
    SELECT
        CASE job
            WHEN 1 THEN '班主任'
            WHEN 2 THEN '讲师'
            WHEN 3 THEN '学工主管'
            WHEN 4 THEN '教研主管'
            WHEN 5 THEN '咨询师'
            ELSE '其他' END pos,
        COUNT(*) total
    FROM emp
    GROUP BY job
    ORDER BY total
</select>
```
此时，我们便可以利用下面的 mapper 接口来接收

```java
@MapKey("pos")
List<Map<String,Object>> countEmpJobData();
```

> 如果查询的记录往 Map 中封装，可以通过 `@MapKey` 注解指定返回的 Map 中的唯一标识是哪一个字段

### 3.3.4 resultMap

`resultMap` 的设计思想是：简单语句零配置，复杂语句只描述关系。单表查询用 `resultType` 就够了，MyBatis 会自动按属性名映射；而关联查询的「一对多」结构，必须用 `resultMap` 显式声明。

#### （1）resultMap 基本结构

```xml
<!-- id：resultMap 唯一标识；type：对应 POJO 类全限定名 -->
<resultMap id="empResultMap" type="com.itheima.pojo.Emp">
    <!-- id 标签：主键列 -->
    <id column="id" property="id"/>
    <!-- result 标签：普通字段
         column：数据库字段名
         property：实体类属性名 -->
    <result column="entry_date" property="entryDate"/>
    <result column="dept_id" property="deptId"/>
    <result column="create_time" property="createTime"/>
    <result column="update_time" property="updateTime"/>
</resultMap>
```

使用时把 `<select>` 的 `resultType` 换成 `resultMap`，值与上面 `id` 保持一致：

```xml
<select id="getById" resultMap="empResultMap">
    select * from emp where id = #{id}
</select>
```

#### （2）collection 封装一对多集合

员工与工作经历是一对多关系：一个 `Emp` 对应多条 `EmpExpr`。在 `resultMap` 里用 `<collection>` 声明这个集合，`ofType` 指定集合元素的类型：

```xml
<resultMap id="empResultMap" type="com.itheima.pojo.Emp">
    <id column="id" property="id"/>
    <result column="name" property="name"/>
    <result column="entry_date" property="entryDate"/>
    <result column="dept_id" property="deptId"/>
    <result column="create_time" property="createTime"/>
    <result column="update_time" property="updateTime"/>

    <!-- 封装 exprList：ofType 指定集合元素的类型 -->
    <collection property="exprList" ofType="com.itheima.pojo.EmpExpr">
        <id column="ee_id" property="id"/>
        <result column="ee_company" property="company"/>
        <result column="ee_job" property="job"/>
        <result column="ee_begin" property="begin"/>
        <result column="ee_end" property="end"/>
        <result column="ee_empid" property="empId"/>
    </collection>
</resultMap>

<!-- 根据 ID 查询员工的详细信息（含工作经历） -->
<select id="getById" resultMap="empResultMap">
    select e.*,
           ee.id      ee_id,
           ee.emp_id  ee_empid,
           ee.begin   ee_begin,
           ee.end     ee_end,
           ee.company ee_company,
           ee.job     ee_job
    from emp e
    left join emp_expr ee on e.id = ee.emp_id
    where e.id = #{id}
</select>
```

> [!NOTE]
> 关联查询时两张表都有 `id` 列，直接 `select e.*, ee.*` 会让两个 `id` 撞车。所以给 `emp_expr` 的列统一加 `ee_` 前缀起别名，再在 `<collection>` 里用 `ee_id`、`ee_company` 等别名一一对应，MyBatis 才能把工作经历正确「塞进」`exprList`。

## 3.4 动态 SQL

**动态 SQL**，就是随用户输入或外部条件变化而变化的 SQL 语句。项目中也用 XML 映射文件编写，常用标签见下表。

| 标签                                    | 作用                                    |
| ------------------------------------- | ------------------------------------- |
| `<if>`                                | 条件成立时拼接 SQL 片段                        |
| `<where>`                             | 动态生成 `where`，并自动去掉多余的 `and` / `or`    |
| `<set>`                               | 动态生成 `set`，并自动去掉多余的逗号，用于修改语句          |
| `<foreach>`                           | 遍历集合，常用于 `in (...)` 查询与批量插入           |
| `<choose>` / `<when>` / `<otherwise>` | 多分支选择，类似 Java 的 `if - else if - else` |
| `<sql>` / `<include>`                 | 抽取并复用公共 SQL 片段                        |

### 3.4.1 if 与 where 标签

- `<if>`：判断条件是否成立，成立（`test` 为 `true`）则把标签内的 SQL 片段拼接进来。
- `<where>`：根据查询条件动态生成 `where` 关键字，并**自动去除条件前面多余的 `and` / `or`**。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper
        PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.itheima.mapper.EmpMapper">

    <select id="list" resultType="com.itheima.pojo.Emp">
        select e.*, d.name as deptName
        from emp as e
        left join dept as d on e.dept_id = d.id
        <where>
            <!-- 姓名：模糊查询，注意用 concat 拼接 % -->
            <if test="name != null and name != ''">
                and e.name like concat('%', #{name}, '%')
            </if>
            <!-- 性别：精确查询 -->
            <if test="gender != null">
                and e.gender = #{gender}
            </if>
            <!-- 入职时间：区间查询 -->
            <if test="begin != null and end != null">
                and e.entry_date between #{begin} and #{end}
            </if>
        </where>
        order by e.id desc
    </select>

</mapper>
```

> [!TIP]
> `<where>` 已经去掉了多余的 `and`，但仍建议**显式书写 `and`**，这样 XML 可读性更好、复制片段时也不会出错。
>
> 用 `<where>` 的前提是：所有条件都是用 `and` 连接的（`or` 混用时它无法正确裁剪，需要用 `<trim>`）。

### 3.4.2 foreach 遍历集合

`<foreach>` 用于遍历集合，常用于 `in (...)` 查询或批量插入 `values (...)`。

| 属性 | 说明 |
| --- | --- |
| `collection` | 集合名称 |
| `item` | 集合遍历出来的元素 / 项 |
| `separator` | 每次遍历使用的分隔符 |
| `open` | 遍历开始前拼接的片段 |
| `close` | 遍历结束后拼接的片段 |
| `index` | 遍历的下标 |

这些属性都是可选的，按实际需求指定即可。

1. `in (...)` 查询

```xml
<!--根据ID删除员工信息-->
<delete id="deleteByIds">
    delete from emp where id in
    <foreach collection="ids" item="id" open="(" separator="," close=")">
        #{id}
    </foreach>
</delete>
```

当 `ids = [1, 2, 3]` 时，拼接结果为：`delete from emp where id in (1,2,3)`。

2. 批量插入 `values (...)`

```xml
<insert id="insertBatch">
    insert into emp_expr (emp_id, begin, end, company, job) values
    <foreach collection="exprList" item="expr" separator=",">
        (#{expr.empId}, #{expr.begin}, #{expr.end}, #{expr.company}, #{expr.job})
    </foreach>
</insert>
```

> [!WARNING]
> 批量插入的 SQL **结尾绝对不能加分号**，否则拼接后变成 `values (...), (...);`，直接报语法错误；一次插入的数据量也别太大，几百条一批即可，再多就拆成多次调用（配合事务）。

### 3.4.3 choose 分支选择

`<choose>` 用于「多个条件互斥、只命中一个」的场景，写法与 Java 的 `if - else if - else` 一一对应：

```xml
<select id="listByCondition" resultType="com.itheima.pojo.Emp">
    select * from emp
    <where>
        <choose>
            <when test="name != null and name != ''">
                and name like concat('%', #{name}, '%')
            </when>
            <when test="deptId != null">
                and dept_id = #{deptId}
            </when>
            <otherwise>
                and gender = 1
            </otherwise>
        </choose>
    </where>
</select>
```

> [!NOTE]
> `<choose>` 遵循「**从上到下只匹配一个**」的规则：这里 `name` 一旦有值，后面的 `deptId` 分支就不会再看。所以它和连续写几个 `<if>` 的效果完全不同——`<if>` 是互不影响的多个条件。

### 3.4.4 sql 片段复用

条件查询的 SQL 常常成对出现（员工列表、部门列表……），可以把公共片段抽成 `<sql>`，用 `<include>` 引用，避免复制粘贴后改一处漏一处：

```xml
<!-- 定义公共片段 -->
<sql id="empCommon">
    e.id, e.username, e.name, e.gender, e.phone, e.job, e.salary, e.image,
    e.entry_date, e.dept_id, e.create_time, e.update_time
</sql>

<select id="list" resultType="com.itheima.pojo.Emp">
    select <include refid="empCommon"/>, d.name as deptName
    from emp as e
    left join dept as d on e.dept_id = d.id
    order by e.id desc
</select>
```

---

# 四、日志

## 4.1 常见日志框架

| 名称 | 说明 |
| --- | --- |
| **JUL** | Java SE 平台自带的官方日志框架。配置简单，但不灵活，性能较差 |
| **SLF4J** | Simple Logging Facade for Java，**日志门面**。只提供一套标准的日志接口与抽象类，不做具体实现，允许应用随时切换底层框架 |
| **Log4j** | Apache 的流行日志框架，配置灵活，支持多种输出目标。需注意 Log4j 1.x 存在严重漏洞，实际使用应选 Log4j 2 |
| **Logback** | 由 Log4j 原作者开发，是 **SLF4J 的参考实现**，性能优于 Log4j，配置更丰富，是 Spring Boot 默认集成的日志实现 |

SLF4J 本身没有实现，它是「门面」；真正干活的是绑定到它的实现（Logback / Log4j2 / JUL）。调用方只依赖 SLF4J API，底层换实现不需要改业务代码。这种「面向抽象编程」的做法就是**门面模式**。

## 4.2 SLF4J 使用

### 4.2.1 手动定义 Logger

三行固定搭配：导入 `Logger` 接口、`LoggerFactory` 工厂类，然后声明一个 `private static final` 日志记录器。

```java
import org.junit.jupiter.api.Test;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class LogTest {

    // 固定搭配：传入当前类的 Class 对象，框架据此输出类名
    private static final Logger log = LoggerFactory.getLogger(LogTest.class);

    @Test
    public void testLog() {
        log.debug("开始计算...");

        int[] nums = {1, 5, 3, 2, 1, 4, 5, 4, 6, 7, 4, 34, 2, 23};
        int sum = 0;
        for (int num : nums) {
            sum += num;
        }

        log.info("计算结果为：{}", sum);
        log.debug("结束计算...");
    }
}
```

### 4.2.2 Lombok @Slf4j 简化定义

Lombok 提供的 `@Slf4j` 注解可以省去手动定义日志记录器这一步。加了注解，等价于自动在类中生成了这一行：

```java
private static final Logger log = LoggerFactory.getLogger(Xxx.class);
```

```java
import lombok.extern.slf4j.Slf4j;

@Slf4j   // 自动生成 log 字段
@RestController
@RequestMapping("/depts")
public class DeptController {

    @Autowired
    private DeptService deptService;

    @GetMapping
    public Result list() {
        log.info("查询部门列表");
        return Result.success(deptService.findAll());
    }
}
```

> [!TIP]
> 有了 `@Slf4j` 就不要再手动 `LoggerFactory.getLogger(...)`，否则会字段重名。

### 4.2.3 占位符传参

**有几个大括号 `{}`，后面就要传几个对应参数**。参数不会被提前拼接，不传参时这一行日志的开销几乎为零，因此推荐用占位符而不是字符串 `+` 拼接。

```java
// 传单个参数
log.info("根据 id 删除部门，id：{}", id);

// 传单个对象
log.info("查询参数 {}", queryParam);
```

多个参数时按顺序一一对应：

```java
log.info("新增部门：{}，创建时间：{}", dept.getName(), dept.getCreateTime());
```

> [!WARNING]
> 大括号和参数**最好一一对应**：占位符比参数多时，多余的 `{}` 会原样输出到日志里；参数比占位符多时，多余的参数会被忽略（除非最后一个参数是 `Throwable`，会被当作异常打印堆栈）。排查日志「不完整」或「多出 `{}`」时，先数一数大括号。

## 4.3 Logback 入门

### 4.3.1 最简 logback.xml

Logback 的配置文件固定叫 `logback.xml`，放在 `src/main/resources` 目录下，**文件名叫错或放错位置都会导致配置不生效**。该文件用于控制日志的输出格式、输出位置与日志开关。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration>

    <!-- 控制台输出 -->
    <appender name="CONSOLE" class="ch.qos.logback.core.ConsoleAppender">
        <encoder>
            <!-- %d 日期 | %thread 线程名 | %-5level 级别左对齐宽 5 | %logger 类名 | %msg 消息 | %n 换行 -->
            <pattern>%d{HH:mm:ss.SSS} [%thread] %-5level %logger - %msg%n</pattern>
            <charset>UTF-8</charset>
        </encoder>
    </appender>

    <!-- 根日志级别 -->
    <root level="INFO">
        <appender-ref ref="CONSOLE"/>
    </root>

</configuration>
```

### 4.3.2 输出位置

常用的两种日志输出位置是**控制台**和**系统文件**。

#### （1）输出到控制台

用 `ConsoleAppender`，开发阶段最常用：

```xml
<appender name="CONSOLE" class="ch.qos.logback.core.ConsoleAppender">
    <encoder class="ch.qos.logback.classic.encoder.PatternLayoutEncoder">
        <pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{50}- %msg%n</pattern>
    </encoder>
</appender>
```

#### （2）输出到文件

需要输出到系统文件时配置 `RollingFileAppender`，日志文件会按天和大小自动滚动归档：

```xml
<appender name="FILE" class="ch.qos.logback.core.rolling.RollingFileAppender">
    <!-- 日志文件的存放位置 -->
    <file>D:/logs/tlias-current.log</file>

    <rollingPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedRollingPolicy">
        <!-- 归档文件名，%d 日期，%i 序号 -->
        <fileNamePattern>D:/logs/tlias-%d{yyyy-MM-dd}-%i.log</fileNamePattern>
        <!-- 最多保留 30 天的历史日志 -->
        <maxHistory>30</maxHistory>
        <!-- 单个文件最大 10MB，超过则滚动到新文件 -->
        <maxFileSize>10MB</maxFileSize>
    </rollingPolicy>

    <encoder class="ch.qos.logback.classic.encoder.PatternLayoutEncoder">
        <pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{50}- %msg%n</pattern>
    </encoder>
</appender>
```

> [!TIP]
> `<file>` 指定当前正在写的日志文件，`<fileNamePattern>` 指定滚动归档后的文件名。`RollingFileAppender` **两个都要写**：只写 `fileNamePattern` 而不写 `file`，日志在程序运行期间不会落盘，只有滚动后才生成文件。

### 4.3.3 日志级别

`<root level="...">` 就是日志总开关：`ALL` 表示全部开启，`OFF` 表示全部关闭。把多个 appender 通过 `appender-ref` 挂到 root 上，即可同时输出到控制台和文件。

```xml
<!-- 开启全部日志：输出到控制台 + 输出到文件 -->
<root level="ALL">
    <appender-ref ref="CONSOLE"/>
    <appender-ref ref="FILE"/>
</root>

<!-- 关闭全部日志：把级别改为 OFF -->
<root level="OFF">
    <appender-ref ref="CONSOLE"/>
    <appender-ref ref="FILE"/>
</root>
```

日志级别指日志信息的类型，**只有大于等于所配置级别的日志才会被输出**。

| 日志级别    | 说明                             | 记录方式               |
| ------- | ------------------------------ | ------------------ |
| `trace` | 追踪，记录程序运行轨迹                    | `log.trace("...")` |
| `debug` | 调试，记录程序调试过程中的信息                | `log.debug("...")` |
| `info`  | 记录一般信息，描述程序运行的关键事件，如网络连接、IO 操作 | `log.info("...")`  |
| `warn`  | 警告信息，记录潜在有害的情况                 | `log.warn("...")`  |
| `error` | 错误信息                           | `log.error("...")` |

优先级由低到高：`trace < debug < info < warn < error < off`。把级别配成 `info`，则 `trace` 和 `debug` 都不会输出。

```xml
<!-- 只输出 info 及以上级别 -->
<root level="INFO">
    <appender-ref ref="CONSOLE"/>
    <appender-ref ref="FILE"/>
</root>
```

> [!TIP]
> **开发阶段**把级别调成 `debug`，方便排查；**生产环境**只留 `info` 及以上，否则日志量会暴涨、磁盘和性能都吃不消。
>
> 想临时看某个类的详细日志、又不改全局配置时，可以在 `application.yml` 里针对包单独设置：
>
> ```yaml
> logging:
>   level:
>     root: info
>     com.itheima.mapper: debug   # 只把 Mapper 层打成 debug
> ```

---

# 五、分页解决方案

## 5.1 分页需求分析

### 5.1.1 前后端约定

分页功能的关键不在 SQL，而在**前后端约定**：前端得告诉后端「我要第几页、每页几条」，后端得告诉前端「一共有多少条、这一页的数据是什么」。约定清楚，两边才能各自独立开发、并行联调。

以员工列表分页为例，接口约定为 `GET /emps?page=1&pageSize=10`：

- **前端请求服务端时传递两个参数：**

```java
@Data
public class EmpQueryParam {

    private Integer page = 1;       // 页码
    private Integer pageSize = 10;  // 每页展示记录数
}
```

- **后端需要响应给前端两组数据：**

1. 所查询到的数据列表（存储到 `List` 集合中）
2. 总记录数

响应结构为「`Result` 外层 + `PageResult` 内层」的嵌套：

```json
{
  "code": 1,
  "msg": "success",
  "data": {
    "total": 12,
    "rows": [
      { "id": 1, "name": "阿里", "gender": 1, "deptName": "研发部" }
    ]
  }
}
```

**为什么前端非要 `total` 不可？** Element Plus 的 `<el-pagination>` 是受控组件，自己并不知道一共有多少条数据，只有拿到 `total` 才能算出总页数、渲染出页码栏。

因此需要一个分页结果对象把两者封装起来：

```java
@Data
@NoArgsConstructor
@AllArgsConstructor
public class PageResult<T> {

    private Long total;      // 总记录数
    private List<T> rows;    // 当前页数据列表
}
```

> [!TIP]
> 字段名属于**约定的一部分**，一旦确定前后端都不能随意改动。后端少返回 `total`、或把它命名成 `count`，前端分页组件就拿不到数据——这类联调问题，十有八九出在约定没对齐，而不是代码写错。
>
> 整个分页流程也就三步：**前端带 `page`、`pageSize` 发请求 → 后端返回 `total` + `rows` → 前端把 `total` 绑到分页组件上**。

## 5.2 手动分页

### 5.2.1 原始方式

以 `GET /emps?page=1&pageSize=10` 为例，Service 层需要**额外查询一次总记录数**，且「算偏移量 → 查总数 → 查数据 → 封装」的流程比较固定，代码冗余。

```java
@Mapper
public interface EmpMapper {

    /** 统计总记录数 */
    @Select("select count(*) from emp e left join dept d on e.dept_id = d.id")
    Long count();

    /** 查询分页数据，limit 第一个参数是偏移量，第二个参数是条数 */
    @Select("select e.*, d.name as deptName from emp e left join dept d on e.dept_id = d.id limit #{offset}, #{pageSize}")
    List<Emp> list(int offset, Integer pageSize);
}
```

```java
@Service
public class EmpServiceImpl implements EmpService {

    @Autowired
    private EmpMapper empMapper;

    @Override
    public PageResult<Emp> list(Integer page, Integer pageSize) {
        // 1. 查询总记录数
        Long total = empMapper.count();

        // 2. 计算分页偏移量：第 n 页的起始索引是 (n - 1) * 每页条数
        int offset = (page - 1) * pageSize;

        // 3. 查询当前页数据
        List<Emp> data = empMapper.list(offset, pageSize);

        // 4. 封装分页结果
        return new PageResult<>(total, data);
    }
}
```

> [!WARNING]
> 手动分页有两个容易翻车的点：
> - **页码没做校验**：前端传 `page=0` 会算出 `-pageSize` 这样的负偏移量，直接抛 SQL 异常。Service 层要么对页码做非空和范围校验，要么开启分页合理化（见 5.3.2）。
> - **总数查询要带上筛选条件**：带条件查询时，`count(*)` 的 where 条件必须与数据查询**完全一致**，否则 `total` 和 `rows` 对不上，分页组件会算错总页数。

## 5.3 PageHelper

PageHelper 是第三方提供的 MyBatis 分页插件，功能强大、使用方便，支持各种单表、多表分页查询。

>  使用后 Mapper 中只需写正常的列表查询，**无需再手动分页**。Service 层在调用 Mapper **之前**设置分页参数，调用**之后**解析分页结果。

### 5.3.1 使用步骤

先引入依赖：

```xml
<dependency>
    <groupId>com.github.pagehelper</groupId>
    <artifactId>pagehelper-spring-boot-starter</artifactId>
    <version>2.1.0</version>
</dependency>
```

再在 Service 层设置分页参数、取分页结果：

```java
@Service
public class EmpServiceImpl implements EmpService {

    @Autowired
    private EmpMapper empMapper;

    @Override
    public PageResult<Emp> list(EmpQueryParam param) {
        // 1. 设置分页参数，必须在查询方法调用之前
        PageHelper.startPage(param.getPage(), param.getPageSize());

        // 2. 查询员工列表，Mapper 中写普通查询即可
        List<Emp> empList = empMapper.list(param);

        // 3. 从返回的 List 中获取分页信息（它实际是 Page 对象）
        PageInfo<Emp> pageInfo = new PageInfo<>(empList);

        // 4. 封装分页结果返回
        return new PageResult<>(pageInfo.getTotal(), pageInfo.getList());
    }
}
```

此时 Mapper 恢复成最朴素的样子：

```xml
<select id="list" resultType="com.itheima.pojo.Emp">
    select e.*, d.name as deptName
    from emp as e
    left join dept as d on e.dept_id = d.id
    order by e.id desc
</select>
```

### 5.3.2 细节与注意事项

PageHelper 分页时会执行两条 SQL，这两条 SQL 都是从 Mapper 中我们编写的 SQL **演变**而来的：

1. 查询总记录数。把原 SQL 的查询字段列表替换成 `count(0)` 进行统计。
2. 分页查询指定页码的数据：在原 SQL 之后拼接 `limit ?, ?`（第一个参数是偏移量，第二个是条数）。

PageHelper 把总记录数与数据列表一起封装到 `Page<Emp>` 对象中，我们拿到查询结果后直接调用它的方法即可。

> [!WARNING]
> - PageHelper 分页时，SQL 语句结尾**一定一定一定不要加分号 `;`**，否则分页失效并抛异常。
> - PageHelper **只会对紧跟其后的第一条 SQL 语句**做分页处理，`startPage()` 和查询方法之间不能插入其他 SQL。

当页码输入负数时，分页结果会异常。开启**分页合理化**参数后，越界的页码会被自动修正：

```yaml
pagehelper:
  # 分页合理化：默认 false；设为 true 后 pageNum <= 0 查第一页，pageNum > 总页数 查最后一页
  reasonable: true
  # 数据库方言
  helper-dialect: mysql
```

> [!TIP]
> `startPage()` 的原理是**拦截器**在下一条 SQL 上做改写，所以它对「代码写在哪一行」没有要求，但对「下一条是不是目标查询」有严格要求。分页查询和总数查询要分开调用，不要试图在一次查询里既分页又统计。

---

# 六、事务

## 6.1 事务基础

### 6.1.1 概念与三步操作

**事务**是一组操作的集合，是一个不可分割的工作单位。事务会把所有操作作为一个整体，一起向系统提交或撤销操作请求，即这些操作**要么同时成功，要么同时失败**。事务控制主要有以下三步：

1. 在这组操作执行**之前**，先开启事务（`start transaction;` 或 `begin;`）。
2. 所有操作**全部成功**，则提交事务（`commit;`）。
3. 只要有**任何一个操作失败**，就回滚事务（`rollback;`）。

```sql
-- 1. 开启事务
begin;

-- 2. 保存员工基本信息
insert into emp (id, name, username, gender, phone, job, salary, entry_date)
values (39, 'Tom', '123456', 1, '13300001111', 1, 4000, '2023-11-01');

-- 3. 保存该员工的工作经历信息（两条）
insert into emp_expr (emp_id, `begin`, `end`, company, job)
values (39, '2019-01-01', '2020-01-01', '百度', '开发'),
       (39, '2020-01-10', '2022-02-01', '阿里', '架构');

-- 4. 以上全部成功则提交，数据正式生效
commit;

-- 5. 若第 2 或 3 步失败，则执行回滚，数据恢复到事务开始前
rollback;
```

> [!TIP]
> `begin`、`end` 在 MySQL 中是关键字，作列名时要用反引号包裹。

### 6.1.2 事务的四大特性

事务必须同时满足 **ACID** 四大特性，缺一不可：

| 特性 | 全称 | 含义 | 通俗理解 |
| --- | --- | --- | --- |
| **原子性** | Atomicity | 事务中的操作要么全部成功，要么全部失败，不允许出现部分成功 | 转账时「扣钱」和「加钱」必须同时完成 |
| **一致性** | Consistency | 事务执行前后，数据库必须从一个**合法状态**变为另一个合法状态 | 转账前后总金额不变 |
| **隔离性** | Isolation | 多个事务并发执行时，彼此之间互不干扰，如同串行执行 | 两个人同时取钱不会把钱取成负数 |
| **持久性** | Durability | 事务一旦提交，数据就被永久保存，即使断电、宕机也不丢失 | 关掉服务再启动，数据还在 |

> [!NOTE]
> 四大特性里，**原子性和持久性由数据库自己保证**，应用层不用操心；**一致性靠业务代码约束**（比如转账前后总额必须相等）；**隔离性由隔离级别控制**，也是最容易出问题的一环——隔离级别越高越安全，但并发性能越低。

## 6.2 Spring 事务 @Transactional

### 6.2.1 注解位置与作用

`@Transactional` 的语义是：**方法执行前开启事务，方法执行完毕提交事务；执行过程中出现异常则回滚事务。** 标注位置有三个层次：

| 位置 | 效果 |
| --- | --- |
| 方法上 | 仅当前方法交由 Spring 进行事务管理 |
| 类上 | 当前类中**所有**方法都交由 Spring 进行事务管理 |
| 接口上 | 该接口下所有实现类中的所有方法都交由 Spring 进行事务管理 |

我们一般在**业务层（Service）** 控制事务。因为一个业务功能往往包含多个数据访问操作，在业务层控制，才能把多个数据访问操作纳入同一个事务范围内。

```java
@Service
public class EmpServiceImpl implements EmpService {

    @Autowired
    private EmpMapper empMapper;
    @Autowired
    private EmpExprMapper empExprMapper;

    @Override
    @Transactional   // 保存员工与保存工作经历，要么全成功要么全回滚
    public void save(Emp emp) {
        // 1. 保存基本信息
        empMapper.save(emp);

        // 2. 保存工作经历
        List<EmpExpr> exprList = emp.getExprList();
        if (exprList != null && !exprList.isEmpty()) {
            exprList.forEach(expr -> expr.setEmpId(emp.getId()));
            empExprMapper.saveBatch(exprList);
        }
    }
}
```

> [!WARNING]
> `@Transactional` 常见失效场景：① 方法不是 `public`；② 类未交给 Spring 管理（没加 `@Service` 等注解）；③ 异常被方法内部 `try-catch` 吞掉；④ 数据库引擎不支持事务（如 MyISAM）；⑤ 在同类内部方法间自调用，绕过代理对象。
>
> ⑤ 最隐蔽：如果 `save()` 内部调用了本类的另一个带 `@Transactional` 的方法，调用走的是 `this` 而不是 Spring 代理对象，事务**根本不会开启**。需要自调用生效时，可以注入自己（`@Lazy`）或从容器里 `AopContext.currentProxy()` 取代理。

### 6.2.2 回滚规则 rollbackFor

**默认情况下，只有抛出 `RuntimeException`（运行时异常）才会回滚事务**，受检异常（编译期异常）默认不回滚。

如果希望**所有异常都回滚**，需要配置 `@Transactional` 的 `rollbackFor` 属性，指定「出现何种异常类型时回滚事务」。

```java
@Service
public class EmpServiceImpl implements EmpService {

    // 无论抛出何种异常都回滚
    @Transactional(rollbackFor = Exception.class)
    public void save(Emp emp) {
        empMapper.save(emp);
    }
}
```

> [!TIP]
> `RuntimeException` 包含 `NullPointerException`、`IllegalArgumentException` 等；受检异常指 `IOException`、`SQLException` 这类编译期就能发现的异常。日常开发**建议统一写 `rollbackFor = Exception.class`**，免得抛了个受检异常结果数据只写了一半。

### 6.2.3 传播行为 propagation

**事务传播行为**指的是：当一个事务方法被另一个事务方法调用时，这个事务方法应该如何进行事务控制。

| 属性值 | 含义 |
| --- | --- |
| `REQUIRED` | 【默认值】需要事务，有则加入，无则创建新事务 |
| `REQUIRES_NEW` | 需要新事务，无论有无，总是创建新事务 |
| `SUPPORTS` | 支持事务，有则加入，无则在无事务状态中运行 |
| `NOT_SUPPORTED` | 不支持事务，在无事务状态下运行；若当前存在事务，则挂起当前事务 |
| `MANDATORY` | 必须有事务，否则抛异常 |
| `NEVER` | 必须没有事务，否则抛异常 |
| `NESTED` | 存在事务则创建嵌套事务（子事务），否则同 `REQUIRED` |

实际开发中**只需重点关注两个**：**`REQUIRED`**、**`REQUIRES_NEW`**：

```java
@Service
public class OrderServiceImpl implements OrderService {

    @Autowired
    private OrderMapper orderMapper;
    @Autowired
    private LogService logService;

    @Override
    @Transactional
    public void submit(Order order) {
        // 1. 保存订单（使用外层事务）
        orderMapper.save(order);

        // 2. 记录日志，使用独立事务，不受外层事务成败影响
        logService.record(order.getId());
    }
}
```

```java
@Service
public class LogServiceImpl implements LogService {

    @Autowired
    private LogMapper logMapper;

    @Override
    @Transactional(propagation = Propagation.REQUIRES_NEW)   // 总是开启新事务
    public void record(Long orderId) {
        logMapper.insert(orderId);
    }
}
```

> [!NOTE]
> 上例的含义：订单保存失败要回滚，但**「订单已提交」这条操作日志必须留下来**，否则排查问题时连「用户点了提交」都看不到。两个事务各管各的，语义才清晰。
>
> 注意 `REQUIRES_NEW` 会**挂起**外层事务并新开一个连接，所以大量使用会额外占用数据库连接，高并发场景要评估连接池大小。

### 6.2.4 事务日志

在 `application.yml` 中开启事务管理日志，就能在控制台看到与事务相关的日志信息（开启、提交、回滚）。

```yaml
logging:
  level:
    org.springframework.transaction: debug
    org.springframework.jdbc.datasource: debug
```

> [!TIP]
> 打开后日志里会出现 `Creating new transaction`、`Committing JDBC transaction`、`Rolling back JDBC transaction` 等关键字，是排查「事务到底有没有生效」的第一手线索。

---

# 七、全局异常处理器

## 7.1 为什么需要全局异常处理

后端接口一旦抛出异常，如果没有统一处理，默认会返回 Spring Boot 错误页或一段 JSON 堆栈：

```json
{
  "timestamp": "2024-01-01 10:00:00",
  "status": 500,
  "error": "Internal Server Error",
  "path": "/emps"
}
```

这对前端和用户都不友好：

- **前端拿不到业务语义**：`msg` 字段是空的，只能弹一个「未知错误」，没法针对性提示；
- **异常堆栈直接暴露**：把类名、SQL、文件路径写到响应里，等于给攻击者送情报；
- **代码重复**：每个 Controller 方法都要写一遍 `try-catch`，Controller 会被淹没在错误处理代码里。

所以要做一个**全局异常处理器**：把「捕获异常 + 打日志 + 封装成统一响应」这三件事收拢到一个类中，Controller 里只写业务。

## 7.2 快速上手

Spring MVC 提供两个注解用于定义全局异常处理器：

| 注解                      | 作用                                                                            |
| ----------------------- | ----------------------------------------------------------------------------- |
| `@RestControllerAdvice` | 声明这是一个**全局**异常处理类，相当于 `@ControllerAdvice` + `@ResponseBody`，方法返回值会自动序列化为 JSON |
| `@ExceptionHandler`     | 声明该方法处理哪种异常；同一个方法也可以处理多种，写成 `{异常1.class, 异常2.class}`                          |

最简实现仅需一个类、一个方法，即可拦截项目中抛出的全部异常：

```java
@Slf4j
@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler
    public Result handleException(Exception e) {
        log.error("程序出错啦~", e);
        return Result.error("出错啦，请联系管理员~");
    }
}
```

> [!TIP]
> 编写全局异常处理器的固定套路就三步：
> 1. 类上加 `@RestControllerAdvice`；
> 2. 方法上加 `@ExceptionHandler(Exception.class)` ，返回 `Result.error(...)`；
> 3. 每个方法里先 `log` 记录，再返回统一响应结果。
>
> 这样 Controller 里就只剩下「接收参数 → 调 Service → 返回结果」，异常处理完全不用写。

## 7.3 处理特定异常

实际开发中，不同异常给用户的提示应该不同：参数填错要提示他改，资源不存在要提示查不到，数据库报错只能吞掉。这类「有业务含义」的异常，建议自己定义：

```java
package com.ljxstar.exception;

import com.ljxstar.pojo.Result;
import lombok.extern.slf4j.Slf4j;
import org.springframework.dao.DuplicateKeyException;
import org.springframework.web.bind.annotation.ExceptionHandler;
import org.springframework.web.bind.annotation.RestControllerAdvice;

/**
 * 全局异常处理类
 */
@Slf4j
@RestControllerAdvice
public class GlobalExceptionHandler {

    @ExceptionHandler
    public Result handleDuplicateKeyException(DuplicateKeyException e) {
        log.error("程序出错啦~", e);
        String message = e.getMessage();
        int i = message.indexOf("Duplicate entry");
        String errMsg = message.substring(i);
        String[] arr = errMsg.split(" ");
        return Result.error(arr[2] + " 已存在");
    }
}
```

> [!IMPORTANT]
> **给前端看的 `msg` 和给排查用的日志，是两回事**：
> - `Result.error(...)` 的内容会直接返回给用户，绝不能包含异常堆栈、SQL、文件路径；
> - 完整的异常信息用 `log.error("服务器异常", e)` 打进日志（第二个参数传 `e`，SLF4J 会自动打印堆栈）。

> [!NOTE]
> Spring MVC 捕获到异常后，会根据异常类型在 Spring 容器中查找匹配的 `@ExceptionHandler` 处理方法，遵循**精确类型优先匹配原则**；如果没有找到对应异常的处理器，最终会匹配 `Exception.class` 通用异常处理方法。
> 同一个异常类型不要写两个处理方法，否则 Spring 会在启动时报 `Ambiguous @ExceptionHandler method mapped for ...` 并启动失败�