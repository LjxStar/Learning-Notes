# 一、Maven 简介

Maven 是 Apache 基金会推出的 Java 项目自动化构建工具，也是目前 Java 生态中事实上的标准构建工具。它把「**下载依赖**」和「**构建打包**」这两件最烦琐的事情统一到了一条命令里：

- **依赖管理**：只需在 `pom.xml` 中声明依赖坐标，Maven 就会自动从远程仓库下载对应的 jar 包到本地仓库，项目中无需再手动复制 jar 包。
- **构建与打包**：通过执行 `mvn clean`、`mvn compile`、`mvn package` 等标准命令，完成清理、编译、测试、打包、安装等整个流程。
- **约定优于配置**：目录结构和 POM 文件的写法有统一的约定，配置量少，团队协作也更规范。

因此，安装 Maven 并完成基本配置，是 Java 后端开发的第一步。

本文脉络：

1. **Maven 简介** —— 它解决什么问题
2. **Maven 的安装与配置** —— 环境变量、本地仓库、仓库镜像
3. **标准目录结构与 pom 文件** —— 项目里都有哪些目录，`pom.xml` 每个标签的含义
4. **Maven 坐标与依赖管理** —— 怎么找依赖、怎么引依赖、怎么排依赖、怎么看依赖
5. **生命周期与构建命令** —— 打包的各个阶段，以及在终端与 IDEA 中分别怎么执行

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

![[Maven本地仓库推荐地址.png]]

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

![[Maven配置本地仓库.png]]

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

![[Maven配置镜像.png]]

至此 Maven 的三项基础配置就完成了。此后只要执行任意一条 Maven 命令，它都会按下面的规则工作：需要依赖时到远程仓库下载并缓存到 `D:\apache-maven-3.9.9\mvn_repo`，需要构建时按固定的阶段顺序完成编译、测试与打包。

# 三、标准目录结构与 pom 文件

## 3.1 标准目录结构

Maven 遵循「约定优于配置」的设计：目录名和文件位置都有约定，只要按约定摆放，配置就能省到最少。下面是一个标准 Maven 项目的目录结构：

```
项目根目录
├── pom.xml                    项目对象模型
├── src
│   ├── main
│   │   ├── java               主程序代码
│   │   ├── resources          主程序资源文件（yml 配置、mapper、静态资源等）
│   └── test
│       ├── java               测试代码
│       └── resources          测试用资源文件
└── target                     构建产物目录，由 Maven 自动生成
    ├── classes                编译后的主程序 class 文件
    ├── test-classes           编译后的测试 class 文件
    └── 项目名-版本号.jar        package 阶段生成的 jar 包
```

其中 `src` 目录是写代码的地方，而 `target` 目录纯粹是产物，可以随时删除，每次构建都会重新生成。

当项目较大、需要按业务拆分时，还可以采用**多模块**结构：最外层是一个 `packaging` 为 `pom` 的父工程（`pom.xml` 中用 `<modules>` 列出子模块），父工程目录下再并列放若干个独立的 `src` 和 `pom.xml`，各模块既能单独构建，也能由父工程统一管理版本号和插件配置。

## 3.2 pom 文件详解

POM 是 Project Object Model（项目对象模型）的缩写，`pom.xml` 就是这个模型的配置文件，它描述了项目「叫什么、是什么版本、需要什么、用什么方式打包」。这个文件默认位于项目根目录，位置固定不可更改。

一个最小可用的 `pom.xml` 如下：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <!-- 项目坐标：三者共同确定一个项目的唯一标识 -->
    <groupId>com.example</groupId>
    <artifactId>demo</artifactId>
    <version>1.0-SNAPSHOT</version>
    <packaging>jar</packaging>

    <!-- 项目描述信息，仅供人阅读，不影响构建 -->
    <name>demo</name>
    <description>我的第一个 Maven 项目</description>

    <!-- 属性：用 ${} 引用，避免版本号重复书写 -->
    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <maven.compiler.release>17</maven.compiler.release>
        <spring.version>6.1.4</spring.version>
    </properties>

    <!-- 声明本项目需要的依赖 -->
    <dependencies>
        <!-- 略 -->
    </dependencies>

    <!-- 打包与插件相关配置 -->
    <build>
        <!-- 略 -->
    </build>
