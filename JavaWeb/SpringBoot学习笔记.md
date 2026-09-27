
1.mybatis 到 mapper（对象） 驼峰与下划线
实体类属性名和数据库表查询返回的字段名一致，mybatis 会自动封装。如果实体类属性名和数据库表查询返回的字段名不一致，不能自动封装。

```java
@Mapper  
public interface DeptMapper {  
  
    /**  
     * 查询所有部门  
     */  
  
    // 方式1 如果使用了驼峰命名法，则需要在SQL中使用AS关键字进行别名映射  
    // @Select("SELECT id, name, create_time AS createTime, update_time AS updateTime FROM dept")  
  
    /*  
    方式2 通过 @Results 及 @Result 进行手动结果映射。  
    @Results({            
	    @Result(property = "createTime", column = "create_time"),            
	    @Result(property = "updateTime", column = "update_time")    
    })    
    @Select("SELECT * FROM dept")     */  
    
    // 方式3 通过全局配置开启驼峰命名法映射  
    @Select("SELECT * FROM dept")  
    List<Dept> findAll();  
}
```
1.在 SQL 语句中，对不一样的列名起别名，别名和实体类属性名一样。
2.在 DeptMapper 接口方法上，通过 @Results 及@Result 进行手动结果映射。
3.如果字段名与属性名符合驼峰命名规则，mybatis 会自动通过驼峰命名规则映射。
```yml
mybatis:
  configuration:
    map-underscore-to-camel-case: true
```


2.RequestMapping
@RequestMapping(value = "/depts", method = RequestMethod._GET_)
- GET 方式：@GetMapping
- POST 方式：@PostMapping
- PUT 方式：@PutMapping
- DELETE 方式：@DeleteMapping

3.mapper （参数为单个或多个变量）到 mybatis （一般与参数名相同）
```java
/**  
 * 根据ID删除部门  
 */  
@Select("DELETE FROM dept WHERE id = #{id}")  
void deleteById(Integer id);
```
如果 mapper 接口方法形参只有一个普通类型的参数，`#{…}` 里面的属性名可以随便写，如：`#{id}`、`#{value}`。

对于 DML 语句来说，执行完毕，也是有返回值的，返回值代表的是增删改操作，影响的记录数，所以可以将执行 DML 语句的方法返回值设置为 Integer。但是一般开发时，是不需要这个返回值的，所以也可以设置为 void。


4.前端（url 简单参数？）到 controller（单个或多个变量） 简单传参
```java
/**  
 * 删除部门  
 * 简单传参  
 */  
@DeleteMapping("/depts")  
  
// 方式1 通过@RequestParam注解获取请求参数  
/*  
public Result delete(@RequestParam("id") Integer id)  
public Result delete(@RequestParam(value = "id") Integer id) {  
    // 删除部门逻辑  
    return Result.success();}  
 */  
  
// 方式2 通过原始的 HttpServletRequest 对象获取请求参数  
/*  
public Result delete(HttpServletRequest request) {  
    String idStr = request.getParameter("id");    int id = Integer.parseInt(idStr);    // 删除部门逻辑  
    return Result.success();}  
 */  
// 方式3 同名参数自动绑定  
public Result delete(Integer id) {  
    deptService.deleteById(id);  
    return Result.success();  
}
```

5. mapper（参数为对象）到 mybatis（对象属性名）
如果在 mapper 接口中，需要传递多个参数，可以把多个参数封装到一个对象中。在 SQL 语句中获取参数的时候，`#{...}` 里面写的是对象的属性名【注意是属性名，不是表的字段名】。

6. 前端（json）到 controller（对象） （json、请求体传参）
```java
/**  
 * 添加部门  
 * json传参  
 */  
@PostMapping("/depts")  
public Result save(@RequestBody Dept dept) {  
    deptService.save(dept);  
    return Result.success();  
}
```

