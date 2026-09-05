# 一、前置基础和环境搭建
## 1.1 anaconda 安装
下载安装包后，双击安装包，可以按默认设置一步步按 Next 按钮就可以。
在 "Advanced Installation Options" 中
![[Pasted image 20260905094717.png]]Add installation to my PATH environment variable 建议不要勾选。
如果勾选会直接把 Anaconda 加到系统环境变量，可能会与电脑其他 Python 版本冲突。

安装完后可打开 **Windows开始菜单**，在程序列表找到文件夹：**anaconda 3**，点开，选择：
**Anaconda Prompt**，执行命令测试安装配置是否成功
```
conda --version
```
## 1.2 anaconda 虚拟环境
在 Anaconda 中，我们可以使用 conda 命令来创建和管理不同的 Python 环境。每个环境可以有不同的 Python 版本和安装的包。
创建虚拟环境方式有两种，一种是在 Anaconda Prompt 中操作
```   
# 查看环境（或者：conda info --envs）
conda env list

# 基础创建（指定环境名 + Python 版本）
conda create -n env_name python=3.12

# 激活虚拟环境
conda activate env_name

# 退出虚拟环境
conda deactivate

# 在环境里安装第三方包（conda找不到的包用pip）
conda install numpy

# 删除虚拟环境
conda remove -n env_name --all
```
另一种是在图形界面 Anaconda Navigator 中操作
![[Pasted image 20260905100756.png|691]]
## 1.3 jupyter notebook 的使用
打开 **Anaconda Prompt (anaconda 3)**
1. 先**激活你想要用的虚拟环境**