</project>
```

各标签的含义如下：

| 标签                         | 作用                                              |
| -------------------------- | ----------------------------------------------- |
| `<modelVersion>`           | POM 的模型版本，Maven 3 中固定为 `4.0.0`，照抄即可无需修改         |
| `<groupId>`                | 项目组 id，通常写成公司或组织域名反写，如 `com.example`            |
| `<artifactId>`             | 模块 id，在同一个 `groupId` 下不能重复                      |
| `<version>`                | 项目版本号；`1.0-SNAPSHOT` 表示仍在开发中的快照版本，正式发布时改为 `1.0` |
| `<packaging>`              | 打包类型，决定构建产物的形式，不写默认是 `jar`                      |
| `<name>` / `<description>` | 项目名称与描述，只给人看，删掉也不影响构建                           |
| `<properties>`             | 属性定义区，相当于常量，通过 `${属性名}` 引用；最常用于统一管理版本号和字符编码     |
| `<dependencies>`           | 声明本项目需要引入的依赖                                    |
| `<dependencyManagement>`   | 统一管理依赖的版本（以及排除哪些传递依赖），自身不会把任何依赖引入项目中            |
| `<build>`                  | 构建配置，包括插件、最终产物名称、目录定制等                          |
| `<repositories>`           | 声明额外的远程仓库，依赖不在中央仓库或镜像中时才需要配置                    |
| `<parent>`                 | 声明父工程，子工程用它继承父工程中的依赖、插件与属性                      |
| `<modules>`                | 列出父工程包含的子模块目录，多模块项目才需要                          |

`<packaging>` 常见的取值有：

- `jar`：普通 Java 项目，也是默认值，产出 `.jar` 文件
- `war`：Web 项目，产出 `.war` 文件，交给 Tomcat 等容器运行
- `pom`：不产出实际构件，专门用于**父工程**（`packaging` 为 `pom` 且含 `<modules>`）或**依赖包聚合**，常用于管理多模块项目和统一版本号

## 3.3 properties：把重复的版本号收进一处

依赖多了以后，同一个第三方库的版本号会在 `<dependencies>` 中反复出现，一旦升级就要改很多处。把它抽到 `<properties>` 中就可以只改一处：

```xml
<properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <maven.compiler.release>17</maven.compiler.release>
    <spring.version>6.1.4</spring.version>
    <jackson.version>2.16.1</jackson.version>
</properties>

<dependencies>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-webmvc</artifactId>
        <version>${spring.version}</version>
    </dependency>
    <dependency>
        <groupId>com.fasterxml.jackson.core</groupId>
        <artifactId>jackson-databind</artifactId>
        <version>${jackson.version}</version>
    </dependency>
</dependencies>
```

Maven 还内置了一批属性可直接引用，例如 `${project.version}` 取当前项目版本、`${project.basedir}` 取项目根目录路径。

# 四、Maven 坐标与依赖管理

Maven 管理依赖的思路可以概括为一句话：**用坐标定位依赖，用 POM 声明依赖，剩下的下载与冲突处理交给 Maven 自己完成**。这一章就按这个顺序展开。

## 4.1 什么是 Maven 坐标

任何一个构件（jar 包）在仓库中都有唯一的「身份证」，称为 **Maven 坐标**，格式是 `groupId:artifactId:version`，用冒号连接。例如 Spring 的 `spring-webmvc`：

```
org.springframework:spring-webmvc:6.1.4
```

三段的含义分别是：

- `groupId`：分组，通常对应组织或公司，同一家发布的所有构件归为一组
- `artifactId`：构件名，一个构件一个名字，在同一 `groupId` 内唯一
- `version`：构件版本，升级依赖改的就是这里

**坐标去哪里查？** 可以通过 [MvnRepository](https://mvnrepository.com/) 检索中央仓库中的构件，输入 `spring-webmvc` 就能看到所有版本，选一个即可。IDEA 中还有更快的办法：在 `pom.xml` 里敲 `groupId:`，IDEA 会自动拉取候选列表，用方向键选中就能补全，写 `version` 时同理。

## 4.2 引入依赖

在 `<dependencies>` 中写一个 `<dependency>` 标签即可，最少只需要三个标签：

```xml
<dependencies>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-webmvc</artifactId>
        <version>6.1.4</version>
    </dependency>
