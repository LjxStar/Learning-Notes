# 一、Maven

Apache Maven 是一种用于 Java 项目的构建工具。通过使用项目对象模型（POM），Maven 可以管理项目的编译、测试以及文档生成工作。

它把「项目」和「构建」解耦：项目只需要在 `pom.xml` 里声明自己是什么、需要什么，Maven 就负责去哪里找依赖、怎么编译、怎么测试、怎么打包、怎么部署。

## 1.1 maven 作用

![[Pasted image 20260926170102.png]]

Maven 最核心的价值是解决两件事：**管理 jar 包** 和 **管理项目**。

1. **管理（下载）jar 包**
   - 依赖 jar 包不再手动拷贝、复制到 `lib` 目录
   - 只要在 `pom.xml` 里写好坐标，Maven 会自动从仓库下载到本地
   - 升级 / 替换 jar 包只需要改一行 `version`，项目里所有用到的地方同步生效
2. **管理项目**
   - 编译、测试、打包、安装、部署全部用统一命令完成
   - 同一套命令在 Eclipse、IDEA、命令行上执行结果一致（约定优于配置）
   - 一个命令即可完成「清理 → 编译 → 测试 → 打包 → 部署」的全部流程

Maven 的三大特征：

- 保留原有项目结构，不破坏已有工程
- 自动将项目整理成规范的目录结构
- 便于管理 jar 包，依赖关系集中、清晰、可追溯

### 1.1.1 标准目录结构

Maven 对目录有强约定，目录名不对就会「找不到源码 / 找不到资源」：

```text
maven-project
├── pom.xml                 必需，项目对象模型文件
├── src
│   ├── main
│   │   ├── java            存放主程序 java 源码
│   │   └── resources       存放主程序资源文件（配置、图片、模板等）
│   └── test
│       ├── java            存放测试代码（JUnit）
│       └── resources       存放测试资源文件
└── target                  编译、打包后的产物目录，可删除，不入版本库
```

> [!tip] 记忆要点
> `main` 放要被执行的代码，`test` 放辅助 main 运行的代码；测试代码不会被打进 jar 包。

## 1.2 Maven 模型

![[Pasted image 20260926170205.png]]

Maven 内部有三大核心模型，缺一不可：

- **项目对象模型（POM，Project Object Model）**：把一个工程抽象成一个 `pom.xml` XML 文件。项目的基本信息、依赖、构建方式、插件配置都写在这里。Maven 通过读取 POM 来回答三个问题：这是什么项目、依赖什么、怎么构建。
- **依赖管理模型（Dependency）**：项目依赖以「坐标」的形式声明在 POM 中，Maven 依据坐标去仓库解析出具体的 jar 文件，并按**传递依赖**规则自动补全间接依赖。
- **插件模型（Plugin）**：Maven 自己并不实现编译、打包这些动作，它把编译（`maven-compiler-plugin`）、测试（`maven-surefire-plugin`）、打包（`maven-jar-plugin`）、部署等能力都封装成插件。插件同样用坐标标识，同样从仓库下载。

> [!abstract] 核心理解
> Maven 核心 = 模型（描述）+ 插件（执行）+ 仓库（存储）。`pom.xml` 只负责「声明」，真正的活由插件干，jar 从仓库取。

## 1.3 maven 仓库

Maven 仓库分为本地仓库、中央仓库和远程仓库
- 本地仓库：自己计算机上的一个目录(用来存储 jar 包)
- 中央仓库：由 Maven 团队维护的全球唯一的。
- 远程仓库(私服)：一般由公司团队搭建的私有仓库

当项目中使用坐标引入对应依赖 jar 包后，查找顺序为： 本地仓库 --> 远程仓库（如果有）--> 中央仓库

### 1.3.1 三类仓库对比

| 仓库     | 位置 / 来源           | 特点                                                |
| ------ | ------------------ | ------------------------------------------------- |
| 本地仓库   | 本机磁盘，默认 `~/.m2/repository` | 第一次下载后缓存在这里，之后优先使用；离线开发靠它    |
| 中央仓库   | `https://repo.maven.apache.org/maven2` | 全球唯一、公开、免费，Maven 官方维护                |
| 远程仓库   | 公司 / 第三方搭建的私服            | 存放公司内部 jar 或中央仓库没有的第三方 jar，需要在 `pom.xml` 中声明 |

