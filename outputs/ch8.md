---

# 八、登录

前面七章解决的都是「一次请求之内」的问题：参数怎么传、SQL 怎么写、事务怎么管。但登录功能有一个绕不开的前提——**HTTP 是无状态协议**，协议本身不记录「上一个请求是谁发的」，所以用户登录成功后随手点进别的页面，服务端就完全不知道他就是刚才那个登录用户。

本章分三步走：先弄清楚登录状态该怎么存、怎么带回来（会话跟踪），再落地本项目采用的令牌方案（JWT），最后用过滤器与拦截器把「校验登录状态」做成统一的横切能力。

| 小节 | 内容 | 要回答的问题 |
| --- | --- | --- |
| 8.1 会话跟踪技术 | Cookie / Session / 令牌 | 登录状态存在哪里、怎么带回来 |
| 8.2 JWT 令牌 | JWT 的结构与生成解析 | 令牌长什么样，怎么生成、怎么校验 |
| 8.3 过滤器 Filter | Servlet 三大组件之一 | 令牌校验该写在哪里 |
| 8.4 拦截器 Interceptor | Spring MVC 提供的拦截机制 | 两者有什么区别，该用哪个 |

## 8.1 会话跟踪技术

### 8.1.1 会话跟踪的作用与业务场景

先说清楚问题。用户在登录页输入账号密码，登录接口校验通过，返回「登录成功」——但这之后，服务端**并没有留下任何记录**，说明这个浏览器来自同一个用户的下一笔请求。而登录功能的价值恰恰在于「登录之后」，所以必须想办法把「浏览器的多次请求」和「同一个登录用户」绑定起来。

这种把多次请求与同一个用户关联、从而在后续请求中识别出该用户的技术，就叫 **会话跟踪（Session Tracking）**。

典型场景：

- **登录态保持**：登录后跳转主页，右上角显示「张三」而不是登录按钮；
- **后台管理系统的权限控制**：没登录访问 `/emps`，要被拦下来送去登录页；
- **购物车**：加购、改数量、结算，服务端要认出这是同一个用户的购物车；
- **在线课程 / 阅读进度**：记录「上次学到第 12 讲」。

会话跟踪有三条主流路线：**Cookie、Session、令牌**，它们的差别只在于「数据存哪里」这一件事。

### 8.1.2 Cookie

**Cookie** 是存放在浏览器中的一段键值对。服务端通过响应头 `Set-Cookie` 让浏览器保存数据，之后浏览器每次访问该网站，都会自动在请求头里带上它。

```java
// 1. 服务端把 Cookie 写进响应，浏览器收到后保存到本地
Cookie cookie = new Cookie("username", "Tom");
cookie.setPath("/");        // 生效路径，"/" 表示全站
cookie.setMaxAge(3600);     // 存活时间，单位秒；不设置则浏览器关闭即失效
resp.addCookie(cookie);

// 2. 之后每次请求，浏览器自动带上：Cookie: username=Tom
Cookie[] cookies = req.getCookies();
String username = null;
if (cookies != null) {
    for (Cookie c : cookies) {
        if ("username".equals(c.getName())) {
            username = c.getValue();
            break;
        }
    }
}
```

`setMaxAge` 的取值决定 Cookie 的生命周期：正数是存活秒数，`0` 表示**立即删除**（常用于「退出登录」时清掉凭证），不设置则是会话级 Cookie，浏览器一关就没了。

Cookie 的特点是**数据在客户端**，服务端要什么就得让客户端自己带回来。问题也就出在这里：

> [!WARNING]
> Cookie 里的内容**可以被用户随意修改**（打开浏览器开发者工具就能改），格式改、签名改都随你。所以 Cookie 只能存「非敏感、便于读取」的东西，如主题、语言偏好、是否记住用户名，**绝不能存登录凭证**，否则伪造一个 Cookie 就能冒充任意用户。

### 8.1.3 Session

**Session** 换了个思路：数据不让浏览器拿，浏览器只拿一个**编号**。

1. 第一次访问时，服务端创建一个 Session 对象（默认存在内存里），生成唯一标识 `JSESSIONID`；
2. 服务端通过响应头 `Set-Cookie: JSESSIONID=xxx` 把这个编号发给浏览器；
3. 之后每次请求，浏览器自动带上 `Cookie: JSESSIONID=xxx`；
4. 服务端拿这个编号找到对应的 Session，取出登录用户等信息。

```java
// 1. 登录成功后，把登录用户存进 Session
HttpSession session = req.getSession();     // 没有就新建
session.setAttribute("loginUser", loginUser);

// 2. 后续请求，从中取出登录用户
HttpSession session = req.getSession();
Object loginUser = session.getAttribute("loginUser");
```

Session 把数据放在服务端，比 Cookie 安全得多，但它有两个绕不开的短板：

- **它本质上还是靠 Cookie 传编号的**：用户禁用 Cookie、或跨域请求带不上 Cookie，Session 就直接失效；
- **数据存在服务端内存**：多台服务器部署时需要额外的 Session 共享方案（Redis），而服务器一重启数据全丢。

> [!TIP]
> Spring 里可以把登录用户直接塞进 `HttpSession`，也可以交给 Spring Security 统一管理。但本项目是**前后端分离**，前端需要的是一个「自己能拿着用、能塞进请求头」的凭证，Session 并不方便。

