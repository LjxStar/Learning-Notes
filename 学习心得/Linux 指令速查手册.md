# Linux 常用命令速查手册（Ubuntu ＆ CentOS）

**阅读约定**

- 命令里的 `文件`、`目录`、`PID` 都是占位符，要换成你自己的实际值。
- 想不起某个命令怎么用：`命令 --help` 看简版，`man 命令` 看手册（按 `q` 退出）。
- 提示符 `$` 表示普通用户，`#` 表示 root。
- 【AI 辅助说明】本文由 AI 生成初稿，本人进行修改、校对、整理与补充。

---

## 〇、动手前先弄清的两件事

### sudo：Ubuntu 的「临时管理员权限」

```bash
sudo 命令        # 用 root 权限执行这一次命令
sudo -i          # 直接变成 root（退出用 exit）
```

- **Ubuntu** 默认不允许 root 登录，装软件、改 `/etc` 下的配置都要在命令前加 `sudo`。第一次会要你输入**当前用户**的密码，输入时屏幕不动是正常的。
- **CentOS** 习惯直接用 root 操作，所以本文标 `[CentOS]` 的地方一般不加 `sudo`。
- 记法：`sudo` 只是给**后面那条命令**临时提权，它本身不是命令。

### 两个系统最容易踩的差别

| 方面     | Ubuntu                              | CentOS                        |
| ------ | ----------------------------------- | ----------------------------- |
| 装软件    | `apt`                               | `yum`（7）/ `dnf`（8+）           |
| 本地包    | `.deb`，`dpkg -i`                    | `.rpm`，`rpm -ivh`             |
| 防火墙    | `ufw`（默认**没开**）                     | `firewalld`（默认**开着**）         |
| 服务管理   | `systemctl`                         | `systemctl`（7+）/ `service`（6） |
| 看网卡 IP | `ip addr`（`ifconfig` 要另装 net-tools） | 同左                            |

> [!tip] 遇到「命令找不到」，先分清是**没装**（去装包）还是**没在 PATH 里**（用 `which` 查）。

---

## 一、目录操作

### 1. ls — 看目录里有什么

```bash
ls                 # 列出当前目录
ls /etc            # 列出指定目录
ls -l              # 详细信息：权限、属主、大小、时间
ls -a              # 连隐藏文件一起看（. 开头的都是隐藏文件）
ls -lh             # 详细信息 + 大小显示成 K/M/G（最常用）
ls -lt             # 按修改时间排序，最新的在最上面
ls -d */           # 只看当前目录下的子目录
```

| 选项   | 说明                   |
| ---- | -------------------- |
| `-l` | 长格式，一行一个文件的详细信息      |
| `-a` | 显示所有文件，包括隐藏文件        |
| `-h` | 大小用人能看懂的单位（要配合 `-l`） |
| `-t` | 按修改时间排序              |
| `-d` | 只看目录本身，不展开里面的内容      |

> 很多系统给 `ls -l` 起了别名 `ll`，直接敲 `ll` 也行。

### 2. cd — 切换目录

```bash
cd /etc        # 用绝对路径进去（从 / 开始写）
cd logs        # 用相对路径进去（当前目录下的 logs）
cd ..          # 回上一级
cd ~           # 回家目录（只敲 cd 也一样）
cd -           # 回到上一次待的目录，来回切换很方便
```

> 路径打一半按 `Tab` 会自动补全，这是少打错字的关键。

### 3. pwd — 我现在在哪

```bash
pwd
```

打印当前目录的完整路径。**执行 `rm`、`mv` 之前先 `pwd` 确认一下位置**，能避免很多事故。

### 4. mkdir — 建目录

```bash
mkdir data                 # 建一个目录
mkdir -p /data/app/logs    # 父目录不存在时一并建出来
mkdir dir{1..5}            # 一次建 dir1~dir5
```

| 选项   | 说明                                 |
| ---- | ---------------------------------- |
| `-p` | 递归创建多级目录，目录已存在也不报错                 |
| `-m` | 建的时候就指定权限，如 `mkdir -m 700 private` |