### 1.3.2 依赖查找流程

1. 坐标写入 `pom.xml` 后，Maven 先去**本地仓库**找。
2. 本地没有 → 检查 `pom.xml` 中配置的**远程仓库**。
3. 远程也没有 → 去**中央仓库**下载。
4. 找到后下载到本地仓库缓存一份，下次直接用本地。

> [!warning] 注意
> jar 包一旦下载到本地仓库并被项目引用，删除项目目录不会删除 jar；改版本号会重新下载新版本，旧版本仍留在本地仓库。

### 1.3.3 常见配置：镜像加速

中央仓库在国外，下载很慢，一般在 `settings.xml` 中配置国内镜像（如阿里云）。`settings.xml` 位于 `${user.home}\.m2\settings.xml`（Maven 安装目录 `conf/settings.xml` 里的是全局配置）：

```xml
<settings>
    <!-- 修改本地仓库位置 -->
    <localRepository>D:\maven\repository</localRepository>

    <mirrors>
        <mirror>
            <id>aliyun</id>
            <name>aliyun maven</name>
            <url>https://maven.aliyun.com/repository/public</url>
            <!-- mirrorOf 为 * 表示替换所有仓库 -->
            <mirrorOf>*</mirrorOf>
        </mirror>
    </mirrors>
</settings>
```

> [!note]
> 镜像只是「换了一个地方下载 jar」，坐标、依赖解析规则完全没变；`mirrorOf` 取值常见有 `*`、`central`、`external:*`。

## 1.4 Maven 坐标

坐标是 Maven 定位一个构件（jar / pom / war）的唯一标识，由三部分组成：

```text
groupId:artifactId:version
```

| 组成          | 含义                                     | 示例             |
| ----------- | -------------------------------------- | --------------- |
| `groupId`   | 组织标识，一般是公司域名反写（`com.` / `org.` 开头） | `org.springframework` |
| `artifactId` | 项目 / 模块名                            | `spring-webmvc`  |
| `version`   | 版本号                                    | `6.1.0`          |

完整示例：

```xml
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-context</artifactId>
    <version>6.1.0</version>
</dependency>
```

- 坐标的完整写法是 `org.springframework:spring-context:6.1.0`
- 三者组合全局唯一，避免不同公司 / 不同项目出现同名 jar 冲突
- `version` 建议统一在 `<properties>` 中管理，便于升级：

```xml
<properties>
    <spring.version>6.1.0</spring.version>
</properties>
```

```xml
<version>${spring.version}</version>
```

## 1.5 依赖管理

### 1.5.1 添加依赖

依赖统一写在 `pom.xml` 的 `<dependencies>` 中，一个 `<dependency>` 就是一组坐标：

```xml
<dependencies>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-context</artifactId>
        <version>6.1.0</version>
    </dependency>
</dependencies>
```

### 1.5.2 依赖传递性

如果 A 依赖 B，B 依赖 C，那么 A 也可以直接使用 C 的依赖。

```text
项目 A → B → C
项目 A 可以使用 B，也可以使用 C
```

好处：开发时只需要引入最直接的那个 jar，其余自动补全，**依赖数量远小于实际需要**。

### 1.5.3 依赖冲突与就近原则

当同一个 jar 被多路径引入且版本不一致时，就会发生依赖冲突：

```text
A → B → D:1.0
A → C → D:2.0
```

Maven 处理规则（**就近原则**）：

- 直接依赖优先于间接依赖（B 距离 A 更近，用 B 的 `D:1.0`）
- 距离相同时，**先声明的依赖优先**（`<dependencies>` 中靠前的生效）
- 想要忽略某个传递依赖，用 `<exclusions>` 排除

### 1.5.4 排除依赖 exclusions

某些传递进来的 jar 项目中用不到，或者会造成版本冲突，可以显式排除：

