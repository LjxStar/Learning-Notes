
# 一、项目结构与开发约定

## 1.1 分层结构

### 1.1.1 三个核心注解

SSM + Spring Boot 项目的代码按 Controller → Service → Mapper 三层组织，每一层由一个固定注解标记，注解位置错了 Spring 就无法完成依赖注入与对象管理。

| 注解                | 标注位置               | 作用                                                   |
| ----------------- | ------------------ | ---------------------------------------------------- |
| `@RestController` | Controller 响应类 层的上 | 等价于 `@Controller` + `@ResponseBody`，方法返回值自动序列化为 JSON |
| `@Service`        | Service 层**实现类**上  | 标记为业务层组件，参与事务管理                                      |
| `@Mapper`         | Mapper 层**接口**上    | 标记为 MyBatis Mapper 接口，自动生成代理实现类                      |

> [!TIP]
> `@Service` 要标在 `service.impl` 下的**实现类**上，不要标在接口上；`@Mapper` 要标在 `mapper` 包的**接口**上。

```java
@Mapper
public interface DeptMapper {

    @Select("SELECT * FROM dept")
    List<Dept> findAll();
}
```

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

### 1.1.2 统一响应结果 Result

后端所有接口统一返回 `Result` 对象，把「业务状态码 + 提示信息 + 数据」三部分封装在一起，前端只需判断 `code` 即可。

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

## 1.2 路径抽取

### 1.2.1 类级 + 方法级 @RequestMapping 组合

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

### 1.2.2 @RequestMapping 及其派生注解

`@RequestMapping` 用于声明请求路径（`value`）与请求方式（`method`）。实际开发中常用它的四个派生注解，可以省略 `method` 属性，写法更简洁：

| 派生注解             | 等价写法                                                             | 请求方式   |
| ---------------- | ---------------------------------------------------------------- | ------ |
| `@GetMapping`    | `@RequestMapping(value = "/xxx", method = RequestMethod.GET)`    | GET    |
| `@PostMapping`   | `@RequestMapping(value = "/xxx", method = RequestMethod.POST)`   | POST   |
| `@PutMapping`    | `@RequestMapping(value = "/xxx", method = RequestMethod.PUT)`    | PUT    |
| `@DeleteMapping` | `@RequestMapping(value = "/xxx", method = RequestMethod.DELETE)` | DELETE |

---

# 二、参数传递

前端到 Controller 的参数传递共分四类：**简单参数**、**复杂对象**、**路径参数**、**JSON 请求体**。

## 2.1 简单参数

以删除部门为例，前端请求 `DELETE /depts?id=1`，有三种接收方式。

### 2.1.1 方式一：HttpServletRequest 原生对象

直接注入 Servlet 原始对象，从请求参数表里按名取值。类型转换需要自己完成，一般不推荐。

```java
@DeleteMapping("/depts")
public Result delete(HttpServletRequest request) {
    // 从请求参数表中取出字符串，再手动转成 Integer
    Integer id = Integer.parseInt(request.getParameter("id"));
    deptService.deleteById(id);
    return Result.success();
}
```

### 2.1.2 方式二：@RequestParam 注解

`@RequestParam` 显式指定请求参数的名称。当形参名与请求参数名不一致时（或未开启 `-parameters` 编译参数）**必须**使用。

```java
@DeleteMapping("/depts")
public Result delete(@RequestParam("id") Integer id) {
    deptService.deleteById(id);
    return Result.success();
}
```

当形参与请求参数名一致时，`@RequestParam("id")` 中的 `"id"` 也可以省略，写成 `@RequestParam Integer id`。


### 2.1.3 方式三：同名参数自动绑定

当**形参名与请求参数名完全一致**时，可以省略 `@RequestParam`，Spring MVC 会自动完成类型转换与绑定。

```java
@DeleteMapping("/depts")
public Result delete(Integer id) {
    deptService.deleteById(id);
    return Result.success();
}
```

## 2.2 路径参数

### 2.2.1 @PathVariable 接收路径变量

把参数直接写进 URL 路径，用 `/{id}` 占位，再通过 `@PathVariable` 取出。适合资源定位的场景。

```java
// 完整路径：GET /depts/1
@GetMapping("/depts/{id}")                  // 路径中用 {id} 占位
public Result getById(@PathVariable Integer id) {
    Dept dept = deptService.getById(id);
    return Result.success(dept);
}
```