### 5. rmdir — 删空目录

```bash
rmdir olddir      # 只能删空目录，目录里有东西会报错
```

用得不多，实际更多是 `rm -r 目录`。知道它的存在，看懂别人的命令即可。

---

## 二、文件操作

### 1. cp — 复制

```bash
cp a.txt /tmp/          # 复制文件到目录
cp a.txt b.txt          # 复制并改名
cp -r dir1 dir2         # 复制整个目录（必须加 -r）
cp -a /data /backup     # 连权限、时间一起复制，适合备份
```

| 选项   | 说明                  |
| ---- | ------------------- |
| `-r` | 递归复制目录              |
| `-a` | 归档复制：保留权限、属主、时间、软链接 |
| `-i` | 目标已存在时先问一句，防止误覆盖    |

### 2. rm — 删除

```bash
rm a.txt        # 删文件
rm -r dir       # 删目录
rm -rf dir      # 强制删目录，不提示
```

| 选项 | 说明 |
| --- | --- |
| `-r` | 递归删除目录 |
| `-f` | 强制删除，不再询问 |
| `-i` | 删之前逐个问，删重要东西时用 |

> [!warning] Linux 没有回收站，删了就没了。`rm -rf /` 会毁掉系统。动手前 `pwd` + `ls` 核对路径，路径里带变量时尤其要小心。

### 3. mv — 移动 / 改名

```bash
mv a.txt /tmp/       # 移动
mv a.txt b.txt       # 改名（同一目录下换个名字就是改名）
mv dir1 /data/       # 移动目录
mv old.txt old.txt.bak   # 备份式改名
```

| 选项   | 说明                        |
| ---- | ------------------------- |
| `-i` | 目标已存在时先确认                 |
| `-f` | 强制覆盖，不提示（默认行为，注意别覆盖掉重要文件） |

> `mv` 覆盖也是不可恢复的，养成先 `ls` 看一眼的习惯。

### 4. touch — 建空文件

```bash
touch a.txt              # 文件不存在就建空文件，存在就更新时间戳
touch file{1..3}.log     # 批量建 file1.log~file3.log
```

### 5. find — 按条件找文件

```bash
find . -name "*.log"              # 在当前目录下按名字找
find / -name "*.conf" 2>/dev/null # 全盘找，顺便屏蔽没权限的报错
find /data -type d -name "logs"   # 只找目录
find / -size +100M                # 找大于 100M 的文件
find /var/log -name "*.log" -mtime +30 -exec rm {} \;   # 删 30 天前的日志
```

| 条件               | 说明                   |
| ---------------- | -------------------- |
| `-name "*.log"`  | 按文件名匹配，支持通配符         |
| `-type f` / `d`  | 文件 / 目录              |
| `-size +100M`    | 大于 100M（`-` 是小于）     |
| `-mtime -7`      | 7 天内改过（`+7` 是超过 7 天） |
| `-exec 命令 {} \;` | 对每个结果执行命令，`{}` 代表文件名 |

> 搜全盘时末尾加 `2>/dev/null`，可以滤掉满屏的 `Permission denied`。

### 6. which — 这个命令装在哪

```bash
which nginx       # 命令的可执行文件在哪
type ll           # 看看 ll 是别名、内建命令还是真程序
```

> 命令明明装了却提示 `command not found`？用 `which` 查一下，多半是没在 `PATH` 里。

---

## 三、查看文件内容

### 1. cat — 一次性全部输出

```bash
cat a.txt               # 显示全部内容
cat -n a.txt            # 带行号
cat a.txt b.txt > c.txt # 合并两个文件
```

> 只适合**小文件**。大文件一刷就看不到开头了，用 `less`。

### 2. less — 分页慢慢看

```bash
less /var/log/syslog      # 打开，按 q 退出
less -N app.log           # 显示行号
less +F /var/log/syslog   # 先当 tail -f 用，Ctrl+C 回到浏览模式
```