### 8.1.4 令牌

**令牌** 采取第三种做法：把登录状态打包成一个字符串发给客户端，客户端每次请求都把它带回来，而服务端**不再保存任何会话数据**，只负责校验这个字符串对不对。

| 方案 | 数据存哪里 | 客户端拿到什么 | 服务端要不要存东西 |
| --- | --- | --- | --- |
| Cookie | 客户端 | 全部数据 | 不用存，但数据可伪造 |
| Session | 服务端 | 一个编号 `JSESSIONID` | 要存，且依赖 Cookie |
| 令牌 | 客户端 | 一个「打包好的登录凭证」 | 不用存，靠校验令牌判断 |

令牌方案同时拿到了「客户端持有」和「服务端无状态」两个优点：前端把令牌存进 `localStorage`，每次请求塞进请求头；服务端只做校验，多台服务器也能随便扩。本项目使用的令牌格式就是 **JWT**。

> [!IMPORTANT]
> 三种方案并不互斥，实际项目中常常混用：用令牌传递登录态，同时用 Cookie 保存主题、语言这类偏好。分清「哪部分数据愿意放在客户端」再选方案，就不会用错。

## 8.2 JWT 令牌

### 8.2.1 JWT 简介

**JWT**（JSON Web Token）是令牌的一种具体格式，可以理解为「**用 JSON 表达、用签名保证没被改过的登录凭证**」。

一个 JWT 令牌长这样，由三段 Base64URL 编码的字符串用 `.` 连起来（示例值，仅用于说明结构）：

```
eyJhbGciOiJIUzI1NiJ9.eyJpZCI6MSwidXNlcm5hbWUiOiJhZG1pbiJ9.3nG1H_aG2XhVJt1TkYFvU9oBcS-hAqZ0t7Qp2sLxdc
```

它的四个特点，正好对应登录功能的需求：

| 特点 | 说明 | 对登录的意义 |
| --- | --- | --- |
| **自包含** | 登录用户的信息直接写在令牌里 | 服务端拿到令牌就知道是谁，校验时不必回查数据库 |
| **无状态** | 服务端不保存任何会话数据 | 多台服务器随便扩，天然支持负载均衡 |
| **可验证** | 令牌末尾有一段签名 | 令牌被篡改、被伪造时能被识别出来 |
| **紧凑** | 只是一段字符串 | 可以放进请求头、URL 参数或 Cookie 中 |

> [!WARNING]
> JWT 的三段内容中，**只有签名是防篡改的，Header 和 Payload 只是 Base64URL 编码，任何人拿到令牌都能解开看内容**。所以 Payload 里只能放 `id`、`username` 这类用于识别的信息，**密码、身份证号等敏感数据一律不能放进去**。

### 8.2.2 JWT 的组成

JWT 由三部分组成，用 `.` 分隔，从左到右依次是 Header、Payload、Signature。

#### （1）Header 头部

固定结构，声明令牌使用的**签名算法**和**令牌类型**（JWT）：

```json
{
  "alg": "HS256",
  "typ": "JWT"
}
```

`alg` 决定了后面签名用什么算法算，也是最需要盯住的地方：一旦服务端在校验时「相信令牌自带的 alg」，攻击者就能把 alg 改成 `none` 来绕过验签。JJWT 在 `verifyWith(KEY)` 时会以**服务端配置的密钥**为准，天然堵住了这条路。

#### （2）Payload 载荷

真正装数据的地方，也是三段中唯一有业务含义的部分。除了 `iat`（签发时间）、`exp`（过期时间）这些标准字段，还可以自定义任意字段：

```json
{
  "id": 1,
  "username": "admin",
  "iat": 1758864000,
  "exp": 1758950400
}
```

常见标准字段：

| 字段 | 全称 | 含义 |
| --- | --- | --- |
| `exp` | expiration | 过期时间，超过后校验必然失败，令牌作废 |
| `iat` | issued at | 签发时间 |
| `iss` | issuer | 签发者，用于确认令牌是谁发的 |
| `sub` | subject | 主题，一般放用户 ID |

> [!TIP]
> 本项目的载荷里放 `id` 和 `username` 两个字段：服务端校验时靠 `id` 判断当前是哪个用户；前端拿到令牌后也可以自行解析出 `username` 来显示用户名。**载荷字段名一旦定下来就不能改**，旧令牌还在用户浏览器里躺着，改名会让所有已签发的令牌集体失效。

#### （3）Signature 签名

签名是把「Header + Payload 的编码结果」用服务端私有密钥算一遍得到的：

```
签名 = HMACSHA256(Base64URL(Header) + "." + Base64URL(Payload), SECRET_KEY)
```

它的作用是**防篡改、防伪造**：Header 或 Payload 被动了一个字节，重算出来的签名就对不上，校验必然失败；攻击者因为拿不到 `SECRET_KEY`，也无法凭空造出一个能通过校验的令牌。

三段合起来，再把每一段都做 Base64URL 编码（把 `+`、`/` 换成 `-`、`_`，去掉结尾的 `=`），就是最终那串令牌。各段的编码结果一一对应：

