# 一、Maven 入门

## 1.1 Maven 介绍

Maven 是 Apache 基金会推出的 Java 项目自动化构建工具，也是目前 Java 生态中事实上的标准构建工具。其主要的功能如下:

- **依赖管理**：只需在 `pom.xml` 中声明依赖坐标，Maven 就会自动从远程仓库下载对应的 jar 包到本地仓库，项目中无需再手动复制 jar 包。
- **构建与打包**：通过执行 `mvn clean`、`mvn compile`、`mvn package` 等标准命令，完成清理、编译、测试、打包、安装等整个流程。
- **约定优于配置**：目录结构和 POM 文件的写法有统一的约定，配置量少，团队协作也更规范。

Maven 的核心思想可以概括成两句话：**用坐标找到构件，用生命周期组织构建**。

> [!tip] Maven 与 Gradle
> 二者都是 Java 主流的构建工具。Maven 以「约定优于配置」、完善的中央仓库生态和低上手成本见长；Gradle 以脚本化、灵活的 DSL 见长，构建速度通常更快。目前 Java 后端（尤其是 Spring Boot 项目）仍以 Maven 为主。

## 1.2 Maven 的安装与配置

入门的第一步是把 Maven 装好并配置妥当。Maven 本身是一个用 Java 编写的命令行工具，**运行它必须先装好 JDK**。从官网下载到的 Maven 是一个 zip 压缩文件，解压后即可使用。

> [!note] 本文路径约定
> - **Maven 安装目录**：`D:\apache-maven-3.9.9`（即 `MAVEN_HOME`）
> - **核心配置文件**：`D:\apache-maven-3.9.9\conf\settings.xml`
> - **本地仓库目录**：`D:\apache-maven-3.9.9\mvn_repo`

### 1.2.1 配置环境变量

Maven 环境变量的配置方式与 JDK 完全一致，先声明一个根目录变量，再把它的 `bin` 目录追加到 `Path` 中。

1. 打开「此电脑 → 右键属性 → 高级系统设置 → 环境变量」，在**系统变量**中新建一个变量：
   - 变量名：`MAVEN_HOME`
   - 变量值：`D:\apache-maven-3.9.9`
2. 选中**系统变量**中的 `Path`，点击「编辑」，在变量值**末尾**追加：`%MAVEN_HOME%\bin`，保存并退出。
3. 在终端中执行 `mvn -v`，出现下面所示的信息即表示配置成功：

![[mvn -v 执行结果.png]]

其中 `Maven home` 指向自己的安装目录、`Java version` 显示当前使用的 JDK 版本，两项都正确就说明环境配置成功。

### 1.2.2 指定本地仓库

Maven 从远程仓库下载下来的所有 jar 包与插件，都会**缓存到本地仓库**中，之后再使用同一个依赖时就无需联网下载，直接从本地读取。

本地仓库的默认路径是用户主目录下的 `.m2\repository`。由于 C 盘空间有限且重装系统后缓存会全部丢失，建议在 Maven 安装目录下新建一个固定目录专门存放 jar 包，本文使用的是 `D:\apache-maven-3.9.9\mvn_repo`（如图所示）：

![[Maven本地仓库推荐地址.png]]

然后进入 `D:\apache-maven-3.9.9\conf` 目录，打开 `settings.xml` 配置文件，找到 `<localRepository>` 标签，取消注释，将标签体内容替换为上一步新建的仓库目录路径即可：

```xml
<!-- localRepository
 | The path to the local repository maven will use to store artifacts.
 |
 | Default: ${user.home}/.m2/repository
-->
<localRepository>D:\apache-maven-3.9.9\mvn_repo</localRepository>
```

>  注意该标签必须直接写在根标签 `<settings>` 之下，且**只允许出现一次**。

修改完成后保存，配置结果如下图所示：

![[Maven配置本地仓库.png]]

### 1.2.3 配置仓库镜像