打开的按键：

| 按键 | 作用 |
| --- | --- |
| 空格 / `PageDown` | 往下翻一屏 |
| `b` / `PageUp` | 往上翻 |
| `/关键字` | 搜关键字，`n` 下一个，`N` 上一个 |
| `G` / `gg` | 跳到最后 / 最前 |
| `q` | 退出 |

> `less` 不会把整个文件读进内存，几百 MB 的日志也不卡。

### 3. tail — 看结尾、盯日志

```bash
tail app.log              # 最后 10 行
tail -n 50 app.log        # 最后 50 行
tail -f app.log           # 实时输出新增内容，Ctrl+C 退出
tail -f app.log | grep ERROR   # 只盯错误
```

| 选项 | 说明 |
| --- | --- |
| `-n 数字` | 显示最后 N 行 |
| `-f` | 实时追踪，看日志的标配 |
| `-F` | 日志被改名 / 切割重建后还能继续跟（按天切割的日志用它） |

### 4. head — 看开头

```bash
head app.log          # 前 10 行
head -n 20 app.log    # 前 20 行
```

### 5. more — 老式分页

只能往下翻（空格翻页、`q` 退出），功能是 `less` 的子集。**平时用 `less` 就行**，知道 `more` 是什么即可。

---

## 四、搜索与处理文本

### 1. grep — 在内容里搜关键字

```bash
grep "error" app.log            # 找出包含 error 的行
grep -i "error" app.log         # 忽略大小写
grep -n "error" app.log         # 带上行号
grep -v "error" app.log         # 排除含 error 的行
grep -r "keyword" /data/        # 递归搜整个目录
grep -E "error|warn" app.log    # 匹配 error 或 warn
grep -A 3 "Exception" app.log   # 顺便显示后面 3 行（看异常栈）
```

| 选项 | 说明 |
| --- | --- |
| `-i` | 忽略大小写 |
| `-n` | 显示行号 |
| `-v` | 反向匹配，排除 |
| `-r` | 递归搜目录 |
| `-c` | 只输出匹配的行数 |
| `-E` | 用扩展正则，可以写 `a|b` |
| `-A` / `-B` 数字 | 连带显示匹配行的后 / 前 N 行 |

### 2. wc — 数一数

```bash
wc -l app.log     # 行数（最常用）
wc -c app.log     # 字节数
```

### 3. sed — 批量改文本

```bash
sed 's/old/new/' a.txt         # 每行第一个 old 换成 new（只输出，不改文件）
sed -i 's/old/new/g' a.txt     # 直接改文件，全部替换
sed -n '10,20p' a.txt          # 只看第 10~20 行
sed '/^#/d' a.txt              # 删掉所有 # 开头的注释行
```

| 写法 | 说明 |
| --- | --- |
| `-i` | 就地修改原文件 |
| `-n` + `p` | 只打印需要的行 |
| `s/旧/新/g` | 替换，`g` 表示一行里全部替换 |
| `/匹配/d` | 删掉匹配的行 |

> [!tip] 用 `-i` 前先备份：`sed -i.bak 's/old/new/g' a.txt` 会顺手生成 `a.txt.bak`。

### 4. awk — 按列取数据

```bash
awk '{print $1}' access.log        # 打印第 1 列
awk -F: '{print $1}' /etc/passwd   # 用 : 当分隔符，取第 1 列
awk '$3 > 100' data.txt            # 只显示第 3 列大于 100 的行
awk '{sum+=$1} END{print sum}' a.txt   # 把第 1 列加起来
```

> `$0` 是整行，`$1` 是第一列，`$2` 第二列……默认按空格/制表符分列。

---

## 五、管道与重定向

这两样不是命令，是把命令接起来的「连接件」，用熟了效率翻倍。

```text
命令1 | 命令2          把命令1 的输出，直接交给命令2 处理
命令 > 文件            输出写进文件（原内容被清空）
命令 >> 文件           输出追加到文件末尾
命令 2> 文件           只把报错写进文件
命令 > 文件 2>&1       正常输出和报错都写进同一个文件
命令 < 文件            把文件内容当输入喂给命令
```

