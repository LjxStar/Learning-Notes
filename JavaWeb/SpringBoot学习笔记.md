
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