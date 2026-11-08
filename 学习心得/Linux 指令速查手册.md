# Linux 常用命令速查手册（Ubuntu & CentOS）

**阅读约定**

- 命令中的 `文件`、`目录`、`PID` 等均为占位符，使用时请替换为实际值；`PID` 指进程 ID。 
- 命令格式为 `命令 [选项] [参数]`。选项通常以 `-` 开头，多个短选项可以合并书写，例如 `ls -lh`。 
- 忘记用法时，执行 `命令 --help` 查看简要说明，或执行 `man 命令` 查看完整手册（按 `q` 退出）。 
- 提示符 `$` 表示普通用户，`#` 表示 root 用户。
- 涉及系统差异时，会分别注明 Ubuntu 与 CentOS。
- 【AI 辅助说明】本文由 AI 生成初稿，本人进行修改、校对、整理与补充。

---

## 〇、动手前需要了解的两件事

### 1. sudo —— 临时获取管理员权限

`sudo` 用于以其他用户（默认是 root）的身份执行命令，最常用于临时获取管理员权限。

```bash
sudo 命令        # 仅对当前这一条命令提权
sudo -i          # 切换到 root 用户（退出用 exit）
```

> [!question] root 用户与 sudo 是什么？
> root 是 Linux 的超级管理员，拥有系统的全部权限。Ubuntu 默认禁止 root 直接登录，日常管理通过 `sudo` 临时提权；CentOS 则习惯直接使用 root 操作。

- **Ubuntu**：安装软件、修改 `/etc` 下的配置通常都要加 `sudo`。第一次执行时会要求输入**当前用户**的密码。
- **CentOS**：习惯直接以 root 操作，因此 CentOS 相关命令一般不加 `sudo`。
- 记忆方法：`sudo` 本身是一个命令，作用是为其后面的命令临时提权。

### 2. Ubuntu 与 CentOS 的系统差异

| 方面    | Ubuntu                              | CentOS                        |
| ----- | ----------------------------------- | ----------------------------- |
| 软件包管理 | `apt`                               | `yum`（7）/ `dnf`（8+）           |
| 本地安装包 | `.deb`，使用 `dpkg -i`                 | `.rpm`，使用 `rpm -ivh`          |
| 防火墙   | `ufw`（默认未启用）                        | `firewalld`（默认启用）             |
| 服务管理  | `systemctl`                         | `systemctl`（7+）/ `service`（6） |
| 查看网卡  | `ip addr`（`ifconfig` 需另装 net-tools） | 同左                            |

---

## 一、目录操作

### 1. ls —— 查看目录内容

`ls` 用于列出目录中的文件和子目录。

```bash
ls                 # 列出当前目录内容
ls /etc            # 列出指定目录内容
ls -lh             # 长格式显示，大小以 K/M/G 为单位（最常用）
ls -a              # 同时显示隐藏文件
ls -lt             # 按修改时间排序，最新的在前
ls -d */           # 只列出当前目录下的子目录
```

| 选项   | 说明                     |
| ---- | ---------------------- |
| `-l` | 长格式，每行显示一个文件的详细信息      |
| `-h` | 以易读单位显示大小，通常与 `-l` 搭配  |
| `-a` | 显示所有文件，包括以 `.` 开头的隐藏文件 |
| `-t` | 按修改时间排序                |
| `-d` | 只显示目录本身，不展开其中的内容       |

> 许多系统为 `ls -l` 设置了别名 `ll`，直接输入 `ll` 也可以。

> [!question] 隐藏文件 ？
> 以 `.` 开头的文件默认不会显示，多为配置文件，例如 `~/.bashrc`。此外，`.` 表示当前目录，`..` 表示上级目录。

### 2. pwd / cd —— 查看与切换目录

**（1）pwd —— 显示当前目录的完整路径**

```bash
pwd
```

**（2）cd —— 切换目录**

```bash
cd /etc        # 绝对路径：从根目录 / 开始
cd logs        # 相对路径：当前目录下的 logs
cd ..          # 返回上一级目录
cd ~           # 返回当前用户的家目录（只输入 cd 效果相同）
cd -           # 返回上一次所在的目录
```

> [!tip] 路径输入到一半时按 `Tab` 键可自动补全，这是减少输入错误的关键习惯。执行 `rm`、`mv` 前先用 `pwd` 确认当前目录，可以避免误操作。

### 3. mkdir —— 创建目录

```bash
mkdir data                 # 创建单个目录
mkdir -p /data/app/logs    # -p：父目录不存在时一并创建，目录已存在也不报错
mkdir dir{1..5}            # 一次创建 dir1 至 dir5
mkdir -m 700 private       # -m：创建时直接指定权限
```

> [!question] 花括号扩展 ？
> `dir{1..5}` 中的 `{1..5}` 是 Shell 提供的花括号扩展，会在执行前展开为 `dir1 dir2 dir3 dir4 dir5`。这是 Shell 的语法，不是 `mkdir` 的选项。

### 4. rmdir —— 删除空目录

```bash
rmdir olddir       # 只能删除空目录，目录中有内容时会报错
rmdir dir1 dir2    # 一次删除多个空目录
```

> 实际工作中更常用 `rm -r 目录`。`rmdir` 仅用于删除空目录，了解其用途即可。

---

## 二、文件操作

### 1. cp —— 复制文件或目录

```bash
cp a.txt /tmp/          # 将文件复制到指定目录
cp a.txt b.txt          # 复制并重命名
cp -r dir1 dir2         # 递归复制整个目录（复制目录必须加 -r）
cp -a /data /backup     # 归档复制，常用于备份（结果为 /backup/data）
```

| 选项 | 说明 |
| --- | --- |
| `-r` | 递归复制目录及其内容 |
| `-a` | 归档模式：递归复制，并保留权限、属主、时间戳和软链接 |
| `-i` | 目标文件已存在时先询问，防止误覆盖 |

