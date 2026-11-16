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

![[Pasted image 20261116100808.png]]
「新建 SpringBoot 项目所需依赖」
**② 创建 Controller**

```java
@RestController  
public class Hello {  
  
    @RequestMapping("/hello")  
    public String sayHello(@RequestParam(value = "name", defaultValue = "Tom") String name) {  
        return "Hello, " + name + "!";  
    }  
}
```

**③ 访问接口**

引入 `Spring Security` 依赖后再访问之前的接口，会自动跳转到默认登录页面：默认用户名是 `user`，密码会输出在控制台。**必须登录之后才能访问接口**。

![[Pasted image 20261115205330.png]]
「自动生成临时密码」
> **小结**：一行依赖就带来了整套过滤器链，所有请求默认被拦截。后续所有改造，本质上都是「替换链条中的某些环节」：把默认登录页换成自定义登录接口、把默认用户源换成数据库、把 Session 换成 JWT + Redis、把默认错误响应换成统一 JSON。

## 2. 认证

### 2.1 登录校验流程

![登录校验流程](file:///D:/workbuddy_repo/spaces/2026_11_12_21-54-24/imgs/2909814-20220815171052398-884692008.png?lastModify=1794747288)
「登录校验流程」
### 2.2 原理初探

要实现自己的登录流程，必须先弄清入门案例中 Spring Security 的执行流程。

#### 2.2.1 过滤器链

Spring Security 的原理本质上是**一条过滤器链**，链中包含提供各种功能的过滤器。下图是入门案例中的核心过滤器：

![Spring Security 过滤器链](file:///D:/workbuddy_repo/spaces/2026_11_12_21-54-24/imgs/2909814-20220815171119043-1855237395.png?lastModify=1794747288)
「过滤器链」
图中只展示了核心过滤器，其它非核心过滤器未在图中展示。三个最关键的过滤器：

|过滤器|职责|
|---|---|
| `UsernamePasswordAuthenticationFilter` |处理在登录页面填写用户名密码后的登录请求；入门案例的认证工作主要由它负责|
| `ExceptionTranslationFilter` |处理过滤器链中抛出的任何 `AccessDeniedException` 和 `AuthenticationException` |
| `FilterSecurityInterceptor` |负责权限校验的过滤器|

可以通过 Debug 查看当前系统中 Spring Security 过滤器链包含哪些过滤器及其顺序：

![Debug 查看过滤器链](file:///D:/workbuddy_repo/spaces/2026_11_12_21-54-24/imgs/2909814-20220815171219110-2115496418.png?lastModify=1794747288)
「debug 下的过滤器链」
#### 2.2.2 认证流程详解

![认证流程详解 1](file:///D:/workbuddy_repo/spaces/2026_11_12_21-54-24/imgs/2909814-20220815171228152-1813787594.png?lastModify=1794747288)
「认证流程」
#### 2.2.3 关键接口速查

| 接口                      | 作用                                                                                                |
| ----------------------- | ------------------------------------------------------------------------------------------------- |
| `Authentication`        | 其实现类表示当前访问系统的用户，封装了用户相关信息                                                                         |
| `AuthenticationManager` | 定义了认证 `Authentication` 的方法                                                                        |
| `UserDetailsService`    | 加载用户特定数据的核心接口，定义了根据用户名查询用户信息的方法                                                                   |
| `UserDetails`           | 提供核心用户信息。`UserDetailsService` 根据用户名查到的用户信息要封装成 `UserDetails` 对象返回，之后这些信息会被封装进 `Authentication` 对象 |

> **一条主线**：`UsernamePasswordAuthenticationFilter` → `AuthenticationManager`（默认实现 `ProviderManager`）→ `UserDetailsService` → `UserDetails` → 回到 `Authentication` → 存入 `SecurityContextHolder`。

### 2.3 自定义认证方案：思路分析

**登录**
1. 自定义登录接口：调用 `ProviderManager` 的方法进行认证；如果认证通过则生成 JWT，并把用户信息存入 Redis。
2. 自定义 `UserDetailsService`：在这个实现类中去查询数据库。

**校验**
1. 定义 JWT 认证过滤器：获取 token；
2. 解析 token 得到其中的 `userid`；
3. 从 Redis 中获取用户信息；
4. 存入 `SecurityContextHolder`。

### 2.4 准备工作：基础设施代码
#### 2.4.1 添加依赖
```xml
<!--redis依赖 -->  
<dependency>  
    <groupId>org.springframework.boot</groupId>  
    <artifactId>spring-boot-starter-data-redis</artifactId>  
</dependency>  
<!-- jjwt  -->  
<dependency>  
    <groupId>io.jsonwebtoken</groupId>  
    <artifactId>jjwt-api</artifactId>  
    <version>0.13.0</version>  
</dependency>  
<dependency>  
    <groupId>io.jsonwebtoken</groupId>  
    <artifactId>jjwt-impl</artifactId>  
    <version>0.13.0</version>  
    <scope>runtime</scope>  
</dependency>  
<dependency>  
    <groupId>io.jsonwebtoken</groupId>  
    <artifactId>jjwt-jackson</artifactId>  
    <version>0.13.0</version>  
    <scope>runtime</scope>  
</dependency>
```

#### 2.4.2 Redis 相关配置
使用 jackson 作为 Redis 的 value 序列化器：
```java
// 统一的 ObjectMapper    
private ObjectMapper buildObjectMapper() {  
    ObjectMapper mapper = new ObjectMapper();  
    // 支持 LocalDateTime / LocalDate        
    mapper.registerModule(new JavaTimeModule());  
    // 日期存 ISO 字符串，而非时间戳  
    mapper.disable(SerializationFeature.WRITE_DATES_AS_TIMESTAMPS);  
    return mapper;  
}  

// JSON 序列化器  
@Bean  
public GenericJackson2JsonRedisSerializer jsonRedisSerializer() {  
    return new GenericJackson2JsonRedisSerializer(buildObjectMapper());  
}  
```
把自定义的 `RedisTemplate` 注入容器：
```java
@Configuration  
public class RedisConfig {    
    @Bean  
    public RedisTemplate<String, Object> redisTemplate(  
            RedisConnectionFactory factory,  
            RedisSerializer<Object> jsonSerializer) {  
  
        RedisTemplate<String, Object> template = new RedisTemplate<>();  
        template.setConnectionFactory(factory);  
  
        StringRedisSerializer stringSerializer = new StringRedisSerializer();  
        template.setKeySerializer(stringSerializer);      // 普通 key
        template.setHashKeySerializer(stringSerializer);  // hash field  
        template.setValueSerializer(jsonSerializer);      // 普通 value        
        template.setHashValueSerializer(jsonSerializer);  // hash value
        template.setDefaultSerializer(jsonSerializer);    // 默认兜底  
  
        template.afterPropertiesSet();                    // 使配置立即生效  
        return template;  
    }
}
```

#### 2.4.3 统一响应类
```java
@JsonInclude(JsonInclude.Include.NON_NULL)  
@Data  
public class ResponseResult<T> {  
    /**  
     * 状态码  
     */  
    private Integer code;  
    /**  
     * 提示信息  
     */  
    private String msg;  
    /**  
     * 返回的数据  
     */  
    private T data;  
   
    public ResponseResult(Integer code, String msg) {  
        this.code = code;  
        this.msg = msg;  
    }  
	 public ResponseResult(Integer code, T data) {  
        this.code = code;  
        this.data = data;  
    }  
	 public ResponseResult(Integer code, String msg, T data) {  
        this.code = code;  
        this.msg = msg;  
        this.data = data;  
    }  
}
```

#### 2.4.4 工具类：`JwtUtil`

负责生成与解析 JWT
```java
@Component  
public class JwtUtil {  
  
    /** 默认密钥：Base64 编码的 32 字节; 生产环境请通过配置覆盖 */  
    private static final String DEFAULT_SECRET =  
            "jAzKB7F4+IFZJEEk283FUMbahEKYhG7UJ3uI8HnisK4";  
    /** 默认有效期：24 小时（毫秒） */  
    private static final long DEFAULT_EXPIRATION_MS = 86_400_000L;  
    /** 默认签发者 */  
    private static final String DEFAULT_ISSUER = "default-issuer";  
  
    @Value("${jwt.secret:" + DEFAULT_SECRET + "}")  
    private String secret;  
  
    @Value("${jwt.expiration-ms:" + DEFAULT_EXPIRATION_MS + "}")  
    private long expirationMs;  
  
    @Value("${jwt.issuer:" + DEFAULT_ISSUER + "}")  
    private String issuer;  
  
    private SecretKey secretKey;  
  
    @PostConstruct  
    public void init() {  
        // Base64 解码并校验密钥强度（32 字节 → HS256）  
        this.secretKey = Keys.hmacShaKeyFor(Decoders.BASE64.decode(secret));  
    }  
  
    // ────── 生成 Token ──────  
    /** 仅 subject */  
    public String generateToken(String subject) {  
        return generateToken(subject, null);  
    }  
  
    /** 仅自定义 claims */  
    public String generateToken(Map<String, Object> claims) {  
        return generateToken(null, claims);  
    }  
  
    /** subject + 自定义 claims */  
    public String generateToken(String subject, Map<String, Object> claims) {  
        Date now = new Date();  
        JwtBuilder builder = Jwts.builder()  
                .id(UUID.randomUUID().toString())            // jti  
                .issuer(issuer)                              // iss  
                .issuedAt(now)                               // iat  
                .expiration(new Date(now.getTime() + expirationMs)) // exp  
                .signWith(secretKey);  
        if (claims != null && !claims.isEmpty()) {  
            builder.claims(claims);  
        }  
        if (subject != null && !subject.isBlank()) {  
            builder.subject(subject);  
        }  
        return builder.compact();  
    }  
  
    // ────── 解析与校验 ──────  
    /** 解析 token，无效或过期抛 JwtException */  
    public Claims parseToken(String token) {  
        return Jwts.parser()  
                .verifyWith(secretKey)  
                .build()  
                .parseSignedClaims(token)  
                .getPayload();  
    }  
  
    /** 安全解析：无效或过期返回 null，不抛异常 */  
    public Claims parseTokenQuietly(String token) {  
        try {  
            return parseToken(token);  
        } catch (JwtException | IllegalArgumentException e) {  
            return null;  
        }  
    }  
  
    /** 是否有效：签名正确且未过期 */  
    public boolean isValid(String token) {  
        return parseTokenQuietly(token) != null;  
    }  
  
    /** 是否已过期 */  
    public boolean isExpired(String token) {  
        try {  
            return parseToken(token).getExpiration().before(new Date());  
        } catch (JwtException | IllegalArgumentException e) {  
            return true;  
        }  
    }  
  
    // ────── 提取字段 ──────  
    public String getSubject(String token) {  
        return parseToken(token).getSubject();  
    }  
  
    /** 获取全部 claims */  
    public Claims getClaims(String token) {  
        return parseToken(token);  
    }  
  
    public Date getExpiration(String token) {  
        return parseToken(token).getExpiration();  
    }  
  
    public String getJti(String token) {  
        return parseToken(token).getId();  
    }  
  
    public String getIssuer(String token) {  
        return parseToken(token).getIssuer();  
    }  
}
```

#### 2.4.5 工具类：`WebUtils`

把字符串（这里是 JSON）直接渲染到客户端，供自定义异常处理器复用：
```java
@Slf4j  
public class WebUtils {  
  
    /**  
     * 直接将字符串渲染输出到客户端  
     * @param response 响应对象  
     * @param content 输出内容（一般是json字符串）  
     * @return null  
     */    public static String renderString(HttpServletResponse response, String content) {  
        try {  
            response.setStatus(200);  
            response.setContentType("application/json");  
            response.setCharacterEncoding("UTF-8");  
            response.getWriter().print(content);  
            response.getWriter().flush();  
        } catch (IOException e) {  
            log.error(e.getMessage(), e);  
        }  
        return null;  
    }  
}
```