`2` 是报错输出、`1` 是正常输出，`>&1` 就是「跟正常输出去同一个地方」。

**常见组合**

```bash
ps -ef | grep java               # 在进程列表里找 java
grep ERROR app.log | wc -l       # 数一数有多少行 ERROR
tail -f app.log | grep ERROR     # 边看日志边过滤
awk '{print $1}' access.log | sort | uniq -c | sort -nr | head   # 访问量前 10 的 IP
命令 > /dev/null 2>&1            # 什么输出都不要（/dev/null 是"黑洞"）
```

> [!warning] `>` 会**立刻清空**目标文件，写错文件名就丢数据。只想存一份又想看结果，用 `tee`：`命令 | tee out.log`。

---

## 六、权限与属主

### 1. 先看懂 `ls -l` 那一串

```text
-rw-r--r--  1  root  root  1024  Jan 1 08:30  app.jar
│└┬┘└┬┘└┬┘     │     │      │
│ │  │  │      │     │      └── 文件大小
│ │  │  │      │     └───────── 属组
│ │  │  │      └─────────────── 属主
│ │  │  └────────────────────── 其他用户 o：r--
│ │  └───────────────────────── 属组 g：r--
│ └──────────────────────────── 属主 u：rw-
└────────────────────────────── 类型：- 文件 / d 目录 / l 链接
```

- 三位一组：`r` 读、`w` 写、`x` 执行，对应数字 `4`、`2`、`1`，加起来就是 `chmod` 用的数字。
- 目录的 `x` 表示「能进去」，没有 `x` 就 `cd` 不进去。

| 数字  | 权限  | 含义         |
| --- | --- | ---------- |
| 7   | rwx | 读 + 写 + 执行 |
| 6   | rw- | 读 + 写      |
| 5   | r-x | 读 + 执行     |
| 4   | r-- | 只读         |

### 2. chmod — 改权限

```bash
chmod 755 script.sh      # 属主全权限，其他人读+执行（脚本常用）
chmod 644 a.txt          # 属主读写，其他人只读（普通文件常用）
chmod +x script.sh       # 加可执行权限（最常用的一种）
chmod -R 755 /data/app   # 递归改整个目录
```

| 选项          | 说明                   |
| ----------- | -------------------- |
| `-R`        | 递归，目录下所有文件一起改        |
| `+x` / `-x` | 加 / 去可执行权限           |
| `u+w`、`g+r` | 按身份改：u 属主、g 属组、o 其他人 |

### 3. chown — 改属主 / 属组

```bash
chown user a.txt               # 改属主
chown user:group a.txt         # 属主和属组一起改
chown -R user:group /data/app  # 递归改（部署后修权限常写这一条）
chgrp -R group /data/app       # 只改属组
```

| 选项 | 说明 |
| --- | --- |
| `-R` | 递归修改 |
| `用户:组` | 用冒号连写，只写用户就只改属主 |

### 4. umask — 新文件的默认权限

```bash
umask          # 查看当前值，一般是 022
umask 022      # 临时改（只对当前终端有效）
```

掩码 `022` 下，新文件是 `644`、新目录是 `755`。普通文件默认不带 `x`，这是刻意的安全设计。

---

## 七、进程与服务

### 1. ps — 看有哪些进程

```bash
ps -ef                  # 所有进程的完整信息
ps aux                  # 所有进程，带 CPU/内存占用
ps -ef | grep java      # 找 java 进程（最常用）
ps -u nginx             # 只看某个用户的进程
```

> `ps` 是「拍一张照」，看完就结束；想持续观察用 `top`。

### 2. top / htop — 实时看资源

```bash
top                # 实时监控，按 q 退出
top -p 1234        # 只盯某个 PID
htop               # 更顺眼的版本，需要先安装
```

top 里常用的键：`P` 按 CPU 排序、`M` 按内存排序、`1` 看每个核、`q` 退出。

