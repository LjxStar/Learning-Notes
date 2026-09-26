# 一、Maven 简介

Maven 是 Apache 基金会推出的 Java 项目自动化构建工具，也是目前 Java 生态中事实上的标准构建工具。它把「**下载依赖**」和「**构建打包**」这两件最烦琐的事情统一到了一条命令里：

- **依赖管理**：只需在 `pom.xml` 中声明依赖坐标，Maven 就会自动从远程仓库下载对应的 jar 包到本地仓库，项目中无需再手动复制 jar 包。
- **构建与打包**：通过执行 `mvn clean`、`mvn compile`、`mvn package` 等标准命令，完成清理、编译、测试、打包、安装等整个流程。
- **约定优于配置**：目录结构和 POM 文件的写法有统一的约定，配置量少，团队协作也更规范。

因此，安装 Maven 并完成基本配置，是 Java 后端开发的第一步。

# 二、Maven 的安装与配置

Maven 本身是免安装的绿色软件，解压后即可使用，整体配置分三步：配置环境变量 → 指定本地仓库 → 指定远程仓库镜像。

> [!tip] 配置文件的两个位置
> Maven 的配置主要写在 `settings.xml` 中，它有两个可能的位置：
>
> 1. **安装目录**下的 `conf\settings.xml`：随 Maven 一起分发，升级或换电脑时需要重新配置。
> 2. **用户目录**下的 `~/.m2\settings.xml`（Windows 中即 `C:\Users\用户名\.m2\settings.xml`）：优先级更高，常用于存放个人或公司的私有配置。
>
> 本文统一修改安装目录下的 `conf\settings.xml`。

## 2.1 配置环境变量

Maven 环境变量的配置方式与 JDK 完全一致，都是两步：先声明一个「主目录」变量，再把它的 `bin` 目录追加到 `Path` 中。

1. 打开「此电脑 → 右键属性 → 高级系统设置 → 环境变量」，在**系统变量**中新建一个变量：
    - 变量名：`MAVEN_HOME`
    - 变量值：Maven 的解压安装目录，即包含 `bin`、`conf`、`lib` 这几个文件夹的那一层目录
2. 选中**系统变量**中的 `Path`，点击「编辑」，在变量值**末尾**追加：`%MAVEN_HOME%\bin`
    - 注意要用英文分号 `;` 与原有内容隔开，且是追加而不是覆盖，否则会破坏已有的 `Path` 配置
3. 点击「确定」保存。为了让新环境变量生效，建议**重新打开**一个终端窗口。
4. 在终端中执行 `mvn -v`，出现如图所示的信息即表示配置成功：

```
Apache Maven 3.9.x (xxx)
Maven home: D:\develop\apache-maven-3.9.x
Java version: 17.0.x, vendor: ..., runtime: ...
OS name: "windows 11", version: "11", arch: "amd64", family: "windows"
```

![[Pasted image 20260926173400.png]]

> [!warning] 报错排查
> 如果提示 `'mvn' 不是内部或外部命令`，依次检查：变量名是否拼写正确、`Path` 末尾是否漏加分号、是否在**新开的**终端中执行。若 `mvn -v` 显示的 Java 版本不是期望值，说明它读到的是 `JAVA_HOME` 指向的 JDK，而非本机其他版本。

## 2.2 配置本地仓库

Maven 从远程仓库下载下来的所有 jar 包与插件，都会**缓存到本地仓库**中，之后再使用同一个依赖时就无需联网下载，直接从本地读取，速度极快。

本地仓库的默认路径是用户主目录下的 `.m2\repository`。由于 C 盘空间有限且重装系统后缓存会全部丢失，建议先在磁盘中新建一个固定目录专门存放（如图所示），路径尽量避免使用中文和空格：

![[Pasted image 20260926172429.png]]

然后进入 Maven 安装目录的 `conf` 目录，打开 `settings.xml` 配置文件，找到被注释掉的 `<localRepository>` 标签：

```xml
<!-- localRepository
 | The path to the local repository maven will use to store artifacts.
 |
 | Default: ${user.home}/.m2/repository
-->
<localRepository>D:\develop\maven-repository</localRepository>
```

具体操作分三步：

1. 把被 `<!--` 和 `-->` 包住的整段注释**删除**，让 `<localRepository>` 标签真正生效；
2. 将标签体内容替换为**上一步新建的存放 jar 包的目录路径**；
3. 注意该标签必须直接写在根标签 `<settings>` 之下，且**只允许出现一次**。

修改完成后保存，配置结果如下图所示：

![[Pasted image 20260926172810.png]]

> [!note] 提前建好目录
> `localRepository` 指向的目录不需要手动创建，Maven 首次构建时会自动生成。但目录所在磁盘要有足够空间，一个中等规模项目的依赖缓存通常在几百 MB 到数 GB。
>
> 修改本地仓库位置后，之前缓存在默认目录中的 jar 包不会被自动搬移，如仍需使用旧缓存，可手动将 `.m2\repository` 下的内容复制到新目录。

## 2.3 配置仓库镜像

Maven 官方提供的**中央仓库**（Central Repository）服务器位于国外，国内直连下载通常比较慢，甚至会超时。为了解决这个问题，阿里巴巴提供了公共的 Maven 仓库镜像，其中基本涵盖了主流开源项目的 jar 包，且国内访问速度很快。

配置方式与本地仓库类似：进入 `conf` 目录打开 `settings.xml`，找到 `<mirrors>` 标签，在 `<mirrors>` 与 `</mirrors>` **之间**为其添加如下子标签：

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

> [!warning] 镜像不是越多越好
> 如果 `central` 已被 `*` 覆盖，就不要再为它单独配置 `<mirror>`，否则 Maven 会因镜像重复而报错。确有多个镜像需求时，应将 `<mirrorOf>` 改写得更精确，例如 `<mirrorOf>*,!companyRepo</mirrorOf>`，表示除 `companyRepo` 外的所有仓库都走该镜像。
>
> 修改镜像地址后建议删除本地仓库中 `com` / `org` 等目录下已缓存的失败文件（`_remote.repositories` 与 `*.lastUpdated` 文件），否则 Maven 可能仍会使用缓存中的失败记录。

## 2.4 验证配置是否生效

完成上述三步后，可以做一次简单验证：

1. 执行 `mvn -v`，确认 Maven 能被识别，且输出的 `Maven home` 指向正确的安装目录；
2. 新建一个测试项目（含 `pom.xml`），执行一次 `mvn compile` 或 `mvn package`：
    - 首次构建时下载依赖会持续数分钟，属于正常现象，构建结束后 jar 包会出现在所配置的本地仓库目录中；
    - 执行 `mvn package` 成功并在 `target` 目录下生成 jar 包，说明环境、依赖下载、打包流程全部正常。

> [!tip] 常见问题
> - **依赖下载仍然很慢**：确认 `settings.xml` 中的镜像配置写在 `<mirrors>` 标签内部，并检查该文件是否被误保存为 `settings.xml.txt`（Windows 记事本默认会加扩展名）。
> - **修改配置不生效**：确认改的是 Maven 实际使用的那个 `settings.xml`（可用 `mvn help:evaluate -Dexpression=settings.localRepository` 查看当前生效的本地仓库路径），并在**新开的**终端中重新执行命令。
> - **在 IDEA 中仍然下载缓慢**：IDEA 默认使用自带的 Maven，需要在 `Settings → Build Tools → Maven` 中把 `User settings file` 指向同一个 `settings.xml`，改动才会同步生效。