> [!TIP]
> 当形参名与路径变量名不一致时，必须写明名称，例如 `@PathVariable("id") Integer deptId`。

## 2.3 JSON 请求体

### 2.3.1 @RequestBody 接收 JSON

当请求体是 JSON 格式（`Content-Type: application/json`）时，用 `@RequestBody` 把 JSON 反序列化成 Java 对象。

```java
@PostMapping("/depts")
public Result save(@RequestBody Dept dept) {
    deptService.save(dept);
    return Result.success();
}
```

请求示例：

```json
{
  "name": "研发部",
  "createTime": "2024-01-01 10:00:00"
}
```

## 2.4 复杂参数封装

### 2.4.1 用实体类封装多个查询条件

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
    log.info("查询请求参数： {}", empQueryParam);
    PageResult<Emp> pageResult = empService.page(empQueryParam);
    return Result.success(pageResult);
}
```

### 2.4.2 常用参数注解补充

| 注解                                            | 作用                                       |
| --------------------------------------------- | ---------------------------------------- |
| `@RequestParam(defaultValue = "1")`           | 设置请求参数的默认值，请求参数缺失时生效                     |
| `@RequestParam(required = false)`             | 参数非必填，`null` 也能通过校验                      |
| `@RequestBody`                                | 接收 JSON 请求体                              |
| `@PathVariable`                               | 接收 URL 路径变量                              |
| `@DateTimeFormat(pattern = "yyyy-MM-dd")`     | Spring MVC 接收前端提交的字符串日期，自动转为 `LocalDate` |

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

MyBatis 支持用注解直接在接口方法上编写 SQL，无需 XML。四种注解与 SQL 类型的对应关系如下：

| 注解 | 对应 SQL | 用途 |
| --- | --- | --- |
| `@Select` | `SELECT` | 查询 |
| `@Insert` | `INSERT` | 新增 |
| `@Update` | `UPDATE` | 修改 |
| `@Delete` | `DELETE` | 删除 |

```java
@Mapper
public interface DeptMapper {

    /** 查询所有部门 */
    @Select("SELECT * FROM dept")
    List<Dept> findAll();

    /** 根据 ID 查询部门 */
    @Select("SELECT * FROM dept WHERE id = #{id}")
    Dept getById(Integer id);

    /** 保存部门 */
    @Insert("INSERT INTO dept (name, create_time, update_time) VALUES (#{name}, #{createTime}, #{updateTime})")
    void save(Dept dept);

    /** 更新部门 */
    @Update("UPDATE dept SET name = #{name}, update_time = #{updateTime} WHERE id = #{id}")
    void update(Dept dept);

    /** 根据 ID 删除部门 */
    @Delete("DELETE FROM dept WHERE id = #{id}")
    void deleteById(Integer id);
}
```

> [!WARNING]
> 注解名必须与 SQL 类型严格对应。写 `@Select("DELETE ...")` 会在运行时报 `BadSqlGrammarException`。

## 3.2 参数与返回值

### 3.2.1 单个或多个简单类型参数

Mapper 方法的形参是单个普通类型时，`#{…}` 里的属性名可以任意写（`#{id}`、`#{value}` 都可以），MyBatis 会把唯一的那一个实参传进去。

```java
/** 根据 ID 删除部门 */
@Delete("DELETE FROM dept WHERE id = #{id}")
void deleteById(Integer id);
```

### 3.2.2 对象类型参数

需要传多个参数时，把它们封装到一个对象中。此时 `#{…}` 里写的是**对象的属性名**，注意是 Java 属性名，不是数据库表的字段名。

```java
@Mapper
public interface EmpMapper {

    /** 保存员工，同时返回自增主键 */
    @Insert("INSERT INTO emp (name, gender, entry_date) VALUES (#{name}, #{gender}, #{entryDate})")
    void save(Emp emp);
}
```

上例中 `#{name}`、`#{gender}`、`#{entryDate}` 取的都是 `Emp` 对象的属性；注意 `entryDate` 对应表字段 `entry_date`，靠的是驼峰命名映射。

## 3.3 结果映射

**核心问题**：实体类属性名与数据库查询返回的字段名不一致时，MyBatis 无法自动封装。字段名与属性名**符合驼峰命名规则**时（`create_time` ↔ `createTime`），MyBatis 会自动完成映射。三种解决方式如下。

