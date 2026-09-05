# 一、前置基础和环境搭建
下载安装包后，双击安装包，可以按默认设置一步步按 Next 按钮就可以。
在 "Advanced Installation Options" 中
![[Pasted image 20260905094717.png]]Add installation to my PATH environment variable 建议不要勾选。
如果勾选会直接把 Anaconda 加到系统环境变量，可能会与电脑其他 Python 版本冲突。

安装完后可打开 **Windows开始菜单**，在程序列表找到文件夹：**anaconda 3**，点开，选择：
**Anaconda Prompt**，执行命令测试安装配置是否成功
```
conda --version
```
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
打开 Anaconda Prompt ，先激活你想要用的虚拟环境，通过 cd 切换到项目位置，输入 jupyter notebook 

# 二、一些小模版
## 2.1 项目初始化完整模板
```
# 导包
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
import seaborn as sns
import os
import random
import warnings
warnings.filterwarnings("ignore")

# Matplotlib 中文和负号显示修复
plt.rcParams["font.family"] = ["SimHei"]
plt.rcParams["axes.unicode_minus"] = False

# ====================== 7. Seaborn / 绘图全局风格 ======================
sns.set_style("whitegrid")   # 白色网格背景
sns.set_context("notebook")

# ====================== 8. 固定随机种子，保证实验可复现 ======================
SEED = 42
np.random.seed(SEED)
random.seed(SEED)

# ====================== 9. 项目根目录路径设置 (跨平台，Windows/Linux/Mac 通用) ======================
# 方式1：当前 notebook 文件所在目录作为根路径
ROOT = Path(os.getcwd())
# 如果你想手动指定根文件夹：ROOT = Path(r"C:/your_project")

# 常用子路径，直接调用 DATA_DIR, OUTPUT_DIR
DATA_DIR = ROOT / "data"
OUTPUT_DIR = ROOT / "output"
FIG_DIR = OUTPUT_DIR / "figures"

# 自动创建文件夹，不存在就新建，不会报错
for folder in [DATA_DIR, OUTPUT_DIR, FIG_DIR]:
    folder.mkdir(exist_ok=True, parents=True)

# ====================== 10. Pandas 显示设置：打印完整表格 ======================
pd.set_option("display.max_rows", None)      # 显示所有行
pd.set_option("display.max_columns", None)   # 显示所有列
pd.set_option("display.width", None)
pd.set_option("display.max_colwidth", 200)

# ====================== 11. Jupyter 内嵌图片显示 ======================
%matplotlib inline

# 如果需要交互式绘图(放大拖拽图片)，打开下面一行
# %matplotlib notebook

# ====================== 12. 打印环境信息校验 ======================
print("✅ 所有包导入成功！")
print(f"numpy版本: {np.__version__}")
print(f"pandas版本: {pd.__version__}")
print(f"项目根目录: {ROOT}")
print(f"数据文件夹: {DATA_DIR}")
```