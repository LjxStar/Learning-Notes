# Redis 五种数据类型命令速查

> [!tip] 阅读约定
> - 成对命令只写主命令，变体缩进附在其下：左/右、阻塞/非阻塞、秒/毫秒。
> - 命名规律：`p` 前缀表示毫秒，`at` 后缀表示绝对时间戳，`nx` 表示目标不存在才执行，`x` 后缀表示目标已存在才操作，`store` 表示把结果写入新 key。
> - 过期时间只作用于 key 本身，五种数据类型都没有独立的过期命令。

## 一、通用 Key 操作

### 检索与判断

- `exists key [key ...]`：判断 key 是否存在，返回存在的数量
- `type key`：返回 key 的数据类型
- `keys pattern`：按通配符匹配 key，复杂度 O(N)，生产环境避免使用；需要遍历时改用 `scan`
- `randomkey`：随机返回库中一个 key

### 重命名与迁移

- `rename key newkey`：重命名，newkey 已存在则覆盖
    - `renamenx key newkey`：仅当 newkey 不存在时才重命名
- `move key db`：把 key 迁移到指定数据库

### 删除

- `del key [key ...]`：删除一个或多个 key，不存在的直接忽略

### 过期与生命周期

- `expire key seconds`：设置秒级相对过期
    - `pexpire key milliseconds`：毫秒级相对过期
- `expireat key timestamp`：按秒级时间戳绝对过期
    - `pexpireat key ms-timestamp`：按毫秒级时间戳绝对过期
- `ttl key`：剩余秒数；`-1` 表示永久，`-2` 表示 key 不存在
    - `pttl key`：剩余毫秒数
- `persist key`：移除过期时间，key 永久有效

## 二、String 字符串

value 最大 512 MB，可存文本、整数和二进制数据，是最常用的缓存与计数类型。

### 基础读写

- `set key value`：设置值，存在则覆盖；可附加 `ex`/`px`/`nx`/`xx` 选项
- `get key`：获取值
- `getset key value`：设置新值并返回旧值
- `append key value`：末尾追加内容，返回追加后的长度
- `setrange key offset value`：从 offset 起覆写，超出部分自动补 `\x00`
- `getrange key start end`：截取子串，闭区间，`-1` 表示末尾
- `strlen key`：返回字节长度，中文按 UTF-8 多字节计算

### 批量读写

- `mset key value [key value ...]`：批量设置
- `mget key [key ...]`：批量获取，不存在的返回 `nil`

### 条件写入与带过期写入

- `setnx key value`：key 不存在才设置，常用于分布式锁
    - `msetnx key value [key value ...]`：全部 key 都不存在才成功
- `setex key seconds value`：设置值并附加秒级过期
    - `psetex key milliseconds value`：毫秒级过期

> [!note] `set key value ex seconds nx` 可在一次写入中同时完成「设值 + 过期 + 不存在才写」，相当于 `setex` 与 `setnx` 的组合。

### 数值增减

- `incr key`：自增 1
    - `incrby key increment`：增加指定整数
- `decr key`：自减 1
    - `decrby key decrement`：减去指定整数
- `incrbyfloat key increment`：增加指定浮点数

> [!warning] 整数命令要求值可解析为整数，否则报错；自增越过 `int64` 边界同样报错。

## 三、List 列表

双向链表，元素有序、可重复，两端插入与弹出均为 O(1)，适合队列、栈和时间线。

### 两端写入

- `lpush key value [value ...]`：从头部插入，一次可插入多个
    - `rpush key value [value ...]`：从尾部插入
- `lpushx key value`：仅当列表存在时头部插入
    - `rpushx key value`：仅当列表存在时尾部插入
- `linsert key before|after pivot value`：在基准元素前/后插入

### 弹出与阻塞

- `lpop key [count]`：从头部弹出，`count` 可一次弹出多个（Redis 6.2+）
    - `rpop key [count]`：从尾部弹出
- `blpop key [key ...] timeout`：阻塞式头部弹出，无元素时等待，`0` 表示永久阻塞
    - `brpop key [key ...] timeout`：阻塞式尾部弹出

### 移除与裁剪

- `lrem key count value`：删除匹配元素
    - `count > 0`：从头往后删 count 个
    - `count < 0`：从尾往前删 |count| 个
    - `count = 0`：删除全部匹配
- `ltrim key start stop`：只保留区间内元素，其余全部删除

### 位置调整与搬移