```
eyJhbGciOiJIUzI1NiJ9.  eyJpZCI6MSwidXNlcm5hbWUiOiJhZG1pbiJ9.  3nG1H_aG2XhVJt1TkYFvU9oBcS-hAqZ0t7Qp2sLxdc
    └── Header ──┘        └──────── Payload ────────┘      └──────── Signature ────────┘
```

> [!IMPORTANT]
> 记住一句话就够用了：**Payload 负责「装数据」，Signature 负责「保数据不被改」**。至于「能不能解密看内容」——不能加密，只能看。

### 8.2.3 JWT 的使用

#### （1）引入依赖

JJWT 从 0.12.0 起把 API 拆成了三个依赖：核心 API（编译时必需）、具体实现和 JSON 处理（运行时必需），三者版本号必须保持一致。

```xml
<!--jwt-->
<!-- 核心 API 依赖（编译时必需） -->
<dependency>
    <groupId>io.jsonwebtoken</groupId>
    <artifactId>jjwt-api</artifactId>
    <version>0.13.0</version>
</dependency>
<!-- 具体实现（运行时必需） -->
<dependency>
    <groupId>io.jsonwebtoken</groupId>
    <artifactId>jjwt-impl</artifactId>
    <version>0.13.0</version>
    <scope>runtime</scope>
</dependency>
<!-- Jackson JSON 处理（运行时必需） -->
<dependency>
    <groupId>io.jsonwebtoken</groupId>
    <artifactId>jjwt-jackson</artifactId>
    <version>0.13.0</version>
    <scope>runtime</scope>
</dependency>
```

> [!TIP]
> 少写任何一个，`compile` 阶段都看不出问题，只有运行时才会抛 `NoClassDefFoundError` / `ClassNotFoundException`。看到这类报错，先回头检查这三个依赖是否齐全、版本是否一致。

#### （2）封装 JWT 工具类

生成令牌和解析令牌是固定套路，封装成一个工具类，全项目共用一把密钥：

```java
package com.itheima.utils;

import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jws;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.security.Keys;

import javax.crypto.SecretKey;
import java.nio.charset.StandardCharsets;
import java.util.Date;
import java.util.Map;

/**
 * JWT工具类，仅生成、解析 JJWT 0.13.0
 */
public class JwtUtils {

    // 签名密钥；HS256 要求密钥长度不少于 256 位（32 字节），否则运行时报 WeakKeyException
    private static final String SECRET_KEY = "mySecretKey123456789012345678901234567890";
    private static final SecretKey KEY = Keys.hmacShaKeyFor(SECRET_KEY.getBytes(StandardCharsets.UTF_8));
    private static final long EXPIRATION = 24 * 60 * 60 * 1000L; // 有效期 24 小时

    /**
     * 生成token，传入自定义claim集合
     * @param claims 自定义载荷，例如 map.put("id",1); map.put("username","admin")
     * @return token字符串
     */
    public static String generateToken(Map<String, Object> claims) {
        return Jwts.builder()
                .claims(claims)                                                // 直接传入自定义载荷map
                .issuedAt(new Date())                                          // 签发时间
                .expiration(new Date(System.currentTimeMillis() + EXPIRATION)) // 过期时间
                .signWith(KEY)                                                 // 签名，算法由密钥长度自动匹配
                .compact();                                                    // 拼装成最终令牌
    }

    /**
     * 解析token，签名错误、过期、格式错误直接抛出异常
     * @param token jwt令牌
     * @return Claims 令牌载荷
     */
    public static Claims parseToken(String token) {
        Jws<Claims> jws = Jwts.parser()
                .verifyWith(KEY)          // 用同一把密钥验签
                .build()
                .parseSignedClaims(token); // 签名不符 / 已过期 / 格式错误 都会抛 JwtException
        return jws.getPayload();
    }
}
```

几个关键点：

- `.signWith(KEY)` 不显式指定算法时，JJWT 会按密钥长度自动匹配合适的算法；需要固定算法就写成 `.signWith(KEY, Jwts.SIG.HS256)`；
- `parseToken` 不用自己判断有效性，「签名不对」「令牌过期」「格式错误」三类问题都会以 `JwtException` 的子类抛出，调用方 catch 住即可；
- `getBytes(StandardCharsets.UTF_8)` 显式指定编码，避免不同机器默认编码不一致导致密钥字节不同、令牌互相验不过。

> [!WARNING]
> - **密钥绝不能硬编码在代码里**，上线等于把钥匙挂在门上。至少要挪到 `application.yml`，生产环境再换成环境变量（做法同 2.6.3 的 AccessKey）；
> - **有效期不宜过长**。24 小时是本项目的教学取值：令牌一旦签发，服务端无法主动作废，只能等它自然过期。有效期越长，用户「退出登录」后手里的令牌还能继续用。生产上通常 1~2 小时，关键操作还需二次校验；
> - HS256 要求密钥**不少于 32 字节**，随手写个短字符串，启动时会直接抛 `WeakKeyException`。

#### （3）登录接口的完整实现

有了工具类，登录接口要做的事就三件：**校验用户名密码 → 生成令牌 → 连同用户信息一起返回**。

Controller 侧仍是普通表单提交，形参名与表单的 `name` 一致、不需要任何注解（见 2.5）：

