# Linux 常用命令速查手册（Ubuntu ＆ CentOS 双版）

> 按用途分类整理，涵盖**目录、文件、文本查看、权限、软件包、环境变量、进程、端口/网络、防火墙、压缩编辑**等十大类高频命令。
>
> 本手册同时适用于 **Ubuntu（Debian 系）** 与 **CentOS（RHEL 系）**。两者有差异的地方以 `[Ubuntu]` / `[CentOS]` 标注，先读「〇、两大发行版的核心差异」。

**阅读约定：**

- `[Ubuntu]` 的命令默认需要加 `sudo`；`[CentOS]` 的命令默认以 root 身份直接执行。两者互换时只需要加减 `sudo`。

---

## 〇、Ubuntu 与 CentOS 的核心差异

| 方面 | Ubuntu（Debian 系） | CentOS（RHEL 系） |
| --- | --- | --- |
| 软件包格式 | `.deb` | `.rpm` |
| 包管理工具 | `apt` / `apt-get` / `dpkg` | `yum`（CentOS 7）/ `dnf`（CentOS 8+）/ `rpm` |
| 装包 / 卸包 | `apt install` / `apt remove` | `yum install` / `yum remove` |
| 防火墙 | `ufw`（默认**未启用**） | `firewalld`（默认**开启**，CentOS 7+）；CentOS 6 用 `iptables` |
| root 使用习惯 | 默认**禁用 root 登录**，日常操作靠 `sudo` | 默认允许 root 登录，习惯直接以 root 操作 |
| 本地包安装 | `dpkg -i xxx.deb` | `rpm -ivh xxx.rpm` |
| 网卡 / IP | `ip addr`；`ifconfig` 需自行装 net-tools | 同左 |
| 服务管理 | `systemctl`（Ubuntu 15.04+） | `systemctl`（CentOS 7+）；CentOS 6 用 `service` / `chkconfig` |
| 依赖处理 | `apt` / `yum` / `dnf` 都会自动处理依赖 | 同左 |

### 为什么 Ubuntu 要用 sudo，而 CentOS 直接操作？

```bash
sudo <命令>     # 以 root 权限执行一次命令
sudo -i         # 切换成 root 的交互式 Shell（相当于登录 root）
sudo su -       # 切换到 root 用户
sudo apt update # 软件包类操作几乎都要加 sudo
```

- **Ubuntu**：出于安全考虑，系统默认**禁止 root 直接登录**，安装系统时创建的普通用户属于 `sudo` 组。凡是写系统目录（如 `/etc`）、装软件、改服务，命令前都要加 `sudo`。第一次使用会提示输入**当前用户的密码**（输入时屏幕无显示，正常现象）。
- **CentOS**：允许 root 直接登录，运维习惯上常以 root 操作，命令前不加 `sudo`。
- **记忆方法**：`sudo` 后面跟的就是你自己要执行的命令，`sudo` 只负责临时提升权限；`sudo -i` / `sudo su -` 则是彻底切到 root。

---

## 一、目录操作类

### 1. ls — 显示目录内容

```bash
ls [选项] [目录]
```

**常用选项：**

| 选项   | 说明                                        |
| ---- | ----------------------------------------- |
| `-a` | 显示所有文件，含 `.` 开头的隐藏文件（`-A` 则隐藏 `.` 和 `..`） |
| `-l` | 长格式：文件类型、权限、属主、大小、修改时间等详细信息               |
| `-h` | 配合 `-l`，以人类可读单位（K/M/G）显示文件大小              |
| `-t` | 按修改时间排序（最新在前）                             |
| `-S` | 按文件大小排序（大在前）                              |
| `-r` | 逆序排列                                      |
| `-R` | 递归列出子目录内容                                 |
| `-d` | 只显示目录本身，不展开内容                             |
| `-i` | 显示文件 inode 号                              |

> `ls -l` 使用频繁，Linux 提供简写别名 `ll`。`[CentOS]` 默认自带；~~`[Ubuntu]` 需在 `~/.bashrc` 中取消注释 `alias ll='ls -alF'` 后执行 `source ~/.bashrc` 生效。~~

