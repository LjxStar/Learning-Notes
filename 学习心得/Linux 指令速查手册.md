# Linux 常用命令速查手册
> 按用途分类整理，涵盖**目录、文件、文本查看、权限、软件包、环境变量、进程、端口/网络、防火墙**等九大类高频命令。

---

## 一、目录操作类

### 1. ls — 显示目录内容

```javascript
ls [-al] [dir]
```

**说明：**

- `-a` 显示所有文件及目录（`.` 开头的隐藏文件也会列出）
- `-l` 除文件名称外，同时列出文件类型（`d` 目录 / `-` 文件）、权限、拥有者、文件大小等详细信息

由于 `ls -l` 使用频繁，Linux 提供了简写方式 `ll`。

**常见用法：**

|   |   |
|---|---|
|命令|作用|
| `ls -al` |查看当前目录所有文件及目录的详细信息|
| `ls -al /etc` |查看 `/etc` 目录下所有文件及目录的详细信息|
| `ll` |查看当前目录文件及目录的详细信息|
| `ls -lh` |以人类可读单位（K/M/G）显示文件大小|
| `ls -lt` |按修改时间排序（最新在前）|
| `ls -lS` |按文件大小排序|

---

### 2. cd — 切换目录

```javascript
cd [dir]
```

|   |   |
|---|---|
|命令|作用|
| `cd /etc` |切换到绝对路径 `/etc` |
| `cd ..` |返回上一级目录|
| `cd ~` 或 `cd` |回到当前用户的家目录|
| `cd -` |回到上一次所在的目录|

---

### 3. pwd — 显示当前路径

```bash
pwd          # 打印当前所在目录的绝对路径
```

### 4. mkdir — 创建目录

```javascript
mkdir [-p] [dir]
```

- `-p` 递归创建多级目录（父目录不存在时自动创建）

```bash
mkdir test                    # 创建单级目录
mkdir -p /data/app/logs       # 递归创建多级目录
```

### 5. rmdir — 删除空目录

```bash
rmdir test        # 只能删除空目录，非空会报错
```

---

## 二、文件操作类

### 1. rm — 删除文件或目录

```javascript
rm [-rf] [file/dir]
```

- `-r` 递归删除目录及其内容
- `-f` 强制删除，不提示确认

| 命令               | 作用                    |
| ---------------- | --------------------- |
| `rm file.txt`    | 删除文件（会询问确认）           |
| `rm -f file.txt` | 强制删除文件，不提示            |
| `rm -r dir`      | 递归删除目录                |
| `rm -rf dir`     | **强制递归删除**（危险操作，谨慎使用） |

> ⚠️ `rm -rf /` 会清空系统，切勿尝试。

### 2. cp — 复制

```javascript
cp [-r] source dest
```

- `-r` 递归复制目录
- `-p` 保留原文件属性（权限、时间戳）
- `-i` 覆盖前提示确认

```bash
cp a.txt /tmp/            # 复制文件
cp -r dir1 dir2           # 复制整个目录
cp -rp dir1 dir2          # 保留属性递归复制
```

### 3. mv — 移动 / 重命名

```bash
mv a.txt /tmp/            # 移动文件
mv a.txt b.txt            # 重命名文件
mv dir1 /data/            # 移动目录
```

### 4. touch — 创建空文件

```bash
touch a.txt               # 创建空文件或更新文件时间戳
```

### 5. find — 查找文件

```bash
find / -name "*.log"              # 按名称查找
find /data -type d -name "logs"   # 只查找目录
find / -size +100M                # 查找大于 100M 的文件
find / -mtime -7                  # 查找 7 天内修改过的文件
```

---

## 三、文本查看类

### 1. cat — 查看 / 拼接文件内容

```javascript
cat [-n] [file]
```

- `-n` 显示行号

```bash
cat a.txt                 # 一次性输出全部内容
cat a.txt b.txt > c.txt   # 合并文件
cat -n a.txt              # 带行号显示
```

> 适合查看**小文件**；查看大文件请用 `more` / `less`。

### 2. more — 分页查看（只能向下）

```bash
more a.txt        # 空格翻页，回车下一行，q 退出
```

### 3. less — 分页查看（可上下翻）

```bash
less a.txt        # 上下键/翻页滚动，/关键字 搜索，q 退出
```

> 推荐 `less`，支持前后翻页与搜索，且不会一次性加载整个文件。

### 4. head — 查看文件头部

