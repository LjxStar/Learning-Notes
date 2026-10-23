# 一、Maven 入门

## 1.1 Maven 介绍

Maven 是 Apache 基金会推出的 Java 项目自动化构建工具，也是目前 Java 生态中事实上的标准构建工具。它把「**下载依赖**」和「**构建打包**」这两件最烦琐的事情统一到了一条命令里：

- **依赖管理**：只需在 `pom.xml` 中声明依赖坐标，Maven 就会自动从远程仓库下载对应的 jar 包到本地仓库，项目中无需再手动复制 jar 包。
- **构建与打包**：通过执行 `mvn clean`、`mvn compile`、`mvn package` 等标准命令，完成清理、编译、测试、打包、安装等整个流程。
- **约定优于配置**：目录结构和 POM 文件的写法有统一的约定，配置量少，团队协作也更规范。

Maven 的核心思想可以概括成两句话：**用坐标找到构件，用生命周期组织构建**。围绕这两点，本篇按「入门准备 → 坐标与依赖管理 → 生命周期与构建」的顺序展开，其中入门部分把介绍、安装配置、目录结构与 `pom.xml` 一次讲清。

> [!tip] Maven 与 Gradle
> 二者都是 Java 主流的构建工具。Maven 以「约定优于配置」、完善的中央仓库生态和低上手成本见长；Gradle 以脚本化、灵活的 DSL 见长，构建速度通常更快。目前 Java 后端（尤其是 Spring Boot 项目）仍以 Maven 为主。

## 1.2 Maven 的安装与配置

入门的第一步是把 Maven 装好并配置妥当。Maven 本身是一个用 Java 编写的命令行工具，**运行它必须先装好 JDK**（Maven 会通过 `JAVA_HOME`，或 `Path` 中的 `java` 命令来定位 JDK）。从官网下载到的 Maven 是一个 zip 压缩文件，解压后即可使用，整体配置分三步：配置环境变量 → 指定本地仓库 → 指定远程仓库镜像；最后再说明全局与用户两份 `settings.xml` 的关系。

> [!note] 本文路径约定
> - **Maven 安装目录**：`D:\apache-maven-3.9.9`（即 `MAVEN_HOME`）
> - **核心配置文件**：`D:\apache-maven-3.9.9\conf\settings.xml`
> - **本地仓库目录**：`D:\apache-maven-3.9.9\mvn_repo`

### 1.2.1 配置环境变量

Maven 环境变量的配置方式与 JDK 完全一致，都是两步：先声明一个「主目录」变量，再把它的 `bin` 目录追加到 `Path` 中。

1. 打开「此电脑 → 右键属性 → 高级系统设置 → 环境变量」，在**系统变量**中新建一个变量：
   - 变量名：`MAVEN_HOME`
   - 变量值：`D:\apache-maven-3.9.9`，即包含 `bin`、`conf`、`lib` 这几个文件夹的那一层目录
2. 选中**系统变量**中的 `Path`，点击「编辑」，在变量值**末尾**追加：`%MAVEN_HOME%\bin`
   - 注意要用英文分号 `;` 与原有内容隔开，且是追加而不是覆盖，否则会破坏已有的 `Path` 配置
3. 点击「确定」保存。为了让新环境变量生效，建议**重新打开**一个终端窗口。
4. 在终端中执行 `mvn -v`，出现下面所示的信息即表示配置成功：

![[mvn -v 执行结果.png]]

其中 `Maven home` 指向自己的安装目录、`Java version` 显示当前使用的 JDK 版本，两项都正确就说明环境变量与 JDK 都已就绪。

### 1.2.2 指定本地仓库

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

### 1.2.3 配置仓库镜像

Maven 官方提供的**中央仓库**（Central Repository，地址为 `https://repo.maven.apache.org/maven2/`）服务器位于国外，国内直连下载通常比较慢，甚至会超时。为了解决这个问题，阿里巴巴提供了公共的 Maven 仓库镜像，其中基本涵盖了主流开源项目的 jar 包，且国内访问速度很快。

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