### 3.3.1 方式一：全局配置开启驼峰映射（推荐）

在 `application.yml` 中开启，一行配置全局生效，无需改动任何 SQL。

```yaml
mybatis:
  configuration:
    # 开启下划线命名到驼峰命名的自动映射，默认 false
    map-underscore-to-camel-case: true
```

开启后，SQL 可以直接写 `SELECT *`：

```java
@Select("SELECT * FROM dept")
List<Dept> findAll();
```

### 3.3.2 方式二：SQL 中使用 AS 起别名

在 SQL 里为不一致的列名起别名，别名与实体类属性名保持一致。

```java
@Select("SELECT id, name, create_time AS createTime, update_time AS updateTime FROM dept")
List<Dept> findAll();
```

### 3.3.3 方式三：@Results + @Result 手动映射

在方法上用注解声明「属性名 ↔ 列名」的对应关系，适合列名与属性名毫无规律的情况。

```java
@Results({
    @Result(property = "createTime", column = "create_time"),
    @Result(property = "updateTime", column = "update_time")
})
@Select("SELECT * FROM dept")
List<Dept> findAll();
```

> [!TIP]
> 三种方式可以叠加使用：全局驼峰映射兜底，个别特殊列再用 `@Results` 补充。

## 3.4 动态 SQL