### 3. kill — 结束进程

```bash
kill 1234            # 礼貌地结束（默认信号 15）
kill -9 1234         # 强杀，实在不走才用
pkill -f app.jar     # 按命令行关键字杀
killall nginx        # 按进程名杀
pgrep -f app.jar     # 先查 PID，确认再杀
```

> [!warning] 先 `kill PID`，等几秒没退再 `kill -9`。强杀会让进程来不及写盘、释放资源，数据库类进程尤其慎用。

### 4. systemctl — 管服务

```bash
systemctl status nginx      # 看状态和最近日志（排障第一步）
systemctl start nginx       # 启动
systemctl stop nginx        # 停止
systemctl restart nginx     # 重启
systemctl reload nginx      # 只重载配置，不断开连接（改完配置优先用它）
systemctl enable nginx      # 开机自启
systemctl disable nginx     # 取消开机自启
journalctl -u nginx -f      # 实时看这个服务的日志
```

> `[Ubuntu]` 这些都要加 `sudo`；CentOS 6 用 `service nginx start`。

### 5. 磁盘和内存够不够

```bash
df -h        # 各分区用了多少（快满了先看这个）
du -sh /var/log   # 某个目录占了多少空间
free -h      # 内存，重点看 available 那一列
uptime       # 开机多久 + 1/5/15 分钟平均负载
uname -a     # 内核版本、系统架构
```

> `df -h` 显示还有空间却写不进文件？多半是 inode 用完了，`df -i` 看一下。想知道空间被谁吃了，从 `/` 开始一层层 `du -sh *` 往下找。

### 6. 让程序在后台跑

```bash
命令 &                                     # 丢到后台
jobs                                       # 看当前终端有哪些后台任务
nohup java -jar app.jar > app.log 2>&1 &   # 关掉终端也不退出（部署常用）
fg %1                                      # 把后台任务调回前台
```

> 长期跑的服务交给 `systemctl` 托管更好（能开机自启、崩了自动拉起），`nohup` 适合临时任务。

---

## 八、网络诊断

> Windows 里看 IP 用 `ipconfig`，Linux 里是 `ip addr`（老命令叫 `ifconfig`）。

### 1. ip — 看 IP 和路由

```bash
ip addr               # 所有网卡和 IP（可简写 ip a）
ip -br addr           # 一行一块网卡，最清爽
ip route              # 路由表，看默认网关
```

老写法 `ifconfig` 要另装 net-tools，新系统默认没有，看懂即可。

### 2. ping — 通不通

```bash
ping -c 4 www.baidu.com    # 发 4 个包就停（Linux 默认一直 ping）
```

| 现象 | 通常说明 |
| --- | --- |
| `Request timeout` | 对端不通，或对端/中间设备禁了 ping |
| 能 ping IP 不能 ping 域名 | DNS 有问题，查 `/etc/resolv.conf` |

> 云服务器普遍禁 ping，ping 不通不代表服务挂了，用 `curl` 或 `ss` 验证更准。

### 3. ss — 端口谁在听

```bash
ss -tunlp                 # 所有监听端口 + 对应进程（最常用）
ss -tunlp | grep 8080     # 8080 被谁占了
ss -tan                   # 所有 TCP 连接
```

| 选项 | 说明 |
| --- | --- |
| `-t` / `-u` | TCP / UDP |
| `-n` | 直接显示端口号，不做解析 |
| `-l` | 只看监听中的端口 |
| `-p` | 显示进程名和 PID |

> `netstat -tunlp` 是它的老写法（要装 net-tools），看到能认出来就行，自己用 `ss`。

### 4. curl — 测接口、看网页

```bash
curl -I https://www.baidu.com      # 只看响应头
curl -v http://localhost:8080/api  # 打印完整请求过程，排障首选
curl -s ifconfig.me                # 看本机公网出口 IP
```