7.前端（url 路径参数/a或者/a/b） 到 controller （单个或多个变量）  路径传参
```java
/**  
 * 根据ID查询部门  
 * 路径传参  
 */  
@GetMapping("/depts/{id}")  
public Result getById(@PathVariable Integer id) {  
    Dept dept = deptService.getById(id);  
    return Result.success(dept);  
}
```

8. mapper 的四种注释
```java
/**  
 * 查询所有部门  
 */   
@Select("SELECT * FROM dept")  
List<Dept> findAll();  
  
/**  
 * 根据ID删除部门  
 */  
@Select("DELETE FROM dept WHERE id = #{id}")  
void deleteById(Integer id);  
  
/**  
 * 保存部门  
 */  
@Insert("INSERT INTO dept (name, create_time, update_time) VALUES (#{name}, #{createTime}, #{updateTime})")  
void save(Dept dept);  
  
/**  
 * 根据ID查询部门  
 */  
@Select("SELECT * FROM dept WHERE id = #{id}")  
Dept getById(Integer id);  
  
/**  
 * 更新部门  
 */  
@Update("UPDATE dept SET name = #{name}, update_time = #{updateTime} WHERE id = #{id}")  
void update(Dept dept);
```


9. 日志技术
- **JUL****：**这是 JavaSE 平台提供的官方日志框架，也被称为 JUL。配置相对简单，但不够灵活，性能较差。
    
- **Slf 4 j****：**（Simple Logging Facade for Java）简单日志门面，提供了一套日志操作的标准接口及抽象类，允许应用程序使用不同的底层日志框架。
- **Log 4 j****：**一个流行的日志框架，提供了灵活的配置选项，支持多种输出目标。
    
- **Logback：**基于 Log 4 j 升级而来，提供了更多的功能和配置选项，性能由于 Log 4 j。

9.1
Logback 入门

最简单的 logback.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
    <!-- 控制台输出 -->
    <appender name="CONSOLE" class="ch.qos.logback.core.ConsoleAppender">
        <encoder>
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
放入 src/main/resources

```java
import org.junit.jupiter.api.Test;  
import org.slf4j.Logger;  
import org.slf4j.LoggerFactory;  
  
public class LogTest {  
    private static final Logger log = LoggerFactory.getLogger(LogTest.class);  
  
    @Test  
    public void testLog(){  
        log.debug("开始计算...");  
        int sum = 0;  
        int[] nums = {1, 5, 3, 2, 1, 4, 5, 4, 6, 7, 4, 34, 2, 23};  
        for (int i = 0; i < nums.length; i++) {  
            sum += nums[i];  
        }  
        log.info("计算结果为: "+sum);  
        log.debug("结束计算...");  
    }  
  
}
```
简单测试在
private static final Logger log = LoggerFactory.getLogger(LogTest.class);固定搭配
import org.slf 4 j.Logger;  
import org.slf 4 j.LoggerFactory;  
两个都是引入 slf 4 j

----

lombok 中提供的@Slf 4 j 注解，可以简化定义日志记录器这步操作。添加了该注解，就相当于在类中定义了日志记录器，就下面这句代码：

`private static Logger log = LoggerFactory. getLogger(Xxx. class);`

9.2 Logback 配置文件
Logback 日志框架的配置文件叫 `logback.xml` 。

该配置文件是对 Logback 日志框架输出的日志进行控制的，可以来配置输出的格式、位置及日志开关等。

常用的两种输出日志的位置：控制台、系统文件。

  

**1). 如果需要输出日志到控制台。添加如下配置：**

```XML
<!-- 控制台输出 -->
<appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
    <encoder class="ch.qos.logback.classic.encoder.PatternLayoutEncoder">
            <!--格式化输出：%d 表示日期，%thread 表示线程名，%-5level表示级别从左显示5个字符宽度，%msg表示日志消息，%n表示换行符 -->
            <pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{50}-%msg%n</pattern>
    </encoder>
</appender>
```

  

**2). 如果需要输出日志到文件。添加如下配置：**