**常见用法：**

| 命令            | 作用                        |
| ------------- | ------------------------- |
| `ls -al`      | 查看当前目录所有文件（含隐藏）的详细信息      |
| `ls -al /etc` | 查看 `/etc` 目录下所有文件及目录的详细信息 |
| `ll`          | 查看当前目录文件及目录的详细信息          |
| `ls -lh`      | 以人类可读单位（K/M/G）显示文件大小      |
| `ls -lt`      | 按修改时间排序（最新在前）             |
| `ls -lS`      | 按文件大小排序                   |

### 2. cd — 切换目录

```bash
cd [目录]
```

| 命令            | 作用                |
| ------------- | ----------------- |
| `cd /etc`     | 切换到绝对路径 `/etc`    |
| `cd ..`       | 返回上一级目录           |
| `cd ~` 或 `cd` | 回到当前用户的家目录        |
| `cd ~user1`   | 进入其他用户（user1）的家目录 |
| `cd -`        | 回到上一次所在的目录        |

### 3. pwd — 显示当前路径

```bash
pwd [-P]        # 打印当前工作目录的绝对路径
```

| 选项   | 说明                           |
| ---- | ---------------------------- |
| `-P` | 显示物理路径（不展开符号链接），`-L` 为默认逻辑路径 |

### 4. mkdir — 创建目录

```bash
mkdir [选项] 目录
```

| 选项 | 说明 |
| --- | --- |
| `-p` | 递归创建多级目录（父目录不存在时自动创建） |
| `-m` | 创建时直接指定权限，如 `mkdir -m 755 dir` |
| `-v` | 显示创建过程 |

```bash
mkdir test                # 创建单级目录
mkdir -p /data/app/logs   # 递归创建多级目录
mkdir -m 700 private      # 创建仅属主可访问的目录
```

### 5. rmdir — 删除空目录

```bash
rmdir [选项] 目录       # 只能删除空目录，非空会报错
```

| 选项 | 说明 |
| --- | --- |
| `-p` | 逐级删除空目录，如 `rmdir -p a/b/c` 会连删 `c`、`b`、`a` |
| `-v` | 显示删除过程 |

> 删除非空目录请用 `rm -rf`（见下文，谨慎使用）。

---

## 二、文件操作类

### 1. rm — 删除文件或目录

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

> `rm -rf /` 会清空系统，切勿尝试。重要目录可用 `rm -ri` 逐项确认。

### 2. cp — 复制

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

### 3. mv — 移动 / 重命名

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
```

### 4. touch — 创建空文件

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
touch -c b.txt            # 文件不存在时不创建
```

### 5. find — 查找文件

```bash
find 路径 [选项] [表达式]
```

| 选项                 | 说明                          | 示例                                       |
| ------------------ | --------------------------- | ---------------------------------------- |
| `-name`            | 按文件名匹配（支持通配符）               | `find / -name "*.log"`                   |
| `-iname`           | 忽略大小写匹配                     | `find / -iname "readme*"`                |
| `-type`            | 按类型：`f` 文件、`d` 目录、`l` 链接    | `find /data -type d -name "logs"`        |
| `-size`            | 按大小：`+100M` 大于、`-100k` 小于   | `find / -size +100M`                     |
| `-mtime`           | 修改天数：`-7` 7 天内、`+7` 超过 7 天  | `find / -mtime -7`                       |
| `-mmin`            | 修改分钟数                       | `find / -mmin -30`                       |
| `-user` / `-group` | 按属主 / 属组查找                  | `find / -user nginx`                     |
| `-perm`            | 按权限查找                       | `find / -perm 777`                       |
| `-exec`            | 对每个结果执行命令：`{}` 代表结果，`\;` 结尾 | `find /tmp -name "*.tmp" -exec rm {} \;` |

