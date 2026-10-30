# Linux 常用命令速查手册（Ubuntu ＆ CentOS 双版）

> 按用途分类整理，覆盖**目录、文件、文本、权限、进程、网络、编辑器、软件包、环境变量、防火墙**等高频场景。
>
> 同时适用于 **Ubuntu（Debian 系）** 与 **CentOS（RHEL 系）**，差异处用 `[Ubuntu]` / `[CentOS]` 标注。初次使用请先读 [[#〇、Ubuntu 与 CentOS 的核心差异]]。

**阅读约定**

- `[Ubuntu]` 的命令默认需要加 `sudo`；`[CentOS]` 的命令默认以 root 身份直接执行，两者互换时只需加减 `sudo`。
- 赶时间可以直接跳到文末《附：命令速查总表》，按命令名反查。
- 【AI 辅助说明】本文由 AI 生成初稿，本人进行修改、校对、整理与补充。

---

## 〇、Ubuntu 与 CentOS 的核心差异

| 方面        | Ubuntu（Debian 系）                    | CentOS（RHEL 系）                                            |
| --------- | ----------------------------------- | --------------------------------------------------------- |
| 软件包格式     | `.deb`                              | `.rpm`                                                    |
| 包管理工具     | `apt` / `apt-get` / `dpkg`          | `yum`（CentOS 7）/ `dnf`（CentOS 8+）/ `rpm`                  |
| 装包 / 卸包   | `apt install` / `apt remove`        | `yum install` / `yum remove`                              |
| 防火墙       | `ufw`（默认**未启用**）                    | `firewalld`（默认**开启**，CentOS 7+）；CentOS 6 用 `iptables`     |
| root 使用习惯 | 默认**禁用 root 登录**，日常操作靠 `sudo`       | 默认**允许 root 登录**，习惯直接以 root 操作                            |
| 本地包安装     | `dpkg -i xxx.deb`                   | `rpm -ivh xxx.rpm`                                        |
| 网卡 / IP   | `ip addr`；`ifconfig` 需自行装 net-tools | 同左                                                        |
| 服务管理      | `systemctl`（Ubuntu 15.04+）          | `systemctl`（CentOS 7+）；CentOS 6 用 `service` / `chkconfig` |
| 网络配置      | `netplan`（18.04+）/ `NetworkManager` | `/etc/sysconfig/network-scripts/` / `nmcli`               |
| 依赖处理      | `apt` / `yum` / `dnf` 都会自动处理依赖      | 同左                                                        |

### 为什么 Ubuntu 要用 sudo，而 CentOS 直接操作？

```bash
sudo <命令>     # 以 root 权限执行一次命令
sudo -i         # 切换成 root 的交互式 Shell（相当于登录 root）
sudo su -       # 切换到 root 用户
sudo apt update # 软件包类操作几乎都要加 sudo
```

- **Ubuntu**：出于安全考虑，系统默认**禁止 root 直接登录**，安装系统时创建的普通用户属于 `sudo` 组。凡是写系统目录（如 `/etc`）、装软件、改服务，命令前都要加 `sudo`。第一次使用会提示输入**当前用户的密码**（输入时屏幕无显示，属正常现象）。
- **CentOS**：允许 root 直接登录，运维习惯上常以 root 操作，命令前不加 `sudo`。
- **记忆方法**：`sudo` 后面跟的就是你自己要执行的命令，`sudo` 只负责临时提升权限；`sudo -i` / `sudo su -` 则是彻底切到 root。

---

## 一、目录操作类

### 1. mkdir — 创建目录

```bash
mkdir [选项] 目录
```

| 选项   | 说明                             |
| ---- | ------------------------------ |
| `-p` | 递归创建多级目录（父目录不存在时自动创建）          |
| `-m` | 创建时直接指定权限，如 `mkdir -m 755 dir` |
| `-v` | 显示创建过程                         |

```bash
mkdir test                # 创建单级目录
mkdir -p /data/app/logs   # 递归创建多级目录
mkdir -m 700 private      # 创建仅属主可访问的目录
mkdir dir{1..5}           # 批量创建 dir1~dir5
```

### 2. rmdir — 删除空目录

```bash
rmdir [选项] 目录       # 只能删除空目录，非空会报错
```

| 选项 | 说明 |
| --- | --- |
| `-p` | 逐级删除空目录，如 `rmdir -p a/b/c` 会连删 `c`、`b`、`a` |
| `-v` | 显示删除过程 |

### 3. ls — 显示目录内容

```bash
ls [选项] [目录]
```

**常用选项**

| 选项   | 说明                                        |
| ---- | ----------------------------------------- |
| `-a` | 显示所有文件，含 `.` 开头的隐藏文件（`-A` 则隐藏 `.` 和 `..`） |
| `-l` | 长格式：文件类型、权限、属主、大小、修改时间等详细信息               |
| `-h` | 配合 `-l`，以人类可读单位（K/M/G）显示文件大小              |
| `-t` | 按修改时间排序（最新在前）                             |
| `-S` | 按文件大小排序（内存大在前）                            |
| `-r` | 逆序排列                                      |
| `-R` | 递归列出子目录内容                                 |
| `-d` | 只显示目录本身，不展开内容                             |

**常见用法**

| 命令            | 作用                            |
| ------------- | ----------------------------- |
| `ls -al`      | 查看当前目录所有文件（含隐藏）的详细信息          |
| `ls -al /etc` | 查看 `/etc` 目录下所有文件及目录的详细信息     |
| `ll`          | 查看当前目录文件及目录的详细信息（`ls -l` 的别名） |
| `ls -lh`      | 以人类可读单位（K/M/G）显示文件大小          |
| `ls -lt`      | 按修改时间排序（最新在前）                 |
| `ls -lS`      | 按文件大小排序                       |
| `ls -d */`    | 只列出当前目录下的子目录                  |

### 4. cd — 切换目录

```bash
cd [目录]
```

| 命令 | 作用 |
| --- | --- |
| `cd /etc` | 切换到绝对路径 `/etc` |
| `cd ..` | 返回上一级目录 |
| `cd ~` 或 `cd` | 回到当前用户的家目录 |
| `cd ~user1` | 进入其他用户（user1）的家目录 |
| `cd -` | 回到上一次所在的目录 |

### 5. pwd — 显示当前路径

```bash
pwd [-P]        # 打印当前工作目录的绝对路径
```

| 选项   | 说明                           |
| ---- | ---------------------------- |
| `-P` | 显示物理路径（不展开符号链接），`-L` 为默认逻辑路径 |

---

## 二、文件操作类

### 1. cp — 复制（增）

```bash
cp [选项] 源 目标
```

| 选项 | 说明 |
| --- | --- |
| `-r` | 递归复制目录 |
| `-p` | 保留原文件属性（权限、时间戳、属主） |
| `-a` | 归档复制：相当于 `-r` 并保留全部属性与软链接，常用于备份 |
| `-i` | 覆盖前提示确认 |
| `-u` | 目标较新时跳过（增量复制） |
| `-v` | 显示复制过程 |

```bash
cp a.txt /tmp/            # 复制文件
cp -r dir1 dir2           # 复制整个目录
cp -rp dir1 dir2          # 保留属性递归复制
cp -a /data /backup       # 归档备份
```

### 2. touch — 创建空文件（增）

```bash
touch [选项] 文件
```

| 选项 | 说明 |
| --- | --- |
| `-c` | 文件不存在时不创建，只更新时间戳 |
| `-t` | 把时间戳改为指定时间，如 `touch -t 202401010830 a.txt` |
| `-d` | 用字符串指定时间，如 `touch -d "2024-01-01 08:30" a.txt` |

```bash
touch a.txt               # 创建空文件或更新文件时间戳
touch file{1..3}.log      # 批量创建 file1~file3.log
touch -c b.txt            # 文件不存在时不创建
```

### 3. rm — 删除文件或目录（删）

```bash
rm [选项] 文件/目录
```

| 选项 | 说明 |
| --- | --- |
| `-r` | 递归删除目录及其内容 |
| `-f` | 强制删除，不提示确认 |
| `-i` | 删除前逐一询问确认（安全习惯） |
| `-v` | 显示删除过程 |

| 命令 | 作用 |
| --- | --- |
| `rm file.txt` | 删除文件（会询问确认） |
| `rm -f file.txt` | 强制删除文件，不提示 |
| `rm -r dir` | 递归删除目录 |
| `rm -rf dir` | **强制递归删除**（危险操作，谨慎使用） |

> [!warning] `rm -rf /` 会清空系统，切勿尝试。重要目录用 `rm -ri` 逐项确认；执行前先 `ls` 或 `pwd` 核对路径，变量参与路径时尤其要小心。

### 4. mv — 移动 / 重命名（改）

```bash
mv [选项] 源 目标
```

| 选项 | 说明 |
| --- | --- |
| `-i` | 覆盖前提示确认 |
| `-f` | 强制覆盖，不提示 |
| `-n` | 目标已存在时不覆盖 |
| `-v` | 显示移动过程 |

```bash
mv a.txt /tmp/            # 移动文件
mv a.txt b.txt            # 重命名文件
mv dir1 /data/            # 移动目录
mv old.txt{,.bak}         # 快速备份改名（old.txt → old.txt.bak）
```

### 5. ln — 创建链接（改）

```bash
ln [选项] 源 目标
```

| 选项 | 说明 |
| --- | --- |
| `-s` | 创建软链接（符号链接），最常用 |
| `-f` | 目标已存在时强制覆盖 |

```bash
ln -s /data/app/current /opt/app   # 软链接（推荐用绝对路径）
ln -s ../logs app/logs             # 相对路径软链接（随父目录移动仍有效）
ln a.txt hard.txt                  # 硬链接（同一 inode，不能跨分区）
ls -l /opt/app                     # 查看链接指向
readlink -f /opt/app               # 解析出真实路径
```

> [!tip] 删除软链接用 `rm 链接名`（不带 `-r`、不带尾部斜杠），否则可能删到被指向目录里的内容。

### 6. find — 查找文件（查）

```bash
find 路径 [选项] [表达式]
```

| 选项 | 说明 | 示例 |
| --- | --- | --- |
| `-name` | 按文件名匹配（支持通配符） | `find / -name "*.log"` |
| `-iname` | 忽略大小写匹配 | `find / -iname "readme*"` |
| `-type` | 按类型：`f` 文件、`d` 目录、`l` 链接 | `find /data -type d -name "logs"` |
| `-size` | 按大小：`+100M` 大于、`-100k` 小于 | `find / -size +100M` |
| `-mtime` | 修改天数：`-7` 7 天内、`+7` 超过 7 天 | `find / -mtime -7` |
| `-mmin` | 修改分钟数 | `find / -mmin -30` |
| `-user` / `-group` | 按属主 / 属组查找 | `find / -user nginx` |
| `-perm` | 按权限查找 | `find / -perm 777` |
| `-exec` | 对每个结果执行命令：`{}` 代表结果，`\;` 结尾 | `find /tmp -name "*.tmp" -exec rm {} \;` |

```bash
find / -name "*.log"              # 按名称查找
find /data -type d -name "logs"   # 只查找目录
find / -size +100M                # 查找大于 100M 的文件
find / -mtime -7                  # 查找 7 天内修改过的文件
find /var/log -name "*.log" -mtime +30 -exec rm {} \;   # 删除 30 天前的日志
find / -name "*.conf" 2>/dev/null # 屏蔽无权限报错
```

> 搜全盘时把 `2>/dev/null` 加在末尾，可以过滤掉大量 `Permission denied` 噪音。

### 7. which / whereis / locate — 定位命令与文件（查）

| 命令 | 作用 |
| --- | --- |
| `which nginx` | 按 `PATH` 查找命令的可执行文件位置 |
| `whereis nginx` | 查找二进制、源码与手册页位置 |
| `type ll` | 判断是别名、内建命令还是外部命令 |
| `locate nginx.conf` | 按数据库快速查文件（需 `updatedb` 更新索引） |
| `readlink -f $(which nginx)` | 解析软链接后的真实路径 |

---

## 三、文本查看与搜索类

> 按「看整体 → 看局部 → 搜内容」排列；小文件用 `cat`，大文件用 `less`，盯日志用 `tail -f`。

### 1. cat — 查看 / 拼接文件内容（查）

```bash
cat [选项] [文件]
```

| 选项 | 说明 |
| --- | --- |
| `-n` | 显示行号 |
| `-b` | 只给非空行加行号 |
| `-A` | 显示所有不可见字符（如行尾 `$`、制表符 `^I`） |
| `-s` | 压缩连续的空行为一行 |

```bash
cat a.txt                 # 一次性输出全部内容
cat a.txt b.txt > c.txt   # 合并文件
cat -n a.txt              # 带行号显示
cat /etc/os-release       # 查看系统版本
```

> 适合查看**小文件**；查看大文件请用 `less`，否则刷屏后无法回看。

### 2. less — 分页查看（可上下翻）

```bash
less [选项] 文件
```

| 选项 / 按键 | 说明 |
| --- | --- |
| `-N` | 显示行号 |
| `-S` | 长行不换行（左右方向键滚动） |
| 上下方向键 / `PageUp` `PageDown` | 上下滚动 |
| `/关键字` | 向下搜索，`n` 下一个，`N` 上一个 |
| `G` / `gg` | 跳到末尾 / 开头 |
| `q` | 退出 |

```bash
less /var/log/syslog          # 分页查看
less +F /var/log/syslog       # 先当 tail -f 用，Ctrl+C 退回浏览模式
```

> `more` 只能向下翻，功能是 `less` 的子集，记住 `less` 一个即可。`less` 不会一次性加载整个文件，看几百 MB 的日志也不卡。

### 3. head — 查看文件头部（查）

```bash
head [选项] 文件
```

| 选项 | 说明 |
| --- | --- |
| `-n 行数` | 显示前 N 行（默认 10 行） |
| `-c 字节数` | 显示前 N 个字节 |
| `-v` | 显示文件名标题 |

```bash
head a.txt            # 默认显示前 10 行
head -n 20 a.txt      # 显示前 20 行
head -20 a.txt        # 同上简写
```

### 4. tail — 查看文件尾部 / 实时追踪（查）

```bash
tail [选项] 文件
```

| 选项 | 说明 |
| --- | --- |
| `-n 行数` | 显示末尾 N 行（默认 10 行） |
| `-f` | **实时追踪文件新增内容**，常用于看日志（Ctrl+C 退出） |
| `-F` | 文件被改名 / 轮转重建后仍能继续跟踪，适合日志按天切割的场景 |
| `-v` | 显示文件名标题 |

```bash
tail a.txt                # 默认显示后 10 行
tail -n 50 a.txt          # 显示后 50 行
tail -f /var/log/syslog   # 实时滚动输出新增日志（Ctrl+C 退出）
tail -100f app.log        # 显示最后 100 行并持续追踪
```

> [!tip] `tail -f` 是排查线上问题的利器，配合 `grep` 使用更佳：`tail -f app.log | grep ERROR`。`-f` 与 `-F` 的区别：日志被 `mv` 或重建后，`-f` 会失去跟踪，`-F` 仍能跟上新文件。

### 5. grep — 文本搜索（查）

```bash
grep [选项] "模式" 文件/目录
```

| 选项 | 说明 |
| --- | --- |
| `-i` | 忽略大小写 |
| `-n` | 显示匹配行行号 |
| `-v` | 反向匹配（排除） |
| `-r` / `-R` | 递归搜索目录（`-R` 会跟随符号链接） |
| `-E` | 使用扩展正则（等价 `egrep`），支持 `\|` 或 |
| `-w` | 整词匹配 |
| `-c` | 只输出匹配的行数 |
| `-A n` / `-B n` | 同时显示匹配行的后 / 前 n 行 |
| `-l` / `-L` | 只列出包含 / 不包含匹配的文件名 |
| `-e 模式` | 指定多个模式，如 `grep -e err -e warn` |
| `--color` | 高亮匹配（多数发行版默认开启） |

```bash
grep "error" app.log              # 搜索包含 error 的行
grep -i "error" app.log           # 忽略大小写
grep -n "error" app.log           # 显示行号
grep -v "error" app.log           # 反向匹配（排除）
grep -r "keyword" /data/          # 递归搜索目录
grep -E "error|warn" app.log      # 匹配 error 或 warn
grep -c "error" app.log           # 统计匹配行数
grep -A 3 "Exception" app.log     # 顺带看异常栈后 3 行
```

### 6. wc — 统计行数 / 字数（查）

```bash
wc -l app.log     # 行数（最常用）
wc -c app.log     # 字节数
wc -w app.log     # 单词数
```

**管道组合（排障高频）**

```bash
tail -f app.log | grep ERROR                      # 实时过滤错误
grep "500" access.log | wc -l                     # 统计 500 请求数
awk '{print $1}' access.log | sort | uniq -c | sort -nr | head   # 访问 IP TOP 10
```

---

## 四、权限与属主类

### 1. chmod — 修改文件权限（改）

```bash
chmod [选项] 权限 文件
```

| 选项 | 说明 |
| --- | --- |
| `-R` | 递归修改目录下所有文件 |
| `-v` | 显示每次修改 |
| `-f` | 忽略错误信息 |
| `--reference=文件` | 参照其他文件的权限来设置 |

权限可用**数字法**（r=4, w=2, x=1）或**符号法**（u/g/o/a 加 `+`/`-`/`=`）。

```bash
chmod 755 script.sh       # rwxr-xr-x
chmod 644 a.txt           # rw-r--r--
chmod +x script.sh        # 添加可执行权限
chmod -R 755 /data/app    # 递归修改目录权限
chmod u+w,g+r a.txt       # 符号法：属主加写、属组加读
```

### 2. chown — 修改属主 / 属组（改）

```bash
chown [选项] 用户[:组] 文件
```

| 选项 | 说明 |
| --- | --- |
| `-R` | 递归修改 |
| `-v` | 显示修改过程 |

```bash
chown user a.txt                  # 修改属主
chown user:group a.txt            # 同时修改属主和属组
chown -R user:group /data/app     # 递归修改
chgrp -R group /data/app          # 只改属组
```

### 3. 权限含义速查（查）

| 数字 | 权限 | 含义 |
| --- | --- | --- |
| 7 | rwx | 读 + 写 + 执行 |
| 6 | rw- | 读 + 写 |
| 5 | r-x | 读 + 执行 |
| 4 | r-- | 只读 |
| 0 | --- | 无权限 |

**权限串怎么读**（`ls -l` 输出）

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

> 权限串第一位代表类型：`d` 目录、`-` 普通文件、`l` 软链接、`b`/`c` 设备文件；其后每 3 位一组，依次为属主（u）、属组（g）、其他用户（o）。

### 4. umask — 新文件默认权限掩码（改）

```bash
umask          # 查看当前掩码，默认通常为 022
umask 022      # 设置掩码（只对当前 Shell 有效）
```

| 掩码 | 新文件权限 | 新目录权限 |
| --- | --- | --- |
| `022` | 644（rw-r--r--） | 755（rwxr-xr-x） |
| `002` | 664 | 775 |

> 普通文件默认不带 `x`（可执行）权限，这是 Linux 的安全设计；目录必须有 `x` 才能进入。永久生效写进 `~/.bashrc` 或 `/etc/profile`（见 [[#十、环境变量类]]）。

---

## 五、进程、服务与系统状态类

### 1. ps — 查看进程（查）

| 命令 | 说明 |
| --- | --- |
| `ps -ef` | 查看所有进程完整信息（System V 风格） |
| `ps aux` | 查看所有进程（BSD 风格，含 CPU/内存占用） |
| `ps -u 用户名` | 只查看某用户的进程 |
| `ps -p PID` | 查看指定 PID |
| `ps -ef --forest` | 树形显示父子进程关系 |

```bash
ps -ef | grep java          # 查找 java 进程
ps aux --sort=-%mem | head  # 按内存占用倒序取前几名
```

### 2. top / htop — 实时监控（查）

| 命令 | 说明 |
| --- | --- |
| `top` | 实时监控进程与系统资源，`q` 退出 |
| `top -p 1234` | 只监控指定的 PID |
| `top -d 2` | 每 2 秒刷新一次 |
| `htop` | 更友好的交互式监控（需 `apt/yum install htop`） |

top 运行中快捷键：`M` 按内存排序、`P` 按 CPU 排序、`k` 杀进程、`1` 查看多核 CPU、`c` 显示完整命令行。

### 3. kill — 终止进程（删）

| 命令 | 说明 |
| --- | --- |
| `kill PID` | 默认发送 TERM(15) 信号，优雅终止 |
| `kill -9 PID` | SIGKILL 强制杀死（不可被拦截，慎用） |
| `killall nginx` | 按进程名终止 |
| `pkill -f keyword` | 按命令行关键字匹配终止 |
| `pgrep -f app.jar` | 按关键字查 PID（杀之前先确认） |
| `kill -l` | 列出所有可用信号 |

```bash
kill -9 1234             # 强杀 PID 1234
pkill -f app.jar         # 杀掉命令行里含 app.jar 的进程
pgrep -f app.jar         # 先看清要杀的是谁
```

> [!warning] 先 `kill PID`，等几秒不退再 `kill -9`。强杀会让进程来不及释放资源、写盘中断，数据库类进程尤其要慎用。

### 4. systemctl — 服务管理（两系统通用）（改）

`[Ubuntu]` 加 `sudo`，`[CentOS]` 直接执行。CentOS 6 及更早版本用 `service` / `chkconfig`。

| 命令 | 作用 |
| --- | --- |
| `systemctl start nginx` | 启动服务 |
| `systemctl stop nginx` | 停止服务 |
| `systemctl restart nginx` | 重启服务 |
| `systemctl reload nginx` | 重载配置（不断连接，优先用它） |
| `systemctl status nginx` | 查看服务状态 + 最近日志 |
| `systemctl enable nginx` | 设置开机自启 |
| `systemctl disable nginx` | 取消开机自启 |
| `systemctl is-enabled nginx` | 查看是否开机自启 |
| `systemctl list-units --type=service` | 列出所有运行中的服务 |
| `journalctl -u nginx -f` | 实时查看某服务日志 |

### 5. 后台运行（改）

| 命令 | 说明 |
| --- | --- |
| `命令 &` | 放入后台运行 |
| `jobs` | 查看当前终端后台任务 |
| `nohup 命令 &` | 后台运行且不随终端关闭而退出 |
| `Ctrl+Z` → `bg` | 把当前前台任务挂起后转入后台 |
| `fg %1` | 把后台任务调回前台 |

```bash
nohup java -jar app.jar > app.log 2>&1 &   # 后台运行 Java 应用并记录日志
jobs -l                                    # 查看后台任务及 PID
```

> 长期运行的服务建议交给 `systemctl` 托管（可开机自启、崩溃自动拉起），`nohup` 只适合临时任务。

### 6. 系统状态速查（查）

| 命令 | 作用 |
| --- | --- |
| `free -h` | 内存使用情况（重点看 available） |
| `df -h` | 磁盘分区使用率 |
| `df -i` | inode 使用率（有空间却写不进文件时看它） |
| `uptime` | 开机时长 + 1/5/15 分钟平均负载 |
| `uname -a` | 内核版本与系统架构 |
| `lscpu` / `nproc` | CPU 型号 / 核心数 |
| `dmesg -T \| tail -50` | 内核日志（查 OOM、磁盘错误） |
| `who` / `w` | 当前登录用户 |

---

## 六、端口与网络诊断类

> 本章解决「通不通、谁在听、慢在哪」。网络不通时按「网卡 → IP → 网关 → 外网 → DNS → 端口 → 防火墙」的顺序逐层排查。

### 1. ip addr — 查看网卡与 IP（查）

```bash
ip addr                 # 查看所有网卡与 IP（可简写 ip a）
ip -br addr             # 一行一块网卡，最清爽
ip addr show eth0       # 只看指定网卡
ip route                # 查看路由表与默认网关
ip route get 8.8.8.8    # 查看到某地址实际走哪块网卡
ip neigh                # 查看 ARP 邻居表
```

| 对比 | 说明 |
| --- | --- |
| `ip addr` / `ip route` | 现代写法，iproute2 自带，两系统默认都有 |
| `ifconfig` / `route` | 旧写法，属 net-tools，需 `[Ubuntu] sudo apt install net-tools` / `[CentOS] yum install net-tools` |

```bash
ifconfig -a             # 旧写法：含未启用的网卡
```

### 2. ss — 查看端口与连接（查）

```bash
ss -tunlp               # 所有 TCP/UDP 监听端口 + 进程（最常用）
ss -tunlp | grep 8080   # 查看指定端口占用
ss -tan                 # 所有 TCP 连接（含已建立）
ss -tnp state established   # 只看已建立的连接
ss -s                   # 连接数统计概览
```

| 选项 | 说明 |
| --- | --- |
| `-t` | 只看 TCP |
| `-u` | 只看 UDP |
| `-n` | 数字形式显示地址和端口（不做域名解析） |
| `-l` | 只显示监听状态 |
| `-p` | 显示对应的进程名和 PID（需 root） |
| `-a` | 显示所有连接（含非监听） |

> `ss` 由 iproute2 自带，两系统都有，比 `netstat` 快得多，优先使用。

### 3. netstat — 传统写法（查）

```bash
netstat -tunlp              # 查看所有监听端口及对应进程
netstat -tunlp | grep 8080  # 查看指定端口占用
netstat -an | grep ESTABLISHED | wc -l   # 统计已建立连接数
```

> `netstat` 属于 net-tools 包，CentOS 7+ 和较新的 Ubuntu 可能未预装：`[CentOS] yum install net-tools`，`[Ubuntu] sudo apt install net-tools`。老文档里常见，能读懂即可。

### 4. lsof — 按端口 / 文件反查进程（查）

```bash
lsof -i:8080                    # 8080 端口被谁占用
lsof -i -P -n | grep LISTEN     # 所有监听端口（-P 不转端口名，-n 不转 IP）
lsof -p 1234                    # 某进程打开了哪些文件
lsof +D /data/logs              # 目录下被打开的文件
```

> [!tip] 删了日志文件但 `df -h` 空间没释放，用 `lsof | grep deleted` 找到仍持有该文件的进程，重启它空间才会真正回收。

### 5. ping — 连通性测试（查）

```bash
ping -c 4 www.baidu.com          # 发 4 个包后自动停止（Linux 默认无限 ping）
ping -i 0.2 -c 50 192.168.1.1    # 0.2 秒一次，快速看丢包率
ping -s 1472 -M do 192.168.1.1   # 测 MTU（1472 + 28 字节头 = 1500）
```

| 结果 | 通常说明 |
| --- | --- |
| `Destination Host Unreachable` | 本机路由 / 网关有问题 |
| `Request timeout` | 对端不通，或对端 / 中间设备禁 ping |
| 能 ping IP 不能 ping 域名 | DNS 问题，查 `/etc/resolv.conf` |

> 云服务器普遍禁 ping，ping 不通不代表服务不可用，用 `nc` / `curl` 测端口更准。

### 6. nc / telnet — 端口连通探测（查）

```bash
nc -zv 192.168.1.10 3306     # 探测端口是否开放（-z 只探测不传输）
nc -zv 192.168.1.10 8000-8010   # 批量探测端口段
nc -l 8080                   # 临时在 8080 起一个监听（测试防火墙用）
telnet 192.168.1.10 80       # 旧写法，通了会显示 Connected
```

> `nc` 在 CentOS 上叫 `ncat`（`yum install nmap-ncat`），Ubuntu 用 `sudo apt install netcat-openbsd`；`telnet` 也需单独安装。

### 7. curl — HTTP 接口调试与下载（查）

```bash
curl -I https://www.baidu.com          # 只看响应头
curl -v http://localhost:8080/api      # 打印完整请求/响应过程（排障首选）
curl -s -o /dev/null -w "%{http_code} %{time_total}s\n" https://example.com   # 只看状态码与耗时
curl -X POST -H "Content-Type: application/json" -d '{"name":"test"}' http://localhost:8080/api
curl -s ifconfig.me                    # 查看本机公网出口 IP
curl -O https://example.com/file.tar.gz   # 按远端文件名保存
```

| 选项 | 说明 |
| --- | --- |
| `-I` | 只查看响应头 |
| `-o 文件名` | 保存响应内容到文件 |
| `-O` | 用远端文件名保存 |
| `-s` | 静默模式（不显示进度条） |
| `-S` | 配合 `-s`，出错时仍打印错误 |
| `-L` | 跟随 301/302 跳转 |
| `-k` | 跳过 HTTPS 证书校验（仅测试环境） |
| `-X POST` | 指定请求方法 |
| `-H "头"` | 添加请求头 |
| `-d "数据"` | 发送 POST 数据 |
| `-u 用户:密码` | Basic 认证 |
| `-x 代理地址` | 走代理 |
| `-w "格式"` | 自定义输出，如 `%{http_code}`、`%{time_total}` |

### 8. wget — 文件下载（查）

```bash
wget http://xxx/file.tar.gz            # 下载文件
wget -c http://xxx/file.tar.gz         # 断点续传（大文件必备）
wget -O newname.tar.gz http://xxx/f    # 改名保存
wget -P /data http://xxx/file.tar.gz   # 下载到指定目录
wget -b http://xxx/big.iso             # 后台下载，日志写 wget-log
```

| 选项 | 说明 |
| --- | --- |
| `-c` | 断点续传 |
| `-O 文件名` | 改名保存 |
| `-P 目录` | 下载到指定目录 |
| `-q` | 静默模式 |
| `-b` | 后台下载 |
| `-r -np -k` | 递归镜像站点（不追父目录、转换链接） |
| `--no-check-certificate` | 跳过证书校验（仅测试环境） |

### 9. dig / nslookup / host — DNS 解析（查）

```bash
dig www.baidu.com +short            # 只看解析结果（最常用）
dig @8.8.8.8 www.baidu.com A        # 指定 DNS 服务器解析
dig -x 8.8.8.8                      # 反向解析（IP → 域名）
dig www.baidu.com +trace            # 从根域开始追踪解析过程
nslookup www.baidu.com              # 老工具，交互式，两系统都有
host www.baidu.com                  # 输出最简洁
cat /etc/resolv.conf                # 当前使用的 DNS 服务器
```

> 域名解析异常先看 `/etc/resolv.conf` 和 `/etc/hosts`：`/etc/hosts` 优先级高于 DNS，配错会「明明改了解析却不生效」。

### 10. traceroute / mtr — 路由追踪（查）

```bash
traceroute -n www.baidu.com    # -n 不做域名反解，速度快
mtr -n www.baidu.com           # 动态持续追踪，看丢包在哪一跳，排障更好用
```

> `traceroute` 需安装（`[Ubuntu] sudo apt install traceroute`，`[CentOS] yum install traceroute`），`mtr` 同理。中间跳 `* * *` 多为设备禁 ICMP，不代表链路断了。

### 11. tcpdump — 抓包（查）

```bash
tcpdump -i any -nn port 8080                    # 抓 8080 端口的包
tcpdump -i eth0 -nn host 192.168.1.10 and port 3306
tcpdump -i eth0 -nn -A -s0 port 80              # -A 以 ASCII 显示内容
tcpdump -i eth0 -nn -w /tmp/http.pcap port 80   # 存成 pcap，下载到本地用 Wireshark 看
tcpdump -i eth0 -nn 'tcp[tcpflags] & tcp-syn != 0'   # 只看 SYN 包
```

| 选项 | 说明 |
| --- | --- |
| `-i 网卡` | 指定网卡，`any` 表示全部 |
| `-nn` | 不解析主机名与端口名 |
| `-c N` | 抓 N 个包后停止 |
| `-w 文件` | 写入 pcap 文件 |
| `-A` / `-X` | 以 ASCII / 十六进制 + ASCII 显示载荷 |
| `-s0` | 抓完整包（不截断） |

> [!tip] 判断「包到没到本机」最好的办法就是抓包：本机抓不到 SYN，说明问题在链路或防火墙；抓到了但没有 SYN-ACK，说明对端没响应或被拦。

### 12. 网络故障排查套路

> [!note] 按顺序走一遍，九成问题能定位
> 1. `ip -br addr` —— 网卡是否 UP、有没有 IP。
> 2. `ping 网关` —— 局域网通不通；`ip route` 确认默认网关。
> 3. `ping 8.8.8.8` —— 出网通不通（能通说明链路 OK）。
> 4. `ping www.baidu.com` —— 不通则是 DNS 问题，查 `/etc/resolv.conf`。
> 5. `ss -tunlp` —— 服务是否在监听，监听在 `0.0.0.0` 还是 `127.0.0.1`（后者外部访问不到）。
> 6. `nc -zv 目标IP 端口` / `curl -v` —— 对端端口是否可达。
> 7. `ufw status` / `firewall-cmd --list-all` —— 本机防火墙；云服务器还要查**安全组**。
> 8. `tcpdump` —— 最后一招，看包到底走到哪一层。

---

## 七、网络配置与远程传输类

> 本章解决「怎么配、怎么连、怎么传」。改网络配置有断连风险，优先在控制台 / 带外环境操作。

### 1. ip — 临时修改 IP 与路由（改）

```bash
sudo ip addr add 192.168.1.10/24 dev eth0     # 临时加一个 IP
sudo ip addr del 192.168.1.10/24 dev eth0     # 删除 IP
sudo ip link set eth0 up                      # 启用网卡
sudo ip link set eth0 down                    # 禁用网卡
sudo ip route add default via 192.168.1.1     # 临时加默认网关
sudo ip route del default                     # 删除默认网关
```

> [!warning] `ip` 命令改的配置**重启即失效**，只用于临时调试。要持久化请改 netplan / nmcli / ifcfg 配置文件。

### 2. 静态 IP — Ubuntu（netplan，18.04+）（改）

```bash
ls /etc/netplan/                       # 找到配置文件，如 01-netcfg.yaml
sudo vi /etc/netplan/01-netcfg.yaml
```

```yaml
network:
  version: 2
  renderer: networkd
  ethernets:
    eth0:
      dhcp4: false
      addresses: [192.168.1.10/24]
      routes:
        - to: default
          via: 192.168.1.1
      nameservers:
        addresses: [8.8.8.8, 114.114.114.114]
```

```bash
sudo netplan try      # 试用 120 秒，配错会自动回滚（推荐）
sudo netplan apply    # 直接生效
```

> [!warning] YAML **只能用空格缩进**，用 Tab 会直接报错。Ubuntu 16.04 及更早版本用 `/etc/network/interfaces`，不是 netplan。

### 3. 静态 IP — nmcli（两系统通用，NetworkManager）（改）

```bash
nmcli device status                                  # 查看网卡状态
nmcli connection show                                # 查看连接配置名
sudo nmcli con mod eth0 ipv4.addresses 192.168.1.10/24
sudo nmcli con mod eth0 ipv4.gateway 192.168.1.1
sudo nmcli con mod eth0 ipv4.dns "8.8.8.8 114.114.114.114"
sudo nmcli con mod eth0 ipv4.method manual
sudo nmcli con up eth0                               # 应用配置
```

> CentOS 8+ / 新版 Ubuntu 桌面版推荐用 `nmcli` 或图形化 `nmtui`，比改配置文件更不容易出错。

### 4. 静态 IP — CentOS 7（ifcfg）（改）

```bash
vi /etc/sysconfig/network-scripts/ifcfg-eth0
```

```ini
TYPE=Ethernet
BOOTPROTO=static
NAME=eth0
DEVICE=eth0
ONBOOT=yes
IPADDR=192.168.1.10
PREFIX=24
GATEWAY=192.168.1.1
DNS1=8.8.8.8
DNS2=114.114.114.114
```

```bash
systemctl restart network     # CentOS 7 生效
nmcli connection reload       # 或只重载配置
```

> CentOS 6 用 `service network restart`；CentOS 8+ 已移除该目录，改用 `nmcli` / `nmtui`。

### 5. hostname / hostnamectl — 主机名（改）

```bash
hostname                 # 查看主机名
hostname -I              # 查看本机所有 IP（排查多网卡很方便）
hostnamectl              # 查看主机名、系统版本、内核、虚拟化信息
sudo hostnamectl set-hostname web01   # 永久修改（CentOS 7+ / Ubuntu 16.04+）
```

> 临时改（重启失效）用 `hostname web01`；`hostnamectl` 才是持久化。

### 6. DNS 与 hosts 配置（改）

```bash
cat /etc/resolv.conf          # 当前 DNS 服务器
cat /etc/hosts                # 本地静态解析，优先级高于 DNS
sudo vi /etc/hosts            # 加一行：192.168.1.10  db.local
```

```text
# /etc/resolv.conf 示例
nameserver 8.8.8.8
nameserver 114.114.114.114
```

> [!warning] 装了 NetworkManager 的系统会**重写** `/etc/resolv.conf`，直接改它重启就丢。正确做法是改网卡配置里的 `DNS1` / `nameservers` / `nmcli ipv4.dns`。

### 7. ssh — 远程登录（查 / 改）

```bash
ssh user@192.168.1.10              # 登录远程主机
ssh -p 2222 user@host              # 指定端口
ssh -i ~/.ssh/id_rsa user@host     # 指定私钥
ssh user@host "df -h"              # 只执行一条命令，不进入交互
ssh -v user@host                   # 登录失败时看详细握手过程
```

**免密登录**

```bash
ssh-keygen -t ed25519 -C "me@laptop"   # 一路回车，生成 ~/.ssh/id_ed25519
ssh-copy-id user@host                  # 把公钥追加到远端 authorized_keys
```

**`~/.ssh/config` 起别名（省去重复输入）**

```text
Host web01
    HostName 192.168.1.10
    User root
    Port 2222

Host db01
    HostName 192.168.1.20
    User deploy
```

配置后直接 `ssh web01` 即可；`scp` / `rsync` 也能用别名。

| 常见报错 | 原因 |
| --- | --- |
| `Permission denied (publickey)` | 公钥没上传、权限过宽（`~/.ssh` 700、`authorized_keys` 600）、`PasswordAuthentication no` |
| `Connection refused` | 对端 sshd 没启动或端口不对：`ss -tunlp \| grep 22` |
| `Connection timed out` | 防火墙 / 安全组没放行 22 端口 |
| `Host key verification failed` | 服务器重装过，`ssh-keygen -R 主机` 清掉旧指纹 |

### 8. scp — 远程复制（查 / 改）

```bash
scp file.tar.gz user@host:/data/       # 本地 → 远程
scp -r dir user@host:/data/            # 递归传目录
scp user@host:/var/log/app.log ./      # 远程 → 本地
scp -P 2222 file user@host:/data/      # 指定端口（注意是大写 P）
```

> `scp` 每传一次都是全新拷贝，大量小文件或需要增量同步时用 `rsync` 更快。

### 9. rsync — 增量同步（查 / 改）

```bash
rsync -avz --progress src/ user@host:/data/src/    # 增量同步（-a 保留属性，-z 压缩传输）
rsync -avz -e "ssh -p 2222" src/ user@host:/data/  # 走非标准 SSH 端口
rsync -avz --exclude="*.log" src/ user@host:/data/ # 排除文件
rsync -avz --dry-run src/ user@host:/data/         # 先演练，只打印不执行
rsync -avz --delete src/ /backup/src/              # 危险：目标端多余文件会被删除
```

| 选项 | 说明 |
| --- | --- |
| `-a` | 归档模式，等价 `-rlptgoD`（保留权限、时间、链接等） |
| `-v` | 显示过程 |
| `-z` | 传输时压缩（内网大文件可去掉） |
| `-P` | 显示进度 + 支持断点续传 |
| `--exclude` | 排除匹配项 |
| `--delete` | 删除目标端多余文件（务必先 `--dry-run`） |

> [!tip] 源路径**结尾带斜杠** `src/` 表示「同步目录里的内容」，不带 `src` 表示「同步目录本身」（目标下会多一层 `src`）。这个斜杠是最容易出错的地方。

---

## 八、编辑器与压缩类

### 1. vi / vim — 文本编辑器（改）

```bash
vi a.txt        # 打开文件
vim a.txt       # 打开文件（增强版）
```

**先记一句话**：vim 有三种模式，一切操作都从**命令模式**出发；任何模式下按 `Esc` 都能回到命令模式——这是最安全的「万能键」，按错了就先 `Esc`。

**模式切换关系**

![[模式切换关系.png]]

| 模式 | 进入方式 | 能做什么 |
| --- | --- | --- |
| 命令模式（普通模式） | 启动即进入；任何模式按 `Esc` 返回 | 移动光标、删除、复制粘贴、撤销，以及进入另两种模式 |
| 插入模式 | 命令模式按 `i`（光标前）/ `a`（光标后）/ `o`（下方新行） | 输入 / 修改文本；完成后按 `Esc` 回到命令模式 |
| 末行模式（命令行模式） | 命令模式按 `:` | 保存、退出、查找替换、设置参数、执行命令 |

> [!warning] 新手最易混淆：`dd`、`yy`、`p`、`u` 这些**不加冒号**，直接按键，属于命令模式；`w`、`q`、`s` 这些**必须加冒号**，属于末行模式。

**命令模式常用操作（不加冒号，直接按键）**

| 分类 | 按键 | 作用 |
| --- | --- | --- |
| 光标移动 | `h` `j` `k` `l` | 左 / 下 / 上 / 右（方向键也可用） |
| | `0` / `$` | 跳到行首 / 行尾 |
| | `gg` / `G` | 跳到文件首行 / 末尾；`3G` 跳到第 3 行 |
| | `w` / `b` | 按单词向后 / 向前跳 |
| 删除 | `x` | 删除光标处字符 |
| | `dd` | 删除整行（`3dd` 删除 3 行） |
| | `d$` / `D` | 删除从光标到行尾 |
| 复制粘贴 | `yy` | 复制整行（`3yy` 复制 3 行） |
| | `p` | 粘贴到下一行 |
| 撤销 / 重做 | `u` / `Ctrl+r` | 撤销 / 重做 |
| 搜索 | `/word` 回车 | 向下搜索；`n` 下一个，`N` 上一个；`?word` 为向上搜索 |

**末行模式常用操作（先按 `:`，输入命令后回车）**

| 命令 | 作用 |
| --- | --- |
| `:w` | 保存 |
| `:q` | 退出（未保存会提示） |
| `:wq` / `:x` | 保存并退出 |
| `:q!` | 不保存强制退出 |
| `:w !sudo tee %` | 忘了加 sudo 打开只读文件时，提权保存 |
| `:set nu` / `:set nonu` | 显示 / 取消行号 |
| `:s/old/new` | 替换当前行第一个匹配 |
| `:%s/old/new/g` | 全文件替换；加 `c` 则逐个确认 |
| `:noh` | 取消搜索高亮 |
| `:行号` | 跳到指定行 |
| `:!命令` | 临时执行 Shell 命令，如 `:!ls` |

**启动 vim 的常用选项**

| 命令 | 作用 |
| --- | --- |
| `vim -R file` | 只读模式打开（防止误改） |
| `vim +n file` | 打开并跳到第 n 行 |
| `vim +/word file` | 打开并定位到 word 首次出现处 |
| `vim -o a.txt b.txt` | 水平分屏打开多个文件（`-O` 为垂直分屏） |

> [!tip] 入门建议：终端执行 `vimtutor`（vim 自带的 30 分钟交互教程）。`[Ubuntu]` 系统自带的是精简版 vim-tiny，装完整版用 `sudo apt install vim`；`[CentOS]` 用 `yum install vim-enhanced`。

### 2. tar — 打包压缩

```bash
tar [选项] 归档文件 [要处理的目录/文件]
```

| 选项 | 说明 |
| --- | --- |
| `-c` | 创建归档（打包） |
| `-x` | 解压 |
| `-z` | 使用 gzip（对应 `.tar.gz` / `.tgz`） |
| `-j` | 使用 bzip2（对应 `.tar.bz2`） |
| `-J` | 使用 xz（对应 `.tar.xz`，压缩率最高） |
| `-v` | 显示处理过程 |
| `-f` | 指定归档文件名（习惯放最后） |
| `-t` | 查看归档内容（不解压） |
| `-C 目录` | 解压到指定目录 |
| `--exclude=路径` | 排除某路径不打进包 |

```bash
tar -czvf logs.tar.gz /data/logs     # 压缩
tar -xzvf logs.tar.gz                # 解压
tar -tzvf logs.tar.gz                # 查看内容不解压
tar -xzvf logs.tar.gz -C /data       # 解压到指定目录
tar -xzvf logs.tar.gz app/config.yml # 只解压其中某个文件
tar -czvf backup.tar.gz /data --exclude="/data/logs"   # 打包时排除日志
```

> 后缀决定选项：`.tar` 仅打包（不加压缩选项）；`.tar.gz` 用 `-z`；`.tar.bz2` 用 `-j`；`.tar.xz` 用 `-J`。记不住就用 `-xzvf` 试，报错再换。

### 3. zip / unzip — 压缩包

```bash
zip -r archive.zip dir/              # 递归压缩目录
unzip archive.zip                    # 解压到当前目录
unzip -l archive.zip                 # 只列出内容
unzip -d /data archive.zip           # 解压到指定目录
unzip -o archive.zip                 # 覆盖已存在文件
zip -r -x "*.log" archive.zip dir/   # 排除指定文件
```

> 未预装时安装：`[Ubuntu] sudo apt install zip unzip`，`[CentOS] yum install zip unzip`。跨平台传文件优先用 zip（Windows 原生支持）。

### 4. gzip / bzip2 / xz — 单文件压缩

```bash
gzip app.log          # 压缩成 app.log.gz，原文件消失
gzip -d app.log.gz    # 解压（等价 gunzip）
gzip -k app.log       # 保留原文件
zcat app.log.gz       # 不解压直接查看内容
```

> 单文件压缩工具不会打包目录，多文件请用 `tar`。

---

## 九、软件包管理类

### 1. apt 系列（Debian / Ubuntu）（改）

`[Ubuntu]` 需要加 `sudo`。

| 命令 | 作用 |
| --- | --- |
| `sudo apt update` | 更新软件源索引（换源后必须执行） |
| `sudo apt upgrade` | 升级已安装软件包 |
| `sudo apt install nginx` | 安装软件包 |
| `sudo apt remove nginx` | 卸载软件包（保留配置） |
| `sudo apt purge nginx` | 彻底卸载（含配置文件） |
| `apt search keyword` | 搜索软件包（不需要 sudo） |
| `apt show nginx` | 查看包详情与版本 |
| `apt list --installed` | 列出已安装包 |
| `apt list --upgradable` | 列出可升级的包 |
| `sudo apt autoremove` | 清理无用的依赖 |
| `sudo apt clean` | 清理下载的缓存包 |

**常用选项**

| 选项 | 说明 |
| --- | --- |
| `-y` | 自动确认，跳过交互提示 |
| `--no-install-recommends` | 不安装推荐包（更精简） |
| `--reinstall` | 重装当前版本（配置被改坏时用） |

```bash
sudo apt install -y nginx        # 自动确认安装
sudo apt install ./pkg.deb       # 安装本地 deb 包
sudo apt -f install              # 修复依赖
```

> `apt` 是面向用户的友好前端，功能与 `apt-get` 等价；脚本中常用 `apt-get` 保证行为稳定。

### 2. yum / dnf（CentOS / RHEL / Fedora）（改）

`[CentOS]` 以 root 身份直接执行。CentOS 7 默认 `yum`，CentOS 8+ 默认 `dnf`（命令兼容，可混用）。

| 命令 | 作用 |
| --- | --- |
| `yum install nginx` | 安装 |
| `yum remove nginx` | 卸载 |
| `yum update` | 升级所有软件包（`yum update nginx` 只升单个） |
| `yum search nginx` | 搜索 |
| `yum info nginx` | 查看包详情 |
| `yum list installed` | 查看已安装 |
| `yum provides "*/nginx.conf"` | 反查某文件属于哪个包 |
| `yum history` | 查看安装 / 卸载历史（可 `undo` 回滚） |
| `yum clean all` | 清理缓存 |

**常用选项**

| 选项 | 说明 |
| --- | --- |
| `-y` | 自动确认，跳过交互提示 |

```bash
yum install -y nginx       # 自动确认安装（dnf 同）
yum install ./pkg.rpm      # 安装本地 rpm 包（自动处理依赖）
```

### 3. 本地包安装与依赖

| 发行版 | 命令 | 说明 |
| --- | --- | --- |
| `[Ubuntu]` | `sudo dpkg -i pkg.deb` | 不自动处理依赖；缺依赖时用 `sudo apt -f install` 修复 |
| `[CentOS]` | `rpm -ivh pkg.rpm` | `-i` 安装、`-v` 详细、`-h` 进度条；不自动处理依赖，推荐优先用 `yum localinstall` |

```bash
dpkg -l | grep nginx     # 查某个 deb 是否已安装
rpm -qa | grep nginx     # 查某个 rpm 是否已安装
rpm -ql nginx            # 列出包安装的所有文件路径
```

> `.deb` 装不进 CentOS，`.rpm` 装不进 Ubuntu。日常优先 `apt` / `yum`（自动处理依赖），`dpkg` / `rpm` 只在本地包场景使用。

### 4. 换源（国内环境加速）

```bash
# Ubuntu：备份后替换为国内镜像源
sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak
sudo sed -i 's@//.*archive.ubuntu.com@//mirrors.aliyun.com@g' /etc/apt/sources.list
sudo apt update

# CentOS 7：换 yum 源
sudo mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.bak
sudo curl -o /etc/yum.repos.d/CentOS-Base.repo https://mirrors.aliyun.com/repo/Centos-7.repo
sudo yum clean all && sudo yum makecache
```

> [!warning] 换源前务必备份原文件；源版本必须与系统版本一致（CentOS 7 用 `Centos-7.repo`），版本错配会导致依赖解析全面失败。

---

## 十、环境变量类

### 1. 查看环境变量（查）

```bash
env                     # 查看所有环境变量
echo $PATH              # 查看某个变量
printenv PATH           # 同上
set | grep JAVA         # 查看所有变量（含未导出的 Shell 变量）
```

| 常见变量 | 含义 |
| --- | --- |
| `PATH` | 命令搜索路径，命令找不到先看它 |
| `HOME` | 当前用户家目录 |
| `LANG` / `LC_ALL` | 语言与字符集，中文乱码时调它 |
| `JAVA_HOME` | JDK 安装路径，Maven / Tomcat 依赖它 |

### 2. 临时设置（改）

```bash
export JAVA_HOME=/usr/local/jdk
export PATH=$PATH:/usr/local/jdk/bin
```

| 命令 | 说明 |
| --- | --- |
| `export -n 变量` | 取消导出（变量仍存在但子进程看不到） |
| `unset 变量` | 直接删除变量 |

> 临时设置只对当前 Shell 及其子进程有效，关掉终端即失效。

### 3. 永久设置（改）

| 文件 | 生效范围 | 加载时机 |
| --- | --- | --- |
| `/etc/profile` | 所有用户 | 登录时加载 |
| `/etc/profile.d/*.sh` | 所有用户 | 登录时加载（推荐，按应用拆分文件） |
| `~/.bashrc` | 当前用户 | 每次打开终端时加载（交互式 Shell） |
| `~/.bash_profile` | 当前用户 | 登录时加载 |

```bash
echo 'export PATH=$PATH:/data/bin' >> ~/.bashrc
source ~/.bashrc        # 使修改立即生效
```

> `source`（或简写 `.`）用于重新加载配置文件，无需重新登录。加载顺序：登录 Shell 先读 `/etc/profile`，再读 `~/.bash_profile`；新开一个终端窗口走的是 `~/.bashrc`。

> [!tip] 想让所有用户都生效，别直接改 `/etc/profile`，在 `/etc/profile.d/` 下新建 `myapp.sh` 更干净，卸载时删文件即可。

---

## 十一、防火墙类

**先记住三个事实：**

1. `[CentOS]` CentOS 7+ 默认安装并**开启** `firewalld`；CentOS 6 用 `iptables`。
2. `[Ubuntu]` 默认安装 `ufw` 但**默认未启用**。
3. 两者底层都是 `iptables` / `nftables` 规则集，`firewall-cmd` 和 `ufw` 只是更友好的前端。

> [!warning] 远程操作防火墙，先放行 SSH 端口再开防火墙，否则会把自己锁在门外。

### 1. ufw — Ubuntu（改）

`[Ubuntu]` 需加 `sudo`。

| 命令 | 作用 |
| --- | --- |
| `sudo ufw enable` | 开启防火墙（默认拒绝入站流量） |
| `sudo ufw status` | 查看状态（`status verbose` 更详细） |
| `sudo ufw status numbered` | 带编号查看规则（删除时用编号更稳） |
| `sudo ufw allow 22/tcp` | 开放端口（也可写服务名 `ssh`） |
| `sudo ufw allow from 192.168.1.0/24` | 放行指定网段 |
| `sudo ufw deny 8080` | 拒绝端口 |
| `sudo ufw delete allow 8080` | 删除规则 |
| `sudo ufw delete 3` | 按编号删除规则 |
| `sudo ufw reload` | 重载规则 |
| `sudo ufw disable` | 关闭防火墙 |

```bash
sudo ufw allow 22/tcp        # 先放行 SSH，再开防火墙，防止把自己锁在外面
sudo ufw enable
sudo ufw allow 8080/tcp
sudo ufw status numbered
```

### 2. firewalld — CentOS 7+（改）

`[CentOS]` 用 systemctl 管理服务生命周期，用 `firewall-cmd` 管理规则。

```bash
systemctl start firewalld            # 启动
systemctl enable firewalld           # 开机自启
systemctl status firewalld           # 查看状态
```

| firewall-cmd 命令 | 作用 |
| --- | --- |
| `firewall-cmd --state` | 查看运行状态 |
| `firewall-cmd --list-all` | 查看所有规则 |
| `firewall-cmd --list-ports` | 只查看已开放端口 |
| `firewall-cmd --add-port=8080/tcp --permanent` | 开放端口（永久生效） |
| `firewall-cmd --remove-port=8080/tcp --permanent` | 关闭端口 |
| `firewall-cmd --add-service=http --permanent` | 开放服务 |
| `firewall-cmd --reload` | 重载配置使其生效 |
| `firewall-cmd --zone=public --add-port=8080/tcp` | 指定区域开放（不加 `--permanent` 仅当前会话有效） |

> [!warning] `firewall-cmd` 修改后必须 `--reload` 才生效；不带 `--permanent` 的规则重启后丢失。

### 3. iptables — 底层规则（两系统通用）（改）

```bash
iptables -L -n                                    # 查看规则
iptables -A INPUT -p tcp --dport 80 -j ACCEPT     # 允许 80 端口
iptables -D INPUT -p tcp --dport 80 -j ACCEPT     # 删除该规则
iptables -F                                       # 清空所有规则（危险）
```

> 直接改 iptables 的规则**重启后丢失**。持久化：`[Ubuntu]` 安装 iptables-persistent（`sudo netfilter-persistent save`）；`[CentOS]` 安装 iptables-services 后用 `service iptables save`。日常场景直接用 ufw / firewalld 更省心。

### 4. 云服务器还要看安全组

> [!note] 云主机（阿里云 / 腾讯云 / AWS）有两层防火墙：系统内的 ufw / firewalld，以及云平台的**安全组**。端口在系统里放行了却仍然不通，99% 是安全组没开。

---

## 附：命令速查总表

按章顺序排列，括号内为最常用的一两个写法。

| 类别 | 核心命令 |
| --- | --- |
| 目录操作（一） | `ls -al` `cd` `pwd` `mkdir -p` `rmdir` `du -sh` `tree -L 1` |
| 文件操作（二） | `cp -a` `rm -rf` `mv` `touch` `ln -s` `find -name` `which` |
| 文本查看（三） | `cat -n` `less -N` `head -n` `tail -f` `grep -rn` `wc -l` |
| 权限属主（四） | `chmod 755` `chown -R` `chgrp` `umask` |
| 进程服务（五） | `ps -ef` `top` `kill -9` `systemctl` `nohup &` `journalctl -u` `free -h` `df -h` |
| 网络诊断（六） | `ip addr` `ss -tunlp` `netstat` `lsof -i:80` `ping -c` `nc -zv` `curl -I` `wget -c` `dig +short` `traceroute` `tcpdump` |
| 网络配置（七） | `ip route` `netplan apply` `nmcli con mod` `ifcfg-eth0` `hostnamectl` `/etc/hosts` `ssh` `scp -r` `rsync -avz` |
| 编辑压缩（八） | `vim` `tar -czvf` `tar -xzvf` `zip -r` `unzip` |
| 软件包（九） | `apt update` `apt install` `apt purge` `yum install` `dnf` `dpkg -i` `rpm -ivh` |
| 环境变量（十） | `env` `export` `unset` `source` `/etc/profile` `~/.bashrc` |
| 防火墙（十一） | `ufw`(Ubuntu) `firewall-cmd`(CentOS) `iptables` |
| 发行版差异 | 见 [[#〇、Ubuntu 与 CentOS 的核心差异]]：sudo / 包管理 / 网络配置 / 防火墙 |