| 选项 | 说明 |
| --- | --- |
| `-I` | 只要响应头 |
| `-v` | 显示请求和响应的全过程 |
| `-o 文件` | 把内容存成文件 |
| `-s` | 静默，不显示进度条 |
| `-L` | 跟随跳转 |

> 下载文件也可以 `wget -c 地址`，`-c` 支持断点续传，比 curl 顺手。

### 5. ssh — 登录远程服务器

```bash
ssh user@192.168.1.10          # 登录
ssh -p 2222 user@host          # 指定端口
ssh user@host "df -h"          # 只执行一条命令就退出
```

**配一次免密，以后不用输密码**

```bash
ssh-keygen -t ed25519          # 一路回车，生成密钥
ssh-copy-id user@host          # 把公钥送到服务器
```

| 报错 | 多半是 |
| --- | --- |
| `Permission denied (publickey)` | 公钥没上传，或 `~/.ssh` 权限过宽（要 700） |
| `Connection refused` | 对端 sshd 没起，或端口不对 |
| `Connection timed out` | 防火墙 / 云安全组没放行 22 端口 |

### 6. scp — 跨机器传文件

```bash
scp file.tar.gz user@host:/data/     # 传到远程
scp -r dir user@host:/data/          # 传整个目录
scp user@host:/var/log/app.log ./    # 从远程拉回来
scp -P 2222 file user@host:/data/    # 指定端口（这里是大写 P）
```

### 7. 不通时按这个顺序查

> [!note] 逐层往下走，大多数问题能定位
> 1. `ip -br addr`：网卡起来了吗，有 IP 吗。
> 2. `ping 网关`：局域网通不通（网关看 `ip route`）。
> 3. `ping 8.8.8.8`：能不能出网。
> 4. `ping www.baidu.com`：不通就是 DNS，查 `/etc/resolv.conf`。
> 5. `ss -tunlp`：服务在听吗，听在 `0.0.0.0` 还是 `127.0.0.1`（后者外面访问不到）。
> 6. `curl -v 地址`：对端到底回没回。
> 7. 本机防火墙 / 云服务器安全组：最容易忘的一层。

---

## 九、编辑与压缩

### 1. vim — 命令行里改文件

```bash
vim a.txt       # 打开（没有就新建）
```

**先记一句话**：vim 有三种模式，任何模式下按 `Esc` 都能回到**命令模式**——按错了就先按 `Esc`。

![[模式切换关系.png]]

| 模式 | 怎么进 | 能干什么 |
| --- | --- | --- |
| 命令模式 | 一打开就是；按 `Esc` 随时回来 | 移动、删除、复制粘贴、撤销 |
| 插入模式 | 命令模式按 `i`（光标前）/ `a`（光标后）/ `o`（下一行） | 打字 |
| 末行模式 | 命令模式按 `:` | 保存、退出、替换 |

> [!warning] 新手最容易混：`dd`、`yy`、`p`、`u` **不加冒号**，直接按键；`:w`、`:q` **必须加冒号**。

**命令模式下（直接按键）**

| 按键 | 作用 |
| --- | --- |
| `h` `j` `k` `l` | 左 / 下 / 上 / 右（方向键也行） |
| `gg` / `G` | 跳到第 1 行 / 最后一行 |
| `dd` | 删一整行（`3dd` 删 3 行） |
| `yy` / `p` | 复制一行 / 粘贴到下一行 |
| `u` | 撤销 |
| `/关键字` | 搜索，`n` 下一个 |

**末行模式下（先按 `:`）**

| 命令 | 作用 |
| --- | --- |
| `:w` | 保存 |
| `:q` | 退出（没保存会拦你） |
| `:wq` 或 `:x` | 保存并退出 |
| `:q!` | 不保存，强行退出 |
| `:set nu` | 显示行号 |
| `:%s/old/new/g` | 全文替换 |

> [!tip] 打开只读文件忘了加 sudo？在末行模式敲 `:w !sudo tee %` 可以提权保存。想练手就敲 `vimtutor`，30 分钟入门。

### 2. tar — 打包压缩