</dependencies>
```

写完后 IDEA 会触发一次依赖下载，把对应的 jar 缓存到本地仓库（`D:\apache-maven-3.9.9\mvn_repo`），随后就能在 `External Libraries` 中看到它。

## 4.3 依赖范围

`<scope>` 决定依赖在编译期、测试期、运行期分别是否可见，也决定它会不会被打进最终产物：

| `scope`    | 主程序编译 | 测试编译 | 打包/运行 | 典型场景                                     |
| ---------- | ----- | ---- | ----- | ---------------------------------------- |
| `compile`  | ✅     | ✅    | ✅     | 默认值，绝大多数依赖都是它                            |
| `provided` | ✅     | ✅    | ❌     | 由运行环境提供，打包时不带。如 `servlet-api`            |
| `runtime`  | ❌     | ✅    | ✅     | 编译用不到、运行时才需要。如 MySQL 驱动                  |
| `test`     | ❌     | ✅    | ❌     | 只在测试中使用，不进产物。如 `junit-jupiter`、`mockito` |
| `import`   | —     | —    | —     | 仅在 `<dependencyManagement>` 中使用，导入一份版本清单 |

最需要留意的是 `provided`：**它的依赖会参与编译，却不会被打进 jar 包**，这正是它存在的意义。例如传统 Web 工程里的写法：

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-tomcat</artifactId>
    <scope>provided</scope>
</dependency>
```

之所以声明为 `provided`，是因为打包后这个工程要丢进外部的 Tomcat 容器里运行，而容器本身已经带了一套 Tomcat，再打进一份必然引起类冲突。

## 4.4 排除依赖

Maven 具备**传递依赖**能力：项目直接依赖 A，A 又依赖 B，B 还依赖 C，那么 B 和 C 都会被自动放进你的 classpath，不需要你手写声明。

传递依赖省事，但也带来两个麻烦：一是引入了自己根本不想要或版本过旧的 jar，二是不同 jar 传递进来的同一个库版本不一致，产生冲突。解决这些问题有两个手段：让 `dependencyManagement` 统一版本，或者用 `<exclusions>` 直接排除。

**排除语法**：在需要排除的 `<dependency>` 内部添加 `<exclusions>`，每个 `<exclusion>` 指定一个 `groupId` 和 `artifactId`：

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
    <version>3.2.0</version>
    <exclusions>
        <!-- 排除掉传递进来的 Tomcat，再自己引入 spring-boot-starter-undertow 替换 -->
        <exclusion>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-tomcat</artifactId>
        </exclusion>
    </exclusions>
