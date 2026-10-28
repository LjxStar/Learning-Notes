# 五、Redis

## 5.1 Redis 基础

### 5.1.1 什么是 Redis

**Redis（Remote Dictionary Server）** 是一个开源的、基于内存的高性能 **Key-Value** 型 NoSQL 数据库。它的数据直接存储在内存中，读写速度远超传统磁盘数据库，常用于缓存、减轻数据库压力、提升系统响应速度等场景。

在苍穹外卖项目中，Redis 并不是用来替代 MySQL，而是作为**缓存中间件**配合使用，常见应用场景有：

- **缓存**：缓存菜品、套餐、分类等高频访问的数据，减少对数据库的查询
- **验证码**：利用 `SETEX` 存储带过期时间的短信验证码，超时自动失效
- **购物车**：使用 `Hash` 结构存储用户购物车，支持单独增删改某个菜品
- **排行榜**：利用 `ZSet` 按销量、热度等分值实现热销榜
- **分布式锁**：利用 `SETNX` 实现多实例环境下的资源互斥
- **消息队列**：利用 `List` 的阻塞命令实现简单的异步任务处理

### 5.1.2 Redis 特点

- **基于内存，性能极高**：数据存放在内存中，读写速度极快，适合高并发场景
- **支持多种数据类型**：内置 String、Hash、List、Set、ZSet，能灵活应对不同业务需求
- **单线程模型（核心命令）**：Redis 核心命令由单线程执行，避免多线程竞争。Redis 6.0+ 引入 I/O 多线程，但命令执行仍是单线程
- **支持过期时间**：可为任意 key 设置 TTL，到期自动删除，适合临时数据
- **支持持久化**：通过 RDB（快照）和 AOF（追加日志）将内存数据持久化到磁盘，防止重启丢失
- **高可用与扩展**：支持主从复制、哨兵、Redis Cluster 等

### 5.1.3 安装与配置

#### （1）启动 Redis 服务端

Windows 环境下，进入 Redis 安装目录，执行：

```bash
redis-server.exe redis.windows.conf
```

![[Pasted image 20261026191812.png]]

> 启动后不要关闭该窗口，按 `Ctrl + C` 可停止服务。

#### （2）客户端连接

新开终端，进入同一目录，执行：

```bash
redis-cli.exe -h localhost -p 6379 -a 123456
```

参数说明：
- `-h`：主机地址，默认 `127.0.0.1`
- `-p`：端口号，默认 `6379`
- `-a`：密码（如未设置可省略）

本机默认端口且已设置密码时也可简写：`redis-cli.exe -a 123456`

#### （3）设置密码

编辑 `redis.windows.conf`，修改：

```text
requirepass 123456
```

重启后未带密码连接会提示 `NOAUTH Authentication required`。

#### （4）图形化工具

推荐使用以下工具便于调试：
- **RedisInsight**（官方推荐，免费跨平台）
- **Another Redis Desktop Manager**（开源轻量）
- **Redis Desktop Manager**（常用）

### 5.1.4 常用数据类型与命令

> 命令不区分大小写，但 **key 严格区分大小写**。可用 `HELP <命令名>` 查看用法。

#### （1）String（字符串）

最基础类型，value 可为字符串、数字、JSON 等。

| 命令                                | 说明                                        |
| --------------------------------- | ------------------------------------------- |
| `SET key value`                   | 设置值，若 key 已存在则覆盖                  |
| `GET key`                         | 获取值，不存在返回 `(nil)`                  |
| `SETEX key seconds value`         | 设置值并指定过期时间（秒）                  |
| `SETNX key value`                 | 仅 key 不存在时才设置，常用于分布式锁      |
| `INCR key`                        | 数值自增 1（value 必须为整数）              |
| `DECR key`                        | 数值自减 1                                  |
| `INCRBY key increment`            | 按指定步长自增                              |
| `MSET k1 v1 ...`                  | 批量设置                                    |
| `MGET k1 k2 ...`                  | 批量获取                                    |

```text
SET name tom
GET name
SETEX code:13800138000 60 1234
INCR article:like:1
```

**适用场景：** 验证码、计数器、Token、JSON 缓存等。

#### （2）Hash（哈希）

适合存储对象，能单独更新某个字段。

| 命令                        | 说明                        |
| ------------------------- | --------------------------- |
| `HSET key field value`    | 设置字段                    |
| `HGET key field`          | 获取字段                    |
| `HGETALL key`             | 获取所有字段和值            |
| `HMGET key field1 ...`    | 批量获取字段                |
| `HKEYS key`               | 获取所有字段名              |
| `HVALS key`               | 获取所有字段值              |
| `HLEN key`                | 字段数量                    |
| `HEXISTS key field`       | 判断字段是否存在            |
| `HDEL key field ...`      | 删除字段                    |