> [!warning] 默认情况下，`cp` 会直接覆盖同名文件，且覆盖后无法恢复。重要操作建议加上 `-i`。

### 2. mv —— 移动或重命名

```bash
mv a.txt /tmp/           # 移动到指定目录
mv a.txt b.txt           # 重命名（同一目录下修改名称）
mv dir1 /data/           # 移动目录
mv old.txt old.txt.bak   # 备份式重命名
```

| 选项   | 说明                         |
| ---- | -------------------------- |
| `-i` | 目标文件已存在时先确认（覆盖不可恢复，建议养成习惯） |

### 3. rm —— 删除文件或目录

```bash
rm a.txt        # 删除文件
rm -r dir       # 递归删除目录
rm -rf dir      # 强制删除目录，不再提示
rm -i a.txt     # 删除前逐个询问
```

| 选项 | 说明 |
| --- | --- |
| `-r` | 递归删除目录及其内容 |
| `-f` | 强制删除，忽略不存在的文件且不再询问 |
| `-i` | 删除前逐个询问，适合删除重要文件 |

> [!warning] Linux 没有回收站，删除后无法恢复。
> `rm -rf /` 会删除系统文件，导致系统损坏。执行前应使用 `pwd` 和 `ls` 核对路径，路径中包含变量时尤其要谨慎。

### 4. touch —— 创建空文件

```bash
touch a.txt              # 文件不存在则创建空文件，存在则更新访问和修改时间
touch file{1..3}.log     # 批量创建 file1.log 至 file3.log
```

### 5. find —— 按条件查找文件

```bash
find . -name "*.log"                             # 在当前目录下按名称查找
find /data -type d -name logs                    # 只查找目录
find / -size +100M 2>/dev/null                   # 查找大于 100M 的文件，并屏蔽权限报错
find /var/log -name "*.log" -mtime +30           # 查找 30 天前的日志（先预览）
find /var/log -name "*.log" -mtime +30 -delete   # 确认无误后删除
```

| 条件                    | 说明                        |
| --------------------- | ------------------------- |
| `-name "*.log"`       | 按文件名匹配，支持通配符              |
| `-type f` / `-type d` | 分别表示文件 / 目录               |
| `-size +100M`         | 大于 100M；`-` 表示小于          |
| `-mtime -7`           | 7 天内修改过；`+7` 表示修改时间超过 7 天 |
| `-exec 命令 {} \;`      | 对每个结果执行命令，`{}` 代表文件名      |

> [!tip] 搜索整个磁盘时，末尾加上 `2>/dev/null` 可以屏蔽大量 `Permission denied` 报错，只保留有效结果。`2>/dev/null` 的含义见「五、管道与重定向」。

### 6. which / type —— 查看命令的来源

**（1）which —— 查看可执行文件的位置**

```bash
which nginx
```

**（2）type —— 判断命令是别名、内建命令还是外部程序**

```bash
type ll
```

> [!question] PATH 是什么？
> `PATH` 是 Shell 用于查找命令的目录列表，各目录之间用冒号分隔。执行命令时，Shell 会按顺序在这些目录中查找对应的可执行文件。

> [!tip] 命令已安装却提示 `command not found` 时，先用 `which` 或 `type` 检查。若找不到，通常是命令所在目录不在 `PATH` 中。

---

## 三、查看文件内容

### 1. cat —— 一次性输出小文件内容

```bash
cat a.txt               # 输出全部内容
cat -n a.txt            # 输出并显示行号
cat a.txt b.txt > c.txt # 合并两个文件
```

> [!summary] 常用选项
> - `-n`：为所有行显示行号。
> - `-b`：为非空行显示行号。
> - `-A`：显示制表符、换行符等不可见字符。

> [!warning] `cat` 只适合查看小文件。大文件会迅速滚过屏幕，无法看清开头，应改用 `less`。

### 2. less / more —— 分页查看文件

**（1）less —— 支持上下翻页和搜索，日常首选**

```bash
less /var/log/syslog      # 打开文件，按 q 退出
less -N app.log           # 显示行号
less +F /var/log/syslog   # 以类似 tail -f 的方式持续输出，按 Ctrl+C 返回浏览模式
```

| 按键 | 作用 |
| --- | --- |
| 空格 / `PageDown` | 向下翻一屏 |
| `b` / `PageUp` | 向上翻一屏 |
| `/关键字` | 搜索，按 `n` 查找下一个、`N` 查找上一个 |
| `g` / `G` | 跳到文件开头 / 结尾 |
| `q` | 退出 |

> [!info] `less` 按需读取文件内容，不会一次性加载到内存，因此查看几百 MB 的日志也不会卡顿。

**（2）more —— 老式分页工具**

```bash
more app.log
```

> [!note] `more` 的功能是 `less` 的子集，只能向下翻页（空格翻页，`q` 退出）。日常使用 `less` 即可；脚本中出现 `more` 时，能识别其作用即可。

### 3. tail —— 查看文件末尾或实时跟踪日志

```bash
tail app.log                   # 显示最后 10 行
tail -n 50 app.log             # 显示最后 50 行
tail -f app.log                # 实时输出新增内容，按 Ctrl+C 退出
tail -f app.log | grep ERROR   # 实时过滤错误日志
```

| 选项      | 说明                          |
| ------- | --------------------------- |
| `-n 数字` | 显示最后 N 行                    |
| `-f`    | 实时跟踪文件新增内容，查看日志的常用方式        |
| `-F`    | 文件被改名或重新创建后仍能继续跟踪，适合按天切割的日志 |

### 4. head —— 查看文件开头

```bash
head app.log          # 显示前 10 行
head -n 20 app.log    # 显示前 20 行
```