```xml
<dependency>
    <groupId>org.springframework</groupId>
    <artifactId>spring-context</artifactId>
    <version>6.1.0</version>
    <exclusions>
        <exclusion>
            <groupId>commons-logging</groupId>
            <artifactId>commons-logging</artifactId>
        </exclusion>
    </exclusions>
</dependency>
```

> [!tip] 排查思路
> 出现 `ClassNotFoundException`、`NoSuchMethodError` 之类的问题时，先 `mvn dependency:tree` 看依赖树，定位是哪个 jar 冲突或缺失。

### 1.5.5 依赖管理 dependencyManagement

`<dependencyManagement>` 声明依赖的**版本，但不真正引入依赖**，用于多模块项目统一版本、子模块引用时不写 `version`：

```xml
<dependencyManagement>
    <dependencies>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>6.1.0</version>
        </dependency>
    </dependencies>
</dependencyManagement>
```

## 1.6 Maven 生命周期

Maven 把项目从「清理」到「部署」的整个过程抽象成一条**生命周期**，共分三类：

- `clean`：清理生命周期（`pre-clean` → `clean` → `post-clean`）
- `default`：默认生命周期（验证、编译、测试、打包、校验、安装、部署）
- `site`：站点文档生命周期（生成项目站点文档，基本不用）

default 生命周期阶段顺序（执行后面的阶段，会自动把前面的都执行一遍）：

| 阶段      | 说明                            | 常用命令            |
| ------- | ----------------------------- | ----------------- |
| validate | 验证项目是否正确，验证是否通过           |                    |
| compile  | 编译项目，生成 `target/classes`    | `mvn compile`     |
| test     | 运行单元测试                        | `mvn test`        |
| package  | 打包，生成 jar / war 包            | `mvn package`     |
| verify   | 校验打包结果是否正确                   |                    |
| install  | 把项目安装到**本地仓库**，供其他项目使用    | `mvn install`     |
| deploy   | 把项目发布到**远程仓库**（私服）        | `mvn deploy`      |

关键理解：`mvn package` 等于 `mvn clean` 之后的 compile + test + package 依次执行；`mvn install` 会把 jar 装进本地仓库，所以自己项目之间互相依赖时必须 install。

## 1.7 常用命令

| 命令                    | 作用                     |
| --------------------- | ---------------------- |
| `mvn clean`           | 清理 `target` 目录           |
| `mvn compile`         | 编译主程序                  |
| `mvn test`            | 运行单元测试                 |
| `mvn package`         | 编译、测试并打包（jar / war）   |
| `mvn install`         | 打包并安装到本地仓库             |
| `mvn deploy`          | 发布到远程仓库               |
| `mvn clean package -DskipTests` | 打包但跳过执行测试（常用）   |
| `mvn clean install -Dmaven.test.skip=true` | 打包并跳过编译测试代码      |
| `mvn dependency:tree` | 打印完整依赖树，排查依赖冲突       |

> [!warning]- 常用参数
> `-DskipTests`：编译测试代码但不执行测试；
> `-Dmaven.test.skip=true`：测试代码连编译都跳过。
> 两种都能加快打包速度，但正式发布前建议补跑测试。

## 1.8 在 IDEA 中使用 Maven

- 打开项目时 IDEA 会自动识别 `pom.xml` 并导入依赖（右下角可看导入进度）。
- 右侧 **Maven 面板**（Lifecycle / Plugins）中双击某个阶段即可执行，如 `clean`、`install`。
- 若依赖报红，按 `Maven 面板 → Reload All Maven Projects` 重新加载。
- 常用设置：`Settings → Build Tools → Maven` 中可指定本地仓库路径和 `settings.xml` 位置。

## 1.9 小结

- Maven 通过 **POM** 描述项目，通过**插件**执行任务，通过**仓库**存放构件。
- 三大核心模型：**项目对象模型、依赖管理模型、插件模型**。
- 依赖查找顺序：**本地仓库 → 远程仓库 → 中央仓库**，下载后缓存到本地。
- 坐标 = `groupId` + `artifactId` + `version`，是构件的唯一标识。
- 依赖冲突遵循**就近原则**：直接依赖优先，同距离时先声明者优先。
- 生命周期三要素：**clean、default、site**，常用的是 default 的 compile / test / package / install / deploy。