```text
HSET cart:1 dish:1001 2
HSET cart:1 dish:1002 1
HGETALL cart:1
```

**适用场景：** 购物车、用户资料等。

#### （3）List（列表）

有序、可重复，双向链表结构。

| 命令                                    | 说明                                                |
| ------------------------------------- | --------------------------------------------------- |
| `LPUSH key value ...`                 | 左端插入                                            |
| `RPUSH key value ...`                 | 右端插入                                            |
| `LPOP key`                            | 左端弹出（非阻塞）                                  |
| `RPOP key`                            | 右端弹出（非阻塞）                                  |
| `LRANGE key start stop`               | 范围查询，`0 -1` 表示全部                          |
| `LLEN key`                            | 列表长度                                            |
| `BLPOP key timeout`                   | 阻塞式左弹出，超时返回 `(nil)`                    |
| `BRPOP key timeout`                   | 阻塞式右弹出                                        |

**适用场景：** 消息队列、最新列表等。

#### （4）Set（集合）

无序、不可重复，支持交并差集。

| 命令                            | 说明                    |
| ----------------------------- | ----------------------- |
| `SADD key member ...`         | 添加成员                |
| `SMEMBERS key`                | 获取全部成员            |
| `SISMEMBER key member`        | 判断是否存在            |
| `SCARD key`                   | 成员数量                |
| `SREM key member ...`         | 移除成员                |
| `SINTER key1 key2`            | 交集                    |
| `SUNION key1 key2`            | 并集                    |
| `SDIFF key1 key2`             | 差集                    |

**适用场景：** 标签、共同关注、去重等。

#### （5）ZSet（有序集合）

成员唯一，每个成员带 `score` 按分值排序。

| 命令                                      | 说明                                        |
| --------------------------------------- | ------------------------------------------- |
| `ZADD key score member ...`             | 添加或更新成员及分值                        |
| `ZSCORE key member`                     | 获取分值                                    |
| `ZINCRBY key increment member`          | 分值自增                                    |
| `ZRANGE key start stop [WITHSCORES]`    | 升序查询                                    |
| `ZREVRANGE key start stop [WITHSCORES]` | 降序查询                                    |
| `ZRANK key member`                      | 升序排名（0-based）                        |
| `ZREVRANK key member`                   | 降序排名（0-based）                        |
| `ZCOUNT key min max`                    | 统计区间成员数                              |
| `ZREM key member ...`                   | 移除成员                                    |
| `ZCARD key`                             | 成员总数                                    |

**适用场景：** 排行榜、热度排序等。

#### （6）通用命令

| 命令                     | 说明                                                  |
| ---------------------- | ----------------------------------------------------- |
| `KEYS pattern`         | 按模式查找 key，**O(N)**，生产环境慎用              |
| `SCAN cursor [MATCH pattern] [COUNT count]` | 增量遍历 key，**生产环境推荐**           |
| `EXISTS key`           | 判断是否存在                                          |
| `TYPE key`             | 查看数据类型                                          |
| `DEL key ...`          | 删除 key                                              |
| `EXPIRE key seconds`   | 设置过期时间                                          |
| `TTL key`              | 剩余时间，`-1` 永不过期，`-2` 不存在              |
| `PERSIST key`          | 移除过期时间                                          |
| `SELECT index`         | 切换数据库（0~15）                                    |
| `FLUSHDB`              | 清空当前库（**慎用**）                                |
| `FLUSHALL`             | 清空所有库（**极度慎用**）                            |
| `INFO`                 | 查看服务器信息                                        |

> Redis 默认有 16 个数据库（`0` ~ `15`），项目中一般只用 `0` 号库。

## 5.2 在 Java 中操作 Redis（Spring Data Redis）

Spring Boot 中推荐使用 **Spring Data Redis** 操作 Redis。它对底层 Jedis/Lettuce 封装，提供 `RedisTemplate` 和 `StringRedisTemplate`，集成最为顺手。

### 5.2.1 快速入门

#### （1）引入依赖

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-data-redis</artifactId>
</dependency>
```

> 默认使用 **Lettuce** 客户端，基于 Netty，线程安全且性能较好。

#### （2）配置连接信息

```yaml
spring:
  data:        # Spring Boot 3.x
    redis:
      host: 127.0.0.1
      port: 6379
      password: 123456
      database: 0
```

> Spring Boot 2.x 用 `spring.redis.*`，Spring Boot 3.x 用 `spring.data.redis.*`。

#### （3）自定义序列化

默认 `RedisTemplate` 用 JDK 序列化，存入的数据在图形化工具中不可读。一般改成 key 用 String、value 用 JSON 序列化。

```java
@Configuration
public class RedisConfig {