**动态 SQL**，就是随用户输入或外部条件变化而变化的 SQL 语句。项目中用 XML 映射文件编写。

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
        SELECT e.*, d.name AS deptName
        FROM emp AS e
        LEFT JOIN dept AS d ON e.dept_id = d.id
        <where>
            <!-- 姓名：模糊查询，注意用 concat 拼接 % -->
            <if test="name != null and name != ''">
                AND e.name LIKE CONCAT('%', #{name}, '%')
            </if>
            <!-- 性别：精确查询 -->
            <if test="gender != null">
                AND e.gender = #{gender}
            </if>
            <!-- 入职时间：区间查询 -->
            <if test="begin != null and end != null">
                AND e.entry_date BETWEEN #{begin} AND #{end}
            </if>
        </where>
        ORDER BY e.id DESC
    </select>

</mapper>
```

> [!TIP]
> `<where>` 已经去掉了多余的 `and`，但仍建议**显式书写 `AND`**，这样 XML 可读性更好、复制片段时也不会出错。

### 3.4.2 foreach 遍历集合

`<foreach>` 用于遍历集合，常用于 `IN (...)` 查询或批量插入。

| 属性 | 说明 |
| --- | --- |
| `collection` | 集合名称 |
| `item` | 集合遍历出来的元素 / 项 |
| `separator` | 每次遍历使用的分隔符 |
| `open` | 遍历开始前拼接的片段 |
| `index` | 遍历的下标 |

这些属性都是**可选的**，按实际需求指定即可。

```xml
<select id="listByIds" resultType="com.itheima.pojo.Emp">
    SELECT * FROM emp
    <where>
        <foreach collection="ids" item="id" open="AND id IN (" separator="," close=")">
            #{id}
        </foreach>
    </where>
</select>
```

当 `ids = [1, 2, 3]` 时，拼接结果为：`SELECT * FROM emp WHERE id IN (1,2,3)`。

## 3.5 主键回填

### 3.5.1 @Options 主键返回

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
    @Insert("INSERT INTO emp (name, gender, entry_date) VALUES (#{name}, #{gender}, #{entryDate})")
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
    List<EmpExpr> exprs = emp.getExprs();
    if (exprs != null && !exprs.isEmpty()) {
        exprs.forEach(expr -> expr.setEmpId(emp.getId()));
        empExprMapper.saveBatch(exprs);
    }
}
```

---

# 四、日志

## 4.1 常见日志框架

### 4.1.1 JUL / SLF4J / Log4j / Logback

| 名称          | 说明                                                                           |
| ----------- | ---------------------------------------------------------------------------- |
| **JUL**     | Java SE 平台自带的官方日志框架。配置简单，但不灵活，性能较差                                           |
| **SLF4J**   | Simple Logging Facade for Java，**日志门面**。只提供一套标准的日志接口与抽象类，不做具体实现，允许应用随时切换底层框架 |
| **Log4j**   | Apache 的流行日志框架，配置灵活，支持多种输出目标。需注意 Log4j 1.x 存在严重漏洞，实际使用应选 Log4j 2             |
| **Logback** | 由 Log4j 原作者开发，是 **SLF4J 的参考实现**，性能优于 Log4j，配置更丰富，是 Spring Boot 默认集成的日志实现     |

SLF4J 本身没有实现，它是「门面」；真正干活的是绑定到它的实现（Logback / Log4j2 / JUL）。调用方只依赖 SLF4J API，底层换实现不需要改业务代码。

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

## 4.3 Logback 配置文件

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

### 4.3.2 输出到控制台

常用的两种日志输出位置是**控制台**和**系统文件**。需要输出到控制台时配置如下：

```xml
<appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
    <encoder class="ch.qos.logback.classic.encoder.PatternLayoutEncoder">
        <pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{50}-%msg%n</pattern>
    </encoder>
</appender>
```

### 4.3.3 输出到文件（按天 + 按大小滚动）

需要输出到系统文件时配置 `RollingFileAppender`，日志文件会按天和大小自动滚动归档：

```xml
<appender name="FILE" class="ch.qos.logback.core.rolling.RollingFileAppender">
    <rollingPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedRollingPolicy">
        <!-- 归档文件名，%d 日期，%i 序号 -->
        <fileNamePattern>D:/logs/tlias-%d{yyyy-MM-dd}-%i.log</fileNamePattern>
        <!-- 最多保留 30 天的历史日志 -->
        <maxHistory>30</maxHistory>
        <!-- 单个文件最大 10MB，超过则滚动到新文件 -->
        <maxFileSize>10MB</maxFileSize>
    </rollingPolicy>

    <encoder class="ch.qos.logback.classic.encoder.PatternLayoutEncoder">
        <pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{50}-%msg%n</pattern>
    </encoder>
</appender>
```

### 4.3.4 日志开关与日志级别

`<root level="...">` 就是日志总开关：`ALL` 表示全部开启，`OFF` 表示全部关闭。把多个 appender 通过 `appender-ref` 挂到 root 上，即可同时输出到控制台和文件。

```xml
<!-- 开启全部日志：输出到控制台 + 输出到文件 -->
<root level="ALL">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
</root>
```

```xml
<!-- 关闭全部日志：把级别改为 OFF -->
<root level="OFF">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
</root>
```

日志级别指日志信息的类型，**只有大于等于所配置级别的日志才会被输出**。

| 日志级别    | 说明                                    | 记录方式               |
| ------- | ------------------------------------- | ------------------ |
| `trace` | 追踪，记录程序运行轨迹 【使用很少】                    | `log.trace("...")` |
| `debug` | 调试，记录程序调试过程中的信息，实际应用中一般视为最低级别 【使用较多】  | `log.debug("...")` |
| `info`  | 记录一般信息，描述程序运行的关键事件，如网络连接、IO 操作 【使用较多】 | `log.info("...")`  |
| `warn`  | 警告信息，记录潜在有害的情况 【使用较多】                 | `log.warn("...")`  |
| `error` | 错误信息 【使用较多】                           | `log.error("...")` |

优先级由低到高：`trace < debug < info < warn < error < off`。把级别配成 `info`，则 `trace` 和 `debug` 都不会输出。

```xml
<!-- 只输出 info 及以上级别 -->
<root level="INFO">
    <appender-ref ref="STDOUT"/>
    <appender-ref ref="FILE"/>
</root>
```

---

# 五、分页

## 5.1 分页需求分析

### 5.1.1 前后端约定

**前端请求服务端时传递两个参数：**

1. 当前页码 `page`
2. 每页显示条数 `pageSize`

**后端需要响应给前端两组数据：**

1. 所查询到的数据列表（存储到 `List` 集合中）
2. 总记录数

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

## 5.2 手动分页

### 5.2.1 原始方式

以 `GET /emps?page=1&pageSize=10` 为例，Service 层需要**额外查询一次总记录数**，且「算偏移量 → 查总数 → 查数据 → 封装」的流程比较固定，代码冗余。

```java
@Mapper
public interface EmpMapper {

    /** 统计总记录数 */
    @Select("SELECT COUNT(*) FROM emp e LEFT JOIN dept d ON e.dept_id = d.id")
    Long count();

    /** 查询分页数据，LIMIT 第一个参数是偏移量，第二个参数是条数 */
    @Select("SELECT e.*, d.name AS deptName FROM emp e LEFT JOIN dept d ON e.dept_id = d.id LIMIT #{offset}, #{pageSize}")
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

## 5.3 PageHelper

**PageHelper 是第三方为 MyBatis 提供的功能强大、方便易用的分页插件，支持任何形式的单表、多表分页查询。**

使用后 Mapper 中只需写正常的列表查询，**无需再手动分页**。Service 层在调用 Mapper **之前**设置分页参数，调用**之后**解析分页结果。

### 5.3.1 使用步骤

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
    SELECT e.*, d.name AS deptName
    FROM emp AS e
    LEFT JOIN dept AS d ON e.dept_id = d.id
    ORDER BY e.id DESC
</select>
```

### 5.3.2 内部执行的两条 SQL

PageHelper 分页时会执行两条 SQL，这两条 SQL 都是从 Mapper 中我们编写的 SQL **演变**而来的：

- **第一条**：查询总记录数。把原 SQL 的查询字段列表替换成 `count(0)` 进行统计。
- **第二条**：分页查询指定页码的数据。在原 SQL 之后拼接 `limit`；由于测试时查的是第一页、起始索引为 0，所以简写为 `limit ?`。

PageHelper 把总记录数与数据列表一起封装到 `Page<Emp>` 对象中，我们拿到查询结果后直接调用它的方法即可。

### 5.3.3 注意事项与合理化

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

---

# 六、事务

## 6.1 事务基础

### 6.1.1 概念与三步操作

**事务**是一组操作的集合，是一个不可分割的工作单位。事务会把所有操作作为一个整体，一起向系统提交或撤销操作请求，即这些操作**要么同时成功，要么同时失败**。

事务控制主要三步：

1. 在这组操作执行**之前**，先开启事务（`START TRANSACTION;` 或 `BEGIN;`）。
2. 所有操作**全部成功**，则提交事务（`COMMIT;`）。
3. 只要有**任何一个操作失败**，就回滚事务（`ROLLBACK;`）。

```sql
-- 1. 开启事务
BEGIN;

-- 2. 保存员工基本信息
INSERT INTO emp (id, name, username, gender, phone, job, salary, entry_date)
VALUES (39, 'Tom', '123456', 1, '13300001111', 1, 4000, '2023-11-01');

-- 3. 保存该员工的工作经历信息（两条）
INSERT INTO emp_expr (emp_id, `begin`, `end`, company, job)
VALUES (39, '2019-01-01', '2020-01-01', '百度', '开发'),
       (39, '2020-01-10', '2022-02-01', '阿里', '架构');

-- 4. 以上全部成功则提交，数据正式生效
COMMIT;

-- 5. 若第 2 或 3 步失败，则执行回滚，数据恢复到事务开始前
ROLLBACK;
```

> [!TIP]
> `begin`、`end` 在 MySQL 中是关键字，作列名时要用反引号包裹。

## 6.2 Spring 事务 @Transactional

### 6.2.1 注解位置与作用

`@Transactional` 的语义是：**方法执行前开启事务，方法执行完毕提交事务；执行过程中出现异常则回滚事务。**

标注位置有三个层次：

| 位置  | 效果                               |
| --- | -------------------------------- |
| 方法上 | 仅当前方法交由 Spring 进行事务管理            |
| 类上  | 当前类中**所有**方法都交由 Spring 进行事务管理    |
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
        List<EmpExpr> exprs = emp.getExprs();
        if (exprs != null && !exprs.isEmpty()) {
            exprs.forEach(expr -> expr.setEmpId(emp.getId()));
            empExprMapper.saveBatch(exprs);
        }
    }
}
```

> [!WARNING]
> `@Transactional` 常见失效场景：① 方法不是 `public`；② 类未交给 Spring 管理（没加 `@Service` 等注解）；③ 异常被方法内部 `try-catch` 吞掉；④ 抛的是 `Error` 而非 `Exception`；⑤ 在同类内部方法间自调用，绕过代理对象。

### 6.2.2 事务日志

在 `application.yml` 中开启事务管理日志，就能在控制台看到与事务相关的日志信息（开启、提交、回滚）。

```yaml
logging:
  level:
    org.springframework.jdbc.support.JdbcTransactionManager: debug