> [!summary] 常用选项
> - `-n 数字`：显示前 N 行，默认为 10 行。
> - `-c 数字`：按字节数显示开头部分。

---

## 四、搜索与文本处理

### 1. wc —— 统计行数、单词数和字节数

```bash
wc -l app.log     # 行数（最常用）
wc -w app.log     # 单词数
wc -c app.log     # 字节数
```

### 2. grep —— 按关键字搜索文本

```bash
grep "error" app.log            # 查找包含 error 的行
grep -in "error" app.log        # 忽略大小写并显示行号
grep -v "error" app.log         # 排除包含 error 的行
grep -rn "keyword" /data/       # 递归搜索目录并显示行号
grep -E "error|warn" app.log    # 使用扩展正则，匹配 error 或 warn
grep -c "error" app.log         # 只输出匹配的行数
grep -A 3 "Exception" app.log   # 同时显示匹配行之后的 3 行
```

| 选项 | 说明 |
| --- | --- |
| `-i` | 忽略大小写 |
| `-n` | 显示行号 |
| `-v` | 反向匹配，排除匹配的行 |
| `-r` | 递归搜索目录 |
| `-c` | 只输出匹配的行数 |
| `-E` | 使用扩展正则表达式，可用竖线表示「或」 |
| `-A` / `-B` / `-C 数字` | 同时显示匹配行之后 / 之前 / 前后各 N 行 |

### 3. sed —— 按行批量修改文本

`sed` 按行读取文本并执行编辑操作，默认只把结果输出到屏幕，不修改原文件。

```bash
sed 's/old/new/' a.txt         # 将每行第一个 old 替换为 new（仅输出，不修改文件）
sed -i 's/old/new/g' a.txt     # 直接修改文件，替换所有匹配
sed -n '10,20p' a.txt          # 只输出第 10 至 20 行
sed '/^#/d' a.txt              # 删除所有以 # 开头的注释行
```

| 写法 | 说明 |
| --- | --- |
| `-i` | 直接修改原文件 |
| `-n` + `p` | 只输出指定行 |
| `s/旧/新/g` | 替换，`g` 表示一行内全部替换 |
| `/匹配/d` | 删除匹配的行 |

> [!tip] 使用 `-i` 前先备份：`sed -i.bak 's/old/new/g' a.txt` 会在修改原文件的同时生成 `a.txt.bak`。

### 4. awk —— 按列提取和处理数据

`awk` 以行为单位读取文本，并默认按空白字符拆分成列，适合提取字段和做简单统计。

```bash
awk '{print $1}' access.log            # 输出第 1 列
awk -F: '{print $1}' /etc/passwd       # 以 : 为分隔符，输出第 1 列
awk '$3 > 100' data.txt                # 输出第 3 列大于 100 的行
awk '{sum+=$1} END{print sum}' a.txt   # 累加第 1 列并输出总和
```

> [!summary] 基本语法
> - `$0`：整行内容；`$1`、`$2`……：第 1 列、第 2 列。
> - `-F 分隔符`：指定列分隔符，默认按空白字符（空格或制表符）分列。
> - `BEGIN{}`：处理第一行之前执行；`END{}`：处理完最后一行后执行。
> - `{print $1}`：对每一行执行的动作。

---

## 五、管道与重定向

管道和重定向用于连接命令、控制输入输出，是 Shell 提供的功能，不是独立命令。

```text
命令1 | 命令2          将命令1 的输出作为命令2 的输入
命令 > 文件            将输出写入文件（会清空原内容）
命令 >> 文件           将输出追加到文件末尾
命令 2> 文件           只将错误输出写入文件
命令 > 文件 2>&1       将正常输出和错误输出写入同一文件
命令 < 文件            将文件内容作为命令的输入
```

> [!question] 标准输出与标准错误？
> 文件描述符是进程访问文件的编号。Linux 为每个进程默认打开三个文件描述符：`0` 标准输入（stdin）、`1` 标准输出（stdout）、`2` 标准错误（stderr）。
> `2>&1` 表示让文件描述符 2 指向文件描述符 1 当前指向的位置，因此正常输出和错误输出会写入同一个文件。

**常见组合**

```bash
ps -ef | grep java               # 在进程列表中查找 java
grep -c ERROR app.log            # 统计包含 ERROR 的行数（等价于 grep ERROR app.log | wc -l）
tail -f app.log | grep ERROR     # 实时过滤错误日志
awk '{print $1}' access.log | sort | uniq -c | sort -nr | head   # 访问量最高的 10 个 IP
命令 > /dev/null 2>&1            # 丢弃所有输出（/dev/null 是空设备）
```

> [!warning] `>` 会立即清空目标文件，文件名写错就会丢失数据。
> 如果既要保存输出又要查看结果，使用 `tee`：`命令 | tee out.log`；需要追加时使用 `tee -a`。

---

## 六、权限与属主

每个文件都有属主（owner）和属组（group）。权限决定谁能读取、写入或执行该文件。

### 1. ls -l —— 读懂权限信息

```text
-rw-r--r--  1  root  root  1024  Jan 1 08:30  app.jar
```

| 位置 | 含义 |
| --- | --- |
| 第 1 位 | 文件类型：`-` 普通文件、`d` 目录、`l` 软链接 |
| 第 2–4 位 | 属主（u）的读、写、执行权限 |
| 第 5–7 位 | 属组（g）的读、写、执行权限 |
| 第 8–10 位 | 其他用户（o）的读、写、执行权限 |
| 其后 | 硬链接数、属主、属组、大小、修改时间、文件名 |

- 三位一组：`r` 读、`w` 写、`x` 执行，对应数字 `4`、`2`、`1`，相加即得到 `chmod` 使用的数字。
- 目录的 `x` 表示可以进入（`cd`），`w` 表示可以在其中创建或删除文件，`r` 表示可以列出目录内容。