    @Bean
    public RedisTemplate<String, Object> redisTemplate(RedisConnectionFactory redisConnectionFactory) {
        RedisTemplate<String, Object> redisTemplate = new RedisTemplate<>();
        redisTemplate.setConnectionFactory(redisConnectionFactory);

        // Key、HashKey
        StringRedisSerializer stringSerializer = new StringRedisSerializer();
        redisTemplate.setKeySerializer(stringSerializer);
        redisTemplate.setHashKeySerializer(stringSerializer);

        // Value、HashValue
        ObjectMapper objectMapper = new ObjectMapper();
        objectMapper.registerModule(new JavaTimeModule());
        objectMapper.disable(SerializationFeature.WRITE_DATES_AS_TIMESTAMPS);
        objectMapper.activateDefaultTyping(
                LaissezFaireSubTypeValidator.instance,
                ObjectMapper.DefaultTyping.NON_FINAL,
                JsonTypeInfo.As.PROPERTY
        );

        GenericJackson2JsonRedisSerializer jsonSerializer = new GenericJackson2JsonRedisSerializer(objectMapper);
        redisTemplate.setValueSerializer(jsonSerializer);
        redisTemplate.setHashValueSerializer(jsonSerializer);
        redisTemplate.afterPropertiesSet();

        return redisTemplate;
    }
}
```

> [!tip] 建议
> 实际项目中更常用 `StringRedisTemplate`。配合 `ObjectMapper` 手动将对象转 JSON 存储、再反序列化，既可读性好又能减少类型信息冗余。

### 5.2.2 常用操作

注入 `StringRedisTemplate`：

```java
@Autowired
private StringRedisTemplate stringRedisTemplate;
```

#### （1）String — `opsForValue()`

| Redis 命令                | Spring 方法                                                   | 说明                                |
| ------------------------- | ------------------------------------------------------------ | ----------------------------------- |
| `SET key value`           | `opsForValue().set(key, value)`                              | 设置值                              |
| `GET key`                 | `opsForValue().get(key)`                                     | 获取值，key 不存在返回 `null`      |
| `SETEX key seconds value` | `opsForValue().set(key, value, seconds, TimeUnit.SECONDS)`   | 设置并指定过期时间                |
| `SETNX key value`         | `opsForValue().setIfAbsent(key, value)`                      | 仅 key 不存在时设置              |
| `SETNX + EXPIRE`          | `opsForValue().setIfAbsent(key, value, timeout, TimeUnit.SECONDS)` | 原子性设置并指定过期时间（推荐） |
| `INCR key`                | `opsForValue().increment(key)`                               | 自增 1                              |
| `INCRBY key delta`        | `opsForValue().increment(key, delta)`                        | 按步长自增                          |
| `MSET`                    | `opsForValue().multiSet(Map<>)`                              | 批量设置                            |
| `MGET`                    | `opsForValue().multiGet(List<>)`                             | 批量获取                            |

```java
ValueOperations<String, String> ops = stringRedisTemplate.opsForValue();
// 验证码（60秒有效）
ops.set("code:13800138000", "123456", 60, TimeUnit.SECONDS);
// 浏览量自增
ops.increment("view:article:1001");
```

#### （2）Hash — `opsForHash()`

| Redis 命令             | Spring 方法                                      | 说明                        |
| ---------------------- | ----------------------------------------------- | --------------------------- |
| `HSET key field value` | `opsForHash().put(key, field, value)`           | 设置字段                    |
| `HGET key field`       | `opsForHash().get(key, field)`                  | 获取字段                    |
| `HGETALL key`          | `opsForHash().entries(key)`                     | 获取全部，返回 `Map`      |
| `HDEL key field`       | `opsForHash().delete(key, field)`               | 删除字段                    |
| `HLEN key`             | `opsForHash().size(key)`                        | 字段数量                    |

```java
HashOperations<String, Object, Object> hashOps = stringRedisTemplate.opsForHash();
hashOps.put("cart:1001", "dishId:5", "2");
hashOps.put("cart:1001", "dishId:8", "1");
Map<Object, Object> cart = hashOps.entries("cart:1001");
```

#### （3）List — `opsForList()`

| Redis 命令              | Spring 方法                                                 | 说明                      |
| ----------------------- | ---------------------------------------------------------- | ------------------------- |
| `LPUSH/RPUSH`           | `leftPush/rightPush(key, value)`                            | 左/右插入                |
| `LPOP/RPOP`             | `leftPop/rightPop(key)`                                    | 左/右弹出（非阻塞）      |
| `BLPOP/BRPOP`           | `leftPop/rightPop(key, timeout, TimeUnit)`                | 左/右弹出（阻塞式）      |
| `LRANGE key start stop` | `range(key, start, stop)`                                  | 范围查询                  |
| `LLEN key`              | `size(key)`                                                | 长度                      |

```java
ListOperations<String, String> listOps = stringRedisTemplate.opsForList();
// 生产端入队
listOps.rightPush("order:queue", "order_123");
// 消费端阻塞出队（最多等5秒）
String orderId = listOps.leftPop("order:queue", 5, TimeUnit.SECONDS);
```

#### （4）Set — `opsForSet()`

| Redis 命令                | Spring 方法                                | 说明          |
| ------------------------- | ----------------------------------------- | ------------- |
| `SADD key member`         | `add(key, member...)`                    | 添加成员      |
| `SMEMBERS key`            | `members(key)`                           | 获取全部      |
| `SISMEMBER key member`    | `isMember(key, member)`                  | 判断是否存在  |
| `SCARD key`               | `size(key)`                              | 成员数量      |
| `SREM key member`         | `remove(key, member...)`                 | 移除成员      |
| `SINTER/SUNION/SDIFF`      | `intersect/union/difference(k1,k2)`      | 交并差集      |

```java
SetOperations<String, String> setOps = stringRedisTemplate.opsForSet();
setOps.add("user:1:tags", "川菜", "火锅");
setOps.add("user:2:tags", "川菜", "甜品");
Set<String> common = setOps.intersect("user:1:tags", "user:2:tags");
```

#### （5）ZSet — `opsForZSet()`

| Redis 命令                            | Spring 方法                                                                            | 说明                        |
| ------------------------------------- | ------------------------------------------------------------------------------------- | --------------------------- |
| `ZADD key score member`               | `add(key, member, score)`                                                             | 添加或更新                  |
| `ZSCORE key member`                   | `score(key, member)`                                                                  | 获取分值                    |
| `ZINCRBY key delta member`            | `incrementScore(key, member, delta)`                                                  | 分值自增                    |
| `ZRANGE ... WITHSCORES`               | `rangeWithScores(key, start, stop)`                                                   | 升序查询（带分值）          |
| `ZREVRANGE ... WITHSCORES`            | `reverseRangeWithScores(key, start, stop)`                                            | 降序查询（带分值，常用）    |
| `ZRANK/ZREVRANK`                      | `rank(key, member)` / `reverseRank(key, member)`                                      | 排名（0-based）             |
| `ZREM`                                | `remove(key, member...)`                                                              | 移除成员                    |
| `ZCARD`                               | `zCard(key)`                                                                          | 成员总数                    |

```java
ZSetOperations<String, String> zSetOps = stringRedisTemplate.opsForZSet();
// 销量+1
zSetOps.incrementScore("dish:sales:rank", "dishId:101", 1);
// Top10
Set<ZSetOperations.TypedTuple<String>> top10 = 
        zSetOps.reverseRangeWithScores("dish:sales:rank", 0, 9);