</dependency>
```

## 4.5 依赖冲突与依赖树

当同一个 jar 通过多条路径被引入且版本不一致时，就会发生依赖冲突。Maven 的仲裁规则是「最短优先」：离当前项目声明越近的版本优先级越高；同一层深度上，则由先声明的那个说了算。

想看清楚依赖到底是怎么被引进来的，在项目根目录执行 `mvn dependency:tree` 即可输出完整依赖树（其中的 `+-` 表示直接依赖，`\-` 表示依赖树中最后一次出现的重复依赖），加上 `-Dincludes=坐标` 还能只看某一部分。IDEA 中查看更直观：Maven 工具窗口切换到 `Dependencies` 标签页，会以树形展开所有依赖（含传递依赖），点某个节点还能在下方直接看到对应的 `pom.xml` 片段。

> [!tip] 更推荐的解法：dependencyManagement
> 实际开发中，版本冲突多数由 Spring Boot 这类框架的**依赖管理**机制解决——父工程的 `<dependencyManagement>` 已经把常用库的版本统一钉死，子模块声明依赖时不再写 `<version>` 即可。要覆盖某个版本，只需要在子模块的 `<dependencyManagement>` 中重写该依赖的版本号，而不是在 `<dependencies>` 里重复声明。
>
> 需要注意：`<dependencyManagement>` 里的条目**只管版本，不会真正引入依赖**。想用某个库，仍然必须在 `<dependencies>` 中再写一遍（此时可以省略 `<version>`），这是初学者最容易搞混的一点。

# 五、生命周期与构建命令

## 5.1 三套生命周期

Maven 把项目构建的过程划分为若干个**阶段（phase）**，并预先固定了这些阶段的执行顺序，这个固定的顺序串就叫**生命周期**。生命周期一共三套：

1. **clean 生命周期**：清理构建产物
   - `pre-clean` → `clean` → `post-clean`
2. **default 生命周期**（也叫构建生命周期，负责编译打包）：编译、测试、打包、安装、发布
   - `validate` → `compile` → `test` → `package` → `verify` → `install` → `deploy`
3. **site 生命周期**：生成项目站点文档，日常开发用得很少
   - `site` → `deploy-site`

日常只会用到 clean 和 default 两套，各个阶段的含义如下：

| 阶段         | 作用                                                             |
| ---------- | -------------------------------------------------------------- |
| `clean`    | 删除 `target` 目录                                             |
| `validate` | 校验项目，检查 `pom.xml` 是否正确、格式是否合法                              |
| `compile`  | 编译 `src/main/java` 下的代码，产物放到 `target/classes`                  |
| `test`     | 运行 `src/test/java` 下的单元测试                                          |
| `package`  | 打包，`jar` 项目生成 `target/demo-1.0-SNAPSHOT.jar`                       |
| `verify`   | 检查打包结果是否符合要求，用于打包后再做一次校验                                 |
| `install`  | 将构件安装到本地仓库，这样其他项目才能通过坐标引用到它                          |
| `deploy`   | 把构件发布到远程仓库（私服或中央仓库），需要在 `pom.xml` 中配置 `distributionManagement` |

**最需要记住的一条规则**：执行某个阶段时，Maven 会自动按顺序把它之前的所有阶段一并执行，而且这个顺序是固定的，不能调整。例如：

- `mvn compile` 实际等于 `validate` + `compile`
- `mvn package` 实际等于 `validate` + `compile` + `test` + `package`
- `mvn install` 实际等于 `mvn package` + `install`

也正因为 `package` 之前会先跑 `test`，所以每次打包都会自动执行一遍测试；如果觉得每次都跑测试太慢，就要用参数跳过去（见 5.2）。

## 5.2 常见构建命令

| 指令                                         | 作用                  |
| ------------------------------------------ | ------------------- |
| `mvn clean`                                | 清理 `target` 目录      |
| `mvn compile`                              | 只编译主程序，不打包          |
| `mvn test`                                 | 执行单元测试              |
| `mvn package`                              | 编译测试后打包生成 jar/war   |
| `mvn install`                              | 打包并安装到本地仓库          |
| `mvn clean package`                        | 先清理，再打包             |
| `mvn clean package -DskipTests`            | 打包，编译测试代码但**不执行测试** |
| `mvn clean package -Dmaven.test.skip=true` | 打包，**不编译也不执行测试**    |
| `mvn clean install`                        | 清理、打包，安装到本地仓库       |

> [!tip] 用 `package` 还是 `install`？
> - 只是想生成一个 jar 包给同事传阅 → 用 `package` 即可
> - 这个 jar 还要被**其他 Maven 项目**当作依赖引用 → 必须用 `install`，把构件放进本地仓库，下一个项目才能通过坐标找到它
> - 这个 jar 要上传给整个团队使用 → 用 `deploy` 发布到公司私服（需先在 `pom.xml` 的 `<distributionManagement>` 中配置私服地址和账号）

## 5.3 在终端中执行

在终端里执行 `mvn` 命令，需要注意三件事。

第一，**必须先进入含 `pom.xml` 的项目根目录**，否则 Maven 会报找不到 `pom.xml`。

```bash
cd D:\project
mvn clean package
```

> [!note] CMD 中切盘符要加 `/d`
> 在 **CMD** 里 `cd D:\project` 只改变当前目录、不切换盘符，命令其实还停留在原盘符上，于是报找不到文件，正确写法是 `cd /d D:\project`；PowerShell 没有这个问题，直接 `cd D:\project` 即可。

第二，**也可以完全不切目录，用 `-f` 参数直接指定 `pom.xml` 的路径**，在同时维护多个项目时很方便：

```bash
mvn -f D:\develop\project\demo\pom.xml clean package
```

第三，**用 Maven Wrapper 可以免装 Maven**。IDEA 创建 Maven 项目时默认会生成 `mvnw.cmd`（Windows）、`mvnw`（macOS / Linux）两个脚本和一个 `.mvn` 目录，它们会自动下载与项目匹配的 Maven 版本。在 Windows 上要执行的是带 `.cmd` 后缀的那个：

```bash
mvnw.cmd clean package
```

## 5.4 在 IDEA 中执行

IDEA 把 Maven 深度集成进了界面，在里面执行生命周期指令有三种方式，各有适用场景。

### 方式 1：Maven 工具窗口双击阶段

这是最直接的方式，适合临时执行单个阶段：

1. 打开 Maven 工具窗口，菜单路径为 `View → Tool Windows → Maven`（快捷键 `Alt + F12` 可以显示/隐藏所有工具窗口）；
2. 展开项目节点，**Lifecycle** 下挂着 `clean`、`validate`、`compile`、`test`、`package`、`verify`、`install`、`deploy` 等阶段；
3. **双击**任意一个阶段即开始执行，构建日志输出在窗口下方的构建控制台中；日志里的 `BUILD SUCCESS` 表示成功，`BUILD FAILURE` 表示失败，输出中的 `ERROR` 行是带链接的，点击可直接跳转到出错的代码行。

这种方式上手最快，但有两个明显短板：

- **一次只能跑一个阶段**，想执行 `clean package` 就必须点两下、跑两轮；
- **不会自动先执行 `clean`**。如果上一次构建在 `target` 里留下了已被删除的 class 文件，重新打出来的 jar 仍会包含这些废弃类，往往要到运行时才报错。

### 方式 2：自定义 Maven 运行配置（推荐）

它把整条指令当作一条命令来执行，可以同时写多个阶段、还能带参数，是日常开发中使用频率最高的方式。

1. 在 IDEA 顶部运行配置下拉框中选择 **Edit Configurations...**，点左上角 `+` 新建配置，在列表中选择 **Maven**；
2. 按下表填写各字段（只填前两项就能跑）：

| 字段                  | 填什么                                                                                     |
| --------------------- | ----------------------------------------------------------------------------------------- |
| `Name`                | 运行配置的名称，随意取，如 `clean package`                                                |
| `Working directory`   | 项目根目录，即 `pom.xml` 所在目录，直接填 `$MODULE_WORKING_DIR$` 即可                          |
| `Command line`        | 要执行的阶段与参数，**不要写 `mvn` 前缀**，多个用空格隔开，如 `clean package`                    |
| `Active profiles`     | 需要激活的 profile，对应 `pom.xml` 中 `<profiles>` 声明的 `id`，不勾选则用默认配置                 |
| `Before launch`       | 可选的构建前置步骤，日常构建 Maven 项目时保持默认即可，不需要额外加 `Build`                      |

3. 保存后即可在右上角一键运行，日志输出在底部的 **Run** 面板。

**`Command line` 栏的写法要点**——这里写的是**去掉 `mvn` 之后的部分**：

| `Command line` 填写内容              | 实际执行的命令                             |
| ----------------------------------- | ------------------------------------- |
| `clean package`                     | `mvn clean package`（最常用）             |
| `clean install`                     | `mvn clean install`                     |
| `clean package -DskipTests`         | `mvn clean package -DskipTests`         |
| `clean package -Dmaven.test.skip=true` | `mvn clean package -Dmaven.test.skip=true` |
| `clean package -P prod`             | `mvn clean package -P prod`，激活 `prod` 这个 profile |
| `clean package -U`                  | `mvn clean package -U`，强制检查远程仓库更新   |
| `clean help:effective-pom`          | `mvn clean help:effective-pom`，导出最终生效的 `pom.xml` |
| `dependency:tree`                   | `mvn dependency:tree`，只出依赖树不构建         |

建议把日常用得最多的 `clean package` 和 `clean install -DskipTests` 各建一个运行配置，切换项目时可以直接复用。

### 方式 3：在 IDEA 内置终端中执行

`Alt + F12` 或 `View → Tool Windows → Terminal` 可以打开 IDEA 内置的终端，这里就是 5.3 讲的那套命令，区别只是省去了切换到外部终端的步骤，目录切换、Tab 补全、`mvnw.cmd` 等能力全都保留。

适合需要临时敲复杂参数、或者想在 IDEA 里一边看代码一边构建的场景。

### 三种方式的对比

| 执行方式            | 一次跑多个阶段 | 可带参数 | 自动 `clean` | 适用场景               |
| ------------------- | -------------- | -------- | ------------ | ---------------------- |
| 双击 Lifecycle 阶段 | ❌              | ❌        | ❌            | 临时跑单个 `compile`/`package` |
| Maven 运行配置      | ✅              | ✅        | ✅（自己写）   | **日常构建，推荐**      |
| 内置终端            | ✅              | ✅        | ✅（自己写）   | 复杂命令、配合 `mvnw` 使用 |

> [!tip] 多模块项目怎么执行
> 多模块项目在 Maven 工具窗口中会展开成一个父工程加若干子模块。此时**双击哪个节点，就只构建哪个模块**；想一次性构建全部模块，要选中**最上层的父工程节点**再执行 `clean install`，Maven 会按照 `pom.xml` 中 `<modules>` 声明的顺序依次构建各个子模块，某个子模块失败会立即中止。

**几个必须检查的 IDEA 设置**（`File → Settings → Build, Execution, Deployment → Build Tools → Maven`）：

| 配置项               | 建议值                                          |
| -------------------- | ----------------------------------------------- |
| `Maven home path`    | 选 `Use Maven home` 并指定 `D:\apache-maven-3.9.9`  |
| `User settings file` | 指向 `D:\apache-maven-3.9.9\conf\settings.xml`  |
| `Local repository`   | 指向 `D:\apache-maven-3.9.9\mvn_repo`           |
| `JDK for importer`   | 与项目所用 JDK 保持一致，避免导入时用错版本        |

> [!warning] IDEA 中执行 Maven 常见的坑
> - **Command line 里多写了 `mvn`**：会报找不到 goal 的错误，这里只填 `clean package` 这样的阶段与参数即可。
> - **Working directory 填错位置**：若不在 `pom.xml` 所在目录，会报 `The goal you specified requires a project to execute but there is no POM in this directory`。
> - **IDEA 用的是自带 Maven 而不是你配置的 3.9.9**：`Maven home path` 没改，于是 `settings.xml` 里的镜像和本地仓库全部被忽略——这既是「明明配了阿里云镜像却依然卡在下载」的原因，也意味着本文 2.2、2.3 两节的配置对 IDEA 并不生效。这是排查此类问题时的第一站。
> - **改完 `pom.xml` 后构建仍用旧依赖**：IDEA 会弹出是否 Reload All Maven Projects 的提示，需要点确认才会重新解析依赖；也可以点 Maven 工具窗口的刷新按钮强制重新加载。
> - **删除或改名了某个类之后仍报旧错误**：多半是 `target` 里残留了旧的 class 文件，执行一次 `clean package` 或 IDEA 的 `Build → Rebuild Project` 即可。
> - **IDEA 里下载依赖很慢、外部终端里却很快**：两者是各自独立的配置，外部终端读的是环境变量 `MAVEN_HOME` 指向的那份，需要回到 `User settings file` 确认路径。