| 数字 | 权限 | 含义 |
| --- | --- | --- |
| 7 | rwx | 读 + 写 + 执行 |
| 6 | rw- | 读 + 写 |
| 5 | r-x | 读 + 执行 |
| 4 | r-- | 只读 |

### 2. chmod —— 修改权限

```bash
chmod 755 script.sh      # 属主 rwx，属组和其他用户 r-x（脚本常用）
chmod 644 a.txt          # 属主读写，属组和其他用户只读（普通文件常用）
chmod +x script.sh       # 添加可执行权限
chmod -R 755 /data/app   # 递归修改整个目录
```

| 写法 | 说明 |
| --- | --- |
| `-R` | 递归修改目录下所有文件和子目录 |
| `+x` / `-x` | 添加 / 移除可执行权限 |
| `u+w`、`g+r`、`o-r` | 按身份修改：u 属主、g 属组、o 其他用户、a 所有用户 |

### 3. chown / chgrp —— 修改属主与属组

**（1）chown —— 修改属主（可同时修改属组）**

```bash
chown user a.txt               # 只修改属主
chown user:group a.txt         # 同时修改属主和属组
chown -R user:group /data/app  # 递归修改（部署后修正权限常用）
```

**（2）chgrp —— 只修改属组**

```bash
chgrp -R dev /data/app
```

> 只有 root 能把文件给别人；普通用户只能在自己的文件上改组（且必须属于目标组）。

### 4. umask —— 新文件的默认权限

`umask` 是权限掩码，用于决定新建文件或目录时默认去掉哪些权限。

```bash
umask          # 查看当前值，通常为 022
umask 022      # 临时修改（仅对当前终端有效）
```

>  掩码 `022` 下，新文件是 `644`、新目录是 `755`（目录默认带 `x`，文件默认不带）。

---

## 七、网络诊断

> [!question] 与 Windows 的差异？
> Windows 使用 `ipconfig` 查看 IP 地址，Linux 使用 `ip addr`。`ifconfig` 是较旧的命令，需要额外安装 net-tools。

### 1. ip / ifconfig —— 查看 IP 地址和路由

**（1）ip —— 新系统默认自带**

```bash
ip addr               # 查看所有网卡和 IP 地址（可简写为 ip a）
ip -br addr           # 每块网卡一行，输出最简洁
ip route              # 查看路由表，确认默认网关
```

| 写法 | 说明 |
| --- | --- |
| `ip addr` | 查看网卡和 IP 地址 |
| `ip -br addr` | 简洁输出，每块网卡一行 |
| `ip -4 addr` | 只显示 IPv4 地址 |
| `ip route` | 查看路由表 |

**（2）ifconfig —— 旧命令，需要安装 net-tools**

```bash
ifconfig eth0
```

### 2. ping —— 测试网络连通性

```bash
ping -c 4 www.baidu.com    # 发送 4 个数据包后停止（不加 -c 会一直发送）
```

| 现象 | 通常说明 |
| --- | --- |
| `Request timeout` | 对端不可达，或对端 / 中间设备禁用了 ICMP |
| 能 ping 通 IP，但不能 ping 通域名 | DNS（域名系统）解析有问题，检查 `/etc/resolv.conf` |

> [!summary] 常用选项
> - `-c 次数`：发送指定数量的数据包后停止。
> - `-i 秒数`：设置发送间隔，默认 1 秒。
> - `-W 秒数`：设置等待响应的超时时间。

### 3. traceroute / mtr —— 追踪路由与定位丢包

**（1）traceroute —— 逐跳显示数据包经过的路由**

```bash
traceroute www.baidu.com             # 显示每一跳的地址和三次往返耗时
traceroute -n www.baidu.com          # 不解析域名，只显示 IP
traceroute -T -p 443 www.baidu.com   # 使用 TCP 443 探测，绕过 ICMP 被拦截的情况
```

**（2）mtr —— 结合 traceroute 与 ping，持续统计每一跳的丢包率**

```bash
mtr www.baidu.com            # 实时界面，按 q 退出
mtr -r -c 10 www.baidu.com   # 发送 10 轮后输出报告，便于分享分析
```

| 现象            | 通常说明                     |
| ------------- | ------------------------ |
| 某一跳显示 `* * *` | 该设备不返回 ICMP 消息，后续跳正常则无影响 |
| 从某一跳开始持续丢包    | 这一跳或它的上游链路有问题            |
| 只有最后一跳丢包      | 目标主机过载或做了限速              |

> [!summary] 安装方法
> - Ubuntu：`sudo apt install traceroute mtr`
> - CentOS：`yum install traceroute mtr`

> [!tip] 判断丢包时，重点看终点或「从某一跳开始持续到终点」的丢包率。只有中间某一跳丢包，通常是该设备对 ICMP 做了限速。

### 4. ss / netstat —— 查看端口占用

**（1）ss —— 速度更快，日常首选**

```bash
sudo ss -tunlp                # 查看所有监听端口及对应进程（需 sudo 才能看到其他用户的进程）
sudo ss -tunlp | grep 8080    # 查看 8080 端口被哪个进程占用
ss -tan                       # 查看所有 TCP 连接
```

**（2）netstat —— 旧命令，需要安装 net-tools**

```bash
netstat -tunlp
```

| 选项 | 说明 |
| --- | --- |
| `-t` / `-u` | 分别显示 TCP / UDP |
| `-n` | 直接显示端口号，不做名称解析 |
| `-l` | 只显示处于监听状态的端口 |
| `-p` | 显示进程名和 PID |

### 5. curl —— 测试接口与查看网页

```bash
curl -I https://www.baidu.com      # 只查看响应头
curl -v http://localhost:8080/api  # 输出完整请求过程，排查故障的首选
curl -s ifconfig.me                # 查看本机公网出口 IP
curl -o page.html https://example.com   # 将内容保存为文件
```