```bash
tar -czvf logs.tar.gz /data/logs    # 打包并压缩
tar -xzvf logs.tar.gz               # 解压到当前目录
tar -xzvf logs.tar.gz -C /data      # 解压到指定目录
tar -tzvf logs.tar.gz               # 只看里面有什么，不解压
```

| 选项 | 说明 |
| --- | --- |
| `-c` / `-x` | 打包 / 解压 |
| `-z` | 用 gzip（`.tar.gz`） |
| `-v` | 显示过程 |
| `-f` | 指定文件名，习惯放最后 |
| `-t` | 查看包内容 |
| `-C 目录` | 解压到指定目录 |

> 记住两招就够：压 `-czvf`、解 `-xzvf`。遇到 `.tar.bz2` 把 `z` 换成 `j`，`.tar.xz` 换成 `J`。

### 3. zip / unzip — 和 Windows 互传用

```bash
zip -r archive.zip dir/       # 压缩目录
unzip archive.zip             # 解压到当前目录
unzip -d /data archive.zip    # 解压到指定目录
unzip -l archive.zip          # 只看内容
```

> 没装的话：`[Ubuntu] sudo apt install zip unzip`，`[CentOS] yum install zip unzip`。跨平台传文件优先 zip，Windows 原生能打开。

### 4. gzip — 单文件压缩

```bash
gzip app.log          # 压成 app.log.gz，原文件消失
gzip -d app.log.gz    # 解压
zcat app.log.gz       # 不解压直接看内容
```

> 只能压单个文件，要打包目录还是用 `tar`。

---

## 十、软件包管理

### 1. apt — Ubuntu

```bash
sudo apt update              # 更新软件源索引（装东西前先执行）
sudo apt install nginx       # 安装
sudo apt remove nginx        # 卸载（留配置）
sudo apt purge nginx         # 连带配置一起删干净
sudo apt install -y nginx    # -y 自动确认
sudo apt -f install          # 依赖没装好时用它修
apt search nginx             # 搜索（不用 sudo）
```

| 选项 | 说明 |
| --- | --- |
| `-y` | 所有提问自动回答 yes |
| `-f install` | 修复缺失的依赖 |

> `apt` 和 `apt-get` 功能基本一样，日常敲 `apt` 就行。

### 2. yum / dnf — CentOS

```bash
yum install nginx       # 安装
yum remove nginx        # 卸载
yum update nginx        # 升级某个包（不带包名就是升级全部）
yum search nginx        # 搜索
yum list installed      # 看看装了什么
yum clean all           # 清理缓存
```

> CentOS 7 用 `yum`，CentOS 8+ 用 `dnf`，两条命令写法一样。

### 3. 装本地包

```bash
sudo dpkg -i pkg.deb       # [Ubuntu] 装完缺依赖：sudo apt -f install
rpm -ivh pkg.rpm           # [CentOS] 更推荐 yum install ./pkg.rpm，会自动处理依赖
```

> `.deb` 装不进 CentOS，`.rpm` 也装不进 Ubuntu。

### 4. 换国内源（下载慢时）

```bash
# Ubuntu：先备份，再换镜像
sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak
sudo sed -i 's@//.*archive.ubuntu.com@//mirrors.aliyun.com@g' /etc/apt/sources.list
sudo apt update
```

> [!warning] 换源前一定备份；源版本要和系统版本对上（CentOS 7 用 `Centos-7.repo`），版本错配会全线报错。

---

## 十一、环境变量

### 1. 看

```bash
env                # 所有环境变量
echo $PATH         # 看某一个变量
```

| 变量 | 含义 |
| --- | --- |
| `PATH` | 命令的搜索路径，命令找不到先看它 |
| `HOME` | 当前用户的家目录 |
| `LANG` | 语言和字符集，中文乱码时调它 |
| `JAVA_HOME` | JDK 装在哪，Maven / Tomcat 靠它找 JDK |

### 2. 临时设（关掉终端就没了）

