
1.mapper 到 mybatis 绑定
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

3.mapper 到 mybatis
```
/**  
 * 根据ID删除部门  
 */  
@Select("DELETE FROM dept WHERE id = #{id}")  
void deleteById(Integer id);
```
如果 mapper 接口方法形参只有一个普通类型的参数，`#{…}` 里面的属性名可以随便写，如：`#{id}`、`#{value}`。

对于 DML 语句来说，执行完毕，也是有返回值的，返回值代表的是增删改操作，影响的记录数，所以可以将执行 DML 语句的方法返回值设置为 Integer。但是一般开发时，是不需要这个返回值的，所以也可以设置为 void。