| 标签          | 含义                                                                        |
| ----------- | ------------------------------------------------------------------------- |
| `<id>`       | 镜像的唯一标识，配置多个镜像时不能重复                                              |
| `<mirrorOf>` | 指定哪些仓库使用该镜像，`*` 表示**所有**仓库（中央仓库与第三方仓库）都走这个镜像                 |
| `<name>`     | 镜像的描述，仅用于阅读 `settings.xml`，方便识别                                              |
| `<url>`      | 镜像仓库的地址，`public` 仓库是中央仓库的聚合，包含了绝大部分常用依赖                             |

配置完成后保存，效果如下图所示：

![[Maven配置镜像.png]]

至此 Maven 的三项基础配置就完成了。此后只要执行任意一条 Maven 命令，它都会按下面的规则工作：需要依赖时到远程仓库下载并缓存到 `D:\apache-maven-3.9.9\mvn_repo`，需要构建时按固定的阶段顺序完成编译、测试与打包。

### 1.2.4 全局配置与用户配置

`settings.xml` 有两个位置，Maven 会把两份配置**合并**使用，同名元素以**用户级**的为准：

| 配置文件              | 位置                                        | 作用范围              |
| ----------------- | ----------------------------------------- | ----------------- |
| 全局 `settings.xml` | `D:\apache-maven-3.9.9\conf\settings.xml` | 对这台电脑上所有的用户与项目都生效 |
| 用户 `settings.xml` | `%USERPROFILE%\.m2\settings.xml`          | 只对当前用户的所有项目生效     |

本篇示例统一写在全局的 `conf\settings.xml` 中；若这台电脑有多个用户，把只属于自己的配置改写到用户级 `settings.xml` 里更稳妥，不会影响别人。

装好并配置完成之后，就可以开始建项目了。Maven 对项目里该有哪些目录、文件放在哪里都有固定约定，先来看标准目录结构；目录中那个 `pom.xml` 再在下一节详细展开。

## 1.3 标准目录结构

Maven 遵循「约定优于配置」的设计：目录名和文件位置都有约定，只要按约定摆放，配置就能省到最少。下面是一个标准 Maven 项目的目录结构：

```
项目根目录
├── pom.xml                     项目对象模型
├── src
│   ├── main
│   │   ├── java                主程序代码
│   │   ├── resources           主程序资源文件（yml 配置、mapper 映射文件等）
│   │   └── webapp              传统 war 项目的静态资源（html、js、css、图片等）
│   └── test
│       ├── java                测试代码
│       └── resources           测试用资源文件
└── target                      构建产物目录，由 Maven 自动生成
    ├── classes                 编译后的主程序 class 文件
    ├── test-classes            编译后的测试 class 文件
    └── artifactId-版本号.jar     package 阶段生成的 jar 包
```

其中 `src` 目录是写代码的地方，而 `target` 目录纯粹是产物，可以随时删除，每次构建都会重新生成。

> [!TIPS]
> 当项目较大、需要按业务拆分时，还可以采用**多模块**结构：最外层是一个 `packaging` 为 `pom` 的父工程（`pom.xml` 中用 `<modules>` 列出子模块），父工程目录下再并列放若干个独立的 `src` 和 `pom.xml`，各模块既能单独构建，也能由父工程统一管理版本号和插件配置。

## 1.4 pom 文件详解与常见标签

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

## 1.5 用属性统一管理版本号

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

`pom.xml` 里最常写的内容就是依赖。下一章来看如何用 Maven 坐标声明依赖，以及传递依赖、版本冲突该怎么处理。

# 二、Maven 坐标与依赖管理

Maven 管理依赖的思路可以概括为一句话：**用坐标定位依赖，用 POM 声明依赖，剩下的下载与冲突处理交给 Maven 自己完成**。这一章按「坐标 → 声明 → 范围 → 冲突处理」的顺序展开。

## 2.1 Maven 坐标

任何一个构件（jar 包）在仓库中都有唯一的「身份证」，称为 **Maven 坐标**，格式是 `groupId:artifactId:version`，用冒号连接。例如 Spring 的 `spring-webmvc`：