| 选项 | 说明 |
| --- | --- |
| `-I` | 只获取响应头（状态码、内容类型等信息） |
| `-v` | 显示请求和响应的完整过程 |
| `-o 文件` | 将内容保存到指定文件 |
| `-s` | 静默模式，不显示进度信息 |
| `-L` | 自动跟随重定向 |

> [!tip] 下载大文件可使用 `wget -c 地址`，`-c` 支持断点续传，网络中断后可以继续下载。

### 6. ssh —— 登录远程服务器

SSH 是一种加密的远程登录协议，`ssh` 是其客户端命令。

```bash
ssh user@192.168.1.10          # 登录远程服务器
ssh -p 2222 user@host          # 指定端口
ssh user@host "df -h"          # 执行一条命令后退出
```

> [!summary] 常用选项
> - `-p 端口`：指定远程 SSH 端口。
> - `-i 私钥文件`：指定用于认证的私钥文件。
> - `-v`：显示调试信息，排查连接问题时使用。

**配置免密登录**

```bash
ssh-keygen -t ed25519          # 生成密钥对（-t 指定算法），一路回车使用默认值
ssh-copy-id user@host          # 将公钥上传到服务器
```

> [!question] 公钥与私钥 ？
> `ssh-keygen` 生成一对密钥：私钥保存在本机 `~/.ssh/` 目录，公钥上传到服务器。登录时，服务器用公钥验证客户端是否持有对应的私钥，私钥本身不会在网络中传输。ed25519 是较新且更安全的密钥算法。

| 报错                              | 通常原因                          |
| ------------------------------- | ----------------------------- |
| `Permission denied (publickey)` | 公钥未上传，或 `~/.ssh` 权限过宽（应为 700） |
| `Connection refused`            | 对端 sshd（SSH 服务端进程）未启动，或端口不正确  |
| `Connection timed out`          | 防火墙 / 云安全组未放行 22 端口           |

---

## 八、编辑与压缩

### 1. vim —— 在命令行中编辑文件

```bash
vim a.txt       # 打开文件（不存在则新建）
```

**vim 有三种模式，任何模式下按 `Esc` 都可以回到命令模式。**

![[模式切换关系.png]]

| 模式 | 进入方式 | 作用 |
| --- | --- | --- |
| 命令模式 | 打开文件即进入；按 `Esc` 返回 | 移动光标、删除、复制粘贴、撤销等 |
| 插入模式 | 在命令模式按 `i`（光标前）、`a`（光标后）、`o`（下一行） | 输入并编辑文本 |
| 末行模式 | 在命令模式按 `:` | 保存、退出、替换等 |

**命令模式常用按键**

| 按键 | 作用 |
| --- | --- |
| `h` `j` `k` `l` | 左 / 下 / 上 / 右移动光标（方向键也可） |
| `gg` / `G` | 跳到第 1 行 / 最后一行 |
| `dd` | 删除一整行（`3dd` 删除 3 行） |
| `yy` / `p` | 复制一行 / 粘贴到下一行 |
| `u` | 撤销 |
| `/关键字` | 搜索，按 `n` 查找下一个 |

**末行模式常用命令**

| 命令 | 作用 |
| --- | --- |
| `:w` | 保存 |
| `:q` | 退出（未保存时会提示） |
| `:wq` 或 `:x` | 保存并退出 |
| `:q!` | 不保存，强制退出 |
| `:set nu` | 显示行号 |
| `:%s/old/new/g` | 全文替换（`%` 表示所有行） |

> [!example] 一次完整的编辑流程
> 1. 执行 `vim a.txt` 打开文件。
> 2. 按 `i` 进入插入模式，输入并编辑文本。
> 3. 按 `Esc` 返回命令模式。
> 4. 输入 `:wq` 保存并退出；若不想保存，输入 `:q!`。

> [!tip] 打开只读文件时忘记加 `sudo`：在末行模式执行 `:w !sudo tee %`，可以提权保存。其中 `%` 表示当前文件名。

### 2. tar —— 打包与压缩

`tar` 用于将多个文件打包成一个归档文件，常与 gzip 等压缩工具配合使用。归档文件是包含多个文件及其目录结构的单一文件。

```bash
tar -czvf logs.tar.gz -C /data logs    # 打包 /data/logs，包内保存相对路径
tar -xzvf logs.tar.gz                  # 解压到当前目录
tar -xzvf logs.tar.gz -C /data         # 解压到指定目录
tar -tzvf logs.tar.gz                  # 查看包内文件，不解压
```

| 选项                 | 说明                                  |
| ------------------ | ----------------------------------- |
| `-c` / `-x` / `-t` | 创建归档 / 解压 / 查看内容，三者互斥               |
| `-z`               | 使用 gzip 压缩（`.tar.gz`）               |
| `-j` / `-J`        | 使用 bzip2（`.tar.bz2`）/ xz（`.tar.xz`） |
| `-v`               | 显示处理过程                              |
| `-f`               | 指定归档文件名，习惯放在最后                      |
| `-C 目录`            | 打包或解压前切换到指定目录                       |

> [!tip] 掌握两个常用组合即可：打包使用 `-czvf`，解压使用 `-xzvf`。打包时先进入目标目录的上级目录，或使用 `-C` 指定目录，否则包内可能包含多余的上级目录路径。

### 3. zip / unzip —— 与 Windows 交换文件

```bash
zip -r archive.zip dir/       # 压缩目录
unzip -l archive.zip          # 查看压缩包内容
unzip archive.zip             # 解压到当前目录
unzip archive.zip -d /data    # 解压到指定目录
```

> [!summary] 常用选项
> - `zip -r`：递归压缩目录。
> - `unzip -l`：只列出压缩包内容，不解压。
> - `unzip -d 目录`：指定解压目录。

