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

![[Maven生命周期.png]]

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

### 4.1.1 单模块的瓶颈

不分模块，所有业务代码都写在同一个工程里。业务一扩张，商品、搜索、购物车、订单、用户中心便陆续堆进同一个仓库，几十上百人同时维护，问题随之而来：

1. **管理与维护困难**：所有团队共用一个代码库，职责边界模糊，改一处代码就要担心牵连别人；
2. **通用组件难以复用**：封装好的实体类、工具类夹在业务代码中间，别的项目组想用，只能整体依赖这个工程。
![[Pasted image 20261115190325.png]]
![[Maven-单模块瓶颈.webp]]

### 4.1.2 分模块设计

分模块设计就是在设计阶段把一个工程按功能或结构拆成若干独立模块：需要哪个模块的能力，引入它的坐标即可；团队也能按模块分工，各自独立开发。
![[Pasted image 20261115190420.png]]
![[Maven-分模块拆分.webp]]

常见做法有三种，大型项目多用第三种：

1. **按功能拆分**：公共组件、商品、搜索、购物车、订单……
2. **按层拆分**：实体类、控制层、业务层、数据访问层；
3. **功能 + 层**：先按功能拆，功能内部再分层。

> [!tip] 先规划，再编码
> 模块边界应当在编码前定清楚，不要等工程写完再反向拆分。本文案例是为了演示拆分过程才这么做。

### 4.1.3 拆分实践

案例工程 `tlias-web-management` 除部门、员工、登录等业务功能外，还有 `pojo` 包下的实体类（`PageBean`、`Result`）和 `utils` 包下的工具类（`Jwts`、阿里云 OSS 工具类）。别的项目组想复用它们，只能整体依赖 `tlias-web-management`：既会把全部业务代码一起加载（**影响性能**），又等于把业务代码公开（**不安全**）。因此按职责拆成三个模块：

- `tlias-pojo`：`pojo` 包下的实体类；
- `tlias-utils`：`utils` 包下的工具类；
- `tlias-web-management`：保留业务代码，按坐标依赖前两个模块。
![[Pasted image 20261115190756.png]]
![[Maven-分模块后依赖.webp]]

**（1）新建 `tlias-pojo` 模块**

新建普通 Maven 模块 `tlias-pojo`（GroupId 填 `com.itheima`，ArtifactId 填 `tlias-pojo`），在其中建包 `com.itheima.pojo`，把原工程的实体类复制过来。

实体类用到了 `@DateTimeFormat` 等 Spring 注解，因此除 Lombok 外还要引入 Spring Boot 基础起步依赖：

```xml
<!-- tlias-pojo/pom.xml -->
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
```

删除原工程的 `pojo` 包，再在 `tlias-web-management/pom.xml` 中按坐标引入新模块：

```xml
<dependency>
    <groupId>com.itheima</groupId>
    <artifactId>tlias-pojo</artifactId>
    <version>1.0-SNAPSHOT</version>
</dependency>
```

**（2）新建 `tlias-utils` 模块**

同法新建 `tlias-utils`，建包 `com.itheima.utils`，把 `Jwts`、OSS 等工具类复制过来：

```xml
<!-- tlias-utils/pom.xml -->
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
    <!-- jjwt 0.9.1 需要手动补齐 JAXB，jaxb-runtime 版本不高于 2.3.3 -->
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
```

删除原工程的 `utils` 包，同样在业务模块中引入：

```xml
<dependency>
    <groupId>com.itheima</groupId>
    <artifactId>tlias-utils</artifactId>
    <version>1.0-SNAPSHOT</version>
</dependency>
```



## 4.2 继承



4.2.1 继承的原因？
模块拆开后，重复的依赖声明成了新负担：同一批依赖每个模块都要写一遍，升级时还得逐个改。继承就是用来消除这份重复的。

三个模块各自独立，公共组件已可被任意项目按坐标复用。但 `lombok`、`spring-boot-starter` 在三个模块里各声明了一遍——升级版本要改三处，这正是接下来「继承」要解决的问题。
### 4.2.2 概念

- **概念**：继承描述两个工程之间的关系，与 Java 继承类似，子模块可以直接继承父工程的配置，最常见的是依赖；
- **作用**：简化依赖配置，统一管理版本；
![[Pasted image 20261115190817.png]]
![[Maven-继承提取共有依赖.webp]]