- `lset key index value`：按下标修改元素
- `rpoplpush src dest`：src 尾部弹出并压入 dest 头部，原子操作
    - `brpoplpush src dest timeout`：阻塞版，可用于可靠队列

### 查询

- `lrange key start stop`：取区间元素，`-1` 表示末尾
- `lindex key index`：按下标取元素
- `llen key`：列表长度

## 四、Hash 哈希

field-value 结构，一个 key 下可存多个字段，适合表示对象的多个属性，也能对单个字段独立更新。

### 字段写入

- `hset key field value [field value ...]`：设置一个或多个字段，返回新增字段数
- `hsetnx key field value`：字段不存在才设置
- `hmset key f1 v1 [f2 v2 ...]`：旧版批量设置，已废弃，用 `hset` 代替

### 字段读取

- `hget key field`：取单个字段值
    - `hmget key field [field ...]`：批量取字段值
- `hgetall key`：取全部 field-value
- `hkeys key`：取所有字段名
    - `hvals key`：取所有字段值
- `hlen key`：字段总数
- `hexists key field`：判断字段是否存在
- `hstrlen key field`：字段值的字节长度

### 字段删除

- `hdel key field [field ...]`：删除一个或多个字段

### 数值累加

- `hincrby key field increment`：字段按整数累加
    - `hincrbyfloat key field increment`：字段按浮点数累加

## 五、Set 集合

无序、元素唯一，支持交、并、差运算，适合去重、标签和关系计算。

### 添加与移除

- `sadd key member [member ...]`：添加成员，重复的自动忽略
- `srem key member [member ...]`：移除指定成员
- `spop key [count]`：随机弹出成员并删除
- `smove src dest member`：把成员原子迁移到目标集合

### 成员查询

- `smembers key`：返回全部成员
- `scard key`：成员总数
- `sismember key member`：判断成员是否存在
- `srandmember key [count]`：随机取成员但不删除；count 为负时允许重复

### 集合运算

- `sinter key [key ...]`：交集
    - `sinterstore dest key [key ...]`：交集写入 dest
- `sunion key [key ...]`：并集
    - `sunionstore dest key [key ...]`：并集写入 dest
- `sdiff key [key ...]`：差集，仅第一个 key 独有的成员
    - `sdiffstore dest key [key ...]`：差集写入 dest

## 六、ZSet 有序集合

元素唯一且带 score，按分数自动排序，分数相同时按成员字典序排列，适合排行榜和延时队列。

### 添加与计分

- `zadd key score member [score member ...]`：添加成员，已存在则更新分数；可附加 `nx`/`xx`/`gt`/`lt`/`ch`
- `zincrby key increment member`：给成员分数累加增量

### 移除

- `zrem key member [member ...]`：删除指定成员
- `zremrangebyrank key start stop`：按排名区间删除
- `zremrangebyscore key min max`：按分数区间删除
- `zremrangebylex key min max`：按字典序区间删除，要求分数相同

### 排名与统计

- `zcard key`：成员总数
- `zscore key member`：查询成员分数
- `zrank key member`：升序排名，从 0 开始
    - `zrevrank key member`：降序排名
- `zcount key min max`：统计分数区间内的成员数
    - `zlexcount key min max`：统计字典序区间内的成员数（同分）
- `zrangebylex key min max [limit offset count]`：按字典序查询（同分）

### 区间查询

- `zrange key start stop [withscores]`：按排名升序取区间
    - `zrevrange key start stop [withscores]`：按排名降序取区间
- `zrangebyscore key min max [withscores] [limit offset count]`：按分数升序查询
    - `zrevrangebyscore key max min [withscores] [limit offset count]`：按分数降序查询

> [!tip] Redis 6.2+ 可用 `zrange key min max byscore|bylex rev limit offset count` 统一表达上述区间查询。

## 速记小结

| 类型 | 结构 | 典型场景 |
|---|---|---|
| **String** | 单值 | 缓存、计数器、分布式锁 |
| **List** | 双向链表 | 消息队列、栈、时间线、分页 |
| **Hash** | field-value | 对象存储（用户、商品） |
| **Set** | 无序集合 | 去重、标签、共同好友、抽奖 |
| **ZSet** | 带分数的有序集合 | 排行榜、延时队列、范围检索 |

> 选型口诀：要顺序用 List，要去重用 Set，要排序用 ZSet，要按字段更新用 Hash，其余单值场景用 String。