> [!tip] 未安装时：Ubuntu 使用 `sudo apt install zip unzip`，CentOS 使用 `yum install zip unzip`。与 Windows 交换文件时优先使用 zip 格式，Windows 可以直接打开。

### 4. gzip —— 压缩单个文件

```bash
gzip app.log          # 压缩为 app.log.gz，原文件会被删除
gzip -d app.log.gz    # 解压（等价于 gunzip）
zcat app.log.gz       # 不解压，直接查看内容
```

> [!summary] 常用选项
> - `-d`：解压，等价于 `gunzip`。
> - `-k`：保留原文件。
> - `-c`：将结果输出到标准输出，保留原文件。

> [!warning] `gzip` 只能压缩单个文件，且默认会删除原文件。需要打包目录时使用 `tar`。

---

## 九、软件包管理

### 1. apt —— Ubuntu 的包管理器

```bash
sudo apt update              # 更新软件源索引（安装前先执行）
sudo apt install nginx       # 安装软件包
sudo apt install -y nginx    # 自动确认，不再询问
sudo apt remove nginx        # 卸载软件包，保留配置文件
sudo apt purge nginx         # 卸载并删除配置文件
sudo apt -f install          # 修复依赖关系
apt search nginx             # 搜索软件包（不需要 sudo）
```

> [!summary] 常用子命令
> - `update`：更新软件源索引（本地保存的可用软件包列表），不升级已安装的软件；升级使用 `upgrade`。
> - `install`：安装软件包；`-y` 自动确认。
> - `remove`：卸载软件包，保留配置文件；`purge` 连配置文件一起删除。
> - `-f install`：修复依赖关系。
> - `search`：搜索软件包。

> `apt` 和 `apt-get` 功能基本一样，日常敲 `apt` 就行。`apt` 不该用在脚本里（输出格式不保证稳定），脚本里写 `apt-get`。

### 2. yum / dnf —— CentOS 的包管理器

```bash
yum install nginx       # 安装
yum remove nginx        # 卸载
yum update nginx        # 升级指定软件包（不带包名则升级全部）
yum search nginx        # 搜索
yum list installed      # 查看已安装的软件包
yum clean all           # 清理缓存
```

> CentOS 7 用 `yum`，CentOS 8+ 用 `dnf`，写法一样。这些命令要用 root 执行。

### 3. dpkg / rpm —— 安装本地软件包

**（1）dpkg —— 安装 Ubuntu 的 `.deb` 包**

```bash
sudo dpkg -i pkg.deb        # 安装；若提示缺少依赖，接着执行 sudo apt -f install
```

**（2）rpm —— 安装 CentOS 的 `.rpm` 包**

```bash
rpm -ivh pkg.rpm            # 更推荐 yum install ./pkg.rpm，可自动处理依赖
```

> [!summary] 参数说明
> - `dpkg -i`：安装本地 `.deb` 包。
> - `rpm -i`：安装；`-v` 显示过程；`-h` 显示进度条。
> - `rpm -q 包名`：查询软件包是否已安装；`rpm -e 包名`：卸载。

> [!warning] `.deb` 与 `.rpm` 不能混用：`.deb` 无法安装到 CentOS，`.rpm` 也无法安装到 Ubuntu。

### 4. 更换软件源 —— 下载缓慢时使用国内镜像

```bash
# Ubuntu：先备份，再替换镜像域名
sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak
sudo sed -i 's@//.*archive.ubuntu.com@//mirrors.aliyun.com@g' /etc/apt/sources.list
sudo apt update
```

> [!warning] 更换前必须备份；源版本要与系统版本匹配（例如 CentOS 7 使用 `Centos-7.repo`），版本不匹配会导致大量报错。

---

## 十、进程与服务

### 1. ps —— 查看进程

```bash
ps -ef                  # 查看所有进程的完整信息
ps aux                  # 查看所有进程，并显示 CPU 和内存占用
ps -ef | grep java      # 查找 java 进程（最常用）
ps -u nginx             # 查看指定用户的进程
```

| 选项 | 说明 |
| --- | --- |
| `-e` | 显示所有进程 |
| `-f` | 显示完整格式，包括 UID（用户 ID）、PPID（父进程 ID）和启动命令 |
| `a` | 显示所有终端上的进程（BSD 风格） |
| `u` | 以用户为中心的格式显示（BSD 风格） |
| `x` | 显示没有控制终端的进程（BSD 风格） |

> [!info] `ps -ef` 与 `ps aux` 的区别
> 两者都能列出所有进程，区别在于语法风格：`-ef` 属于 UNIX 风格，`aux` 属于 BSD 风格。输出字段略有不同，日常任选一种即可。

> [!tip] `ps` 只显示命令执行瞬间的进程快照；需要持续观察时使用 `top`。

### 2. top / htop —— 实时查看系统资源

```bash
top                # 实时监控，按 q 退出
top -p 1234        # 只监控指定 PID
htop               # 界面更友好的版本，需要先安装
```

| 按键  | 作用               |
| --- | ---------------- |
| `P` | 按 CPU 使用率排序      |
| `M` | 按内存使用率排序         |
| `1` | 显示每个 CPU 核心的使用情况 |
| `q` | 退出               |

> [!question] 如何理解 top 的输出？
> - `load average`：1、5、15 分钟内的平均负载，需要结合 CPU 核心数判断。
> - `%CPU` / `%MEM`：进程占用的 CPU / 物理内存百分比。
> - `RES`：进程实际占用的物理内存；`VIRT`：进程使用的虚拟内存总量。

### 3. kill / pkill / killall —— 终止进程

**（1）kill —— 按 PID 终止进程**

```bash
kill 1234            # 发送默认信号 15（TERM），请求进程正常退出
kill -9 1234         # 发送信号 9（KILL），强制终止
```