```
org.springframework:spring-webmvc:6.1.4
```

三段的含义分别是：

- `groupId`：分组，通常对应组织或公司，同一家发布的所有构件归为一组
- `artifactId`：构件名，一个构件一个名字，在同一 `groupId` 内唯一
- `version`：构件版本，升级依赖改的就是这里

**坐标去哪里查？** 可以通过 [MvnRepository](https://mvnrepository.com/) 检索中央仓库中的构件，输入 `spring-webmvc` 就能看到所有版本，选一个即可。IDEA 中还有更快的办法：在 `pom.xml` 里敲 `groupId:`，IDEA 会自动拉取候选列表，用方向键选中就能补全，写 `version` 时同理。

## 2.2 引入依赖

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

## 2.3 依赖范围

`<scope>` 决定依赖在编译期、测试期、运行期分别是否可见，也决定它会不会被打进最终产物：

| `scope`    | 主程序编译 | 测试编译 | 运行时 | 打包进产物 | 典型场景                                     |
| ---------- | ----- | ---- | --- | ----- | ---------------------------------------- |
| `compile`  | ✅     | ✅    | ✅   | ✅     | 默认值，绝大多数依赖都是它                            |
| `provided` | ✅     | ✅    | ✅   | ❌     | 由运行环境提供，打包时不带。如 `servlet-api`            |
| `runtime`  | ❌     | ✅    | ✅   | ✅     | 编译用不到、运行时才需要。如 MySQL 驱动                  |
| `test`     | ❌     | ✅    | ❌   | ❌     | 只在测试中使用，不进产物。如 `junit-jupiter`、`mockito` |
| `import`   | —     | —    | —   | —     | 仅在 `<dependencyManagement>` 中使用，导入一份版本清单 |

最需要留意的是 `provided`：**它会参与编译，也会出现在运行时的类路径上，但不会被打进 jar 包**，最终由运行环境（容器）提供，这才是它存在的意义。例如传统 Web 工程里的写法：

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-tomcat</artifactId>
    <scope>provided</scope>
</dependency>
```

之所以声明为 `provided`，是因为打包后这个工程要丢进外部的 Tomcat 容器里运行，而容器本身已经带了一套 Tomcat，再打进一份必然引起类冲突。

> [!note] `runtime` 与 `provided` 别搞混
> 两者在「是否参与编译」上正好相反：`runtime` **编译时不可见、运行时才需要**（如 MySQL 驱动）；`provided` **编译时可见、但不进产物**（如 `servlet-api`）。判断口诀：*运行时才用到的写 `runtime`，由容器提供的写 `provided`*。

## 2.4 传递依赖与依赖冲突

Maven 具备**传递依赖**能力：项目直接依赖 A，A 又依赖 B，B 还依赖 C，那么 B 和 C 都会被自动放进你的 classpath，不需要你手写声明。

传递依赖省事，但也会带来麻烦：一是引入了自己根本不想要或版本过旧的 jar，二是同一个库通过多条路径被引入且版本不一致，形成冲突。冲突往往在编译期看不出来，等到运行时报 `NoSuchMethodError` 才暴露，排查成本很高。

真的撞上冲突时，Maven 的仲裁规则是「最短优先」：离当前项目声明越近的版本优先级越高；同一层深度上，则由先声明的那个说了算。

想看清楚依赖到底是怎么被引进来的，在项目根目录执行 `mvn dependency:tree` 即可输出完整依赖树，图例如下：

- `+-`：直接依赖
- `\-`：重复出现的依赖，已被省略（树中只保留最后一次出现的那次）
- `|`：该依赖下面还有下一层依赖
- `*`：该依赖重复出现，被省略

再加上 `-Dincludes=groupId:artifactId` 还能只看某一部分，例如 `mvn dependency:tree -Dincludes=org.springframework`。IDEA 中查看更直观：Maven 工具窗口切换到 `Dependencies` 标签页，会以树形展开所有依赖（含传递依赖），点某个节点还能在下方直接看到对应的 `pom.xml` 片段。

依赖树把冲突暴露出来之后，处理方式主要有两种：不需要的传递依赖直接排除，版本不统一的则交给父工程统一管理。

## 2.5 排除依赖

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

排除之后如果还需要同类功能，记得自己再显式引入替代依赖，例如上面注释里提到的，用 `spring-boot-starter-undertow` 替换 Tomcat。

## 2.6 用 dependencyManagement 统一版本

实际开发中，版本冲突多数由 Spring Boot 这类框架的**依赖管理**机制解决：父工程的 `<dependencyManagement>` 把常用库的版本统一钉死，子模块声明依赖时不再写 `<version>`。

```xml
<!-- 父工程：只声明版本，不真正引入依赖 -->
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
            <version>2.16.1</version>
        </dependency>
    </dependencies>
</dependencyManagement>

<!-- 子模块：省略 version，版本由父工程决定 -->
<dependencies>
    <dependency>
        <groupId>com.fasterxml.jackson.core</groupId>
        <artifactId>jackson-databind</artifactId>
    </dependency>
</dependencies>
```

要覆盖某个版本，只需在子模块自己的 `<dependencyManagement>` 中重写该依赖的版本号，而不是在 `<dependencies>` 里重复声明。

> [!note] dependencyManagement 只管版本，不引入依赖
> 它里面的条目**不会把任何依赖加进项目**。想用某个库，仍然必须在 `<dependencies>` 中声明（此时可省略 `<version>`），这是初学者最容易搞混的一点。

Spring Boot 的 `spring-boot-dependencies` 这种「版本清单」，则是通过 `<scope>import</scope>` 导入到 `<dependencyManagement>` 中的，这正是依赖范围表格里 `import` 的用途。

# 三、生命周期与项目构建

## 3.1 生命周期

Maven 把项目构建的过程划分为若干个**阶段（phase）**，并预先固定了这些阶段的执行顺序，这个固定的顺序串就叫**生命周期**。生命周期一共三套：

1. **clean 生命周期**：清理构建产物
   - `pre-clean` → `clean` → `post-clean`
2. **default 生命周期**（也叫构建生命周期，负责编译打包）
   - `validate` → `compile` → `test` → `package` → `verify` → `install` → `deploy`
3. **site 生命周期**：生成项目站点文档，日常开发用得很少
   - `pre-site` → `site` → `post-site` → `site-deploy`

> [!note] 上面的阶段是常用的简化版
> `default` 生命周期实际包含几十个阶段（如 `process-resources`、`test-compile`、`prepare-package`、`integration-test` 等），它们被夹在相邻的两个常用阶段之间，平时不需要记住；Maven 会自动把它们依次执行。

日常只会用到 clean 和 default 两套，各个阶段的含义如下：

| 阶段         | 作用                                                             |
| ---------- | -------------------------------------------------------------- |
| `clean`    | 删除 `target` 目录                                                 |
| `validate` | 校验项目，检查 `pom.xml` 是否正确、格式是否合法                                  |
| `compile`  | 编译 `src/main/java` 下的代码，产物放到 `target/classes`                  |
| `test`     | 运行 `src/test/java` 下的单元测试                                      |
| `package`  | 打包，`jar` 项目生成 `target/demo-1.0-SNAPSHOT.jar`                   |
| `verify`   | 检查打包结果是否符合要求，用于打包后再做一次校验                                       |
| `install`  | 将构件安装到本地仓库，这样其他项目才能通过坐标引用到它                                    |
| `deploy`   | 把构件发布到远程仓库（私服或中央仓库），需要在 `pom.xml` 中配置 `distributionManagement` |

**最需要记住的一条规则**：执行某个阶段时，Maven 会自动按顺序把它之前的所有阶段一并执行，而且这个顺序是固定的，不能调整。例如：

- `mvn compile` 实际等于 `validate` + `compile`
- `mvn package` 实际等于 `validate` + `compile` + `test` + `package`
- `mvn install` 实际等于 `mvn package` + `install`

也正因为 `package` 之前会先跑 `test`，所以每次打包都会自动执行一遍测试；如果觉得每次都跑测试太慢，就要用参数跳过去。

> [!tip] 阶段 ≠ 动作
> 阶段只是构建流程上的一个节点，真正干活的是**绑定**在该阶段上的插件目标（goal）。例如 `compile` 阶段绑定了 `maven-compiler-plugin` 的 `compile` 目标，`package` 阶段绑定了 `maven-jar-plugin` 的 `jar` 目标。所以阶段不能被跳过，但可以通过改写插件配置来微调它的行为。

## 3.2 项目构建

### 3.2.1 常见构建命令

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

> [!note] 两个容易混淆的用法
> - **`-DskipTests` 与 `-Dmaven.test.skip=true`**：前者只跳过**执行**测试（测试代码照常编译），后者连编译都跳过，跳得更快但也就测不到编译错误了。
> - **`-P`（profile）**：用于激活 `pom.xml` 中 `<profiles>` 里定义的环境配置，例如 `mvn clean package -P prod` 会使用 id 为 `prod` 的那套配置（不同的仓库地址、激活的依赖等）。

### 3.2.2 在终端中执行

在终端里执行 `mvn` 命令，需要注意三件事。

**方式 1**：**必须先进入含 `pom.xml` 的项目根目录**，否则 Maven 会报找不到 `pom.xml`。

```bash
cd D:\project
mvn clean package
```

> [!note] CMD 中切盘符要加 `/d`
> 在 **CMD** 里 `cd D:\project` 只改变当前目录、不切换盘符，命令其实还停留在原盘符上，于是报找不到文件，正确写法是 `cd /d D:\project`；PowerShell 没有这个问题，直接 `cd D:\project` 即可。

**方式 2**：**也可以完全不切目录，用 `-f` 参数直接指定 `pom.xml` 的路径**，在同时维护多个项目时很方便：

```bash
mvn -f D:\develop\project\demo\pom.xml clean package
```

**方式 3**：**用 Maven Wrapper 可以免装 Maven**。IDEA 新建 Maven 项目时可以勾选「使用 Maven Wrapper」，会生成 `mvnw.cmd`（Windows）、`mvnw`（macOS / Linux）两个脚本和一个 `.mvn` 目录，它们会自动下载与项目匹配的 Maven 版本；在已有项目中也可以执行 `mvn wrapper:wrapper` 手动生成。在 Windows 上要执行的是带 `.cmd` 后缀的那个：

```bash
mvnw.cmd clean package
```

### 3.2.3 在IDEA中执行

IDEA 把 Maven 深度集成进了界面，在里面执行生命周期指令有两种主要方式。

**方式 1**：Maven 工具窗口双击阶段

`View → Tool Windows → Maven` 打开 Maven 工具窗口，展开项目节点，**Lifecycle** 下挂着 `clean`、`validate`、`compile`、`test`、`package`、`verify`、`install`、`deploy` 等阶段，**双击**任意一个即可执行。构建日志输出在窗口下方，`BUILD SUCCESS` 表示成功，其中的 `ERROR` 是可点击链接，能直接跳到出错的代码行。

上手最快，但有两个短板：一次只能跑一个阶段；**不会自动先执行 `clean`**——若 `target` 里残留了已删除的 class，打出来的 jar 仍会带着它们，往往到运行时才报错。

**方式 2**：自定义 Maven 运行配置

把整条指令当成一条命令执行，可以写多个阶段、还能带参数，日常开发主要用这个。

1. 在顶部运行配置下拉框中选择 **Edit Configurations...**，点 `+` 新建配置，选择 **Maven** 类型；
2. `Working directory` 填项目根目录，即 `pom.xml` 所在位置，直接填 `$MODULE_WORKING_DIR$` 即可；
3. `Command line` 填要执行的阶段与参数——**注意不要写 `mvn` 前缀**，多个用空格隔开，如 `clean package`、`clean install -DskipTests`、`clean package -P prod`；
4. 保存后即可在右上角一键运行，日志输出在底部的 **Run** 面板。