### 4.2.2 单继承与继承链

`tlias-web-management` 是 Spring Boot 项目，本应继承 `spring-boot-starter-parent` 以取得官方的版本管理；但 **Maven 不支持多继承**，一个工程只能有一个父工程。

解法是把继承链拉长：三个子模块都继承 `tlias-parent`，再由 `tlias-parent` 继承 `spring-boot-starter-parent`，与 Java 里 A 继承 B、B 继承 C 是同一个道理。

```text
spring-boot-starter-parent     官方父工程：统一版本管理
        ↑ 继承
  tlias-parent                 自定义父工程：共有依赖 + 版本管理 + 聚合
        ↑ 继承
  ├── tlias-pojo
  ├── tlias-utils
  └── tlias-web-management
```
![[Pasted image 20261115190915.png]]
![[Maven-多重继承链.webp]]

### 4.2.3 三步落地

**（1）创建父工程，打包方式设为 `pom`**

新建 Maven 模块 `tlias-parent`，把 `<packaging>` 改成 `pom`（默认是 `jar`）；父工程不写代码，`src` 目录可以直接删除，只留一个 `pom.xml`。
![[Pasted image 20261115190924.png]]
![[Maven-父工程打包方式pom.webp]]

```xml
<!-- tlias-parent/pom.xml（省略 XML 声明与 project 根标签） -->
<parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>3.2.8</version>
    <relativePath/> <!-- 从仓库查找父工程，不走本地相对路径 -->
</parent>

<groupId>com.itheima</groupId>
<artifactId>tlias-parent</artifactId>
<version>1.0-SNAPSHOT</version>
<packaging>pom</packaging>
```

> [!tip] 三种打包方式
> `jar`：普通模块，Spring Boot 工程基本都是 jar，内嵌 Tomcat 直接运行；`war`：普通 Web 程序，需部署到外部 Tomcat；`pom`：父工程或聚合工程，不写代码，只做依赖与构建管理。

**（2）子模块声明 `<parent>`**

三个子模块的 `pom.xml` 都加上这段（以 `tlias-utils` 为例）：

```xml
<parent>
    <groupId>com.itheima</groupId>
    <artifactId>tlias-parent</artifactId>
    <version>1.0-SNAPSHOT</version>
    <relativePath>../tlias-parent/pom.xml</relativePath>
</parent>

<artifactId>tlias-utils</artifactId>
<version>1.0-SNAPSHOT</version>
```

> [!warning] 注意
> - 声明继承后，子模块坐标中的 `groupId` 可以省略，自动继承父工程；
> - `relativePath` 指定父工程 pom 的相对位置（`../` 表示上一级目录），不写则从本地仓库、远程仓库查找；
> - 父子工程对同一依赖声明了不同版本时，**以子工程的版本为准**；
> - 子模块若直接建在父工程目录下（多数企业项目的做法），`relativePath` 与后面 `<modules>` 的路径都要相应简化为 `tlias-pojo` 这类同级目录。

**（3）共有依赖上提**

在父工程追加 `<dependencies>`，子模块删掉重复的声明即可。`spring-boot-starter` 的版本已由 `spring-boot-starter-parent` 统一管理，不必再写：

```xml
<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter</artifactId>
    </dependency>
    <dependency>
        <groupId>org.projectlombok</groupId>
        <artifactId>lombok</artifactId>
        <version>1.18.34</version>
    </dependency>
</dependencies>
```
![[Pasted image 20261115190939.png]]
![[Maven-子模块继承lombok.webp]]

### 4.2.4 依赖管理

继承解决了「共有依赖写多遍」，但还有另一类依赖：只有部分模块需要，版本却必须统一。比如项目里还有 `tlias-web-system`、`tlias-web-report`，它们与 `tlias-web-management` 都用 jjwt，而 `tlias-pojo`、`tlias-utils` 用不到。这类依赖放进父工程 `<dependencies>` 会强制所有子模块引入，只能谁用谁声明，于是版本号散落在各个模块——升级时极易漏改，同一依赖便留下多个版本。
![[Pasted image 20261115191002.png]]
![[Maven-jwt版本散落各模块.webp]]

**版本锁定**：父工程用 `<dependencyManagement>` 声明版本，子模块引入时省略 `<version>`。