```bash
find / -name "*.log"              # 按名称查找
find /data -type d -name "logs"   # 只查找目录
find / -size +100M                # 查找大于 100M 的文件
find / -mtime -7                  # 查找 7 天内修改过的文件
find /var/log -name "*.log" -mtime +30 -exec rm {} \;   # 删除 30 天前的日志
```

---

## 三、文本查看类

### 1. cat — 查看 / 拼接文件内容

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
```

> 适合查看**小文件**；查看大文件请用 `more` / `less`。

### 2. more — 分页查看（只能向下翻）

```bash
more [选项] 文件
```

| 按键 / 选项 | 说明 |
| --- | --- |
| 空格键 / 回车 | 下一页 / 下一行 |
| `q` | 退出 |
| `+行号` | 从指定行开始显示，如 `more +50 a.txt` |
| `-数字` | 每屏显示行数 |

### 3. less — 分页查看（可上下翻，推荐）

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

> 推荐 `less`：支持前后翻页与搜索，且不会一次性加载整个文件。

### 4. head — 查看文件头部

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

### 5. tail — 查看文件尾部（重点）

```bash
tail [选项] 文件
```

| 选项 | 说明 |
| --- | --- |
| `-n 行数` | 显示末尾 N 行（默认 10 行） |
| `-f` | **实时追踪文件新增内容**，常用于看日志（Ctrl+C 退出） |
| `-F` | 文件被改名/轮转重建后仍能继续跟踪，适合日志按天切割的场景 |
| `-v` | 显示文件名标题 |

```bash
tail a.txt                # 默认显示后 10 行
tail -n 50 a.txt          # 显示后 50 行
tail -f /var/log/syslog   # 实时滚动输出新增日志（Ctrl+C 退出）
tail -100f app.log        # 显示最后 100 行并持续追踪
```

> `tail -f` 是排查线上问题的利器，配合 `grep` 使用更佳：`tail -f app.log | grep ERROR`。`-f` 与 `-F` 的区别：日志文件被 `mv` 或重建后，`-f` 会失去跟踪，`-F` 仍能跟上新的文件。

### 6. grep — 文本搜索

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
```

---

## 四、权限管理类

### 1. chmod — 修改文件权限

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

### 2. chown — 修改属主 / 属组

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

### 3. 权限含义速查

| 数字 | 权限 | 含义 |
| --- | --- | --- |
| 7 | rwx | 读 + 写 + 执行 |
| 6 | rw- | 读 + 写 |
| 5 | r-x | 读 + 执行 |
| 4 | r-- | 只读 |
| 0 | --- | 无权限 |

> 权限串第一位代表类型：`d` 目录、`-` 普通文件、`l` 软链接、`b`/`c` 设备文件。每 3 位一组，依次为属主（u）、属组（g）、其他用户（o）。

### 4. umask — 新文件默认权限掩码（补充）

```bash
umask          # 查看当前掩码，默认通常为 022
umask 022      # 设置掩码
```

| 掩码 | 新文件权限 | 新目录权限 |
| --- | --- | --- |
| `022` | 644（rw-r--r--） | 755（rwxr-xr-x） |
| `002` | 664 | 775 |

> 普通文件默认不带 `x`（可执行）权限，这是 Linux 的安全设计；目录必须有 `x` 才能进入。

---

## 五、软件包管理类

### 1. apt 系列（Debian / Ubuntu）

`[Ubuntu]` 需要加 `sudo`。

| 命令 | 作用 |
| --- | --- |
| `sudo apt update` | 更新软件源索引（换源后必须执行） |
| `sudo apt upgrade` | 升级已安装软件包 |
| `sudo apt install nginx` | 安装软件包 |
| `sudo apt remove nginx` | 卸载软件包（保留配置） |
| `sudo apt purge nginx` | 彻底卸载（含配置文件） |
| `apt search keyword` | 搜索软件包（不需要 sudo） |
| `apt list --installed` | 列出已安装包 |
| `sudo apt autoremove` | 清理无用的依赖 |
| `sudo apt clean` | 清理下载的缓存包 |