```javascript
head [-n] [file]
```

```bash
head a.txt            # 默认显示前 10 行
head -n 20 a.txt      # 显示前 20 行
head -20 a.txt        # 同上简写
```

### 5. tail — 查看文件尾部（重点）

```javascript
tail [-nf] [file]
```

- `-n` 指定行数
- `-f`**（常考常用）实时追踪文件新增内容**，常用于查看日志

```bash
tail a.txt                # 默认显示后 10 行
tail -n 50 a.txt          # 显示后 50 行
tail -f /var/log/syslog   # 实时滚动输出新增日志（Ctrl+C 退出）
tail -100f app.log        # 显示最后 100 行并持续追踪
```

> 🔥 `tail -f` 是排查线上问题的利器，配合 `grep` 使用更佳：`tail -f app.log | grep ERROR`

### 6. grep — 文本搜索

```bash
grep "error" app.log              # 搜索包含 error 的行
grep -i "error" app.log           # 忽略大小写
grep -n "error" app.log           # 显示行号
grep -v "error" app.log           # 反向匹配（排除）
grep -r "keyword" /data/          # 递归搜索目录
```

---

## 四、权限管理类

### 1. chmod — 修改文件权限

```javascript
chmod [-R] 权限 file
```

权限可用**数字法**（r=4, w=2, x=1）或**符号法**（u/g/o/a + - =）。

```bash
chmod 755 script.sh       # rwxr-xr-x
chmod 644 a.txt           # rw-r--r--
chmod +x script.sh        # 添加可执行权限
chmod -R 755 /data/app    # 递归修改目录权限
chmod u+w,g+r a.txt       # 符号法修改
```

### 2. chown — 修改属主 / 属组

```bash
chown user a.txt                  # 修改属主
chown user:group a.txt            # 同时修改属主和属组
chown -R user:group /data/app     # 递归修改
```

### 3. 权限含义速查

|   |   |   |
|---|---|---|
|数字|权限|含义|
|7|rwx|读+写+执行|
|6|rw-|读+写|
|5|r-x|读+执行|
|4|r--|只读|

> 第一位代表类型：`d` 目录、`-` 普通文件、`l` 软链接。

---

## 五、软件包管理类

### 1. apt 系列（Debian / Ubuntu）

|   |   |
|---|---|
|命令|作用|
| `apt update` |更新软件源索引|
| `apt upgrade` |升级已安装的软件包|
| `apt install nginx` |安装软件包|
| `apt remove nginx` |卸载软件包（保留配置）|
| `apt purge nginx` |彻底卸载（含配置）|
| `apt search keyword` |搜索软件包|
| `apt list --installed` |列出已安装包|
| `apt autoremove` |清理无用依赖|

```bash
apt install -y nginx        # 自动确认安装
apt install ./pkg.deb       # 安装本地 deb 包
```

### 2. yum / dnf（CentOS / RHEL / Fedora）

```bash
yum install nginx           # 安装
yum remove nginx            # 卸载
yum update                  # 升级
yum search nginx            # 搜索
yum list installed          # 查看已安装
```

### 3. 其他常用

```bash
dpkg -i pkg.deb             # Debian 安装本地包
rpm -ivh pkg.rpm            # RHEL 安装本地包
```

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

### 3. 永久设置

|   |   |
|---|---|
|文件|生效范围|
| `/etc/profile` |所有用户（登录时加载）|
| `~/.bashrc` |当前用户（每次打开终端加载）|
| `~/.bash_profile` |当前用户（登录时加载）|

```bash
echo 'export PATH=$PATH:/data/bin' >> ~/.bashrc
source ~/.bashrc        # 使修改立即生效
```

> `source`（或 `.`）用于重新加载配置文件，无需重新登录。

---

## 七、进程管理类

|   |   |
|---|---|
|命令|作用|
| `ps -ef` |查看所有进程详细信息|
| `ps aux` |查看所有进程（含 CPU/内存占用）|
| `top` |实时监控进程与系统资源（`q` 退出）|
| `htop` |更友好的交互式进程监控（需安装）|
| `kill PID` |终止指定进程|
| `kill -9 PID` |强制杀死进程|
| `killall nginx` |按名称杀死进程|
| `pkill -f keyword` |按关键字匹配杀死进程|
| `jobs` |查看后台任务|
| `nohup cmd &` |后台运行且不随终端关闭而退出|

