# 1.Ubuntu 虚拟机 MySQL 8.0 部署 + Windows 远程连接

1. **apt 安装 MySQL**
```bash
# 更新软件源
sudo apt update

# 安装最新版本 
# sudo apt install -y mysql-server 
# 安装指定版本 
sudo apt install -y mysql-server-8.0
```
如果不加 `-y` 会在安装过程中，系统将提示你设置 MySQL 的 root 密码。确保密码足够强，且记住它，因为你将在以后需要用到它。


2. **MySQL 服务启停 & 开机自启**
```bash
# 查看状态
sudo systemctl status mysql
# 启动
sudo systemctl start mysql
# 停止
sudo systemctl stop mysql
# 重启（修改配置后必须执行）
sudo systemctl restart mysql

# 设置开机自启
sudo systemctl enable mysql
# 取消开机自启
sudo systemctl disable mysql
```

3. **修改配置，允许外部访问**

配置文件路径：`/etc/mysql/mysql.conf.d/mysqld.cnf`
```bash
sudo vim /etc/mysql/mysql.conf.d/mysqld.cnf
```

找到：
```ini
bind-address = 127.0.0.1
```

修改：
```ini
bind-address = 0.0.0.0
```

保存退出后需要重启 mysql
```bash
sudo systemctl restart mysql
```

验证监听端口（可选）
```bash
sudo ss -tulpn | grep 3306
# 看到 0.0.0.0:3306 代表监听所有IP，可远程访问
```

4. **MySQL 修改密码、远程授权（MySQL8.0）**

登录虚拟机本地 mysql
```bash
sudo mysql
# sudo mysql -uroot -p 没密码直接回车就能登录成功。
```

修改密码
```mysql
-- 创建允许任意IP访问的root用户，设置密码
ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '1234';
```
这个 root 账号仅仅能够在本机localhost上访问，我们在 windows 上是无法访问的。如果需要在 window 上或其他服务器上也能远程访问，需要创建一个账号，用于远程访问的。

```mysql
CREATE USER 'root'@'%' IDENTIFIED BY '1234';

GRANT ALL PRIVILEGES ON *.* TO 'root'@'%';

FLUSH PRIVILEGES;
```
`%` = 允许任意 IP 远程访问；密码自行替换。

5. **Ubuntu 防火墙 ufw**
```bash
# 放行3306端口
sudo ufw allow 3306/tcp
# 重载防火墙
sudo ufw reload
# 查看防火墙状态
sudo ufw status
```
提示 `未启用防火墙` 代表 ufw 没有开启，端口默认放行，无需操作。

6. **Windows 宿主机远程连接**
```bash
mysql -h 192.168.100.128 -P 3306 -u root -p
```
- `-h`：目标 MySQL 主机 IP
- `-P`：大写 P，端口号
- `-u`：用户名
- `-p`：回车后输入密码（输入不回显）