Maven 官方提供的 [中央仓库](https://repo.maven.apache.org/maven2/) 服务器位于国外，国内直连下载通常比较慢，甚至会超时。为了解决这个问题，阿里巴巴提供了公共的 Maven 仓库镜像，其中基本涵盖了主流开源项目的 jar 包，且国内访问速度很快。

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

| 标签           | 含义                                           |
| ------------ | -------------------------------------------- |
| `<id>`       | 镜像的唯一标识，配置多个镜像时不能重复                          |
| `<mirrorOf>` | 指定哪些仓库使用该镜像，`*` 表示**所有**仓库（中央仓库与第三方仓库）都走这个镜像 |
| `<name>`     | 镜像的描述，仅用于阅读 `settings.xml`，方便识别              |
| `<url>`      | 镜像仓库的地址，`public` 仓库是中央仓库的聚合，包含了绝大部分常用依赖      |

配置完成后保存，效果如下图所示：

![[Maven配置镜像.png]]

### 1.2.4 全局配置与用户配置

`settings.xml` 有两个位置，Maven 会把两份配置**合并**使用，同名元素以**用户级**的为准：

| 配置文件              | 位置                                        | 作用范围              |
| ----------------- | ----------------------------------------- | ----------------- |
| 全局 `settings.xml` | `D:\apache-maven-3.9.9\conf\settings.xml` | 对这台电脑上所有的用户与项目都生效 |
| 用户 `settings.xml` | `C:\Users\username>\.m2\settings.xml`     | 只对当前用户的所有项目生效     |

本篇示例统一写在全局的 `conf\settings.xml` 中；若这台电脑有多个用户，Maven 优先加载全局 `settings.xml`，随后加载用户 `settings.xml`；二者存在相同配置项时，用户配置覆盖全局配置。

## 1.3 标准目录结构

Maven 遵循「约定优于配置」的设计：目录名和文件位置都有约定，只要按约定摆放，配置就能省到最少。下面是一个标准 Maven 项目的目录结构：

```
项目根目录
├── pom.xml                     项目核心配置文件
├── src
│   ├── main
│   │   ├── java                主程序代码
│   │   └── resources           主程序资源文件（yml 配置、mapper 映射文件等）
│   │
│   └── test
│       ├── java                测试代码
│       └── resources           测试用资源文件
│
├── .mvn                        项目级Maven配置（可选）
└── target                      构建产物目录，由 Maven 自动生成
    ├── classes                 编译后的主程序 class 文件
    ├── test-classes            编译后的测试 class 文件
    └── artifactId-版本号.jar    package 阶段生成的 jar 包
```

其中 `src` 目录是写代码的地方，而 `target` 目录纯粹是产物，可以随时删除，每次构建都会重新生成。

> [!TIPS]
> 当项目较大、需要按业务拆分时，还可以采用**多模块**结构：最外层是一个 `packaging` 为 `pom` 的父工程，`pom.xml` 中用 `<modules>` 列出子模块，父工程目录下再并列放若干个独立的 `src` 和 `pom.xml`，各模块既能单独构建，也能由父工程统一管理版本号和插件配置。

## 1.4 POM 详解与常见标签

POM 是 Project Object Model（项目对象模型）的缩写，`pom.xml` 就是这个模型的配置文件。这个文件默认位于项目根目录，位置固定不可更改。

```xml
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0
         http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <!-- 固定4.0.0，POM模型版本，几乎不会变 -->
    <modelVersion>4.0.0</modelVersion>

    <!-- 项目坐标，一个项目的唯一标识 -->
    <groupId>com.example</groupId>  <!-- 组织/公司，反向域名 -->
    <artifactId>demo</artifactId>  <!-- 项目/模块名，组内唯一 -->
    <version>1.0-SNAPSHOT</version>  <!-- SNAPSHOT：快照版；RELEASE：正式版 -->
    
    <!-- 打包方式 -->
    <!-- jar：普通jar包；war：web工程；pom：父工程/聚合工程 -->
    <packaging>jar</packaging>

    <!-- 项目描述信息 -->
    <name>demo</name>
    <description>我的第一个 Maven 项目</description>

    <!-- 定义常量，后面用 `${key}` 引用，统一管理 -->
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
    <!-- 构建配置，包括插件、最终产物名称、目录定制等 -->
    <build>
        <!-- 略 -->
    </build>
</project>
```

# 二、Maven 坐标与依赖管理

Maven 管理依赖的思路可以概括为一句话：**用坐标定位依赖，用 POM 声明依赖，剩下的下载与冲突处理交给 Maven 自己完成**。

## 2.1 Maven 坐标

任何一个构件（jar、war、pom）在仓库中都有唯一的「身份证」，由三个核心部分组成：**groupId、artifactId、version**，称为 **Maven 坐标** (GAV)。例如 Spring 的 `spring-webmvc`：

```xml
<dependency>  
    <groupId>org.springframework</groupId>  
    <artifactId>spring-webmvc</artifactId>  
    <version>6.1.4</version>  
</dependency>
```

三段的含义分别是：

- `groupId`：分组，通常对应组织或公司，同一家发布的所有构件归为一组
- `artifactId`：构件名，一个构件一个名字，在同一 `groupId` 内唯一
- `version`：构件版本，升级依赖改的就是这里

> [!question] 坐标去哪里查？
>  可以通过 [MvnRepository](https://mvnrepository.com/) 检索中央仓库中的构件，输入 `spring-webmvc` 就能看到所有版本，选一个即可。也可以在 idea 内搜索添加依赖  [idea maven查找并添加依赖-CSDN博客](https://blog.csdn.net/m0_56919521/article/details/117070302)

## 2.2 引入依赖与依赖范围

在 `<dependencies>` 中写一个 `<dependency>` 标签即可，基本格式如下：

```xml
<dependencies>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-webmvc</artifactId>
        <version>6.1.4</version>
		<scope>compile</scope>
    </dependency>
</dependencies>
```

其中 `<scope>` 决定依赖在编译期、测试期、运行期分别是否可见，也决定它会不会被打进最终产物：

| `scope`    | 主程序编译 | 测试编译 | 运行时 | 打包进产物 | 典型场景                                     |
| ---------- | ----- | ---- | --- | ----- | ---------------------------------------- |
| `compile`  | ✅     | ✅    | ✅   | ✅     | 默认值，绝大多数依赖都是它                            |
| `provided` | ✅     | ✅    | ✅   | ❌     | 由运行环境提供，打包时不带。如 `servlet-api`            |
| `runtime`  | ❌     | ✅    | ✅   | ✅     | 编译用不到、运行时才需要。如 MySQL 驱动                  |
| `test`     | ❌     | ✅    | ❌   | ❌     | 只在测试中使用，不进产物。如 `junit-jupiter`、`mockito` |
| `import`   | —     | —    | —   | —     | 仅在 `<dependencyManagement>` 中使用，导入一份版本清单 |

`provided` **编译、测试阶段有效；打包时不会打入 jar/war，运行时容器提供，不会出现在最终包内**，**不会跟着包一起部署**。例如传统 Web 工程里的写法：

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-tomcat</artifactId>
    <scope>provided</scope>
</dependency>
```

之所以声明为 `provided`，是因为打包后这个工程要丢进外部的 Tomcat 容器里运行，而容器本身已经带了一套 Tomcat，再打进一份必然引起类冲突。

> [!question] `runtime` 与 `provided` 的区别？
> 两者在「是否参与编译」上正好相反：`runtime` **编译时不可见、运行时才需要**（如 MySQL 驱动）；`provided` **编译时可见、但不进产物**（如 `servlet-api`）。判断口诀：*运行时才用到的写 `runtime`，由容器提供的写 `provided`*。

## 2.3 传统依赖

Maven 拥有**传递依赖**特性：若项目直接依赖 A，A 依赖 B，B 又依赖 C，则 B、C 会自动被引入到当前项目的 classpath，无需手动声明。例如进行 **SpringBoot** 进行开发，我们只需要引入一个 starter 依赖

![[Maven传递依赖.png]]

其集成了所有 Web 开发常见的依赖，我们只需要引入这一个依赖，其他依赖会自动的通过 Maven 依赖传递传递进来。

## 2.4 依赖冲突与排除依赖

传递依赖简化了开发，但也带来问题：一是引入多余、不需要或是版本老旧的依赖；二是同一个构件通过多条依赖链引入、版本不一致，产生**依赖冲突**。

可在 `<dependency>` 内配置 `<exclusions>` 排除指定传递依赖，每个 `<exclusion>` 通过 `groupId` 和 `artifactId` 定位要排除的构件：

```xml
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
    <version>3.2.0</version>
    <exclusions>
        <exclusion>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-tomcat</artifactId>
        </exclusion>
    </exclusions>
</dependency>
```

## 2.5 依赖版本管理

在多模块项目中，可以在父 POM 中使用 `<dependencyManagement>` 统一管理依赖版本：

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

子模块引用时只需声明 groupId 和 artifactId，无需指定版本。

> [!note] dependencyManagement 只管版本，不引入依赖
> 它里面的条目**不会把任何依赖加进项目**。想用某个库，仍然必须在 `<dependencies>` 中声明，此时可省略 `<version>`。

# 三、生命周期与项目构建

## 3.1 生命周期

Maven 把项目构建的过程划分为若干个**阶段（phase）**，并预先固定了这些阶段的执行顺序，这个固定的顺序串就叫**生命周期**。生命周期一共三套：

![[Pasted image 20261115173337.png]]

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

**最需要记住的一条规则**：执行某个阶段时，Maven 会自动按顺序把它之前的所有阶段一并执行，而且这个顺序是固定的，不能调整。

也正因为 `package` 之前会先跑 `test`，所以每次打包都会自动执行一遍测试；如果觉得每次都跑测试太慢，就要用参数跳过去。

> [!tip] 阶段 ≠ 动作
> 阶段只是构建流程上的一个节点，真正干活的是**绑定**在该阶段上的插件目标（goal）。例如 `compile` 阶段绑定了 `maven-compiler-plugin` 的 `compile` 目标，`package` 阶段绑定了 `maven-jar-plugin` 的 `jar` 目标。所以阶段不能被跳过，但可以通过改写插件配置来微调它的行为。

## 3.2 常见构建命令

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
| `mvn deploy`                               | 编译、测试、打包、安装、上传到远程私服 |

> [!question] 用 `package`、`install` 还是 `deploy` ？
> - 只是想生成一个 jar 包给同事传阅 → 用 `package` 即可
> - 这个 jar 还要被**其他 Maven 项目**当作依赖引用 → 必须用 `install`，把构件放进本地仓库，下一个项目才能通过坐标找到它
> - 这个 jar 要上传给整个团队使用 → 用 `deploy` 发布到公司私服

> **`-DskipTests` 与 `-Dmaven.test.skip=true`**：前者只跳过**执行**测试（测试代码照常编译），后者连编译都跳过，跳得更快但也就测不到编译错误了。

## 3.3 项目构建

**（1）在终端中执行**

在终端里执行 `mvn` 命令，需要注意三件事。

**方式 1**：必须**先进入含 `pom.xml` 的项目根目录**，否则 Maven 会报找不到 `pom.xml`。

```bash
cd D:\project
mvn clean package
```

**方式 2**：也可以完全不切目录，**用 `-f` 参数直接指定 `pom.xml` 的路径**，在同时维护多个项目时很方便：

```bash
mvn -f D:\develop\project\demo\pom.xml clean package
```

**（2）IDEA 中执行**

IDEA 把 Maven 深度集成进了界面，在里面执行生命周期指令有两种主要方式。

**方式 1**：Maven 工具窗口双击阶段

![[Maven可视化项目构建.png]]

**方式 2**：自定义 Maven 运行配置

把整条指令当成一条命令执行，可以写多个阶段、还能带参数。

1. 在顶部运行配置下拉框中选择编辑配置，点 `+` 新建配置，选择 **Maven** 类型；
2. 工作目录填项目根目录，即 `pom.xml` 所在位置；
3. 运行处填要执行的阶段与参数——**注意不要写 `mvn` 前缀**，多个用空格隔开。
4. 保存后即可在右上角一键运行，日志输出在底部的 **Run** 面板。

![[Maven自定义运行配置.png]]

# 四、分模块项目

## 4.1 分模块设计
### 4.1.1 单模块工程的瓶颈

项目不分模块，意味着所有业务代码都写在同一个 Java 工程里。随着业务扩张，工程里会堆进商品、搜索、购物车、订单、用户中心等功能，几十甚至上百人同时维护一个工程，会带来两个问题：

1. **项目管理和维护困难**：所有团队共用一个代码库，职责边界模糊；
    
2. **通用组件难以复用**：自己封装的实体类、工具类、通用组件被夹在业务代码里，其他项目组想用，只能整体依赖这个工程。
    

![所有业务代码挤在一个 Java 项目中](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig01.webp?lastModify=1794737492)


### 4.1.2 分模块设计
**分模块设计**是指在项目设计阶段，就把一个大工程按功能或结构拆成若干个各自独立的模块。需要复用某个模块的能力时，直接引入它的依赖坐标即可；团队也可以按模块分工，几个人负责订单模块，另几个人负责购物车模块。

![按功能拆分为多个独立模块](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig02.webp?lastModify=1794737492)

> **结论**：分模块设计将项目按功能或结构拆成若干子模块，方便管理维护与扩展，也方便模块间相互调用、资源共享。

### 4.1.3 三种拆分策略

1. **按功能模块拆分**：公共组件、商品模块、搜索模块、购物车模块、订单模块等；
    
2. **按层拆分**：公共组件、实体类、控制层、业务层、数据访问层；
    
3. **按功能模块 + 层拆分**：两者结合，大型企业项目常见。
    

![三种拆分策略示意](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig03.webp?lastModify=1794737492)

> 图 3 · 按功能、按层、功能 + 层三种拆分思路。

> **原则**：先设计模块，再编码；不要把工程开发完毕后再反向拆分。本文案例是为演示而反向拆分的，真实项目应当一开始就规划好模块结构。

## 4 .2 实践案例

案例工程除了部门管理、员工管理、登录认证等业务功能，还有 `pojo` 包下的实体类（`PageBean`、`Result`）和 `utils` 包下的工具类（`Jwts`、阿里云 OSS 工具类等）。其他项目组想复用这些公共组件时，有两条路：

|方案|做法|问题|
|---|---|---|
|方案一|直接依赖 `tlias-web-management` |① 只想共享 pojo / utils，却会把全部业务类都加载进来，**影响性能**；② 业务代码对外公开，**不安全**|
|方案二|分模块设计（采用）|把公共设施抽成独立模块，按需引入坐标|

方案二的模块划分：

- `tlias-pojo`：抽取 pojo 包下的实体类；
    
- `tlias-utils`：抽取 utils 包下的工具类；
    
- `tlias-web-management`：保留业务代码，通过依赖坐标引入前两个模块。
    

![tlias-web-management 依赖 pojo 与 utils 模块](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig04.webp?lastModify=1794737492)

> 图 4 · 拆分后的依赖关系：业务模块依赖两个公共模块。





在 IDEA 中新建普通 Maven 模块，命名 `tlias-pojo`，保持包名与原工程一致（`com.itheima.pojo`），再把原工程 pojo 包下的实体类复制过来。

![新建 tlias-pojo 模块](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig05.webp?lastModify=1794737492)

> 图 5 · 新建 Maven 模块，Build system 选 Maven，设置 GroupId / ArtifactId。

![创建 com.itheima.pojo 包](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig06.webp?lastModify=1794737492)

> 图 6 · 在模块中建包，包名与原工程保持一致。

![复制实体类到 tlias-pojo](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig07.webp?lastModify=1794737492)

> 图 7 · 将原工程的实体类移动到新模块。

`src/main/java` 下的实体类用到了 `@DateTimeFormat` 等 Spring 注解，因此除了 Lombok，还需引入 Spring Boot 基础起步依赖。

** `tlias-pojo/pom.xml` **

<dependencies>  
    <dependency>  
        <groupId>org.projectlombok</groupId>  
        <artifactId>lombok</artifactId>  
        <version>1.18.34</version>  
    </dependency>  
    <dependency>  
        <groupId>org.springframework.boot</groupId>  
        <artifactId>spring-boot-starter</artifactId>  
        <version>3.2.8</version>  
    </dependency>  
</dependencies>

随后删除原工程的 pojo 包，并在 `tlias-web-management/pom.xml` 中引入：

<dependency>  
    <groupId>com.itheima</groupId>  
    <artifactId>tlias-pojo</artifactId>  
    <version>1.0-SNAPSHOT</version>  
</dependency>



同样建包 `com.itheima.utils`，把原工程 utils 包下的工具类复制过来。

![新建 tlias-utils 模块](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig08.webp?lastModify=1794737492)

> 图 8 · 新建第二个公共模块。

![创建 com.itheima.utils 包](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig09.webp?lastModify=1794737492)

> 图 9 · 工具类模块保持原包名。

![复制工具类到 tlias-utils](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig10.webp?lastModify=1794737492)

> 图 10 · 将 JWT、OSS 等工具类移动到新模块。

** `tlias-utils/pom.xml`（拆分初版，暂未做版本管理）**

<dependencies>  
    <!-- JWT 依赖 -->  
    <dependency>  
        <groupId>io.jsonwebtoken</groupId>  
        <artifactId>jjwt</artifactId>  
        <version>0.9.1</version>  
    </dependency>  
    <dependency>  
        <groupId>com.aliyun.oss</groupId>  
        <artifactId>aliyun-sdk-oss</artifactId>  
        <version>3.17.4</version>  
    </dependency>  
    <dependency>  
        <groupId>javax.xml.bind</groupId>  
        <artifactId>jaxb-api</artifactId>  
        <version>2.3.1</version>  
    </dependency>  
    <dependency>  
        <groupId>javax.activation</groupId>  
        <artifactId>activation</artifactId>  
        <version>1.1.1</version>  
    </dependency>  
    <!-- jaxb-runtime 版本不高于 2.3.3 -->  
    <dependency>  
        <groupId>org.glassfish.jaxb</groupId>  
        <artifactId>jaxb-runtime</artifactId>  
        <version>2.3.3</version>  
    </dependency>  
    <dependency>  
        <groupId>org.springframework.boot</groupId>  
        <artifactId>spring-boot-starter</artifactId>  
        <version>3.2.8</version>  
    </dependency>  
    <dependency>  
        <groupId>org.projectlombok</groupId>  
        <artifactId>lombok</artifactId>  
        <version>1.18.34</version>  
    </dependency>  
</dependencies>

删除原工程的 utils 包，并在业务模块中引入：

<dependency>  
    <groupId>com.itheima</groupId>  
    <artifactId>tlias-utils</artifactId>  
    <version>1.0-SNAPSHOT</version>  
</dependency>

**拆分结果**：`tlias-pojo`、`tlias-utils`、`tlias-web-management` 三个模块，公共设施可被任意其他项目按坐标复用。但此时三个模块各写了一遍 `lombok`，依赖配置开始重复 —— 这正是继承要解决的问题。


## 二、继承：让配置只写一次

![三个模块各自配置一遍 lombok](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig11.webp?lastModify=1794737492)

> 图 11 · 拆分后的新问题：同一依赖在三个模块里各声明一次。

### 2.1 概念与作用

- **概念**：继承描述两个工程之间的关系，与 Java 继承类似，子模块可以继承父工程的配置信息，最常见的是依赖关系的继承；
    
- **作用**：简化依赖配置、统一管理依赖；
    
- **做法**：新建父工程 `tlias-parent`，把各模块共有的依赖提到父工程，三个子模块继承它即可，无需各自声明。
    

![把共有依赖提取到父工程](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig12.webp?lastModify=1794737492)

> 图 12 · 把重复依赖上提到父工程，子模块自动继承。

子模块侧的核心配置：

<parent>  
    <groupId>...</groupId>  
    <artifactId>...</artifactId>  
    <version>...</version>  
    <relativePath>....</relativePath>  
</parent>

### 2.2 关键矛盾：Spring Boot 的单继承限制

`tlias-web-management` 是 Spring Boot 项目，本应继承 `spring-boot-starter-parent`；但 **Maven 不支持多继承**，一个项目只能有一个父工程，继承了官方父工程就无法继承自定义的 `tlias-parent`。

解法是利用 **多重继承链**：与 Java 一样，Maven 允许 A 继承 B、B 继承 C。让三个子模块都继承 `tlias-parent`，再由 `tlias-parent` 继承 `spring-boot-starter-parent`。

![多重继承链：子模块 → tlias-parent → spring-boot-starter-parent](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig13.webp?lastModify=1794737492)

> 图 13 · 用多重继承链同时获得自定义配置与 Spring Boot 官方依赖管理。

spring-boot-starter-parent          （官方父工程：统一版本管理）  
        ↑ 继承  
   tlias-parent                     （自定义父工程：共有依赖 + 版本锁定 + 聚合）  
        ↑ 继承  
   ├── tlias-pojo  
   ├── tlias-utils  
   └── tlias-web-management

### 2.3 三步落地

#### 第 1 步：创建父工程，打包方式设为 `pom`

新建 Maven 模块 `tlias-parent`，设置 `<packaging>pom</packaging>`（默认是 `jar`）；父工程不写代码，`src` 目录可直接删除。

![创建 tlias-parent 并设置 packaging 为 pom](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig14.webp?lastModify=1794737492)

> 图 14 · 新建父工程模块。

![父工程删除 src 目录](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig15.webp?lastModify=1794737492)

> 图 15 · 父工程只保留一个 pom.xml。

** `tlias-parent/pom.xml`（骨架）**

<?xml version="1.0" encoding="UTF-8"?>  
<project xmlns="http://maven.apache.org/POM/4.0.0"  
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">  
    <modelVersion>4.0.0</modelVersion>  
​  
    <parent>  
        <groupId>org.springframework.boot</groupId>  
        <artifactId>spring-boot-starter-parent</artifactId>  
        <version>3.2.8</version>  
        <relativePath/> <!-- 从仓库查找父工程，不走本地相对路径 -->  
    </parent>  
​  
    <groupId>com.itheima</groupId>  
    <artifactId>tlias-parent</artifactId>  
    <version>1.0-SNAPSHOT</version>  
    <packaging>pom</packaging>  
​  
    <properties>  
        <maven.compiler.source>17</maven.compiler.source>  
        <maven.compiler.target>17</maven.compiler.target>  
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>  
    </properties>  
</project>

> **三种打包方式**：`jar` 普通模块（Spring Boot 项目基本都是 jar，内嵌 Tomcat 运行）；`war` 普通 Web 程序（需部署到外部 Tomcat）；`pom` 父工程或聚合工程，不写代码，仅做依赖管理。

#### 第 2 步：子模块声明 `<parent>`

三个子模块的 `pom.xml` 均按如下方式配置（以 `tlias-utils` 为例）：

<parent>  
    <groupId>com.itheima</groupId>  
    <artifactId>tlias-parent</artifactId>  
    <version>1.0-SNAPSHOT</version>  
    <relativePath>../tlias-parent/pom.xml</relativePath>  
</parent>  
​  
<artifactId>tlias-utils</artifactId>  
<version>1.0-SNAPSHOT</version>

> **注意**
> 
> - 声明继承后，子模块坐标中的 `groupId` 可省略，会自动继承父工程；
>     
> - `relativePath` 指定父工程 pom 的相对位置（`../` 表示上一级目录），不写则从本地仓库 / 远程仓库查找；
>     
> - 父子工程声明同一依赖的不同版本时，**以子工程的版本为准**。
>     

#### 第 3 步：共有依赖上提到父工程

在父工程 pom 中追加 `<dependencies>`，子模块会自动继承，子模块里重复的声明即可删除。

<dependencies>  
    <dependency>  
        <groupId>org.springframework.boot</groupId>  
        <artifactId>spring-boot-starter</artifactId>  
        <version>3.2.8</version>  
    </dependency>  
    <dependency>  
        <groupId>org.projectlombok</groupId>  
        <artifactId>lombok</artifactId>  
        <version>1.18.34</version>  
    </dependency>  
</dependencies>

![子工程继承父工程中的 lombok 依赖](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig16.webp?lastModify=1794737492)

> 图 16 · 子模块删除重复声明后，仍能正常解析到父工程继承下来的 Lombok。

### 2.4 工程结构的两种形态

形态 A（演示用：基于已有工程反向拆分，模块与父工程平级）  
project/  
├── tlias-parent/  
├── tlias-pojo/  
├── tlias-utils/  
└── tlias-web-management/  
​  
形态 B（企业实际：先建父工程，子模块置于其下，层级更清晰）  
tlias-parent/  
├── pom.xml  
├── tlias-pojo/  
├── tlias-utils/  
└── tlias-web-management/

两种结构都能正常工作，`relativePath` 与 `<modules>` 中的相对路径按实际层级调整即可；差别只在可读性。

---

## 三、版本锁定：让版本只改一处

### 3.1 场景：非共有依赖的版本散落问题

并非所有依赖都是各模块共有的。假设项目里还有 `tlias-web-system`、`tlias-web-report` 等模块，它们和 `tlias-web-management` 都用到 jjwt，而 `tlias-pojo`、`tlias-utils` 并不需要 —— 这类依赖不该放进父工程的 `<dependencies>`（否则所有子模块都会被强制引入），只能谁用谁声明。

![三个子模块各自写 jwt 0.9.1](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig17.webp?lastModify=1794737492)

> 图 17 · 同一依赖版本在多个模块中重复书写。

问题出在升级时：把 0.9.1 升到 0.9.2，必须逐个模块翻 `pom.xml` 改 `<version>`，模块一多极易漏改，导致版本不一致。解法就是 **Maven 的版本锁定**。

### 3.2 `<dependencyManagement>` 的基本用法

父工程统一声明版本，**不引入依赖**：

<!-- 统一管理依赖版本 -->  
<dependencyManagement>  
    <dependencies>  
        <!-- JWT 令牌 -->  
        <dependency>  
            <groupId>io.jsonwebtoken</groupId>  
            <artifactId>jjwt</artifactId>  
            <version>0.9.1</version>  
        </dependency>  
    </dependencies>  
</dependencyManagement>

子模块仍需引入依赖，但**无需写 `<version>` **：

<dependencies>  
    <!-- JWT 令牌 -->  
    <dependency>  
        <groupId>io.jsonwebtoken</groupId>  
        <artifactId>jjwt</artifactId>  
    </dependency>  
</dependencies>

> **注意**
> 
> - `<dependencyManagement>` 只统一管理版本，并不会把依赖真正引入，这与 `<dependencies>` 不同；
>     
> - 子模块要用仍需自行引入，只是不用指定版本；变更版本只需改父工程一处；
>     
> - Spring Boot 项目中大量依赖不写版本也能用，正是因为 `spring-boot-starter-parent` 已经用 `<dependencyManagement>` 统一管好了版本。
>     

### 3.3 案例：把 tlias-utils 的版本交给父工程

**父工程追加**：

<dependencyManagement>  
    <dependencies>  
        <!-- JWT 依赖 -->  
        <dependency>  
            <groupId>io.jsonwebtoken</groupId>  
            <artifactId>jjwt</artifactId>  
            <version>0.9.1</version>  
        </dependency>  
        <dependency>  
            <groupId>com.aliyun.oss</groupId>  
            <artifactId>aliyun-sdk-oss</artifactId>  
            <version>3.17.4</version>  
        </dependency>  
        <dependency>  
            <groupId>javax.xml.bind</groupId>  
            <artifactId>jaxb-api</artifactId>  
            <version>2.3.1</version>  
        </dependency>  
        <dependency>  
            <groupId>javax.activation</groupId>  
            <artifactId>activation</artifactId>  
            <version>1.1.1</version>  
        </dependency>  
        <!-- 版本不高于 2.3.3 -->  
        <dependency>  
            <groupId>org.glassfish.jaxb</groupId>  
            <artifactId>jaxb-runtime</artifactId>  
            <version>2.3.3</version>  
        </dependency>  
    </dependencies>  
</dependencyManagement>

** `tlias-utils/pom.xml` 去掉全部 `<version>` **：

<dependencies>  
    <!-- JWT 依赖 -->  
    <dependency>  
        <groupId>io.jsonwebtoken</groupId>  
        <artifactId>jjwt</artifactId>  
    </dependency>  
    <dependency>  
        <groupId>com.aliyun.oss</groupId>  
        <artifactId>aliyun-sdk-oss</artifactId>  
    </dependency>  
    <dependency>  
        <groupId>javax.xml.bind</groupId>  
        <artifactId>jaxb-api</artifactId>  
    </dependency>  
    <dependency>  
        <groupId>javax.activation</groupId>  
        <artifactId>activation</artifactId>  
    </dependency>  
    <!-- 版本不高于 2.3.3 -->  
    <dependency>  
        <groupId>org.glassfish.jaxb</groupId>  
        <artifactId>jaxb-runtime</artifactId>  
    </dependency>  
</dependencies>

### 3.4 属性配置：版本号集中管理

版本号即使都在父工程里，也散落在多个 `<version>` 标签中。用自定义属性 + 属性引用，可以集中到 `<properties>` 一处：

<!-- 1) 自定义属性 -->  
<properties>  
    <lombok.version>1.18.34</lombok.version>  
</properties>  
​  
<!-- 2) 引用属性 -->  
<dependency>  
    <groupId>org.projectlombok</groupId>  
    <artifactId>lombok</artifactId>  
    <version>${lombok.version}</version>  
</dependency>

![属性配置：自定义 property 并在版本处引用](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig18.webp?lastModify=1794737492)

> 图 18 · 版本号统一定义在 `<properties>`，通过 `${xxx.version}` 引用。

** `tlias-parent/pom.xml` 最终形态**（含继承、共有依赖、版本锁定、属性集中管理；后续升级版本只改 `<properties>`）：

<?xml version="1.0" encoding="UTF-8"?>  
<project xmlns="http://maven.apache.org/POM/4.0.0"  
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">  
    <modelVersion>4.0.0</modelVersion>  
​  
    <parent>  
        <groupId>org.springframework.boot</groupId>  
        <artifactId>spring-boot-starter-parent</artifactId>  
        <version>3.2.8</version>  
        <relativePath/>  
    </parent>  
​  
    <groupId>com.itheima</groupId>  
    <artifactId>tlias-parent</artifactId>  
    <version>1.0-SNAPSHOT</version>  
    <packaging>pom</packaging>  
​  
    <properties>  
        <maven.compiler.source>17</maven.compiler.source>  
        <maven.compiler.target>17</maven.compiler.target>  
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>  
​  
        <spring-boot.version>3.2.8</spring-boot.version>  
        <lombok.version>1.18.34</lombok.version>  
        <jwt.version>0.9.1</jwt.version>  
        <aliyun.oss.version>3.17.4</aliyun.oss.version>  
        <jaxb.version>2.3.1</jaxb.version>  
        <javax.activation.version>1.1.1</javax.activation.version>  
        <jaxb.runtime.version>2.3.3</jaxb.runtime.version>  
    </properties>  
​  
    <!-- 共有依赖：子模块自动继承 -->  
    <dependencies>  
        <dependency>  
            <groupId>org.springframework.boot</groupId>  
            <artifactId>spring-boot-starter</artifactId>  
            <version>${spring-boot.version}</version>  
        </dependency>  
        <dependency>  
            <groupId>org.projectlombok</groupId>  
            <artifactId>lombok</artifactId>  
            <version>${lombok.version}</version>  
        </dependency>  
    </dependencies>  
​  
    <!-- 版本锁定：只管版本，不引入依赖 -->  
    <dependencyManagement>  
        <dependencies>  
            <dependency>  
                <groupId>io.jsonwebtoken</groupId>  
                <artifactId>jjwt</artifactId>  
                <version>${jwt.version}</version>  
            </dependency>  
            <dependency>  
                <groupId>com.aliyun.oss</groupId>  
                <artifactId>aliyun-sdk-oss</artifactId>  
                <version>${aliyun.oss.version}</version>  
            </dependency>  
            <dependency>  
                <groupId>javax.xml.bind</groupId>  
                <artifactId>jaxb-api</artifactId>  
                <version>${jaxb.version}</version>  
            </dependency>  
            <dependency>  
                <groupId>javax.activation</groupId>  
                <artifactId>activation</artifactId>  
                <version>${javax.activation.version}</version>  
            </dependency>  
            <dependency>  
                <groupId>org.glassfish.jaxb</groupId>  
                <artifactId>jaxb-runtime</artifactId>  
                <version>${jaxb.runtime.version}</version>  
            </dependency>  
        </dependencies>  
    </dependencyManagement>  
</project>

### 3.5 高频面试题

** `<dependencyManagement>` 与 `<dependencies>` 的区别？**

|| `<dependencies>` | `<dependencyManagement>` |
|---|---|---|
|是否引入依赖|是，直接引入|否，只声明版本|
|子模块是否自动继承|是，无需再声明|否，需自行引入依赖|
|子模块是否要写版本|不需要（已继承）|不需要（由父工程锁定）|
|典型用途|所有模块都需要的公共依赖|只有部分模块用到、但版本必须统一的依赖|

---

## 四、聚合：一次构建全部模块

### 4.1 场景：手工按依赖顺序构建的痛点

分模块之后模块关系错综复杂：`tlias-web-management` 的父工程是 `tlias-parent`，同时依赖 `tlias-pojo`、`tlias-utils`。

![模块间的依赖关系](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig19.webp?lastModify=1794737492)

> 图 19 · 父工程与三个子模块的关系；业务模块还依赖两个公共模块。

打包 `tlias-web-management` 时，Maven 会到本地仓库查找父工程和两个依赖模块，而本地仓库里此时还没有它们。于是必须先对 `tlias-parent`、`tlias-pojo`、`tlias-utils` 分别执行 `install` 装进本地仓库，再对业务模块执行 `package`。模块一多、依赖一复杂，这种按依赖顺序逐个构建的方式就难以维护。

**聚合**正是为此而生：通过聚合实现项目的一键构建（清理、编译、测试、打包、安装等）。

### 4.2 概念

- **聚合**：将多个模块组织成一个整体，同时进行项目构建；
    
- **聚合工程**：一个不含业务功能的「空」工程，有且仅有一个 pom 文件。通常**继承关系中的父工程与聚合关系中的聚合工程是同一个**，此处即 `tlias-parent`；
    
- **作用**：快速构建项目，无须按依赖关系手动逐个构建，直接在聚合工程上执行即可。
    

![聚合：一个空工程统一驱动多个模块](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig20.webp?lastModify=1794737492)

> 图 20 · 聚合工程作为构建入口，统一驱动所有子模块。

### 4.3 实现

在聚合工程中通过 `<modules>` 列出要聚合的子模块（路径相对父工程 pom）：

<!-- 聚合其他模块 -->  
<modules>  
    <module>../tlias-pojo</module>  
    <module>../tlias-utils</module>  
    <module>../tlias-web-management</module>  
</modules>

若采用 2.4 节「形态 B」（子模块放在父工程目录下），路径相应写成 `<module>tlias-pojo</module>` 等。

之后编译、打包、安装都不必逐模块操作：在 `tlias-parent` 上执行一次 `package`，被聚合的所有模块都会执行 `package`。

![聚合工程执行 package 一键打包全部模块](file:///D:/workbuddy_repo/spaces/2026_11_14_00-52-47/images/maven-fig21.webp?lastModify=1794737492)

> 图 21 · 在聚合工程执行 package，全部模块同步构建。

一键 clean、compile、test、package、install 同理，均由聚合工程统一驱动。
----



其他

用属性统一版本号

`<properties>` 本质上就是一个键值对集合，用来存放会被反复引用的常量。把依赖版本抽进去之后，升级时只需要改这一处：

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

引用时写 `${属性名}` 即可。Maven 还内置了一批属性可以直接使用，例如 `${project.version}` 取当前项目版本、`${project.basedir}` 取项目根目录、`${project.build.directory}` 取构建输出目录（默认是 `target`）。

属性适合管理零散、少量的版本号；如果依赖很多、又有父子工程，更常用的是下面这种做法。


---