```bash
ps -ef | grep java          # 查找 java 进程
top -p 1234                 # 只监控 PID 1234
```

---

## 八、端口 / 网络类

### 1. 端口查看

```bash
netstat -tunlp              # 查看所有监听端口及对应进程
netstat -tunlp | grep 8080  # 查看指定端口占用
ss -tunlp                   # netstat 的现代替代，速度更快
lsof -i:8080                # 查看 8080 端口被哪个进程占用
```

参数说明：`-t` TCP、`-u` UDP、`-n` 数字显示、`-l` 监听状态、`-p` 显示进程。

### 2. 网络连通性

```bash
ping www.baidu.com              # 测试连通性
curl http://localhost:8080      # 请求 URL / 接口测试
wget http://xxx/file.tar.gz     # 下载文件
telnet 192.168.1.1 80           # 测试端口是否可通
traceroute www.baidu.com        # 路由追踪
ifconfig / ip addr              # 查看网卡与 IP 信息
```

---

## 九、防火墙类

### 1. firewalld（CentOS 7+ / RHEL）

```bash
systemctl start firewalld            # 启动
systemctl enable firewalld           # 开机自启
systemctl status firewalld           # 查看状态

firewall-cmd --state                 # 查看运行状态
firewall-cmd --list-all              # 查看所有规则

firewall-cmd --add-port=8080/tcp --permanent    # 开放端口（永久）
firewall-cmd --remove-port=8080/tcp --permanent # 关闭端口
firewall-cmd --reload                # 重载配置使其生效

firewall-cmd --add-service=http --permanent     # 开放服务
```

### 2. ufw（Ubuntu）

```bash
ufw enable                # 开启防火墙
ufw status                # 查看状态
ufw allow 22/tcp          # 开放端口
ufw deny 8080             # 禁止端口
ufw delete allow 8080     # 删除规则
```

### 3. iptables（通用底层）

```bash
iptables -L -n                       # 查看规则
iptables -A INPUT -p tcp --dport 80 -j ACCEPT   # 允许 80 端口
service iptables save                # 保存规则
```

---

## 十、压缩与编辑类（补充）

### 1. tar — 打包压缩

```javascript
tar [-czxvf] file.tar.gz dir
```

- `-c` 创建归档、`-x` 解压、`-z` gzip 压缩、`-v` 显示过程、`-f` 指定文件名

```bash
tar -czvf logs.tar.gz /data/logs     # 压缩
tar -xzvf logs.tar.gz                # 解压
tar -tzvf logs.tar.gz                # 查看内容不解压
```

### 2. vi / vim — 文本编辑器

```bash
vi a.txt        # 打开文件
vim a.txt       # 打开文件（增强版）
```

**三种模式：**

|   |   |   |
|---|---|---|
|模式|说明|切换|
|命令模式|默认进入，可移动光标、删除|按 `Esc` 进入|
|插入模式|可输入文字|按 `i` / `a` / `o` 进入|
|末行模式|保存、退出、查找替换|按 `:` 进入|

**常用操作：**

|   |   |
|---|---|
|操作|命令|
|保存退出| `:wq` 或 `:x` |
|不保存退出| `:q!` |
|保存| `:w` |
|查找|命令模式 `/keyword`，`n` 下一个|
|替换| `:%s/old/new/g` |
|删除整行|命令模式 `dd` |
|复制 / 粘贴行| `yy` / `p` |
|跳到文件末尾 / 开头| `G` / `gg` |
|撤销| `u` |

---

## 附：命令速查总表

|   |   |
|---|---|
|类别|核心命令|
|目录操作| `ls` `cd` `pwd` `mkdir` `rmdir` |
|文件操作| `rm` `cp` `mv` `touch` `find` |
|文本查看| `cat` `more` `less` `head` `tail -f` `grep` |
|权限管理| `chmod` `chown` |
|软件包| `apt install` `yum` `dpkg` `rpm` |
|环境变量| `export` `env` `source` `/etc/profile` |
|进程管理| `ps` `top` `kill` `nohup` |
|端口网络| `netstat` `ss` `lsof` `curl` `ping` |
|防火墙| `firewall-cmd` `ufw` `iptables` |
|压缩编辑| `tar` `vi` `vim` |

---

这份手册覆盖了你提到的全部命令与分类。如果你想让我把某一类（比如 `vim` 快捷键、`iptables` 规则、Shell 脚本基础）展开成更详细的单独文档，或者导出一个可打印的版本，告诉我就行。