```

### 6.2.3 回滚规则：rollbackFor

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

### 6.2.4 传播行为：propagation

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

实际开发中**只需重点关注两个**：

- **`REQUIRED`**：大部分场景用默认的即可。
- **`REQUIRES_NEW`**：当不希望事务之间相互影响时使用。例如「下单前记录日志」，无论订单保存成功与否，都必须保证日志记录成功。

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
        logService.record(order);
    }
}
```

```java
@Service
public class LogServiceImpl implements LogService {

    @Override
    @Transactional(propagation = Propagation.REQUIRES_NEW)   // 总是开启新事务
    public void record(Order order) {
        logMapper.insert(order);
    }
}
```



七

1.事务四大特性
2.文件上传
上传文件的原始 form 表单，要求表单必须具备以下三点（上传文件页面三要素）：

- 表单必须有 file 域，用于选择要上传的文件
    
- 表单提交方式必须为 POST：通常上传的文件会比较大，所以需要使用 POST 提交方式
    
- 表单的编码类型 enctype 必须要设置为：multipart/form-data：普通默认的编码格式是不适合传输大型的二进制数据的，所以在文件上传时，表单的编码格式必须设置为 multipart/form-data
2.1 本地存储
请加上一些解释
```java
package com.ljxstar.controller;  
  
import com.ljxstar.pojo.Result;  
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
        log.info("上传文件: {}", file.getOriginalFilename());  
  
        if (!file.isEmpty()) {  
            try {  
                // 获取文件原始名称和扩展名  
                String originalFilename = file.getOriginalFilename();  
                String fileExtension = originalFilename.substring(originalFilename.lastIndexOf("."));  
                String newFileName = UUID.randomUUID().toString() + fileExtension;  
  
                // 创建上传目录  
                File newFile = new File(UPLOAD_DIR + newFileName);  
  
                if (!newFile.getParentFile().exists()) {  
                    newFile.getParentFile().mkdirs();  
                }  
  
                // 保存文件到指定目录  
                file.transferTo(newFile);  
                log.info("文件上传成功: {}", newFile.getAbsolutePath());  
  
            } catch (Exception e) {  
                log.error("文件上传失败", e);  
                return Result.error("文件上传失败");  
            }  
        }  
        return Result.success();  
    }  
}
```