```bash
export JAVA_HOME=/usr/local/jdk
export PATH=$PATH:/usr/local/jdk/bin   # 注意别漏掉 $PATH，否则原有路径全没了
unset JAVA_HOME                        # 删掉这个变量
```

### 3. 永久设

| 文件 | 谁生效 | 什么时候加载 |
| --- | --- | --- |
| `~/.bashrc` | 当前用户 | 每开一个终端（最常用） |
| `/etc/profile` | 所有用户 | 登录时 |
| `/etc/profile.d/xxx.sh` | 所有用户 | 登录时（推荐，好卸载） |

```bash
echo 'export PATH=$PATH:/data/bin' >> ~/.bashrc
source ~/.bashrc      # 让改动立刻生效
```

> 改完不 `source` 就只有新开的终端才生效，这是新手最常卡住的地方。

---

## 十二、防火墙

> 两者都是 `iptables` 的友好外壳，底层一样。远程改防火墙，**先放行 22 端口再开**，否则会把自己关在门外。

### 1. ufw — Ubuntu

```bash
sudo ufw allow 22/tcp       # 先放行 SSH
sudo ufw enable             # 再开启
sudo ufw status             # 看状态和规则
sudo ufw allow 8080/tcp     # 开放端口
sudo ufw delete allow 8080  # 删规则
sudo ufw disable            # 关闭
```

> Ubuntu 默认**没开** ufw，`status` 显示 `inactive` 就是没启用。

### 2. firewall-cmd — CentOS 7+

```bash
systemctl status firewalld                        # 看服务状态
firewall-cmd --list-ports                         # 已开放的端口
firewall-cmd --add-port=8080/tcp --permanent      # 开放端口（永久）
firewall-cmd --remove-port=8080/tcp --permanent   # 关闭端口
firewall-cmd --reload                             # 重载，让改动生效
firewall-cmd --list-all                           # 看全部规则
```

> [!warning] 不加 `--permanent` 只是临时生效，重启就没了；加了 `--permanent` 还必须 `--reload`。

### 3. iptables — 底层规则

```bash
iptables -L -n                                # 看规则
iptables -A INPUT -p tcp --dport 80 -j ACCEPT # 放行 80
iptables -D INPUT -p tcp --dport 80 -j ACCEPT # 删除这条规则
```

> 这样改的规则重启就丢；日常用 ufw / firewalld 更省心。

> [!note] 云服务器（阿里云 / 腾讯云 / AWS）还有一层**安全组**。系统里放行了端口却还是不通，基本就是安全组没开。

---

## 附：速查总表

| 类别 | 常用命令 |
| --- | --- |
| 目录 | `ls -lh` `cd` `pwd` `mkdir -p` `rmdir` |
| 文件 | `cp -r` `rm -rf` `mv` `touch` `find -name` `which` |
| 看内容 | `cat -n` `less -N` `tail -f` `head -n` `more` |
| 搜文本 | `grep -rn` `wc -l` `sed -i 's/a/b/g'` `awk '{print $1}'` |
| 管道重定向 | 管道 `\|`、`>`、`>>`、`2>&1`、`/dev/null`、`tee` |
| 权限 | `chmod 755` `chmod +x` `chown -R` `chgrp` `umask` |
| 进程服务 | `ps -ef` `top` `kill -9` `systemctl` `journalctl -u` `nohup &` |
| 系统状态 | `df -h` `du -sh` `free -h` `uptime` `uname -a` |
| 网络 | `ip addr` `ss -tunlp` `ping -c` `curl -I` `ssh` `scp` `netstat`(老) |
| 编辑压缩 | `vim` `tar -czvf` `tar -xzvf` `zip -r` `unzip` |
| 软件包 | `apt update` `apt install` `apt purge` `yum install` `dnf` `dpkg -i` `rpm -ivh` |
| 环境变量 | `env` `export` `unset` `source` `~/.bashrc` `/etc/profile` |
| 防火墙 | `ufw`(Ubuntu) `firewall-cmd`(CentOS) `iptables` |
