# 1.配置本地仓库

![[Pasted image 20260926172429.png]]

进入到 conf 目录下打开 `settings.xml` 配置文件，定位到 53 行左右，复制 `<localRepository>` 标签，粘贴到注释的外面。复制之前新建的用来存储 jar 包的路径，替换掉 `<localRepository>` 标签体内容

![[Pasted image 20260926172810.png]]

# 2.配置仓库镜像

由于中央仓库在国外，所以下载 jar 包速度可能比较慢，而阿里公司提供了一个远程仓库，里面基本也都有开源项目的 jar 包。
进入到 conf 目录下打开 settings.xml ，定位到 160 行左右，