// 排名
Long rank = zSetOps.reverseRank("dish:sales:rank", "dishId:101");
```

#### （6）通用操作

| 方法                                        | 对应命令 | 说明                                                                 |
| ------------------------------------------- | -------- | -------------------------------------------------------------------- |
| `hasKey(key)`                               | `EXISTS` | 判断是否存在                                                        |
| `delete(key)`                               | `DEL`    | 删除 key                                                             |
| `expire(key, timeout, TimeUnit)`             | `EXPIRE` | 设置过期时间                                                        |
| `getExpire(key, TimeUnit.SECONDS)`          | `TTL`    | 获取剩余秒数（`-1` 永不过期，`-2` 不存在）                        |
| `persist(key)`                              | `PERSIST`| 移除过期时间                                                        |
| `type(key)`                                 | `TYPE`   | 获取数据类型                                                        |
| `keys(pattern)`                             | `KEYS`   | 按模式查找（**仅调试用，生产禁止**）                                |
| `scan(ScanOptions)`                         | `SCAN`   | 分批迭代查找（**生产推荐**）                                        |

```java
// 生产环境推荐用 SCAN 遍历
ScanOptions options = ScanOptions.scanOptions().match("dish:*").count(100).build();
try (Cursor<String> cursor = stringRedisTemplate.scan(options)) {
    while (cursor.hasNext()) {
        String key = cursor.next();
        // ...
    }
}
```

> [!warning] 重要提示
> `KEYS` 命令会阻塞 Redis 主线程，数据量较大时极易影响线上服务。遍历 key 时请优先使用 `SCAN`。
