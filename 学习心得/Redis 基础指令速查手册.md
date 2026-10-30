
> 规则：成对命令（左/右、阻塞/非阻塞、秒/毫秒）主命令写第一行，同类变体换行缩进补充；解释言简意赅
>
> TA：过期仅 key 级别，类型本身无过期命令；P=毫秒，无 P=秒；AT=绝对时间戳

# 一、通用 key 操作

## 1.1 增

无（key 随写入自动创建）

## 1.2 删

1. **DEL key [key ...]**：删除指定键，不存在 key 忽略

## 1.3 改

1. **MOVE key db**：迁移 key 到目标数据库
2. **RENAME key newkey**：重命名 key，newkey 存在则覆盖
   1. **RENAMENX key newkey**：仅 newkey 不存在时重命名

## 1.4 查

1. **KEYS pattern**：通配查找 key（生产禁用）
2. **TYPE key**：返回 key 对应数据类型
3. **EXISTS key [key ...]**：判断 key 是否存在，返回存在数量
4. **RANDOMKEY**：随机返回库中一个 key

## 1.5 过期时间

1. **EXPIRE key seconds**：设置 key 秒级相对过期
   1. **PEXPIRE key milliseconds**：毫秒级相对过期
2. **EXPIREAT key timestamp**：秒级时间戳绝对过期
   1. **PEXPIREAT key ms-timestamp**：毫秒时间戳绝对过期
3. **TTL key**：查看剩余过期秒数；-1 永久，-2 key 不存在
   1. **PTTL key**：查看剩余过期毫秒数
4. **PERSIST key**：清除过期，key 永久有效

# 二、String 字符串

## 2.1 增

1. **SET key value**：设置键值，存在覆盖
2. **MSET key value [key value ...]**：批量设置键值对
3. **SETEX key seconds value**：设置值 + 秒级过期
   1. **PSETEX key ms value**：设置值 + 毫秒级过期
4. **SETNX key value**：key 不存在才设置（分布式锁）
   1. **MSETNX key value [key value ...]**：批量设置，全部 key 不存在才成功

## 2.2 删

无，使用通用 **DEL**

## 2.3 改

1. **GETSET key value**：设置新值，返回旧值
2. **SETRANGE key offset value**：从偏移量覆写字符串
3. **INCR key**：数字自增 1
   1. **INCRBY key increment**：数字增加指定增量
4. **DECR key**：数字自减 1
   1. **DECRBY key decrement**：数字减去指定减量
5. **APPEND key value**：字符串末尾追加内容

## 2.4 查

1. **GET key**：获取 key 的值
   1. **MGET key1 [key2 ...]**：批量获取多个 key
2. **GETRANGE key start end**：截取字符串 [start,end]
3. **STRLEN key**：获取字符串字节长度

## 2.5 TA

无

# 三、List 列表（双向链表，有序可重复）

## 3.1 增

1. **LPUSH key val1 [val2...]**：头部插入元素
   1. **RPUSH key val1 [val2...]**：尾部插入元素
2. **LPUSHX key value**：仅列表存在，头部插入
   1. **RPUSHX key value**：仅列表存在，尾部插入
3. **LINSERT key BEFORE|AFTER pivot value**：基准元素前后插入

## 3.2 删

1. **LPOP key**：弹出头部元素
   1. **RPOP key**：弹出尾部元素
2. **BLPOP key [key...] timeout**：阻塞左弹出，无元素等待
   1. **BRPOP key [key...] timeout**：阻塞右弹出
3. **LREM key count value**：删除匹配元素
   1. count>0：从头往后删 count 个
   2. count<0：从尾往前删 |count| 个
   3. count=0：删除全部匹配

## 3.3 改

1. **LSET key index value**：按下标修改元素
2. **LTRIM key start stop**：裁剪列表，保留区间内元素
3. **RPOPLPUSH src dest**：src 右弹，移入 dest 头部
   1. **BRPOPLPUSH src dest timeout**：阻塞版 RPOPLPUSH

## 3.4 查

1. **LRANGE key start stop**：获取区间元素，-1 代表末尾
2. **LINDEX key index**：按下标获取元素
3. **LLEN key**：获取列表长度

## 3.5 TA

无

# 四、Hash 哈希（key-field-value，适合对象）

## 4.1 增

1. **HSET key field value**：设置单个字段
   1. **HMSET key f1 v1 [f2 v2...]**：批量设置字段（新版 HSET 支持批量）
2. **HSETNX key field value**：字段不存在才设置

## 4.2 删

1. **HDEL key field [field...]**：删除哈希字段

## 4.3 改

1. **HINCRBY key field increment**：字段整数自增
   1. **HINCRBYFLOAT key field increment**：字段浮点数自增

## 4.4 查

1. **HGET key field**：获取单个字段值
   1. **HMGET key f1 [f2...]**：批量获取字段
2. **HGETALL key**：获取全部 field-value
3. **HKEYS key**：获取所有字段名
   1. **HVALS key**：获取所有字段值
4. **HLEN key**：哈希字段总数
5. **HEXISTS key field**：判断字段是否存在
6. **HSTRLEN key field**：字段值字节长度

## 4.5 TA

无

# 五、Set 集合（无序，元素唯一，交并差集）

## 5.1 增

1. **SADD key member [member...]**：添加成员，重复自动忽略

## 5.2 删

1. **SREM key member [member...]**：删除指定成员
2. **SPOP key [count]**：随机弹出成员并删除
3. **SMOVE src dest member**：成员迁移到目标集合

## 5.3 改

无直接修改，先删再加

## 5.4 查

1. **SMEMBERS key**：返回集合全部成员
2. **SCARD key**：集合元素总数
3. **SISMEMBER key member**：判断成员是否存在
4. **SRANDMEMBER key [count]**：随机取成员，不删除
5. **SINTER key1 [key2...]**：集合交集
   1. **SINTERSTORE dest k1 [k2...]**：交集存入目标 key
6. **SUNION key1 [key2...]**：集合并集
   1. **SUNIONSTORE dest k1 [k2...]**：并集存入目标 key
7. **SDIFF key1 [key2...]**：集合差集（k1 独有）
   1. **SDIFFSTORE dest k1 [k2...]**：差集存入目标 key

## 5.5 TA

无

# 六、ZSet 有序集合（成员唯一，带 score 排序）

## 6.1 增

1. **ZADD key score member [score member ...]**：添加成员，存在则更新分数

## 6.2 删

1. **ZREM key member [member...]**：删除指定成员
2. **ZREMRANGEBYRANK key start stop**：按排名区间删除
3. **ZREMRANGEBYSCORE key min max**：按分数区间删除
4. **ZREMRANGEBYLEX key min max**：字典序区间删除（同分）

## 6.3 改

1. **ZINCRBY key increment member**：成员分数增加增量

## 6.4 查

1. **ZCARD key**：有序集合成员总数
2. **ZSCORE key member**：查询成员分数
3. **ZRANK key member**：升序排名（从 0 开始）
   1. **ZREVRANK key member**：降序排名
4. **ZRANGE key start stop [WITHSCORES]**：升序取区间
   1. **ZREVRANGE key start stop [WITHSCORES]**：降序取区间
5. **ZRANGEBYSCORE key min max [WITHSCORES] [LIMIT ...]**：按分数升序查询
   1. **ZREVRANGEBYSCORE key max min [WITHSCORES]**：按分数降序查询
6. **ZCOUNT key min max**：统计分数区间数量
   1. **ZLEXCOUNT key min max**：字典序区间计数（同分）
7. **ZRANGEBYLEX key min max [LIMIT ...]**：字典序查询（同分）

## 6.5 TA

无