**常用选项：**

| 选项 | 说明 |
| --- | --- |
| `-y` | 自动确认，跳过交互提示 |
| `--no-install-recommends` | 不安装推荐包（更精简） |

```bash
sudo apt install -y nginx        # 自动确认安装
sudo apt install ./pkg.deb       # 安装本地 deb 包
```

> `apt` 是面向用户的友好前端，功能与 `apt-get` 等价；脚本中常用 `apt-get` 保证行为稳定。

### 2. yum / dnf（CentOS / RHEL / Fedora）

`[CentOS]` 以 root 身份直接执行。CentOS 7 默认 `yum`，CentOS 8+ 默认 `dnf`（dnf 命令与 yum 兼容，可混用）。

| 命令 | 作用 |
| --- | --- |
| `yum install nginx` | 安装 |
| `yum remove nginx` | 卸载 |
| `yum update` | 升级所有软件包 |
| `yum search nginx` | 搜索 |
| `yum list installed` | 查看已安装 |
| `yum clean all` | 清理缓存 |

**常用选项：**

| 选项 | 说明 |
| --- | --- |
| `-y` | 自动确认，跳过交互提示 |

```bash
yum install -y nginx       # 自动确认安装（dnf 同）
yum install ./pkg.rpm      # 安装本地 rpm 包
```

### 3. 本地包安装与依赖

| 发行版 | 命令 | 说明 |
| --- | --- | --- |
| `[Ubuntu]` | `sudo dpkg -i pkg.deb` | 不自动处理依赖；缺依赖时用 `sudo apt -f install` 修复 |
| `[CentOS]` | `rpm -ivh pkg.rpm` | `-i` 安装、`-v` 详细、`-h` 进度条；不自动处理依赖，推荐优先用 `yum localinstall` |

> `.deb` 装不进 CentOS，`.rpm` 装不进 Ubuntu。日常使用请优先 `apt` / `yum`（自动处理依赖），`dpkg` / `rpm` 只在本地包场景使用。

---

## 六、环境变量类

### 1. 查看环境变量

```bash
env                     # 查看所有环境变量
echo $PATH              # 查看某个变量
printenv PATH           # 同上
```

### 2. 临时设置（当前会话有效）

```bash
export JAVA_HOME=/usr/local/jdk
export PATH=$PATH:/usr/local/jdk/bin
```

| 选项 | 说明 |
| --- | --- |
| `export -n 变量` | 取消导出（变量仍存在但子进程看不到） |
| `unset 变量` | 直接删除变量 |

### 3. 永久设置

| 文件 | 生效范围 | 加载时机 |
| --- | --- | --- |
| `/etc/profile` | 所有用户 | 登录时加载 |
| `~/.bashrc` | 当前用户 | 每次打开终端时加载（交互式 Shell） |
| `~/.bash_profile` | 当前用户 | 登录时加载 |

```bash
echo 'export PATH=$PATH:/data/bin' >> ~/.bashrc
source ~/.bashrc        # 使修改立即生效
```

> `source`（或简写 `.`）用于重新加载配置文件，无需重新登录。判断语句顺序：登录 Shell 会先读 `/etc/profile`，再读 `~/.bash_profile`；而新开一个终端窗口走的是 `~/.bashrc`。

---

## 七、进程与服务管理类

### 1. ps — 查看进程

| 选项 | 说明 |
| --- | --- |
| `ps -ef` | 查看所有进程完整信息（System V 风格） |
| `ps aux` | 查看所有进程（BSD 风格，含 CPU/内存占用） |
| `ps -u 用户名` | 只查看某用户的进程 |
| `ps -p PID` | 查看指定 PID |

```bash
ps -ef | grep java          # 查找 java 进程
```

### 2. top / htop — 实时监控

| 命令 | 说明 |
| --- | --- |
| `top` | 实时监控进程与系统资源，`q` 退出 |
| `top -p 1234` | 只监控指定的 PID |
| `top -d 2` | 每 2 秒刷新一次 |
| `htop` | 更友好的交互式监控（需安装） |