> [!summary] 常用信号
> - `15`（TERM）：默认信号，请求进程正常退出，进程可以清理资源。
> - `9`（KILL）：强制终止，进程无法捕获或忽略。
> - `1`（HUP）：挂起信号，许多服务收到后会重新加载配置。

**（2）pkill / killall —— 按名称或关键字终止进程**

```bash
pgrep -f app.jar     # 先查询 PID，确认后再终止
pkill -f app.jar     # 按完整命令行关键字终止
killall nginx        # 按进程名终止
```

> [!warning] 应先执行 `kill PID`，等待几秒确认进程未退出后再使用 `kill -9`。强制终止会使进程来不及保存数据或释放资源，数据库类进程尤其要谨慎。

### 4. nohup / & —— 让程序在后台运行

```bash
命令 &                                     # 将程序放到当前终端的后台运行
jobs                                       # 查看当前终端的后台任务及任务编号
fg %1                                      # 将编号为 1 的后台任务调回前台
nohup java -jar app.jar > app.log 2>&1 &   # 关闭终端后程序仍继续运行（部署常用）
```

> [!question] `nohup` 与 `&` 的区别？
> `&` 只是将程序放到后台运行，终端关闭时进程可能收到 SIGHUP 信号而退出。`nohup` 会让进程忽略 SIGHUP，因此两者常一起使用。

> [!tip] 长期运行的服务建议交给 `systemctl` 管理（支持开机自启、异常退出后自动重启）；`nohup` 更适合临时任务。

### 5. systemctl —— 管理系统服务

```bash
systemctl status nginx      # 查看服务状态和最近的日志
systemctl start nginx       # 启动服务
systemctl stop nginx        # 停止服务
systemctl restart nginx     # 重启服务
systemctl reload nginx      # 重新加载配置，不中断现有连接
systemctl enable nginx      # 设置开机自启
systemctl disable nginx     # 取消开机自启
journalctl -u nginx -f      # 实时查看该服务的日志
```

> [!summary] 常用子命令
> - `start` / `stop` / `restart`：启动 / 停止 / 重启服务。
> - `reload`：重新加载配置，不中断现有连接（服务支持时）。
> - `enable` / `disable`：设置 / 取消开机自启。
> - `status`：查看服务状态和最近日志。

> [!tip] `enable` 只设置开机自启，不会立即启动服务；需要同时执行 `systemctl start nigix`。Ubuntu 上这些命令通常需要加 `sudo`；CentOS 6 使用 `service nginx start`。

---

## 十一、系统状态

### 1. df / du —— 查看磁盘空间

`df` 用于查看各分区的磁盘使用情况；分区是磁盘上划分出的独立存储区域。

```bash
df -h              # 查看各分区的使用情况
df -i              # 查看 inode 使用情况
du -sh /var/log    # 查看指定目录占用的总空间
du -sh * | sort -h # 查看当前目录下各项目的大小，并从小到大排序
```

> [!summary] 常用选项
> - `-h`：以 K/M/G 等易读单位显示大小，`df` 和 `du` 都支持。
> - `-i`：仅 `df` 支持，查看 inode 使用情况。
> - `-s`：仅 `du` 支持，只输出汇总大小，不展开子目录明细。
> - `-a`：`du` 的选项，同时显示文件和目录。

> [!question] inode 是什么？
> inode（索引节点）是文件系统中用于存储**文件元数据**的数据结构。每个文件或目录都对应一个 inode，其中记录文件类型、权限、属主、大小、时间戳以及数据块的位置，但不包含文件名和文件内容。文件名保存在目录项中，并指向对应的 inode。

> [!tip] `du -sh *` 只统计当前目录下可见的项目（不含隐藏文件）；`sort -h` 能按 K/M/G 正确排序。

### 2. free —— 查看内存使用情况

```bash
free -h
```

> [!summary] 输出字段
> - `total`：总内存。
> - `used`：已使用内存。
> - `free`：完全空闲的内存。
> - `buff/cache`：内核缓冲区和缓存，可在需要时回收。
> - `available`：估计可供新程序使用的内存，重点看这一列。

### 3. uptime / uname —— 查看负载与内核信息

```bash
uptime             # 开机时长及 1、5、15 分钟平均负载
uname -a           # 内核版本、主机名、系统架构
nproc              # 查看 CPU 核心数
```

> [!summary] uname 常用选项
> - `-a`：显示全部信息。
> - `-r`：只显示内核版本。
> - `-m`：显示硬件架构，如 `x86_64`。

---

## 十二、环境变量

环境变量是 Shell 和程序共享的键值对，用于保存路径、语言、程序配置等信息。

### 1. env —— 查看环境变量

```bash
env                # 查看所有环境变量
echo $PATH         # 查看指定变量的值
```

| 变量 | 含义 |
| --- | --- |
| `PATH` | 命令的搜索路径，命令找不到时先检查它 |
| `HOME` | 当前用户的家目录 |
| `LANG` | 语言和字符集，中文乱码时调整它 |
| `JAVA_HOME` | JDK（Java 开发工具包）的安装目录，Maven / Tomcat 依靠它查找 JDK |

### 2. export —— 临时设置环境变量

```bash
export JAVA_HOME=/usr/local/jdk
export PATH=$PATH:/usr/local/jdk/bin   # 注意保留 $PATH，否则原有路径会丢失
unset JAVA_HOME                        # 删除变量
```

> [!question] `$变量名`  ？
> `$变量名` 表示引用变量的值。`PATH=$PATH:/new/dir` 的含义是：在原有 `PATH` 的基础上追加 `/new/dir`。

> [!warning] `PATH=$PATH:新目录` 中的 `$PATH` 不能省略，否则原有路径会全部丢失，当前终端将无法找到大部分命令。重新打开终端可以恢复。

### 3. bashrc / profile —— 永久设置环境变量

