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
public class HelloController {  
  
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

#### 2.4.6 用户实体类 `User`
```java
/**  
 * 用户表(User)实体类  
 */  
@Data  
@AllArgsConstructor  
@NoArgsConstructor  
public class User implements Serializable {

    private Long id;            // 主键ID  
    private String userName;    // 用户名  
    private String nickName;    // 昵称  
    private String password;    // 密码（加密存储）  
    private String status;      // 账号状态：0正常，1停用  
    private String email;       // 邮箱  
    private String phoneNumber; // 手机号  
    private String sex;         // 性别：0男，1女，2未知  
    private String avatar;      // 头像地址  
    private String userType;    // 用户类型：0管理员，1普通用户  
    private Long createBy;      // 创建人ID  
    private Date createTime;    // 创建时间  
    private Long updateBy;      // 更新人ID  
    private Date updateTime;    // 更新时间  
    private Integer delFlag;    // 删除标记：0未删除，1已删除（逻辑删除）  
    
}
```

### 2.5 实现步骤

#### 2.5.1 数据库校验用户

由前面的分析可知：可以自定义一个 `UserDetailsService`，让 Spring Security 使用我们的实现，在其中从数据库查询用户名和密码。

**准备工作 ①：创建数据库与用户表**

```sql
-- 如果存在则删除数据库
DROP DATABASE IF EXISTS security;
-- 创建数据库
CREATE DATABASE security DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
-- 使用数据库
USE security;

-- 创建用户表 sys_user
CREATE TABLE sys_user (
    id BIGINT(20) NOT NULL AUTO_INCREMENT COMMENT '主键',
    user_name VARCHAR(64) NOT NULL DEFAULT 'NULL' COMMENT '用户名',
    nick_name VARCHAR(64) NOT NULL DEFAULT 'NULL' COMMENT '昵称',
    password VARCHAR(64) NOT NULL DEFAULT 'NULL' COMMENT '密码',
    status CHAR(1) DEFAULT '0' COMMENT '账号状态（0 正常 1 停用）',
    email VARCHAR(64) DEFAULT NULL COMMENT '邮箱',
    phonenumber VARCHAR(32) DEFAULT NULL COMMENT '手机号',
    sex CHAR(1) DEFAULT NULL COMMENT '用户性别（0 男，1 女，2 未知）',
    avatar VARCHAR(128) DEFAULT NULL COMMENT '头像',
    user_type CHAR(1) NOT NULL DEFAULT '1' COMMENT '用户类型（0 管理员，1 普通用户）',
    create_by BIGINT(20) DEFAULT NULL COMMENT '创建人的用户 id',
    create_time DATETIME DEFAULT NULL COMMENT '创建时间',
    update_by BIGINT(20) DEFAULT NULL COMMENT '更新人',
    update_time DATETIME DEFAULT NULL COMMENT '更新时间',
    del_flag INT(11) DEFAULT '0' COMMENT '删除标志（0 代表未删除，1 代表已删除）',
    PRIMARY KEY (id)
) ENGINE=INNODB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='用户表';
```

**准备工作 ②：引入 MyBatis-Plus 与 MySQL 驱动**

```xml
<dependency>  
    <groupId>com.baomidou</groupId>  
    <artifactId>mybatis-plus-spring-boot3-starter</artifactId>  
    <version>3.5.5</version>  
</dependency>
<dependency>  
    <groupId>com.mysql</groupId>  
    <artifactId>mysql-connector-j</artifactId>  
    <scope>runtime</scope>  
</dependency>
```

**准备工作 ③：配置数据库信息**
```yml
spring:
	datasource:  
	  url: jdbc:mysql://localhost:3306/security?useSSL=false&serverTimezone=Asia/Shanghai  
	  username: root  
	  password: 131452  
	  driver-class-name: com.mysql.cj.jdbc.Driver
```


**准备工作 ④：定义 Mapper 接口**

```java
public interface UserMapper extends BaseMapper<User> {  
}
```

**准备工作 ⑤：修改 User 实体类**

在类名上加 `@TableName(value = "sys_user")`，在 `id` 字段上加 `@TableId(type = IdType.AUTO)`：

**准备工作 ⑥：配置 Mapper 扫描**

在启动类配置主键扫描
```java
@MapperScan("com.quickstart.mapper")  
```

**核心代码 ①：实现 `UserDetailsService`** 

创建一个类实现 `UserDetailsService` 接口并重写其中的方法，根据用户名从数据库中查询用户信息：
```java
@Service  
public class UserDetailServiceImpl implements UserDetailsService {  
    @Autowired    
    private UserMapper userMapper;  
  
    @Override  
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {  
        User user = userMapper.selectOne(new LambdaQueryWrapper<User>().eq(User::getUserName, username));  
        if (user == null) {  
            throw new UsernameNotFoundException("用户不存在");  
        }  
        //TODO 根据用户查询权限信息 添加到LoginUser中  
        return new LoginUser(user);  
    }  
}
```

**核心代码 ②：定义 `LoginUser` 实现 `UserDetails`** 

因为 `UserDetailsService` 方法的返回值是 `UserDetails` 类型，所以需要定义一个类实现该接口，把用户信息封装在其中：

```java
@AllArgsConstructor  
public class LoginUser implements UserDetails {  
    private User user;  
    @Override  
    public Collection<? extends GrantedAuthority> getAuthorities() {  
        return null;  
    }  
  
    @Override  
    public String getPassword() {  
        return user.getPassword();  
    }  
  
    @Override  
    public String getUsername() {  
        return user.getUserName();  
    }  
  
    @Override  
    public boolean isAccountNonExpired() {  
        return true;  
    }  
  
    @Override  
    public boolean isAccountNonLocked() {  
        return true;  
    }  
  
    @Override  
    public boolean isCredentialsNonExpired() {  
        return true;  
    }  
  
    @Override  
    public boolean isEnabled() {  
        return true;  
    }  
}
```

**注意**：如果要测试，需要往用户表中写入用户数据；如果希望密码以明文存储，需要在密码前加 `{noop}`，例如： 
![[Pasted image 20261116173136.png]]
这样登录时就可以用 `admin` 作为用户名、`1234` 作为密码来登录。

实际项目中不会把密码明文存储在数据库中。默认使用的 `PasswordEncoder` 要求数据库中的密码格式为 `{id}password`，它会根据 `id` 判断密码的加密方式；但我们一般不采用这种方式，所以需要替换 `PasswordEncoder`。

一般使用 Spring Security 提供的 `BCryptPasswordEncoder`：只需把 `BCryptPasswordEncoder` 对象注入 Spring 容器，Spring Security 就会使用它进行密码校验。

定义一个 Spring Security 配置类：

```java
@Configuration  
public class SecurityConfig {  
    @Bean    public PasswordEncoder passwordEncoder() {  
        return new BCryptPasswordEncoder();  
    }  
}
```

#### 2.5.3 登录接口

接下来需要自定义登录接口，并让 Spring Security 对该接口放行，使用户访问时无需登录。

在接口中通过 `AuthenticationManager` 的 `authenticate` 方法进行用户认证，因此需要在 `SecurityConfig` 中把 `AuthenticationManager` 注入容器。认证成功后生成一个 JWT 放入响应返回；为了让用户下次请求时能通过 JWT 识别出具体是哪个用户，还需要把用户信息存入 Redis。

**controller**

```java
@RestController
@RequestMapping("/user")  
public class LoginController {  
    @Autowired    private LoginService loginService;  
  
    @PostMapping("/login")  
    public ResponseResult<Map<String, String>> login(String username, String password) {  
        Map<String, String> result = loginService.login(username, password);  
        return new ResponseResult<>(200, "登录成功！", result);  
    }  
}
```

**service**

```java
@Service  
public class LoginServiceImpl implements LoginService {  
	// 86400000 milliseconds = 1 day  
    private final static Long defaultExpire = 86400000L; 
  
    @Autowired  
    private AuthenticationManager authenticationManager;  
    @Autowired  
    private JwtUtil jwtUtil;  
    @Autowired  
    private RedisTemplate<String, LoginUser> redisTemplate;  
  
    @Override  
    public Map<String, String> login(String username, String password) {  
        // 创建一个Authentication对象  
        Authentication authentication = new UsernamePasswordAuthenticationToken(username, password);  
        // 进行身份验证  
        Authentication result = authenticationManager.authenticate(authentication);  
  
        // 生成JWT令牌  
        LoginUser loginUser = (LoginUser) result.getPrincipal();  
        Long userId = loginUser.getUser().getId();  
        String token = jwtUtil.generateToken(userId.toString());  
  
        // 将登录用户信息存储在Redis中  
        redisTemplate.opsForValue().set("login:" + token, loginUser, defaultExpire, TimeUnit.MILLISECONDS);  
  
        // 返回  
        Map<String, String> map = new HashMap<>();  
        map.put("token", token);  
        return map;  
    }  
}
```

**SecurityConfig（放行登录接口 + 无状态会话 + 暴露 AuthenticationManager）**

```java
@Bean  
public SecurityFilterChain securityFilterChain(HttpSecurity http) throws Exception {  
    http            
    		.csrf(AbstractHttpConfigurer::disable)  
            .sessionManagement(session -> session.sessionCreationPolicy(SessionCreationPolicy.STATELESS))  
            .authorizeHttpRequests(authorize -> authorize  
                    // 放行登录接口  
                    .requestMatchers("/user/login").permitAll()  
                    .anyRequest().authenticated());  
  
    return http.build();  
}
```

#### 2.5.4 JWT 认证过滤器

自定义一个过滤器：获取请求头中的 token → 解析取出 `userid` → 用 `userid` 去 Redis 中获取对应的 `LoginUser` 对象 → 封装 `Authentication` 对象存入 `SecurityContextHolder`。

```java
@Component  
public class JwtAuthenticationTokenFilter extends OncePerRequestFilter {  
    @Autowired    
    private JwtUtil jwtUtil;  
    @Autowired  
    RedisTemplate redisTemplate;  
  
    @Override  
    protected void doFilterInternal(HttpServletRequest request, HttpServletResponse response, FilterChain filterChain) throws ServletException, IOException {  
        String token = request.getHeader("token");  
        // 如果没有token，则放行到后面的过滤器  
        if (!StringUtils.hasText(token)) {  
            filterChain.doFilter(request, response);  
            return;  
        }  
  
        // 获取用户id  
        String subject;  
        try {  
            subject = jwtUtil.getSubject(token);  
        } catch (Exception e) {  
            throw new RuntimeException("token无效");  
        }  
  
        // 从redis中获取用户信息  
        String key = "login:" + subject;  
        LoginUser user = (LoginUser) redisTemplate.opsForValue().get(key);  
        if (user == null) {  
            throw new RuntimeException("用户未登录");  
        }  
  
        //存入SecurityContextHolder  
        // TODO 取权限信息封装到Authentication中  
        UsernamePasswordAuthenticationToken authentication = new UsernamePasswordAuthenticationToken(user, null, null);  
        SecurityContextHolder.getContext().setAuthentication(authentication);  
  
        // 继续处理请求  
        filterChain.doFilter(request, response);  
    }  
}
```

把过滤器加入过滤器链（放在 `UsernamePasswordAuthenticationFilter` 之前）：
```java
@Bean  
public SecurityFilterChain securityFilterChain(HttpSecurity http) throws Exception {  
    http            .csrf(AbstractHttpConfigurer::disable)  
            .sessionManagement(session -> session.sessionCreationPolicy(SessionCreationPolicy.STATELESS))  
            .authorizeHttpRequests(authorize -> authorize  
                    // 放行登录接口  
                    .requestMatchers("/user/login").permitAll()  
                    .anyRequest().authenticated());  
  
    // 将 JwtAuthenticationTokenFilter 添加到 UsernamePasswordAuthenticationFilter 之前  
    http.addFilterBefore(jwtAuthenticationTokenFilter, UsernamePasswordAuthenticationFilter.class);  
    return http.build();  
}
```

#### 2.5.5 登出接口

定义一个退出登录接口：从 `SecurityContextHolder` 中获取认证信息，删除 Redis 中对应的数据即可。

**controller**

```java
@RestController  
@RequestMapping("/user")  
public class LoginController {  
    @Autowired  
    private LoginService loginService;  
  
    @RequestMapping("/logout")  
    public ResponseResult<String> logout() {  
        loginService.logout();  
        return new ResponseResult<>(200, "登出成功！");  
    }  
}
```

**service**

```java
@Service  
public class LoginServiceImpl implements LoginService {  
    @Autowired  
    private RedisTemplate<String, Object> redisTemplate;  

    @Override  
    public void logout() {  
        // 获取当前登录用户  
        Authentication authentication = SecurityContextHolder.getContext().getAuthentication();  
        LoginUser user = (LoginUser) authentication.getPrincipal();  
        Long userId = user.getUser().getId();  
  
        // 删除Redis中的用户信息  
        redisTemplate.delete("login:" + userId);  
    }  
}
```

### 2.6 认证链路回顾

| 步骤  | 参与方                                    | 关键动作                                                                                     |
| --- | -------------------------------------- | ---------------------------------------------------------------------------------------- |
| 1   | `LoginController` / `LoginServiceImpl` | 用用户名密码构造 `UsernamePasswordAuthenticationToken`，调用 `authenticationManager.authenticate()` |
| 2   | `UserDetailsServiceImpl`               | 按用户名查库，查不到抛异常；查到后封装为 `LoginUser` 返回                                                      |
| 3   | `LoginServiceImpl`                     | 用 `userId` 生成 JWT，`login:userId` 为 key 把 `LoginUser` 存入 Redis，返回 token                   |
| 4   | `JwtAuthenticationTokenFilter`         | 每次请求解析 token → Redis 取用户 → 写入 `SecurityContextHolder`                                    |
| 5   | `LoginServiceImpl.logout()`            | 从上下文取用户 id，删除 Redis 中的登录缓存                                                               |

> **为什么必须 STATELESS**：认证状态不再由 Session 保存，而是由「JWT + Redis + 每次请求重建 SecurityContext」共同维护，所以配置里关闭了 CSRF 并把会话策略设为 `SessionCreationPolicy.STATELESS`。

## 3. 授权

### 3.1 权限系统的作用

以学校图书馆管理系统为例：普通学生登录后只能看到借书、还书相关功能，不应看到并使用添加书籍信息、删除书籍信息等功能；而图书馆管理员账号登录后才应该能使用这些功能。

总结起来就是：**不同的用户可以使用不同的功能**，这正是权限系统要实现的效果。

不能只依赖前端判断用户权限来决定显示哪些菜单、哪些按钮。因为一旦有人知道了对应功能的接口地址，就可以绕过前端直接发请求执行操作。所以还需要在**后台**进行权限判断：判断当前用户是否有所需权限，必须具有所需权限才能进行相应操作。

### 3.2 授权基本流程

在 Spring Security 中，默认由 `FilterSecurityInterceptor` 进行权限校验。它会从 `SecurityContextHolder` 中获取 `Authentication`，再获取其中的权限信息，判断当前用户是否拥有访问当前资源所需的权限。

因此在项目中只需要做两件事：

1. 把当前登录用户的权限信息也存入 `Authentication`；
2. 设置我们的资源所需要的权限。

### 3.3 授权实现

#### 3.3.1 限制访问资源所需权限

Spring Security 提供了基于注解的权限控制方案，这也是项目中主要采用的方式：用注解指定访问对应资源所需的权限。使用前需要先开启相关配置：

```java
@EnableMethodSecurity(prePostEnabled = true)
```

然后即可使用 `@PreAuthorize` 注解修饰接口方法：

```java
@RestController  
public class HelloController {  
  
    @RequestMapping("/hello")
    @PreAuthorize("hasAuthority('test')")
    public String sayHello(@RequestParam(value = "name", defaultValue = "Tom") String name) {  
        return "Hello, " + name + "!";  
    }  
}
```

#### 3.3.2 封装权限信息

前面编写 `UserDetailsServiceImpl` 时提到：查询出用户后还要获取对应的权限信息，封装到 `UserDetails` 中返回。这里先把权限信息写死，用于测试。

修改之前定义的 `UserDetails` 实现类 `LoginUser`，使其能封装权限信息：

```java
package com.quickstart.common;  
  
import com.fasterxml.jackson.annotation.JsonIgnore;  
import com.quickstart.entity.User;  
import lombok.Getter;  
import lombok.NoArgsConstructor;  
import org.springframework.security.core.GrantedAuthority;  
import org.springframework.security.core.authority.SimpleGrantedAuthority;  
import org.springframework.security.core.userdetails.UserDetails;  
  
import java.util.Collection;  
import java.util.List;  
import java.util.stream.Collectors;  
  
@Getter  
@NoArgsConstructor  
public class LoginUser implements UserDetails {  
    private User user;  
    private List<String> permissions; // 权限集合  
  
    public LoginUser(User user, List<String> permissions) {  
        this.user = user;  
        this.permissions = permissions;  
    }  
  
    // 存储 SpringSecurity 所需要的权限集合  
    private List<GrantedAuthority> authorities;  
  
    @Override  
    @JsonIgnore    public Collection<? extends GrantedAuthority> getAuthorities() {  
        if (authorities != null) {  
            return authorities;  
        }  
        authorities = permissions.stream().map(SimpleGrantedAuthority::new).collect(Collectors.toList());  
        return authorities;  
    }  
  
    @Override  
    @JsonIgnore    public String getPassword() {  
        return user.getPassword();  
    }  
  
    @Override  
    @JsonIgnore    public String getUsername() {  
        return user.getUsername();  
    }  
  
    @Override  
    @JsonIgnore    public boolean isAccountNonExpired() {  
        return true;  
    }  
  
    @Override  
    @JsonIgnore    public boolean isAccountNonLocked() {  
        return true;  
    }  
  
    @Override  
    @JsonIgnore    public boolean isCredentialsNonExpired() {  
        return true;  
    }  
  
    @Override  
    @JsonIgnore    public boolean isEnabled() {  
        return true;  
    }  
}
```
其中 @JsonIgnore；`getAuthorities()` 采用懒加载方式把字符串权限转换为 `SimpleGrantedAuthority`。

`LoginUser` 修改完后，就可以在 `UserDetailsServiceImpl` 中把权限信息封装进 `LoginUser`。先写死权限测试，后面再从数据库查询：

```java
@Service  
public class UserDetailServiceImpl implements UserDetailsService {  
    @Autowired    private UserMapper userMapper;  
  
    @Override  
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {  
        User user = userMapper.selectOne(new LambdaQueryWrapper<User>().eq(User::getUsername, username));  
        if (user == null) {  
            throw new UsernameNotFoundException("用户不存在");  
        }  
        //TODO 根据用户查询权限信息 添加到LoginUser中  
        List<String> permissions = new ArrayList<>(List.of("test"));  
        return new LoginUser(user, permissions);  
    }  
}
```

#### 3.3.3 从数据库查询权限信息

##### 3.3.3.1 RBAC 权限模型

**RBAC**（Role-Based Access Control）即「基于角色的权限控制」，是目前开发者最常使用、也相对易用和通用的权限模型。

![RBAC 权限模型](file:///D:/workbuddy_repo/spaces/2026_11_12_21-54-24/imgs/2909814-20220815171513580-99682924.png?lastModify=1794964911)

##### 3.3.3.2 准备工作：建库建表

```sql
CREATE DATABASE IF NOT EXISTS security DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;

USE security;

-- ----------------------------
-- 菜单表 sys_menu
-- ----------------------------
DROP TABLE IF EXISTS `sys_menu`;
CREATE TABLE `sys_menu` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '菜单ID',
  `menu_name` varchar(64) NOT NULL COMMENT '菜单名称',
  `path` varchar(200) DEFAULT NULL COMMENT '路由地址',
  `component` varchar(255) DEFAULT NULL COMMENT '前端组件路径',
  `visible` char(1) DEFAULT '0' COMMENT '是否显示(0显示 1隐藏)',
  `status` char(1) DEFAULT '0' COMMENT '状态(0正常 1停用)',
  `perms` varchar(100) DEFAULT NULL COMMENT '权限标识，如:system:user:list',
  `icon` varchar(100) DEFAULT '#' COMMENT '菜单图标',
  `create_by` bigint(20) DEFAULT NULL COMMENT '创建人ID',
  `create_time` datetime DEFAULT NULL COMMENT '创建时间',
  `update_by` bigint(20) DEFAULT NULL COMMENT '更新人ID',
  `update_time` datetime DEFAULT NULL COMMENT '更新时间',
  `del_flag` int(11) DEFAULT '0' COMMENT '删除标记(0未删除 1已删除)',
  `remark` varchar(500) DEFAULT NULL COMMENT '备注',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COMMENT='菜单表';

-- ----------------------------
-- 角色表 sys_role
-- ----------------------------
DROP TABLE IF EXISTS `sys_role`;
CREATE TABLE `sys_role` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '角色ID',
  `name` varchar(128) DEFAULT NULL COMMENT '角色名称',
  `role_key` varchar(100) DEFAULT NULL COMMENT '角色标识，如admin',
  `status` char(1) DEFAULT '0' COMMENT '状态(0正常 1停用)',
  `del_flag` int(11) DEFAULT '0' COMMENT '删除标记(0未删除 1已删除)',
  `create_by` bigint(20) DEFAULT NULL COMMENT '创建人ID',
  `create_time` datetime DEFAULT NULL COMMENT '创建时间',
  `update_by` bigint(20) DEFAULT NULL COMMENT '更新人ID',
  `update_time` datetime DEFAULT NULL COMMENT '更新时间',
  `remark` varchar(500) DEFAULT NULL COMMENT '备注',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COMMENT='角色表';

-- ----------------------------
-- 角色菜单关联表 sys_role_menu（RBAC中间表）
-- ----------------------------
DROP TABLE IF EXISTS `sys_role_menu`;
CREATE TABLE `sys_role_menu` (
  `role_id` bigint(20) NOT NULL COMMENT '角色ID',
  `menu_id` bigint(20) NOT NULL COMMENT '菜单ID',
  PRIMARY KEY (`role_id`,`menu_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='角色和菜单关联表';

-- ----------------------------
-- 用户表 sys_user
-- ----------------------------
DROP TABLE IF EXISTS `sys_user`;
CREATE TABLE `sys_user` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '用户ID',
  `user_name` varchar(64) NOT NULL COMMENT '用户名',
  `nick_name` varchar(64) NOT NULL COMMENT '用户昵称',
  `password` varchar(64) NOT NULL COMMENT '密码（加密存储）',
  `status` char(1) DEFAULT '0' COMMENT '账号状态(0正常 1停用)',
  `email` varchar(64) DEFAULT NULL COMMENT '邮箱',
  `phonenumber` varchar(32) DEFAULT NULL COMMENT '手机号',
  `sex` char(1) DEFAULT NULL COMMENT '性别(0男 1女 2未知)',
  `avatar` varchar(128) DEFAULT NULL COMMENT '头像地址',
  `user_type` char(1) NOT NULL DEFAULT '1' COMMENT '用户类型(0管理员 1普通用户)',
  `create_by` bigint(20) DEFAULT NULL COMMENT '创建人ID',
  `create_time` datetime DEFAULT NULL COMMENT '创建时间',
  `update_by` bigint(20) DEFAULT NULL COMMENT '更新人ID',
  `update_time` datetime DEFAULT NULL COMMENT '更新时间',
  `del_flag` int(11) DEFAULT '0' COMMENT '删除标记(0未删除 1已删除)',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COMMENT='用户表';

-- ----------------------------
-- 用户角色关联表 sys_user_role（RBAC中间表）
-- ----------------------------
DROP TABLE IF EXISTS `sys_user_role`;
CREATE TABLE `sys_user_role` (
  `user_id` bigint(20) NOT NULL COMMENT '用户ID',
  `role_id` bigint(20) NOT NULL COMMENT '角色ID',
  PRIMARY KEY (`user_id`,`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户和角色关联表';
```

**按用户 id 查询权限标识的 SQL**（用户 → 用户角色 → 角色 → 角色菜单 → 菜单，只取正常状态、去重）：

```sql
SELECT 
	DISTINCT m.`perms`
FROM
	sys_user_role ur
	LEFT JOIN `sys_role` r ON ur.`role_id` = r.`id`
	LEFT JOIN `sys_role_menu` rm ON ur.`role_id` = rm.`role_id`
	LEFT JOIN `sys_menu` m ON m.`id` = rm.`menu_id`
WHERE
	user_id = 1
	AND r.`status` = 0
	AND m.`status` = 0
```

**菜单实体类 `Menu` **

```java
/**  
 * 菜单表实体类，对应数据库 sys_menu，RBAC权限模型里的菜单&权限点  
 */  
@TableName("sys_menu")                // // 绑定数据库表名 sys_menu@Data                                 // // lombok：自动生成get、set、toString、equals、hashCode  
@AllArgsConstructor                   // // lombok：全参构造  
@NoArgsConstructor                    // // lombok：无参构造  
@JsonInclude(JsonInclude.Include.NON_NULL) // // jackson序列化时，值为null的字段不返回给前端  
public class Menu implements Serializable { // // Serializable：支持序列化，可放入redis缓存、跨服务传输  
  
    @TableId                           // // 标记该字段是主键  
    private Long id;                   // // 菜单主键id  
    private String menuName;           // // 菜单名称  
    private String path;               // // vue路由地址  
    private String component;          // // 前端vue组件路径  
    private String visible;            // // 是否显示菜单：0显示 1隐藏  
    private String status;             // // 菜单启用状态：0正常 1停用  
    private String perms;              // // 权限标识，用于@PreAuthorize鉴权，例：system:user:list  
    private String icon;               // // 菜单图标  
  
    private Long createBy;             // // 创建人用户id  
    private Date createTime;           // // 创建时间  
    private Long updateBy;             // // 更新人用户id  
    private Date updateTime;           // // 更新时间  
    private Integer delFlag;           // // 逻辑删除标记：0未删除 1已删除  
    private String remark;             // // 备注信息  
}
```
##### 3.3.3.3 代码实现

只需要根据用户 id 查询到其所对应的权限信息。先定义一个 Mapper，提供根据 `userid` 查询权限信息的方法：
```java
public interface MenuMapper extends BaseMapper<Menu> {
    List<String> selectPermsByUserId(Long id);
}
```

因为是自定义方法，需要创建对应的 Mapper XML 文件并定义 SQL 语句：

```java
<?xml version="1.0" encoding="UTF-8" ?>  
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >  
<mapper namespace="com.quickstart.mapper.MenuMapper">  
    <select id="selectPermsById" resultType="java.lang.String">  
        SELECT  
            DISTINCT m.`perms`        FROM            sys_user_role ur                LEFT JOIN `sys_role` r ON ur.`role_id` = r.`id`                LEFT JOIN `sys_role_menu` rm ON ur.`role_id` = rm.`role_id`                LEFT JOIN `sys_menu` m ON m.`id` = rm.`menu_id`        WHERE            user_id = #{id}          AND r.`status` = 0          AND m.`status` = 0    </select>  
</mapper>
```
最后在 `UserDetailsServiceImpl` 中调用该 Mapper 方法，把查询到的权限信息封装进 `LoginUser`：

```java
@Service  
public class UserDetailServiceImpl implements UserDetailsService {  
    private final UserMapper userMapper;  
    private final MenuMapper menuMapper;  
  
    public UserDetailServiceImpl(UserMapper userMapper, MenuMapper menuMapper) {  
        this.userMapper = userMapper;  
        this.menuMapper = menuMapper;  
    }  
  
    @Override  
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {  
        User user = userMapper.selectOne(new LambdaQueryWrapper<User>().eq(User::getUsername, username));  
        if (user == null) {  
            throw new UsernameNotFoundException("用户不存在");  
        }  
        List<String> permissions = menuMapper.selectPermsById(user.getId());  
        // List<String> permissions = new ArrayList<>(List.of("test"));  
        return new LoginUser(user, permissions);  
    }  
}
```

**授权闭环**：`menuMapper.selectPermsByUserId()` → `LoginUser.permissions` → `getAuthorities()` → `Authentication` → `FilterSecurityInterceptor` / `@PreAuthorize("hasAuthority('...')")` 完成校验。