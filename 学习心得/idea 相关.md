
# idea 创建 SpringBoot 项目时，官方脚手架与阿里云脚手架的区别？

⚙️ 编译参数 `-parameters` 的默认配置

`@RequestParam` 在简单类型上可以省略，但这依赖于编译时开启 `-parameters` 参数。

- **官方脚手架**：其 `spring-boot-starter-parent` 从 **Spring Boot 3.2** 开始已默认开启 `-parameters`，因此在 Maven 编译下可以省略。
- **阿里云脚手架**：其生成的 `pom.xml` 结构不同（通常不继承官方 parent），但**只要它使用的 Spring Boot 版本 ≥ 3.2，其依赖的编译插件配置也会遵循这一默认行为**。如果你选择了较旧的 Spring Boot 2.x 版本，则可能需要手动确保编译参数已开启。