| 文件 | 生效范围 | 加载时机 |
| --- | --- | --- |
| `~/.bashrc` | 当前用户 | 每打开一个交互式终端（常用） |
| `/etc/profile` | 所有用户 | 登录时 |
| `/etc/profile.d/xxx.sh` | 所有用户 | 登录时（推荐） |

```bash
echo 'export PATH=$PATH:/data/bin' >> ~/.bashrc
source ~/.bashrc      # 使修改立即生效
```

> [!tip] 修改后如果不执行 `source`，改动只会在新打开的终端中生效，这是初学者常遇到的问题。

---

## 十三、防火墙

`ufw` 和 `firewalld` 都是 iptables 的友好封装，底层仍然使用 iptables。远程修改防火墙时，**先放行 22 端口再启用防火墙**，否则可能断开 SSH 连接。

> [!warning] 远程操作防火墙前，务必确认 22 端口已放行。否则一旦启用防火墙，可能立即失去远程连接，只能通过控制台恢复。

### 1. ufw —— Ubuntu 的防火墙

```bash
sudo ufw allow 22/tcp            # 先放行 SSH
sudo ufw enable                  # 再启用防火墙
sudo ufw status                  # 查看状态和规则
sudo ufw allow 8080/tcp          # 开放端口
sudo ufw delete allow 8080/tcp   # 删除规则
sudo ufw disable                 # 关闭防火墙
```

| 子命令 | 说明 |
| --- | --- |
| `allow 端口/协议` | 放行指定端口 |
| `delete allow 端口/协议` | 删除对应规则 |
| `enable` / `disable` | 启用 / 关闭防火墙 |
| `status` | 查看状态和规则 |

> Ubuntu 默认未启用 ufw，`status` 显示 `inactive` 即表示未启用。ufw 的规则立即生效并持久化，重启后不会丢失。

### 2. firewall-cmd —— CentOS 7 及以上的防火墙

```bash
sudo systemctl status firewalld                       # 查看防火墙服务状态
sudo firewall-cmd --list-ports                        # 查看已开放的端口
sudo firewall-cmd --add-port=8080/tcp --permanent     # 开放端口（永久生效）
sudo firewall-cmd --remove-port=8080/tcp --permanent  # 关闭端口
sudo firewall-cmd --reload                            # 重新加载，使永久规则生效
sudo firewall-cmd --list-all                          # 查看全部规则
```

| 选项 | 说明 |
| --- | --- |
| `--list-ports` | 查看已开放的端口 |
| `--add-port=端口/协议` | 开放端口 |
| `--remove-port=端口/协议` | 关闭端口 |
| `--permanent` | 永久生效，需要配合 `--reload` |
| `--reload` | 重新加载规则 |
| `--list-all` | 查看全部规则 |

> [!warning] 不加 `--permanent` 的规则只临时生效，重启后丢失；加了 `--permanent` 后还必须执行 `--reload` 才会生效。

### 3. iptables —— 底层防火墙规则

```bash
sudo iptables -L -n                                  # 查看规则
sudo iptables -A INPUT -p tcp --dport 80 -j ACCEPT   # 放行 80 端口
sudo iptables -D INPUT -p tcp --dport 80 -j ACCEPT   # 删除对应规则
```

| 选项 | 说明 |
| --- | --- |
| `-L` | 列出规则 |
| `-n` | 以数字形式显示 IP 和端口，不解析名称 |
| `-A 链` | 在指定链末尾追加规则 |
| `-D 链` | 删除指定链中的规则 |
| `-p 协议` | 指定协议，如 tcp、udp |
| `--dport 端口` | 指定目标端口 |
| `-j 动作` | 指定动作，如 ACCEPT、DROP |

> 这样添加的规则重启后会丢失。需要持久化时使用 `iptables-save` 保存并在开机时恢复；日常管理建议使用 ufw 或 firewalld。

> [!note] 云服务器还有一层安全组
> 阿里云、腾讯云、AWS 等云平台提供安全组，相当于在实例外层再加一道防火墙。系统内已放行端口但仍无法访问时，通常需要在安全组中放行对应端口。

---

## 附：速查总表

| 类别    | 常用命令                                                                            |
| ----- | ------------------------------------------------------------------------------- |
| 目录    | `ls -lh`、`cd -`、`pwd`、`mkdir -p`、`rmdir`                                        |
| 文件    | `cp -r`、`mv`、`rm -rf`、`touch`、`find -name`、`which`                              |
| 查看内容  | `cat -n`、`less -N`、`tail -f`、`head -n`、`more`                                   |
| 搜索文本  | `grep -rn`、`wc -l`、`sed -i 's/a/b/g'`、`awk '{print $1}'`                        |
| 管道重定向 | 管道符、`>`、`>>`、`2>&1`、`/dev/null`、`tee`                                           |
| 权限    | `chmod 755`、`chmod +x`、`chown -R`、`chgrp`、`umask`                               |
| 网络    | `ip a`、`ping -c`、`traceroute`、`mtr -r`、`ss -tunlp`、`curl -I`、`ssh`、`scp`        |
| 编辑压缩  | `vim`、`tar -czvf`、`tar -xzvf`、`zip -r`、`unzip`、`gzip -d`                        |
| 软件包   | `apt update`、`apt install`、`apt purge`、`yum install`、`dnf`、`dpkg -i`、`rpm -ivh` |
| 进程服务  | `ps -ef`、`top`、`kill -9`、`systemctl`、`journalctl -u`、`nohup &`                  |
| 系统状态  | `df -h`、`du -sh`、`free -h`、`uptime`、`uname -a`                                  |
| 环境变量  | `env`、`export`、`unset`、`source`、`~/.bashrc`、`/etc/profile`                      |
| 防火墙   | `ufw`（Ubuntu）、`firewall-cmd`（CentOS）、`iptables`                                 |