top 运行中快捷键：`M` 按内存排序、`P` 按 CPU 排序、`k` 杀进程、`1` 查看多核 CPU。

### 3. kill — 终止进程

| 命令 | 说明 |
| --- | --- |
| `kill PID` | 默认发送 TERM(15) 信号，优雅终止 |
| `kill -9 PID` | SIGKILL 强制杀死（不可被拦截，慎用） |
| `killall nginx` | 按进程名终止 |
| `pkill -f keyword` | 按命令行关键字匹配终止 |
| `kill -l` | 列出所有可用信号 |

```bash
kill -9 1234             # 强杀 PID 1234
pkill -f app.jar         # 杀掉命令行里含 app.jar 的进程
```

### 4. 后台运行

| 命令 | 说明 |
| --- | --- |
| `命令 &` | 放入后台运行 |
| `jobs` | 查看当前终端后台任务 |
| `nohup 命令 &` | 后台运行且不随终端关闭而退出 |

```bash
nohup java -jar app.jar > app.log 2>&1 &   # 后台运行 Java 应用并记录日志
```

### 5. systemctl — 服务管理（两系统通用）

`[Ubuntu]` 加 `sudo`，`[CentOS]` 直接执行。CentOS 6 及更早的版本用 `service` / `chkconfig`。

| 命令                           | 作用       |
| ---------------------------- | -------- |
| `systemctl start nginx`      | 启动服务     |
| `systemctl stop nginx`       | 停止服务     |
| `systemctl restart nginx`    | 重启服务     |
| `systemctl status nginx`     | 查看服务状态   |
| `systemctl enable nginx`     | 设置开机自启   |
| `systemctl disable nginx`    | 取消开机自启   |
| `systemctl is-enabled nginx` | 查看是否开机自启 |

---

## 八、端口 / 网络类

### 1. 端口查看

```bash
netstat -tunlp              # 查看所有监听端口及对应进程
netstat -tunlp | grep 8080  # 查看指定端口占用
ss -tunlp                   # netstat 的现代替代，速度更快、默认自带
lsof -i:8080                # 查看 8080 端口被哪个进程占用
```

| 选项 | 说明 |
| --- | --- |
| `-t` | 只看 TCP |
| `-u` | 只看 UDP |
| `-n` | 数字形式显示地址和端口（不做域名解析） |
| `-l` | 只显示监听状态 |
| `-p` | 显示对应的进程名和 PID |
| `-a` | 显示所有连接（含非监听） |

> `netstat` 属于 net-tools 包，CentOS 7+ 和较新的 Ubuntu 可能未预装：`[CentOS] yum install net-tools`，`[Ubuntu] sudo apt install net-tools`。优先使用 `ss`（iproute2 自带，两系统都有）。`lsof` 也常需单独安装。

### 2. 网络连通性

```bash
ping -c 4 www.baidu.com         # 测试连通性，-c 指定次数（Linux 默认无限 ping，Ctrl+C 停）
curl http://localhost:8080      # 请求 URL / 接口测试
wget http://xxx/file.tar.gz     # 下载文件
telnet 192.168.1.1 80           # 测试端口是否可通
traceroute www.baidu.com        # 路由追踪
ip addr                         # 查看网卡与 IP（现代写法，默认自带）
ifconfig                        # 旧写法，需安装 net-tools
```

**curl 常用选项：**

| 选项 | 说明 |
| --- | --- |
| `-I` | 只查看响应头 |
| `-o 文件名` | 保存响应内容到文件 |
| `-s` | 静默模式（不显示进度条） |
| `-X POST` | 指定请求方法 |
| `-H "头"` | 添加请求头 |
| `-d "数据"` | 发送 POST 数据 |

```bash
curl -I https://www.baidu.com          # 只看响应头
curl -X POST -H "Content-Type: application/json" -d '{"name":"test"}' http://localhost:8080/api
```

**wget 常用选项：**