```XML
<!-- 按照每天生成日志文件 -->
<appender name="FILE" class="ch.qos.logback.core.rolling.RollingFileAppender">
    <rollingPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedRollingPolicy">
        <!-- 日志文件输出的文件名, %i表示序号 -->
        <FileNamePattern>D:/tlias-%d{yyyy-MM-dd}-%i.log</FileNamePattern>
        <!-- 最多保留的历史日志文件数量 -->
        <MaxHistory>30</MaxHistory>
        <!-- 最大文件大小，超过这个大小会触发滚动到新文件，默认为 10MB -->
        <maxFileSize>10MB</maxFileSize>
    </rollingPolicy>

    <encoder class="ch.qos.logback.classic.encoder.PatternLayoutEncoder">
        <!--格式化输出：%d 表示日期，%thread 表示线程名，%-5level表示级别从左显示5个字符宽度，%msg表示日志消息，%n表示换行符 -->
        <pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{50}-%msg%n</pattern>
    </encoder>
</appender>
```

  

**3). 日志开关配置 （开启日志（****ALL****），取消日志（OFF））**

```XML
<!-- 日志输出级别 -->
<root level="ALL">
    <!--输出到控制台-->
    <appender-ref ref="STDOUT" />
    <!--输出到文件-->
    <appender-ref ref="FILE" />
</root>
```


9.2 日志级别
日志级别指的是日志信息的类型，日志都会分级别，常见的日志级别如下（优先级由低到高）：

|       |                                        |                  |
| ----- | -------------------------------------- | ---------------- |
| 日志级别  | 说明                                     | 记录方式             |
| trace | 追踪，记录程序运行轨迹 【使用很少】                     | log.trace("...") |
| debug | 调试，记录程序调试过程中的信息，实际应用中一般将其视为最低级别 【使用较多】 | log.debug("...") |
| info  | 记录一般信息，描述程序运行的关键事件，如：网络连接、io 操作 【使用较多】 | log.info("...")  |
| warn  | 警告信息，记录潜在有害的情况 【使用较多】                  | log.warn("...")  |
| error | 错误信息 【使用较多】                            | log.error("...") |

可以在配置文件 `logback.xml` 中，灵活的控制输出那些类型的日志。（大于等于配置的日志级别的日志才会输出）

```XML
<!-- 日志输出级别 -->
<root level="info">
    <!--输出到控制台-->
    <appender-ref ref="STDOUT" />
    <!--输出到文件-->
    <appender-ref ref="FILE" />
</root>
```

9.3 传参
有几个大括号，逗号后面就要传几个对应参数
```java
log.info("根据 id 删除部门, id: {}" , id);
```

10. 路径抽取
一个完整的请求路径，应该是类上的 @RequestMapping 的 value 属性 + 方法上的 @RequestMapping 的 value 属性。
把公共的路径都放到类的@RequestMapping 上避免重复


11. 三种注释
Controller（RestController） controller 上的类
service 放到 service.impl 下的对应实现类上
mapper （resposity）放到 mapper 包的对应接口


12.分页
1. 前端在请求服务端时，传递的参数
    
    1. 当前页码 page
        
    2. 每页显示条数 pageSize
        
2. 后端需要响应什么数据给前端
    
    1. 所查询到的数据列表（存储到 List 集合中）
        
    2. 总记录数


```Java
@Data
@NoArgsConstructor
@AllArgsConstructor
public class PageResult {
        private Long total; //总记录数
        private List rows; //当前页数据列表
}
```

12.1 原始方式



对于/emps?page=1&pageSize=10
service 层需要额外获取总计数，不会自动分页参数这些过程都相对固定
```java
@Override  
public PageResult<Emp> list(Integer page, Integer pageSize) {  
    // 查询总记录数  
    Long total = empMapper.count();  
  
    // 计算分页参数  
    int offset = (page - 1) * pageSize;  
  
    // 查询分页数据  
    List<Emp> data = empMapper.list(offset, pageSize);  
  
    // 返回分页结果  
    return new PageResult<Emp>(total, data);  
}
```