```java
@Slf4j
@RestController
public class LoginController {

    @Autowired
    private EmpService empService;

    /**
     * 登录：前端以 application/x-www-form-urlencoded 提交用户名和密码
     */
    @PostMapping("/login")
    public Result login(Emp emp) {
        log.info("登录请求：用户名 {}", emp.getUsername());
        LoginInfo loginInfo = empService.login(emp);
        return Result.success(loginInfo);
    }
}
```

Service 层把校验、生成令牌、组装返回值串起来：

```java
@Override
public LoginInfo login(Emp emp) {
    // 1. 用户名与密码一起交给数据库匹配，匹配不上就查不出数据
    Emp empLogin = empMapper.selectUserByUsername(emp);
    if (empLogin == null) {
        // 2. 校验失败：抛业务异常，由第七章的全局异常处理器统一转成 Result.error 返回
        throw new BusinessException("用户名或密码错误");
    }

    // 3. 校验通过，把用户信息写进 JWT 载荷
    Map<String, Object> claims = new HashMap<>();
    claims.put("id", empLogin.getId());
    claims.put("username", empLogin.getUsername());

    // 4. 生成令牌，与令牌一起返回给前端
    String token = JwtUtils.generateToken(claims);
    return new LoginInfo(empLogin.getId(), empLogin.getUsername(), empLogin.getName(), token);
}
```

对应的 Mapper 查询——密码不写在校验代码里，而是**作为查询条件直接交给数据库匹配**：

```java
@Mapper
public interface EmpMapper {

    /** 根据用户名和密码查询员工，用于登录校验 */
    @Select("select * from emp where username = #{username} and password = #{password}")
    Emp selectUserByUsername(Emp emp);
}
```

`#{username}`、`#{password}` 取的都是 `Emp` 对象的属性（见 3.2.2）。方法名叫 `selectUserByUsername` 却带着密码条件，实现与命名略有出入，按语义更贴切的名字是 `login`。

登录成功后返回的对象是 `LoginInfo`，除了基本资料，重点是 `token`：

```java
@Data
public class LoginInfo {

    private Integer id;
    private String username;
    private String name;
    private String token;   // 后续所有请求都要带上它
}
```