```xml
<!-- 父工程：只声明版本，不引入依赖 -->
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>io.jsonwebtoken</groupId>
            <artifactId>jjwt</artifactId>
            <version>0.9.1</version>
        </dependency>
    </dependencies>
</dependencyManagement>

<!-- 子模块：不写 version，版本由父工程决定 -->
<dependency>
    <groupId>io.jsonwebtoken</groupId>
    <artifactId>jjwt</artifactId>
</dependency>
```

> [!important] `dependencyManagement` 只管版本，不引入依赖
> - 其中的条目**不会把依赖加进任何模块**，这一点与 `<dependencies>` 完全不同；
> - 子模块要用仍需自己声明，只是不必写版本，升级时只改父工程一处；
> - Spring Boot 项目里大量依赖不写版本也能用，正是因为 `spring-boot-starter-parent` 已在 `<dependencyManagement>` 中把版本管好了。

**属性配置**：版本号即使都收进父工程，仍散落在多个 `<version>` 里。用自定义属性把它们提到 `<properties>` 一处：

```xml
<properties>
    <lombok.version>1.18.34</lombok.version>
</properties>

<dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <version>${lombok.version}</version>
</dependency>
```

`<properties>` 本质上是一个键值对集合，专门存放会被反复引用的常量，引用时写 `${属性名}` 即可。

### 4.2.5 父工程最终形态

把继承、共有依赖、版本管理与属性合到一起，`tlias-parent/pom.xml` 的最终形态如下（省略 XML 声明与 `project` 根标签）：

```xml
<parent>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-parent</artifactId>
    <version>3.2.8</version>
    <relativePath/>
</parent>

<groupId>com.itheima</groupId>
<artifactId>tlias-parent</artifactId>
<version>1.0-SNAPSHOT</version>
<packaging>pom</packaging>

<properties>
    <maven.compiler.source>17</maven.compiler.source>
    <maven.compiler.target>17</maven.compiler.target>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>

    <lombok.version>1.18.34</lombok.version>
    <jwt.version>0.9.1</jwt.version>
    <aliyun.oss.version>3.17.4</aliyun.oss.version>
</properties>

<!-- 共有依赖：子模块自动继承 -->
<dependencies>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter</artifactId>
    </dependency>
    <dependency>
        <groupId>org.projectlombok</groupId>
        <artifactId>lombok</artifactId>
        <version>${lombok.version}</version>
    </dependency>
</dependencies>

<!-- 依赖管理：只管版本，不引入依赖 -->
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
        <!-- JAXB 三件套同理，略 -->
    </dependencies>
</dependencyManagement>
```

日后升级某个依赖，只改 `<properties>` 一处即可。

## 4.3 聚合

### 4.3.1 为什么要聚合

分模块之后，模块之间的关系变得错综复杂：`tlias-web-management` 的父工程是 `tlias-parent`，同时又依赖 `tlias-pojo`、`tlias-utils`。
![[Pasted image 20261115191037.png]]
![[Maven-模块间依赖关系.webp]]

打包业务模块时，Maven 要到本地仓库查找它的父工程和两个依赖模块，而仓库里此时还没有它们。于是只能先按依赖顺序对 `tlias-parent`、`tlias-pojo`、`tlias-utils` 逐个 `install`，再对业务模块执行 `package`。模块一多、依赖一复杂，这种手工排序就难以为继——聚合就是为了实现项目的一键构建。

### 4.3.2 概念

- **聚合**：把多个模块组织成一个整体，一起参与构建；
- **聚合工程**：不含业务功能的「空」工程，只有一个 pom 文件。实际项目中，聚合工程通常与继承关系中的父工程是同一个，此处即 `tlias-parent`；
- **作用**：不必再按依赖关系手动逐个构建，在聚合工程上执行一次即可。

### 4.3.3 配置

在聚合工程中用 `<modules>` 列出要聚合的子模块（路径相对父工程的 pom）：

```xml
<modules>
    <module>../tlias-pojo</module>
    <module>../tlias-utils</module>
    <module>../tlias-web-management</module>
</modules>
```

此后在 `tlias-parent` 上执行一次 `package`，被聚合的所有模块都会按依赖顺序跟着执行 `package`；`clean`、`compile`、`test`、`install` 同理，不必再关心模块之间的先后顺序。
![[Pasted image 20261115191057.png]]
![[Maven-聚合一键打包.webp]]