| 选项 | 说明 |
| --- | --- |
| `-c` | 断点续传 |
| `-O 文件名` | 改名保存 |
| `-q` | 静默模式 |
| `-P 目录` | 下载到指定目录 |

---

## 九、防火墙类

**先记住三个事实：**

1. `[CentOS]` CentOS 7+ 默认安装并**开启** `firewalld`；CentOS 6 用 `iptables`。
2. `[Ubuntu]` 默认安装 `ufw` 但**默认未启用**。
3. 两者底层都是 `iptables` / `nftables` 规则集，`firewall-cmd` 和 `ufw` 只是更友好的前端。

### 1. ufw — Ubuntu

`[Ubuntu]` 需加 `sudo`。

| 命令 | 作用 |
| --- | --- |
| `sudo ufw enable` | 开启防火墙（默认拒绝入站流量） |
| `sudo ufw status` | 查看状态（`status verbose` 更详细） |
| `sudo ufw allow 22/tcp` | 开放端口（也可写服务名 `ssh`） |
| `sudo ufw allow from 192.168.1.0/24` | 放行指定网段 |
| `sudo ufw deny 8080` | 拒绝端口 |
| `sudo ufw delete allow 8080` | 删除规则 |
| `sudo ufw reload` | 重载规则 |

```bash
sudo ufw enable
sudo ufw allow 22/tcp        # 先放行 SSH，再开防火墙，防止把自己锁在外面
sudo ufw allow 8080/tcp
sudo ufw status
```

### 2. firewalld — CentOS 7+（RHEL 系）

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
| `firewall-cmd --add-port=8080/tcp --permanent` | 开放端口（永久生效） |
| `firewall-cmd --remove-port=8080/tcp --permanent` | 关闭端口 |
| `firewall-cmd --add-service=http --permanent` | 开放服务 |
| `firewall-cmd --reload` | 重载配置使其生效 |
| `firewall-cmd --zone=public --add-port=8080/tcp` | 指定区域开放（不加 `--permanent` 仅当前会话有效） |

> **容易踩的坑**：`firewall-cmd` 修改后必须 `--reload` 才生效；不带 `--permanent` 的规则重启后丢失。

### 3. iptables — 底层规则（两系统通用）

```bash
iptables -L -n                            # 查看规则
iptables -A INPUT -p tcp --dport 80 -j ACCEPT   # 允许 80 端口
iptables -D INPUT -p tcp --dport 80 -j ACCEPT   # 删除该规则
iptables -F                               # 清空所有规则（危险）
```

> 直接改 iptables 的规则**重启后丢失**。持久化：`[Ubuntu]` 安装 iptables-persistent（`sudo netfilter-persistent save`）；`[CentOS]` 安装 iptables-services 后用 `service iptables save`。日常场景直接用 ufw / firewalld 更省心。

### 4. 常见需求对照：开放 8080 端口

| 发行版 | 执行命令 |
| --- | --- |
| `[Ubuntu]` | `sudo ufw allow 8080/tcp` |
| `[CentOS]` 7+ | `firewall-cmd --add-port=8080/tcp --permanent && firewall-cmd --reload` |
| 通用底层 | `iptables -A INPUT -p tcp --dport 8080 -j ACCEPT` |

---

## 十、压缩与编辑类