> [!WARNING]
> - **失败时不要 `return null`**。Controller 拿到 `null` 会返回一个 HTTP 200 + 空响应体，前端按 `code` 判断就彻底懵了。正确做法是抛出业务异常（这里的 `BusinessException`），由[[#七、全局异常处理器|全局异常处理器]]统一转成 `Result.error`，前端拿到的结构与其他接口完全一致；
> - **载荷字段名要和校验端一致**。这里放的是 `id`，那么 8.4 的拦截器里就必须用 `claims.get("id", Integer.class)` 取值。写成 `userId` 会得到 `null`，而 `null` 又会被当成合法用户 id 继续放行——这种 bug 排查起来特别费时间；
> - **别让密码跟着 `select *` 跑到前端去**。上面这条 SQL 查出的 `Emp` 里带着 `password`，一旦被 Controller 直接返回，密码就明文发到了浏览器。数据库里存的也应该是**哈希值**而非明文（且不要用 MD5 这种能被彩虹表反查的算法）。给 `Emp` 的 `password` 加 `@JsonIgnore`，或另定义一个不含密码的 VO 再返回。

#### （4）前端如何携带令牌

登录之后前端要做两件事：把令牌**存起来**，并在每次请求前**自动塞进请求头**。以 axios 为例：

```js
// 1. 登录成功后，把令牌存进 localStorage
localStorage.setItem('token', res.data.data.token);

// 2. 在 axios 请求拦截器中统一携带
axios.interceptors.request.use(config => {
  const token = localStorage.getItem('token');
  if (token) config.headers.token = token;
  return config;
});
```

于是服务端后续收到的每个请求都长这样：

```http
GET /emps?page=1&pageSize=10 HTTP/1.1
token: eyJhbGciOiJIUzI1NiJ9.eyJpZCI6MSwidXNlcm5hbWUiOiJhZG1pbiJ9.3nG1H_aG...
```

**服务端要做的事情也因此变了**：不再是「判断这次请求是不是来自一个已登录的人」，而是「判断这个令牌是不是我签发的、还有没有过期」。这正是接下来两小节要解决的问题——**谁来统一做这个校验**？如果每个 Controller 方法里各写一遍，早晚会有人漏掉。

> [!TIP]
> 前端与后端分属不同端口，属于跨域请求，还需要后端开启 CORS（在 `WebConfig` 中重写 `addCorsMappings`）并把 `token` 加入允许的请求头，否则浏览器会直接把请求拦下来。

## 8.3 过滤器 Filter

令牌要靠后端统一校验，Spring 提供了两种横切机制：**过滤器 Filter** 与**拦截器 Interceptor**。两者都能在请求到达业务代码之前执行校验，本节先讲过滤器，下一节再讲拦截器，最后对比二者差异。

**Filter** 是 Servlet 规范定义的**三大组件之一**（Servlet、Filter、Listener），属于 Servlet 层，Tomcat 原生支持：只要配置了过滤器，访问 web 服务器上的任何资源都必须先经过它，处理完才会到达目标资源。

> [!IMPORTANT]
> 因为 Filter 属于 Servlet 层，**它拦截的范围比 Spring 的 Controller 广得多**：静态资源、`/error` 错误页、其他 Servlet 都会经过它。这是它和拦截器最本质的区别之一，8.4.5 会详细对比。

### 8.3.1 快速入门

在 `filter` 包下创建 `Filter` 的实现类，并重写它的三个方法：

```java
@Slf4j
@WebFilter(urlPatterns = "/*")   // 配置过滤器要拦截的请求路径
public class DemoFilter implements Filter {

    //初始化方法, web服务器启动, 创建Filter实例时调用, 只调用一次
    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        log.info("init ...");
    }

    //拦截到请求时,调用该方法,可以调用多次
    @Override
    public void doFilter(ServletRequest servletRequest, ServletResponse servletResponse, FilterChain chain)
            throws IOException, ServletException {
        log.info("拦截到了请求...");
        //放行
        chain.doFilter(servletRequest, servletResponse);
    }

    //销毁方法, web服务器关闭时调用, 只调用一次
    @Override
    public void destroy() {
        log.info("destroy ...");
    }
}
```

| 方法 | 调用时机 | 调用次数 | 说明 |
| --- | --- | --- | --- |
| `init()` | 服务器启动、创建 Filter 实例时 | 1 次 | 适合做初始化，如读取配置、预热缓存 |
| `doFilter()` | 每次拦截到请求 | 多次 | **唯一能拦截逻辑的地方**，代码写在这里 |
| `destroy()` | 服务器关闭时 | 1 次 | 做资源释放 |

`@WebFilter` 用来声明这是一个过滤器，并通过 `urlPatterns` 指定它要拦截哪些路径。不过光加 `@WebFilter` 还不够——Spring Boot 默认并不扫描 Servlet 组件，必须在**启动类**上再加一个 `@ServletComponentScan` 开启支持：

```java
@ServletComponentScan     // 开启 SpringBoot 项目对 Servlet 组件（Filter / Servlet / Listener）的支持
@SpringBootApplication
public class TliasSystemBackEndApplication {

    public static void main(String[] args) {
        SpringApplication.run(TliasSystemBackEndApplication.class, args);
    }
}
```

> [!WARNING]
> **没有 `@ServletComponentScan`，`@WebFilter` 就只是一个普通注解，过滤器完全不会生效**，而且启动时没有任何报错——这是排查「过滤器怎么没拦住」时最常见的原因。

### 8.3.2 放行：FilterChain

过滤器最核心的概念是**放行**。`doFilter()` 的第三个参数 `chain` 就是 `FilterChain`（过滤器链），只有调用它的 `chain.doFilter()` 才表示「放行」，请求才能继续访问后面的资源。于是 `doFilter()` 里的代码天然分成两段：

```java
@Override
public void doFilter(ServletRequest req, ServletResponse resp, FilterChain chain)
        throws IOException, ServletException {
    // ===== 放行之前的逻辑：写在 chain.doFilter() 之前 =====
    log.info("准备放行...");
    chain.doFilter(req, resp);   // 放行
    // ===== 放行之后的逻辑：写在 chain.doFilter() 之后 =====
    log.info("已返回");
}
```

| 位置 | 语义 |
| --- | --- |
| `chain.doFilter()` **之前** | 请求还没到目标资源，可以在这里做校验、拦截 |
| `chain.doFilter()` **之后** | 目标资源已执行完、响应正在回写，可以在这里做清理、统计耗时 |

> [!IMPORTANT]
> 「不放行」意味着**不调用** `chain.doFilter()`，同时自己往响应里写出内容。少了 `chain.doFilter()` 请求就到不了目标资源，多调一次又等于放行——两者只能二选一。

### 8.3.3 拦截路径

`@WebFilter` 的 `urlPatterns` 用的是 **Servlet 规范的 URL 匹配规则**：

| 拦截路径 | `urlPatterns` 值 | 含义 |
| --- | --- | --- |
| 拦截具体路径 | `/login` | 只有访问 `/login` 路径时，才会被拦截 |
| 目录拦截 | `/emps/*` | 访问 `/emps` 下的所有资源，都会被拦截 |
| 拦截所有 | `/*` | 访问所有资源，都会被拦截，含 `/depts/1/2` 这样的多级路径 |
| 按扩展名 | `*.jpg` | 拦截所有 jpg 请求，静态资源专属用法 |

> [!WARNING]
> 注意过滤器里的 `/*` **匹配的是所有层级**（Servlet 规范如此），这和拦截器的 `/*` 语义完全不同，后者只匹配一级路径。对比见 8.4.3。

### 8.3.4 执行流程

一次请求经过过滤器的完整过程如下：

![[Pasted image 20261003104118.png]]

翻译成文字，一共五步：

1. 浏览器发起请求，请求先到达 Tomcat；
2. `doFilter()` 被调用，先执行放行前的逻辑；
3. 调用 `chain.doFilter()` 放行，请求进入 Spring 环境，由 `DispatcherServlet` 接收并转给 Controller；
4. Controller 方法执行完毕，响应沿原路返回，再次回到 `doFilter()` 中执行放行后的逻辑；
5. 响应最终写回浏览器。

### 8.3.5 过滤器链

项目中配置多个过滤器时，它们会按先后顺序串成一条**过滤器链**，请求依次穿过每一个过滤器。链上「放行前」的逻辑按注册顺序执行，「放行后」的逻辑则按相反顺序执行（像洋葱一样层层包裹）：

![[Pasted image 20261003104358.png]]

对于 `@WebFilter` 注册的过滤器，**执行顺序取决于过滤器类名字符串的自然排序**。所以不要指望靠调整代码位置来控制执行顺序，那是不受控的；需要明确顺序时，用 `FilterRegistrationBean` 显式注册，或者干脆写在同一个过滤器的 `doFilter()` 里。

### 8.3.6 用过滤器做令牌校验

现在把本章的主线接上：令牌校验就写在 `doFilter()` 的放行之前，校验不通过就**不调用 `chain.doFilter()`**，直接返回 401，请求永远到不了 Controller。

```java
@Slf4j
@WebFilter(urlPatterns = "/*")
public class TokenFilter implements Filter {

    @Override
    public void doFilter(ServletRequest req, ServletResponse resp, FilterChain chain)
            throws IOException, ServletException {
        HttpServletRequest request = (HttpServletRequest) req;
        HttpServletResponse response = (HttpServletResponse) resp;

        // 1. 登录接口放行，否则用户永远拿不到令牌
        if ("/login".equals(request.getRequestURI())) {
            chain.doFilter(req, resp);
            return;
        }

        // 2. 取出请求头中的令牌
        String token = request.getHeader("token");
        if (token == null || token.isBlank()) {
            log.info("请求路径：{}，未携带令牌", request.getRequestURI());
            writeUnauthorized(response, "未登录，请先登录");
            return;
        }

        // 3. 解析并校验令牌：签名不对 / 已过期 / 格式错误 都会抛 JwtException
        try {
            Claims claims = JwtUtils.parseToken(token);
            CurrentHolder.setCurrentId(claims.get("id", Integer.class));
        } catch (JwtException | IllegalArgumentException e) {
            log.info("请求路径：{}，令牌无效：{}", request.getRequestURI(), e.getMessage());
            writeUnauthorized(response, "令牌无效或已过期，请重新登录");
            return;
        }

        // 4. 校验通过，放行；放行之后再清理当前用户信息
        try {
            chain.doFilter(req, resp);
        } finally {
            CurrentHolder.clear();
        }
    }

    /** 以统一的响应结构返回 401 */
    private void writeUnauthorized(HttpServletResponse response, String msg) throws IOException {
        response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
        response.setContentType("application/json;charset=UTF-8");
        response.getWriter().write("{\"code\":0,\"msg\":\"" + msg + "\"}");
    }
}
```

这里有三个与后面拦截器版本形成鲜明对比的点，务必留意：

- **放行方式是 `return`**：过滤器没有返回值，「不放行」就是不调用 `chain.doFilter()` 然后 `return`；
- **清理逻辑写在 `doFilter()` 的 `finally` 里**，而拦截器有专门的 `afterCompletion()` 回调（见 8.4.1）；
- **`TokenFilter` 不是 Spring 管理的 bean**，所以无法用 `@Autowired` 注入 `ObjectMapper` 来序列化 JSON，只能自己拼字符串。

> [!NOTE]
> `urlPatterns` **只有「拦截哪些路径」，没有「排除哪些路径」**的语法。所以登录接口要么像上面这样在代码里 `if` 判断，要么把 `urlPatterns` 写细成 `/emps/*`。这一点上拦截器有现成的 `excludePathPatterns`，用起来干净得多。

## 8.4 拦截器 Interceptor

上一节用过滤器完成了令牌校验，但它毕竟工作在 Servlet 层，有诸多不便：不能注入 bean、不能排除路径、连静态资源都要拦一遍。**拦截器 Interceptor** 是 Spring MVC 提供的机制，专门用来**动态拦截 Controller 方法的执行**，位置更靠内、配置也更灵活，实际项目中的登录校验通常都用它。

### 8.4.1 快速入门

在 `interceptor` 包下创建 `HandlerInterceptor` 的实现类，重写它常用的三个方法：

```java
@Slf4j
@Component   // 交由 Spring 管理，才能在 WebConfig 中注入
public class DemoInterceptor implements HandlerInterceptor {

    //目标资源方法执行前执行。 返回true：放行，返回false：不放行
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler)
            throws Exception {
        log.info("preHandle ....");
        return true; //true表示放行
    }

    //目标资源方法执行后执行，此时 Controller 已返回、视图尚未渲染
    @Override
    public void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler,
                           ModelAndView modelAndView) throws Exception {
        log.info("postHandle ...");
    }

    //视图渲染完毕后执行，最后执行
    @Override
    public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler,
                                Exception ex) throws Exception {
        log.info("afterCompletion ....");
    }
}
```

| 方法 | 执行时机 | 返回值 | 说明 |
| --- | --- | --- | --- |
| `preHandle()` | Controller 方法执行**前** | `boolean` | 返回 `false` 直接中断请求，Controller 根本不会执行 |
| `postHandle()` | Controller 方法执行**后** | `void` | 只能拿到 handler 和 `ModelAndView`，响应尚未提交 |
| `afterCompletion()` | 请求处理**彻底结束**后 | `void` | `ex` 非空说明本次请求抛过异常，常用于清理资源 |

> [!IMPORTANT]
> `afterCompletion()` 是释放资源的**可靠时机**。`postHandle()` 在 `@RestController` 返回 JSON 的场景下几乎拿不到可用的 `ModelAndView`，而且 Controller 抛异常时它根本不会被执行；只有 `afterCompletion()` 一定会被触发（前提是 `preHandle()` 返回了 `true`）。

写好的拦截器只是「一个类」，还必须在 `config` 包下的配置类 `WebConfig` 中注册才算启用。`WebConfig` 实现 `WebMvcConfigurer` 接口，重写 `addInterceptors` 方法：

```java
@Configuration
public class WebConfig implements WebMvcConfigurer {

    @Autowired
    private DemoInterceptor demoInterceptor;

    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        //注册自定义拦截器对象
        registry.addInterceptor(demoInterceptor).addPathPatterns("/**");
    }
}
```

### 8.4.2 令牌校验

把登录校验的逻辑搬进 `preHandle()`，步骤与 8.3.6 的过滤器版本完全一致，只是换成拦截器的写法：

```java
@Slf4j
@Component
public class TokenInterceptor implements HandlerInterceptor {

    @Autowired
    private ObjectMapper objectMapper;   // TokenFilter 不是 Spring bean 注入不了；拦截器可以

    @Override
    public boolean preHandle(HttpServletRequest req, HttpServletResponse resp, Object handler)
            throws Exception {
        // 1. 只拦截 Controller 方法，静态资源等直接放行
        if (!(handler instanceof HandlerMethod)) {
            return true;
        }

        // 2. 取出请求头中的令牌
        String token = req.getHeader("token");
        if (token == null || token.isBlank()) {
            log.info("请求路径：{}，未携带令牌", req.getRequestURI());
            writeUnauthorized(resp, "未登录，请先登录");
            return false;
        }

        // 3. 解析并校验令牌：签名不对 / 已过期 / 格式错误 都会抛 JwtException
        try {
            Claims claims = JwtUtils.parseToken(token);
            CurrentHolder.setCurrentId(claims.get("id", Integer.class));
        } catch (JwtException | IllegalArgumentException e) {
            log.info("请求路径：{}，令牌无效：{}", req.getRequestURI(), e.getMessage());
            writeUnauthorized(resp, "令牌无效或已过期，请重新登录");
            return false;
        }

        // 4. 校验通过，放行请求
        return true;
    }

    @Override
    public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex)
            throws Exception {
        // 请求处理完成（包括抛异常的情况），清理当前登录用户
        CurrentHolder.clear();
    }

    /** 以统一的 Result 结构返回 401 */
    private void writeUnauthorized(HttpServletResponse response, String msg) throws IOException {
        response.setStatus(HttpServletResponse.SC_UNAUTHORIZED);
        response.setContentType("application/json;charset=UTF-8");
        // 复用第一章的 Result，前端拿到的响应结构与其他接口完全一致
        response.getWriter().write(objectMapper.writeValueAsString(Result.error(msg)));
    }
}
```

注册时，拦截器比过滤器多出一个很实用的能力——**排除路径**：

```java
@Configuration
public class WebConfig implements WebMvcConfigurer {

    @Autowired
    private TokenInterceptor tokenInterceptor;

    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        //设置拦截器拦截的请求路径（/** 表示任意级路径），排除登录请求
        registry.addInterceptor(tokenInterceptor)
                .addPathPatterns("/**")
                .excludePathPatterns("/login");
    }
}
```

这样 `/login` 就不必在代码里 `if` 判断，注册时排除即可，校验代码里只剩「取令牌 → 验令牌 → 放行或 401」这条主线。

> [!TIP]
> - 使用 Knife4j / OpenAPI 文档时，除了 `/login`，`/doc.html`、`/swagger-ui/**`、`/v3/api-docs/**` 也要加进 `excludePathPatterns`，否则接口文档页面自己先被 401 拦住了；
> - `TokenInterceptor` 类上有 `@Component` 却没有在 `WebConfig` 里注册，是不会被执行的——**创建了 bean ≠ 启用了拦截器**。

### 8.4.3 拦截路径

注册时通过 `addPathPatterns("要拦截的路径")` 指定拦截哪些资源，通过 `excludePathPatterns("不拦截的路径")` 指定排除哪些资源。拦截器用的是 **Ant 风格路径表达式**，切记 `*` 与 `**` 的区别：

| 拦截路径 | 含义 | 举例 |
| --- | --- | --- |
| `/*` | 一级路径 | 能匹配 `/depts`、`/emps`、`/login`，不能匹配 `/depts/1` |
| `/**` | 任意级路径 | 能匹配 `/depts`、`/depts/1`、`/depts/1/2` |
| `/depts/*` | `/depts` 下的一级路径 | 能匹配 `/depts/1`，不能匹配 `/depts`、`/depts/1/2` |
| `/depts/**` | `/depts` 下的任意级路径 | 能匹配 `/depts`、`/depts/1`、`/depts/1/2`，不能匹配 `/emps/1` |

> [!WARNING]
> 这是本节最容易踩的坑：**过滤器的 `/*` 匹配所有层级，拦截器的 `/*` 只匹配一级**。在拦截器里想写「全部拦截」，必须是 `/**` 而不是 `/*`。对比 8.3.3 的过滤器路径表。

### 8.4.4 执行流程

把过滤器和拦截器串起来看，一次请求的完整链路如下：

![[Pasted image 20261003110138.png]]

用文字走一遍：

1. 浏览器访问部署在 web 服务器上的应用，请求先被**过滤器**拦截，执行放行前的逻辑；
2. 过滤器放行后，请求进入 Spring 环境。由于 Tomcat 并不认识我们编写的 Controller 程序，只认识 Servlet 程序，所以 Spring Web 环境中提供了一个非常核心的 Servlet：**DispatcherServlet（前端控制器）**，所有请求都会先到 DispatcherServlet，再由它转给 Controller；
3. 定义了拦截器后，请求在执行 Controller 方法**之前**被拦截器拦下，执行 `preHandle()`：返回 `true` 才继续访问 Controller 中的方法，返回 `false` 则不放行，Controller 中的方法也不会执行；
4. Controller 中的方法执行完毕后，再回过来执行 `postHandle()` 与 `afterCompletion()`，然后返回 DispatcherServlet；
5. 最后回到过滤器中放行之后的这一部分逻辑，执行完毕，最终给浏览器响应数据。

### 8.4.5 拦截器与过滤器的区别

两者的**执行流程高度相似**——都是「请求到达 → 前置逻辑 → 目标资源 → 后置逻辑 → 响应返回」——但它们属于不同的层，关注点也不同：

| 对比项 | 过滤器 Filter | 拦截器 Interceptor |
| --- | --- | --- |
| 所属规范 / 层次 | Servlet 规范，Tomcat 原生支持 | Spring MVC 提供，属于 Spring 框架 |
| 拦截接口 | `jakarta.servlet.Filter` | `HandlerInterceptor` |
| 拦截范围 | **所有资源**：Controller、静态资源、`/error`、其他 Servlet | **Spring MVC 管理的资源**：只有 Controller 等 handler 会经过，静态资源不拦 |
| 放行方式 | 显式调用 `chain.doFilter()`；不调用即不放行 | `preHandle()` 返回 `true` 放行、`false` 中断 |
| 路径配置 | `@WebFilter(urlPatterns = "/*")`，**只能指定拦截哪些，没有排除语法** | `addPathPatterns` / `excludePathPatterns`，**两个都能配** |
| 路径匹配规则 | Servlet 规范：`/*` 匹配所有层级 | Ant 表达式：`/*` 只匹配一级，`/**` 才是任意级 |
| 是否 Spring bean | **否**，由 Servlet 容器创建，`@Autowired` 注入不生效 | **是**，加 `@Component` 即可注入任意依赖 |
| 后置回调 | 写在 `chain.doFilter()` 之后，用 `finally` 兜底 | 有专门的 `postHandle()`、`afterCompletion()` 回调 |
| 执行顺序 | 类名字符串自然排序 | 注册顺序（`addInterceptor` 的调用顺序） |

一句话总结各自的适用场景：

> [!IMPORTANT]
> - **过滤器**工作在最外层，适合处理**编码、跨域、字符集**这类所有请求都要做的通用事情；
> - **拦截器**工作在 Controller 门口，能排除静态资源、能注入业务组件，适合处理**登录校验、权限控制**这类只针对业务接口的事情。
>
> **本项目的登录校验用拦截器实现**，原因就落在最后两行：它能直接 `excludePathPatterns("/login")`，还能注入 `ObjectMapper` 复用统一的 `Result` 结构；换成过滤器既要在代码里手写 JSON 字符串，又要自己判断 `/login`。

## 8.5 CurrentHolder：把当前登录用户传给业务层

拦截器校验通过后只做了一件事：把令牌里的用户 id 放进 `CurrentHolder`。它之所以需要存在，是因为**业务层常常要知道「当前操作人是谁」**（比如新增员工要记录创建人、更新时要防止越权），而 Controller 方法的形参里只有业务参数，拿不到令牌。

`CurrentHolder` 的实现只用了一个 `ThreadLocal`：一个请求由一个线程处理，用 `ThreadLocal` 存「当前线程的用户 id」，业务层随时能静态取到，不同请求之间又天然隔离。

```java
public class CurrentHolder {

    private static final ThreadLocal<Integer> CURRENT_ID = new ThreadLocal<>();

    /** 保存当前登录用户的 id */
    public static void setCurrentId(Integer id) {
        CURRENT_ID.set(id);
    }

    /** 获取当前登录用户的 id */
    public static Integer getCurrentId() {
        return CURRENT_ID.get();
    }

    /** 清除 */
    public static void clear() {
        CURRENT_ID.remove();
    }
}
```

业务层直接静态调用即可：

```java
@Override
public void save(Emp emp) {
    // 联调时打印当前操作人，正式项目可换成写入操作日志
    log.info("当前操作人：{}", CurrentHolder.getCurrentId());
    empMapper.save(emp);
}
```

> [!WARNING]
> `ThreadLocal` 所在的线程是**被复用的**：请求结束后线程不会销毁，而是还给线程池等待下一个请求。所以**必须**在请求结束时调用 `CurrentHolder.clear()`，否则下一个请求复用同一线程时，会读到上一个用户的 id——这是越权，比不校验还危险。清理动作放在拦截器的 `afterCompletion()` 里最稳妥（过滤器则放在 `doFilter()` 的 `finally` 中）。