```java
@Select("SELECT COUNT(*) FROM emp left join dept on emp.dept_id = dept.id")  
Long count();  
  
@Select("SELECT emp.*, dept.name As dept_name FROM emp left join dept on emp.dept_id = dept.id LIMIT #{offset}, #{pageSize}")  
List<Emp> list(int offset, Integer pageSize);
```


12.2 PageHelper
**PageHelper 是第三方提供的 Mybatis 框架中的一款功能强大、方便易用的分页插件，支持任何形式的单标、多表的分页查询。**


当使用了 PageHelper 分页插件进行分页，就无需再 Mapper 中进行手动分页了。在 Mapper 中我们只需要进行正常的列表查询即可。在 Service 层中，调用 Mapper 的方法之前设置分页参数，在调用 Mapper 方法执行查询之后，解析分页结果，并将结果封装到 PageResult 对象中返回。


```java
// 使用 PageHelper 分页插件方式  
@Override  
public PageResult<Emp> list(Integer page, Integer pageSize) {  
    // 设置分页参数  
    PageHelper.startPage(page, pageSize);  
  
    // 查询员工列表  
    List<Emp> empList = empMapper.list();  
    Page<Emp> empPage = (Page<Emp>) empList;  
  
    // 返回分页结果  
    return new PageResult<Emp>(empPage.getTotal(), empPage.getResult());  
}
```

行了两条 SQL 语句，而这两条 SQL 语句，其实是从我们在 Mapper 接口中定义的 SQL 演变而来的。

- 第一条 SQL 语句，用来查询总记录数。其实就是将我们编写的SQL语句进行的改造增强，将查询返回的字段列表替换成了 `count(0)` 来统计总记录数。
- 第二条SQL语句，用来进行分页查询，查询指定页码对应的数据列表。其实就是将我们编写的SQL语句进行的改造增强，在SQL语句之后拼接上了limit进行分页查询，而由于测试时查询的是第一页，起始索引是0，所以简写为limit ？。

而 PageHelper 在进行分页查询时，会执行上述两条 SQL 语句，并将查询到的总记录数，与数据列表封装到了 `Page<Emp>` 对象中，我们再获取查询结果时，只需要调用 Page 对象的方法就可以获取。

> [!TIP]
> - PageHelper 实现分页查询时，SQL 语句的结尾一定一定一定不要加分号(;).。
>     
> - PageHelper 只会对紧跟在其后的第一条 SQL 语句进行分页处理。

当我们在测试的时候，页码输入负数，查询是有问题的，查不到对应的数据了。
```Java
pagehelper:
  reasonable: true
  helper-dialect: mysql
```
reasonable：分页合理化参数，默认值为 false。当该参数设置为 true 时，pageNum<=0时会查询第一页，pageNum>pages（超过总数时），会查询最后一页。默认 false 时，直接根据参数进行查询。

13. 前端到 controller 的一些细节
@RequestParam(defaultValue="默认值") //设置请求参数默认值

@DateTimeFormat(pattern = "yyyy-MM-dd") LocalDate begin // Spring MVC 接收前端提交的字符串日期，自动转为 `LocalDate` 对象

分页条件查询中，请求参数比较多
定义一个实体类，来封装这几个请求参数。 **【需要保证，****前端传递的请求参数和实体类的属性名是一样的****】
```java

package com.itheima.pojo;

import lombok.Data;
import org.springframework.format.annotation.DateTimeFormat;
import java.time.LocalDate;

@Data
public class EmpQueryParam {
    
    private Integer page = 1; //页码
    private Integer pageSize = 10; //每页展示记录数
    private String name; //姓名
    private Integer gender; //性别
    @DateTimeFormat(pattern = "yyyy-MM-dd")
    private LocalDate begin; //入职开始时间
    @DateTimeFormat(pattern = "yyyy-MM-dd")
    private LocalDate end; //入职结束时间
    
}
```

```Java
@GetMapping
public Result page(EmpQueryParam empQueryParam) {
    log.info("查询请求参数： {}", empQueryParam);
    PageResult pageResult = empService.page(empQueryParam);
    return Result.success(pageResult);
}
```