**MultipartFile 常见方法：**

- `String getOriginalFilename();` //获取原始文件名
    
- `void transferTo(File dest);` //将接收的文件转存到磁盘文件中
    
- `long getSize();` //获取文件的大小，单位：字节
    
- `byte[] getBytes();` //获取文件内容的字节数组
    
- `InputStream getInputStream();` //获取接收到的文件内容的输入流

上传一个较大的文件(超出 1 M)时发现，后端程序报错：

```yml
spring：
  servlet:
    multipart:
      max-file-size: 10MB
      max-request-size: 100MB
```


2.2 阿里云 OSS
2.2.1 准备
开通 OSS 云服务 -》创建 Bucket -》不要开通公共访问、选择公共读 （解释下为什么这样）
2.2.2 简单入门
1.安装 sdk
在 `pom.xml` 添加如下依赖，并将 `<version>` 替换为在 [Maven Repository](https://mvnrepository.com/artifact/com.aliyun/alibabacloud-oss-v2) 查询到的最新版本号：
```xml
<dependency>
    <groupId>com.aliyun</groupId>
    <artifactId>alibabacloud-oss-v2</artifactId>
    <version><!-- 填写最新版本号--></version>
</dependency>
```

### **配置访问凭证**
**创建 AccessKey** -》 **配置 AccessKey**
将 RAM 用户的 AccessKey 写入环境变量作为凭证。

在 [RAM 控制台](https://ram.console.aliyun.com/users/create)，创建**使用永久 AccessKey 访问**的 RAM 用户，保存 AccessKey，然后为该用户授予 `AliyunOSSFullAccess` 权限。
```SQL
set OSS_ACCESS_KEY_ID=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
set OSS_ACCESS_KEY_SECRET=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```
```Shell
setx OSS_ACCESS_KEY_ID "%OSS_ACCESS_KEY_ID%"
setx OSS_ACCESS_KEY_SECRET "%OSS_ACCESS_KEY_SECRET%"
```
```Shell
echo %OSS_ACCESS_KEY_ID%
echo %OSS_ACCESS_KEY_SECRET%
```