### 1. tar — 打包压缩

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
tar -czvf backup.tar.gz /data --exclude="/data/logs"   # 打包时排除日志
```

> 后缀决定选项：`.tar` 仅打包（不加压缩选项）；`.tar.gz` 用 `-z`；`.tar.bz2` 用 `-j`；`.tar.xz` 用 `-J`。

### 2. zip / unzip — 压缩包（补充）

```bash
zip -r archive.zip dir/              # 递归压缩目录
unzip archive.zip                    # 解压到当前目录
unzip -d /data archive.zip           # 解压到指定目录
unzip -o archive.zip                 # 覆盖已存在文件
```

> 未预装时安装：`[Ubuntu] sudo apt install zip unzip`，`[CentOS] yum install zip unzip`。

### 3. vi / vim — 文本编辑器（三种模式重点理顺）

```bash
vi a.txt        # 打开文件
vim a.txt       # 打开文件（增强版）
```

**先记一句话**：vim 有三种模式，一切操作都从**命令模式**出发；任何模式下按 `Esc` 都能回到命令模式——这是最安全的"万能键"，按错了就先 `Esc`。

**模式切换关系：**

```text
启动 vim ───────────────────▶ 命令模式（默认进入）
命令模式 ---- i / a / o ---▶ 插入模式
命令模式 -------- : -------▶ 末行模式
插入模式 / 末行模式 -- Esc --▶ 命令模式
```

| 模式 | 进入方式 | 能做什么 |
| --- | --- | --- |
| 命令模式（普通模式） | 启动即进入；任何模式按 `Esc` 返回 | 移动光标、删除、复制粘贴、撤销，以及进入另两种模式 |
| 插入模式 | 命令模式按 `i`（光标前）/ `a`（光标后）/ `o`（下方新行） | 输入 / 修改文本；完成后按 `Esc` 回到命令模式 |
| 末行模式（命令行模式） | 命令模式按 `:` | 保存、退出、查找替换、设置参数、执行命令 |

> **新手最易混淆的点**：`dd`、`yy`、`p`、`u` 这些**不加冒号**，直接按键，属于命令模式；`w`、`q`、`s` 这些**必须加冒号**，属于末行模式。

**命令模式常用操作（不加冒号，直接按键）：**

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

**末行模式常用操作（先按 `:`，输入命令后回车）：**

| 命令 | 作用 |
| --- | --- |
| `:w` | 保存 |
| `:q` | 退出（未保存会提示） |
| `:wq` / `:x` | 保存并退出 |
| `:q!` | 不保存强制退出 |
| `:set nu` / `:set nonu` | 显示 / 取消行号 |
| `:s/old/new` | 替换当前行第一个匹配 |
| `:%s/old/new/g` | 全文件替换；加 `c` 则逐个确认 |
| `:noh` | 取消搜索高亮 |
| `:行号` | 跳到指定行 |
| `:!命令` | 临时执行 Shell 命令，如 `:!ls` |

**启动 vim 的常用选项：**

| 命令 | 作用 |
| --- | --- |
| `vim -R file` | 只读模式打开（防止误改） |
| `vim +n file` | 打开并跳到第 n 行 |
| `vim +/word file` | 打开并定位到 word 首次出现处 |
| `vim -o a.txt b.txt` | 水平分屏打开多个文件（`-O` 为垂直分屏） |

> **入门建议**：终端执行 `vimtutor`（vim 自带的 30 分钟交互教程）。`[Ubuntu]` 系统自带的是精简版 vim-tiny，装完整版用 `sudo apt install vim`；`[CentOS]` 用 `yum install vim-enhanced`。

---

## 附：命令速查总表

| 类别    | 核心命令                                               |
| ----- | -------------------------------------------------- |
| 目录操作  | `ls` `cd` `pwd` `mkdir` `rmdir`                    |
| 文件操作  | `rm` `cp` `mv` `touch` `find`                      |
| 文本查看  | `cat` `more` `less` `head` `tail -f` `grep`        |
| 权限管理  | `chmod` `chown` `chgrp` `umask`                    |
| 软件包   | `apt` `yum` `dnf` `dpkg` `rpm`                     |
| 环境变量  | `export` `env` `source` `/etc/profile` `~/.bashrc` |
| 进程与服务 | `ps` `top` `kill` `nohup` `systemctl`              |
| 端口网络  | `netstat` `ss` `lsof` `curl` `wget` `ping`         |
| 防火墙   | `ufw`(Ubuntu) `firewall-cmd`(CentOS) `iptables`    |
| 压缩编辑  | `tar` `zip` `vi` `vim`                             |
| 发行版差异 | 见「〇」节：sudo / 包管理 / 防火墙对照                           |