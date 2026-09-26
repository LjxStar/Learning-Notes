# 一、Maven 简介

Maven 是 Apache 基金会推出的 Java 项目自动化构建工具，也是目前 Java 生态中事实上的标准构建工具。它把「**下载依赖**」和「**构建打包**」这两件最烦琐的事情统一到了一条命令里：

- **依赖管理**：只需在 `pom.xml` 中声明依赖坐标，Maven 就会自动从远程仓库下载对应的 jar 包到本地仓库，项目中无需再手动复制 jar 包。
- **构建与打包**：通过执行 `mvn clean`、`mvn compile`、`mvn package` 等标准命令，完成清理、编译、测试、打包、安装等整个流程。
- **约定优于配置**：目录结构和 POM 文件的写法有统一的约定，配置量少，团队协作也更规范。

因此，安装 Maven 并完成基本配置，是 Java 后端开发的第一步。

# 二、Maven 的安装与配置

从官网下载到的 Maven 是一个 zip 压缩文件，解压后即可使用，整体配置分三步：配置环境变量 → 指定本地仓库 → 指定远程仓库镜像。

> [!note] 本文路径约定
> - **Maven 安装目录**：`D:\apache-maven-3.9.9`（即 `MAVEN_HOME`）
> - **核心配置文件**：`D:\apache-maven-3.9.9\conf\settings.xml`
> - **本地仓库目录**：`D:\apache-maven-3.9.9\mvn_repo`


## 2.1 配置环境变量

Maven 环境变量的配置方式与 JDK 完全一致，都是两步：先声明一个「主目录」变量，再把它的 `bin` 目录追加到 `Path` 中。

1. 打开「此电脑 → 右键属性 → 高级系统设置 → 环境变量」，在**系统变量**中新建一个变量：
    - 变量名：`MAVEN_HOME`
    - 变量值：`D:\apache-maven-3.9.9`，即包含 `bin`、`conf`、`lib` 这几个文件夹的那一层目录
2. 选中**系统变量**中的 `Path`，点击「编辑」，在变量值**末尾**追加：`%MAVEN_HOME%\bin`
    - 注意要用英文分号 `;` 与原有内容隔开，且是追加而不是覆盖，否则会破坏已有的 `Path` 配置
3. 点击「确定」保存。为了让新环境变量生效，建议**重新打开**一个终端窗口。
4. 在终端中执行 `mvn -v`，出现下面所示的信息即表示配置成功：

```
Apache Maven 3.9.9 (...)
Maven home: D:\apache-maven-3.9.9
Java version: 17.0.x, vendor: ..., runtime: ...
OS name: "windows 11", version: ..., arch: "amd64", family: "windows"
```

## 2.2 配置本地仓库

Maven 从远程仓库下载下来的所有 jar 包与插件，都会**缓存到本地仓库**中，之后再使用同一个依赖时就无需联网下载，直接从本地读取，速度极快。

本地仓库的默认路径是用户主目录下的 `.m2\repository`。由于 C 盘空间有限且重装系统后缓存会全部丢失，建议在 Maven 安装目录下新建一个固定目录专门存放 jar 包，本文使用的是 `D:\apache-maven-3.9.9\mvn_repo`（如图所示）：

![[Pasted image 20260926172429.png]]

然后进入 `D:\apache-maven-3.9.9\conf` 目录，打开 `settings.xml` 配置文件，找到被注释掉的 `<localRepository>` 标签：

```xml
<!-- localRepository
 | The path to the local repository maven will use to store artifacts.
 |
 | Default: ${user.home}/.m2/repository
-->
<localRepository>D:\apache-maven-3.9.9\mvn_repo</localRepository>
```

具体操作分三步：

1. 把被 `<!--` 和 `-->` 包住的整段注释**删除**，让 `<localRepository>` 标签真正生效；
2. 将标签体内容替换为**上一步新建的存放 jar 包的目录路径** `D:\apache-maven-3.9.9\mvn_repo`；
3. 注意该标签必须直接写在根标签 `<settings>` 之下，且**只允许出现一次**。

修改完成后保存，配置结果如下图所示：

![[Pasted image 20260926172810.png]]

## 2.3 配置仓库镜像

Maven 官方提供的**中央仓库**（Central Repository）服务器位于国外，国内直连下载通常比较慢，甚至会超时。为了解决这个问题，阿里巴巴提供了公共的 Maven 仓库镜像，其中基本涵盖了主流开源项目的 jar 包，且国内访问速度很快。

配置方式与本地仓库类似：进入 `D:\apache-maven-3.9.9\conf` 目录打开 `settings.xml`，找到 `<mirrors>` 标签，在 `<mirrors>` 与 `</mirrors>` **之间**为其添加如下子标签：

```xml
<mirrors>
	<mirror>
		<id>aliyunmaven</id>
		<mirrorOf>*</mirrorOf>
		<name>aliyunmaven</name>
		<url>https://maven.aliyun.com/repository/public</url>
	</mirror>
</mirrors>
```

各子标签的含义如下：

| 标签          | 含义                                                                       |
| ----------- | ------------------------------------------------------------------------ |
| `<id>`       | 镜像的唯一标识，配置多个镜像时不能重复                                             |
| `<mirrorOf>` | 指定哪些仓库使用该镜像，`*` 表示**所有**仓库（中央仓库与第三方仓库）都走这个镜像                 |
| `<name>`     | 镜像的描述，仅用于阅读 `settings.xml`，方便识别                                             |
| `<url>`      | 镜像仓库的地址，`public` 仓库是中央仓库的聚合，包含了绝大部分常用依赖                        |

配置完成后保存，效果如下图所示：

![[Pasted image 20260926173141.png]]

