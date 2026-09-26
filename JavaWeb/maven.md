# 1.配置本地仓库

![[Pasted image 20260926172429.png]]

进入到 conf 目录下打开 `settings.xml` 配置文件，定位到 53 行左右，复制 `<localRepository>` 标签，粘贴到注释的外面。复制之前新建的用来存储 jar 包的路径，替换掉 `<localRepository>` 标签体内容

![[Pasted image 20260926172810.png]]

# 2.配置仓库镜像

由于中央仓库在国外，所以下载 jar 包速度可能比较慢，而阿里公司提供了一个远程仓库，里面基本也都有开源项目的 jar 包。
进入到 conf 目录下打开 settings.xml ，定位到 160 行左右，在 `<mirrors>` 标签下为其添加子标签 `<mirror>`，内容如下：

```xml
<mirror>
	<id>aliyunmaven</id>
	<mirrorOf>*</mirrorOf>
	<name>aliyunmaven</name>
	<url>https://maven.aliyun.com/repository/public</url>
</mirror>
```

注意配置的位置，在 `<mirrors> ... </mirrors>` 中间添加配置。如下图所示：

![[Pasted image 20260926173141.png]]

# 3.配置环境变量

Maven 环境变量的配置类似于 JDK 环境变量配置一样

1. 在系统变量处新建一个变量 MAVEN_HOME。 MAVEN_HOME 环境变量的值，设置为 maven 的解压安装目录
2. 在Path中进行配置。 PATH环境变量的值，设置为：%MAVEN_HOME%\bin
3. 打开终端输入 `mvn -v`，出现如图所示表示安装成功。

![[Pasted image 20260926173400.png]]