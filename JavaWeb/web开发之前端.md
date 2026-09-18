Web 标准（也称网页标准）是由一系列标准组成的规范体系，大部分由 W3C（World Wide Web Consortium，万维网联盟）负责制定，主要由以下三个部分组成：

- **结构层（HTML）**：负责网页的内容结构，定义页面上有什么（标题、段落、图片、表格等）。
- **表现层（CSS）**：负责网页的样式展示，控制外观（颜色、字体、布局、间距等）。
- **行为层（JavaScript）**：负责网页的交互行为，实现动态效果、响应用户操作。

![[前端三件套的关系.png|598]]

三者之间形成「结构、样式、行为」分工体系：HTML 提供基础内容，CSS 美化页面外观，JavaScript 控制逻辑与互动。浏览器先解析 HTML，再套用 CSS，最后执行 JavaScript。这样的分工设计，有助于提升网页的维护性与扩展性，是现代网页开发的基础架构。

# 一、前端三件套

## 1.1 HTML 页面结构与常用标签

### 1.1.1 HTML 标准骨架

任何 HTML 页面都遵循同一套标准骨架，先把骨架结构记熟，写页面就不会乱：

```html
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>页面标题</title>
</head>
<body>
    <!-- 页面主体内容 -->
</body>
</html>
```

- `<!DOCTYPE html>` 声明文档类型，告诉浏览器该文件是 HTML 5 文档，有助于浏览器正确解析和渲染网页内容。
- `<html>` 定义整个 HTML 文档的根元素，所有 HTML 元素必须包含在 `<html>` 标签内。
- `<head>` 网页头部，存放浏览器解析所需的元信息（CSS 样式、页面标题、字符编码等），对用户不可见。
- `<body>` 网页主体，存放展示给用户的所有内容（文字、图片、视频、表单等）。
- `<title>` 定义浏览器标签栏显示的页面标题。

### 1.1.2 常用核心标签

HTML 的标签种类繁多，先掌握最常用的几类，其余用到时查阅即可。

1. **文本类标签**

文本类标签用于调整文字的展示效果，常用标签如下：

| 标签                 | 作用   | 说明                     |
| ------------------ | ---- | ---------------------- |
| `<h1>~<h6>`        | 标题标签 | 一级到六级，权重依次降低           |
| `<p>`              | 段落标签 | 自带上下外边距                |
| `<br>`             | 换行标签 | 单标签，强制文本换行             |
| `<hr>`             | 水平分隔 | 单标签，表示主题的分隔           |
| `<strong>` / `<b>` | 文本加粗 | `<strong>` 语义权重更高，推荐使用 |
| `<em>` / `<i>`     | 文本倾斜 | `<em>` 语义权重更高，推荐使用     |
| `<ins>` / `<u>`    | 下划线  | `<ins>` 语义权重更高         |
| `<del>` / `<s>`    | 删除线  | `<del>` 语义权重更高         |

2. **媒体与链接标签**

下面的标签用于在页面中嵌入图片、视频、音频，或实现页面跳转：

- **图片标签 `<img>`**

```html
<img src="图片路径" alt="加载失败提示" width="宽度" height="高度" title="鼠标悬浮提示">
```

- **视频标签 `<video>`**

```html
<video src="视频地址" controls autoplay muted loop width="宽度"></video>
```

`controls`：显示播放控件；`autoplay`：自动播放；`muted`：静音；`loop`：循环播放。

- **音频标签 `<audio>`**

```html
<audio src="音频地址" controls autoplay muted loop></audio>
```

音频标签的属性含义与 `<video>` 完全一致，只是没有画面显示。

- **超链接标签 `<a>`**

```html
<a href="链接地址" target="_blank" title="悬浮提示">链接文本</a>
```

`target="_blank"`：在新窗口打开链接；`target="_self"`：在当前页面打开（默认值）。

3. **容器与列表标签**

- `<div>`：块级容器，独占一行，常用于页面布局，可设置宽高。
- `<span>`：行内容器，一行可放置多个，常用于局部文字修饰，不可设置宽高。

- **无序列表**
```html
<ul>
    <li>列表项1</li>
    <li>列表项2</li>
</ul>
```
- **有序列表**
```html
<ol start="起始序号">
    <li>列表项1</li>
    <li>列表项2</li>
</ol>
```

4. **注释**

注释是写给开发人员看的说明文字，浏览器不会渲染显示：

```html
<!-- 这是注释，浏览器不渲染 -->
<input type="text" name="名称" value="默认值" placeholder="提示文字">
```

### 1.1.3 HTML 字符实体

有些特殊符号无法被浏览器直接解析，需要使用对应的字符实体来替代：

| 符号  | 字符实体     | 说明         |
| --- | -------- | ---------- |
| 空格  | `&nbsp;` | 不换行空格      |
| <   | `&lt;`   | 小于号 / 左尖括号 |
| >   | `&gt;`   | 大于号 / 右尖括号 |
| &   | `&amp;`  | 与号 / 和     |

## 1.2 CSS 的引入与选择器

### 1.2.1 三种引入方式

CSS 样式要作用到页面上，主要有以下三种引入方式，实际开发中推荐使用外部样式：

1. **行内样式**：写在标签的 `style` 属性中，优先级最高。

```html
<div style="color:red;">行内样式</div>
```

2. **内部样式**：写在 `<head>` 的 `<style>` 标签中，仅当前页面生效。

```html
<head>
  <style>
    /* css 写在这里 */
  </style>
</head>
```

3. **外部样式**：独立的 `.css` 文件，通过 `<link>` 标签引入，可多页面复用，开发中推荐使用。

```html
<link rel="stylesheet" href="style.css">
```

### 1.2.2 CSS 选择器

选择器用于精准地选中页面元素并应用样式，掌握以下几类常用的即可应对大多数场景：

1. **元素名称选择器**：格式 `元素名称 { 样式 }`，选中页面中所有该标签元素。

```css
/* 元素选择器 */
p {
    text-indent: 2em; /* 首行缩进 2em */
    line-height: 2; /* 行高 2 倍 */
}
```

2. **class 类选择器**：格式 `.class属性值 { 样式 }`，选中 class 等于该值的元素，以 `.` 开头。

```css
/* 类选择器 */
.title {
    color: green; /* 常见有四种颜色表示方式：关键字、rgb、rgba、十六进制 */
}
```

```html
<p class="title">类选择器文本</p>
```

3. **id 选择器**：格式 `#id属性值 { 样式 }`，选中 id 等于该值的元素，以 `#` 开头，在页面中唯一。

```css
/* id 选择器 */
#main {
    border: 1px solid #333;
}
```

```html
<div id="main"></div>
```

4. **并集选择器**：格式 `选择器1, 选择器2 { 样式 }`，多个选择器统一设置样式，用逗号分隔。

```css
/* 并集选择器 */
h1, .title {
    font-weight: bold; /* 设置文字字体粗细 */
}
```

5. **属性选择器**：根据元素是否拥有某属性、或属性值是否等于指定值来选中元素。

```css
/* 选中拥有 href 属性的 a 标签 */
a[href] {
    text-decoration: none; /* 取消超链接下划线 */
}

/* 选中 type 严格等于 password 的 input */
input[type="password"] {
    background: #f5f5f5; /* 背景颜色 */
}
```

6. **子元素选择器**：格式 `元素1 > 元素2 { 样式 }`，只选中元素 1 的**直接子元素**中的元素 2（不包含更深层后代）。

```css
/* 子元素选择器：只选中 #nav 的直接子 li，不选中嵌套更深层的 li */
#nav > li {
    display: inline-block;
}
```

7. **后代选择器**：格式 `元素1 元素2 { 样式 }`，选中元素 1 内部所有的元素 2 后代。

```css
/* 后代选择器 元素1 元素2 */
#main p {
    margin: 0 auto; /* 横向居中；4 个值表示上右下左，2 个值表示上下、左右 */
}
```

8. **伪类选择器**：格式 `选择器:伪类 { 样式 }`，选中元素的特定状态或位置，无需额外添加 class。

```css
/* 鼠标悬浮时变色 */
a:hover {
    color: red;
}

/* 输入框获得焦点时高亮 */
input:focus {
    border-color: #409eff;
}

/* 选中无序列表中的第一个 li */
ul li:first-child {
    font-weight: bold;
}

/* 选中偶数行（斑马纹效果） */
tr:nth-child(even) {
    background-color: #f5f5f5;
}
```

**完整示例**：

```html
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <title>css选择器示例</title>
    <!-- 外部样式引入：./ 当前目录（可省略），../ 上级目录 -->
    <link rel="stylesheet" href="css/style.css">
    <!-- 内部样式 -->
    <style>
        /* 元素选择器 */
        p {
            text-indent: 2em; /* 首行缩进 2em */
            line-height: 2; /* 行高 2 倍 */
        }
        /* 类选择器 */
        .title {
            color: green; /* 常见有四种颜色表示方式：关键字、rgb、rgba、十六进制 */
        }
        
        /* id 选择器 */
        #main {
            border: 1px solid #333;
        }
        /* 并集选择器 */
        h1, .title {
            font-weight: bold; /* 设置文字字体粗细 */
        }
        /* 属性选择器 */
        a[href] {
            text-decoration: none; /* 取消超链接下划线 */
        }
        input[type="password"] {
            background: #f5f5f5; /* 背景颜色 */
        }
        /* 子元素选择器：只选中直接子元素 */
        #nav > li {
            display: inline-block;
        }
        /* 伪类选择器 */
        a:hover {
            color: red;
        }
        /* 后代选择器：选中所有后代 */
        #main p {
            margin: 0 auto; /* 横向居中；4 个值表示上右下左，2 个值表示上下、左右 */
        }
    </style>
</head>
<body>
    <!-- 行内样式 -->
    <div style="margin:10px;">行内样式测试</div>

    <h1>标题</h1>
    <p class="title">类选择器文本</p>
    <div id="main">
        <p>后代选择器生效段落</p>
    </div>
    <a href="https://baidu.com">链接</a>
    <input type="password" placeholder="请输入密码">
</body>
</html>
```

## 1.3 CSS 盒子模型

### 1.3.1 什么是盒子模型

网页中所有元素都可以看作一个**矩形盒子**，由 4 部分组成：`内容区（content）`、`内边距（padding）`、`边框（border）`、`外边距（margin）`。

> 盒子总宽度 / 高度 = content + padding + border + margin

### 1.3.2 盒子模型的四大组成

1. **content 内容区**：元素实际存放文字、图片的区域，大小由 `width / height` 控制。
2. **padding 内边距**：内容与边框之间的距离，背景色会延伸到此区域。

```css
padding: 上 右 下 左;
padding: 10px;      /* 四个方向统一 */
padding: 10px 20px; /* 上下、左右 */
```

3. **border 边框**：盒子的边线，包裹在 padding 外侧。

```css
border: 宽度 样式 颜色;
border: 1px solid #000;
```

4. **margin 外边距**：盒子与其他盒子之间的距离，**背景不会显示在 margin 区域**。

> **margin 塌陷（合并）**：垂直方向相邻的块级元素，外边距会合并（取最大值，而非相加）。

### 1.3.3 两种盒子模式 box-sizing

| 模式 | 说明 | 实际宽度计算 |
| --- | --- | --- |
| `content-box`（默认） | `width / height` 仅代表 content 大小 | 实际宽 = width + padding + border |
| `border-box` | `width / height` 包含 content + padding + border | 实际宽 = width（内边距和边框向内压缩） |

> 记忆口诀：content-box 向外撑，border-box 向内挤。开发中推荐全局设置 `box-sizing: border-box;`，便于布局计算。

### 1.3.4 布局标签 div / span

想给页面划分区块、控制排版，最常用的两个标签就是 `<div>` 和 `<span>`：

- `<div>`：块级元素，独占一行，默认宽度等于父元素宽度，可设置宽高。
- `<span>`：行内元素，一行可显示多个，宽度由内容撑开，不可设置宽高。

```html
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8" />
  <meta http-equiv="X-UA-Compatible" content="IE=edge" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>盒子模型</title>
  <style>
    div {
      width: 200px;              /* 宽度 */
      height: 200px;             /* 高度 */
      box-sizing: border-box;    /* 指定 width/height 为盒子的宽高 */
      background-color: aquamarine; /* 背景色 */

      padding: 20px;             /* 内边距，上右下左，可简写为 padding: 20px; */
      border: 10px solid red;    /* 边框，宽度 线条类型 颜色 */
      margin: 30px;              /* 外边距，上右下左，可简写为 margin: 30px; */
    }
  </style>
</head>
<body>
  <div>A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A</div>
</body>
</html>
```

![[前端-盒子模型.png]]

## 1.4 Flex 布局

Flex 布局（又称弹性布局）是网页的主流布局方案，**可以轻松实现居中、等分、自适应排列**，完美替代传统的浮动与 margin 布局，解决了传统布局高度塌陷、对齐困难等问题。只需给父元素设置 `display: flex;` 即可开启弹性布局，子元素会自动成为弹性项。

Flex 的所有属性都作用于开启 `display: flex` 的父盒子，用来控制子元素的整体排列规则。

### 1.4.1 布局方向 flex-direction

设置主轴方向，决定子元素的排列方向：

```css
flex-direction: row;            /* 默认值，水平方向从左到右 */
flex-direction: column;         /* 垂直方向从上到下 */
flex-direction: row-reverse;    /* 水平方向从右到左 */
flex-direction: column-reverse; /* 垂直方向从下到上 */
```

### 1.4.2 换行规则 flex-wrap

控制子元素溢出容器时是否换行，默认不换行、压缩元素：

```css
flex-wrap: nowrap;        /* 默认，不换行，子元素自动压缩 */
flex-wrap: wrap;          /* 自动换行，超出容器宽度自动换行 */
flex-wrap: wrap-reverse;  /* 反向换行 */
```

### 1.4.3 主轴对齐 justify-content

控制子元素在**主轴方向**的对齐方式（最常用的居中属性）：

```css
justify-content: flex-start;    /* 默认，主轴起点对齐 */
justify-content: flex-end;      /* 主轴终点对齐 */
justify-content: center;        /* 主轴居中对齐（水平居中） */
justify-content: space-between; /* 两端对齐，子元素间距均分，左右无间距 */
justify-content: space-around;  /* 子元素两侧间距均分，左右有间距 */
justify-content: space-evenly;  /* 所有间距完全均等 */
```

### 1.4.4 侧轴对齐 align-items

控制单行子元素在**侧轴方向**的对齐方式：

```css
align-items: stretch;    /* 默认，子元素拉伸铺满容器高度 */
align-items: flex-start; /* 侧轴起点对齐 */
align-items: flex-end;   /* 侧轴终点对齐 */
align-items: center;     /* 侧轴居中对齐（垂直居中） */
align-items: baseline;   /* 按文字基线对齐 */
```

## 1.5 表单 form

### 1.5.1 表单作用

表单用于**收集用户信息**，并将数据提交给后端服务器，核心标签是作为表单容器的 `<form>`。

```html
<form action="提交地址" method="提交方式">
  <!-- 表单控件：输入框、单选、复选、下拉、按钮等 -->
</form>
```

- `action`：提交到后端接口的地址（后端接收数据的 url）。
- `method`：提交方式，常用 `get` / `post`。
  - `get`：参数拼在 url 上，长度有限，适合查询，但不安全。
  - `post`：数据放在请求体中携带，适合提交隐私、大量数据。

### 1.5.2 表单常用控件 input

`<input>` 是单标签，**type 属性决定控件类型**。

> **重要核心规则**：表单提交时，控件必须写 **name 属性**，后端才能拿到数据；id 仅用于 label 关联、JS 获取元素。

1. **文本输入框**

```html
<input type="text" name="username" placeholder="请输入用户名">
```

`placeholder`：输入框提示文字，不属于输入值。

2. **密码框**

```html
<input type="password" name="pwd" placeholder="请输入密码">
```

3. **单选框 radio**

name 属性控制分组互斥：
- name 相同为同一组：同组单选框**互斥，只能选中一个**。
- name 不同为不同组：多组单选**相互独立**，可以同时选中多个。

```html
<!-- 性别单选：同 name，只能三选一 -->
<input type="radio" name="gender" value="man" id="man" checked>
<label for="man">男</label>

<input type="radio" name="gender" value="woman" id="woman">
<label for="woman">女</label>

<input type="radio" name="gender" value="secret" id="secret">
<label for="secret">保密</label>
```

`value`：提交给后端的真实数值。
`checked`：默认选中当前选项。

4. **复选框 checkbox**

可以多选，同一组选项 `name` 可以相同，后端会收到数组形式的数据。

```html
<input type="checkbox" name="hobby" value="game" id="game">
<label for="game">游戏</label>

<input type="checkbox" name="hobby" value="read" id="read" checked>
<label for="read">阅读</label>
```

5. **数字框 number**

```html
<input type="number" name="age">
```

6. **邮箱 email、手机号 tel**

```html
<input type="email" name="email">
<input type="tel" name="phone">
```

7. **按钮系列**

- **提交按钮 submit**：点击后自动提交 form 表单。

```html
<input type="submit" value="登录">
```

- **重置按钮 reset**：一键清空表单所有输入内容。

```html
<input type="reset" value="重置">
```

- **普通按钮 button**：不会自动提交，配合 JS 使用。

```html
<input type="button" value="点击" onclick="fn()">
```

补充：`<button>` 标签默认 `type="submit"`。

```html
<button type="submit">提交</button>
<button type="button">普通按钮</button>
```

### 1.5.3 label 标签

点击文字即可自动选中对应的输入框，扩大点击区域，提升用户体验。

**两种标准写法**

1. `for` + `id` 绑定

```html
<input type="radio" name="gender" value="male" id="male">
<label for="male">男生</label>
```

2. 嵌套写法

```html
<label>
  <input type="checkbox" name="read"> 已阅读协议
</label>
```

### 1.5.4 下拉选择框 select + option

```html
<select name="city">
  <option value="gz">广州</option>
  <option value="sz">深圳</option>
  <option value="sh" selected>上海</option>
</select>
```

`selected`：默认选中当前下拉选项。

### 1.5.5 文本域 textarea

```html
<textarea name="msg" rows="4" cols="30" placeholder="请输入留言"></textarea>
```

`rows`：显示行数；`cols`：显示字符宽度；默认支持拖动右下角缩放大小。

## 1.6 表格 table

### 1.6.1 基础标签

`table` 是表格最外层的容器；`tr` 代表表格中的一行；`td` 是普通单元格，用来存放数据内容；`th` 是表头单元格，文字默认加粗并居中。

```html
<table>
  <tr>
    <th>姓名</th>
    <th>年龄</th>
    <th>性别</th>
  </tr>
  <tr>
    <td>小明</td>
    <td>18</td>
    <td>男</td>
  </tr>
  <tr>
    <td>小红</td>
    <td>21</td>
    <td>女</td>
  </tr>
</table>
```

### 1.6.2 表格分区标签

可以把表格分成表头、主体、底部三个区域，提升代码可读性，且浏览器会优先加载表格主体 `tbody`：
- `thead`：存放表头内容。
- `tbody`：存放表格主要数据。
- `tfoot`：存放表格底部汇总信息。

```html
<table border="1">
  <thead>
    <tr>
      <th>科目</th>
      <th>分数</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Java</td>
      <td>90</td>
    </tr>
    <tr>
      <td>Python</td>
      <td>88</td>
    </tr>
  </tbody>
  <tfoot>
    <tr>
      <td>总分</td>
      <td>178</td>
    </tr>
  </tfoot>
</table>
```

### 1.6.3 合并单元格

`colspan` 用于横向跨列合并单元格；`rowspan` 用于纵向跨行合并单元格。合并单元格后，需要手动删除被合并的 `td` 标签，只保留写了合并属性的单元格。

```html
<table border="1">
  <tr>
    <td colspan="2">跨2列</td>
  </tr>
  <tr>
    <td rowspan="2">跨2行</td>
    <td>内容1</td>
  </tr>
  <tr>
    <td>内容2</td>
  </tr>
</table>
```

## 1.7 JavaScript 的引入

**JavaScript** 是一门跨平台、面向对象的脚本语言，用来控制网页行为、实现人机交互效果，主要包括以下三部分：
- **ECMAScript**：规定 JS 基础语法核心知识，包括变量、数据类型、流程控制、函数、对象等。
- **BOM**（浏览器对象模型）：用于操作浏览器本身，如页面弹窗、地址栏操作、关闭窗口等。
- **DOM**（文档对象模型）：用于操作 HTML 文档，如改变标签内的内容、字体样式等。

1. **行内引入**：直接把 JS 代码写在标签的事件属性中，点击等动作触发代码执行。不推荐大量使用，结构和代码混在一起，不方便维护。

```html
<!-- 点击按钮弹出提示 -->
<button onclick="alert('你好')">点击我</button>
```

2. **内部引入**：使用 `script` 标签把 JS 代码直接写在页面里。`script` 标签可以放在页面的 `head` 或 `body` 中，推荐放在 `body` 末尾，等页面元素加载完成后再执行 JS，避免找不到页面标签。

```html
<script>
// 这里写 js 代码
alert('内部js')
</script>
```

3. **外部引入**：新建独立的 js 文件，在 HTML 中通过 `script` 标签的 `src` 属性引入文件。注意：
- `script` 标签写了 `src` 属性后，标签内部不能再写 JS 代码，写了也不会执行。
- 外部 JS 文件中只包含 JS 代码，不包含 `<script>` 标签。
- 引入外部 JS 的 `<script>` 标签必须是双标签。

```html
<!-- 引入外部js文件 -->
<script src="test.js"></script>
```

4. **ES6 Module 模块化**：通过 `type="module"` 开启，默认延迟执行，支持 `import` 语法。

```js
// 此处以 Vue 3 引入为例
<div id="app">{{ message }}</div>

// 引入模块化的 js 时，需要设置 type="module"
<script type="module">
  import { createApp, ref } from 'https://unpkg.com/vue@3/dist/vue.esm-browser.js'

  createApp({
    setup() {
      const message = ref('Hello Vue!')
      return {
        message
      }
    }
  }).mount('#app')
</script>
```

## 1.8 JS 核心语法

### 1.8.1 变量

变量用来存储数据，使用关键字 `var`、`let`、`const` 声明变量：
- `var`：旧版本变量，可以重复声明，存在变量提升，开发中不推荐。
- `let`：可以修改变量的值，不允许重复声明，存在块级作用域。
- `const`：常量，声明之后不能修改值，必须在声明时赋值。

```js
let name = "张三";
let age = 18;
const PI = 3.14;
```

### 1.8.2 数据类型

分为简单数据类型和复杂数据类型。

**简单数据类型**

1. `string` 字符串：文本内容，使用单引号或双引号包裹。
2. `number` 数字：整数和小数。
3. `boolean` 布尔值：只有 `true` 和 `false` 两个值，代表真和假。
4. `undefined` 未定义：变量声明了但没有赋值，默认就是 `undefined`。
5. `null` 空值：表示一个空对象。

**复杂数据类型**

`object` 对象：用来存放一组相关的数据。

```js
// 对象示例：存放一个人的信息
let person = {
  name: "李四",
  age: 20,
  gender: "男",
  hello: function () {
    console.log("你好啊！");
  } // 此处为函数，后面会提及
};
// 获取对象里面的数据
console.log(person.name);
console.log(person.age);
```

### 1.8.3 JSON

**JSON（JavaScript Object Notation，JavaScript 对象表示法）** 是一种轻量级的数据交换格式，用于在网络中传输数据。目前前后端数据交互几乎都使用 JSON 格式。

JSON 的写法与 JS 对象非常相似，但有两点区别：

- JSON 中的**属性名（键）必须加双引号**。
- JSON 中**不能写函数、注释、尾逗号**。

```json
{
  "name": "李四",
  "age": 20,
  "gender": "男"
}
```

**对象与字符串之间的转换**：前后端传输数据时，数据总是以字符串形式在网络上传递，因此需要用到以下两个方法：

1. **`JSON.stringify(对象)`**：把 JS 对象转换成 JSON 字符串，用于发送数据给后端。

```js
let person = { name: "李四", age: 20 };
let jsonStr = JSON.stringify(person);
console.log(jsonStr); // {"name":"李四","age":20}
```

2. **`JSON.parse(字符串)`**：把 JSON 字符串解析成 JS 对象，用于接收后端返回的数据。

```js
let jsonStr = '{"name":"李四","age":20}';
let person = JSON.parse(jsonStr);
console.log(person.name); // 李四
console.log(person.age);  // 20
```

> 记忆：`stringify` 把对象转化成字符串（string），`parse` 把字符串解析成对象，二者互为逆操作。

### 1.8.4 输出方式

1. `console.log()`：在浏览器控制台打印内容，调试代码最常用。

```js
console.log("测试");
```

2. `alert()`：弹出提示框。

```js
alert("弹出消息");
```

3. `document.write()`：在页面输出内容。

```js
document.write("页面文字");
```

### 1.8.5 流程控制

1. `if` 判断语句

```js
if (条件) {
  // 条件成立执行代码
} else {
  // 条件不成立执行代码
}
```

2. `for` 循环

```js
for (let i = 0; i < 10; i++) {
  // 循环执行代码
}
```

### 1.8.6 函数

函数的作用：封装一段代码，实现代码复用，需要调用才会执行。

1. **普通函数**

```js
function 函数名(参数列表) {
  // 函数体代码
  return 返回值;
}
```

参数：调用函数时传入的数据，多个参数用逗号隔开。
`return`：把结果返回给调用位置，同时结束函数；没有 `return` 时，函数默认返回 `undefined`。

```js
function sayHi() {
  console.log("你好");
}
// 调用函数
sayHi();

function sum(a, b) {
  return a + b;
}
// 调用函数
let result = sum(10, 20);
console.log(result);
```

2. **匿名函数**

没有函数名的函数，一般赋值给变量使用，有两种定义方式。

通过关键字：

```js
let 变量名 = function (参数列表) {
  // 函数体代码
  return 返回值;
};
```

通过箭头：

```js
let add = (a, b) => {
  return a + b;
};
```

定义函数后，可通过变量名来调用函数：

```js
let sayHello = function () {
  console.log("匿名函数执行");
};
// 调用匿名函数
sayHello();

let calc = function (a, b) {
  return a - b;
};
let res = calc(50, 20);
console.log(res);
```

3. **对象方法**

在 JavaScript 中对象的语法格式如下：

```js
let 对象名 = {
  属性名1: 属性值1,
  属性名2: 属性值2,
  属性名3: 属性值3,
  方法名称: function (形参列表) {}
};
```

通过如下语法调用属性：

```js
对象名.属性名;
```

通过如下语法调用方法：

```js
对象名.方法名();
```

## 1.9 JS DOM 与事件监听

浏览器加载 HTML 页面后，会把页面上的所有标签转换成对象，DOM 就是用来操作这些页面元素的。通过 DOM 可以修改页面内容、修改标签属性、修改样式，还可以绑定事件。

HTML 文档被浏览器解析后，封装成的对象分为：
- **Document**：整个文档对象
- **Element**：元素对象
- **Attribute**：属性对象
- **Text**：文本对象
- **Comment**：注释对象

### 1.9.1 操作节点

1. **获取元素**

```html
<div id="box">我是盒子</div>
<ul>
  <li>列表1</li>
  <li>列表2</li>
</ul>
<div class="item">项目</div>
<div class="box">第一个盒子</div>
```

- 根据 id 获取元素

```js
// 获取页面上 id="box" 的 div
let box = document.getElementById("box");
```

- 根据标签名获取元素

```js
// 获取页面上所有 li 标签，得到集合
let lis = document.getElementsByTagName("li");
```

- 根据类名获取元素

```js
// 获取页面上 class 为 item 的元素集合
let items = document.getElementsByClassName("item");
```

- querySelector 选择器获取

```js
// 获取页面上 class 为 box 的第一个元素
let one = document.querySelector(".box");
```

- querySelectorAll

```js
// 获取页面上全部 li 元素，返回集合
let all = document.querySelectorAll("li");
```

2. **操作元素内容**

`innerText` 获取或设置元素里面的纯文本，不识别 HTML 标签；`innerHTML` 获取或设置内容，可以识别 HTML 标签。

```js
// 获取页面中的 div
let div = document.querySelector("div");
// 获取文本
console.log(div.innerText);
// 修改文本
div.innerText = "新文字";

// 修改内容，可解析标签
div.innerHTML = "<b>加粗文字</b>";
```

3. **操作元素属性**

- **普通属性**

```html
<input type="text" value="初始内容">
```

```js
// 获取页面上的 input 标签
let input = document.querySelector("input");
// 获取 value 属性
console.log(input.value);
// 修改 value 属性
input.value = "新内容";
```

- **自定义属性**

```html
<div data-id="5"></div>
```

`getAttribute` 获取自定义属性；`setAttribute` 设置自定义属性；`removeAttribute` 删除属性。

```js
// 获取页面中的 div
let div = document.querySelector("div");
// 获取自定义属性 data-id
let val = div.getAttribute("data-id");
// 设置自定义属性 data-id
div.setAttribute("data-id", 10);
// 删除 data-id 属性
div.removeAttribute("data-id");
```

4. **操作元素样式**

```html
<div class="box"></div>
```

- **通过 style 修改行内样式**

```js
// 获取 class 为 box 的 div
let box = document.querySelector(".box");
box.style.width = "200px";
box.style.backgroundColor = "red";
```

CSS 里面的短横线样式，在 JS 中要改为驼峰命名，例如 `background-color` 写成 `backgroundColor`。

- **操作类名 className，修改多个样式**

```js
// 获取 class 为 box 的 div
let box = document.querySelector(".box");
box.className = "active";
```

### 1.9.2 事件监听

事件就是用户的操作动作，例如点击、鼠标移入。

事件三要素：事件源、事件类型、事件处理函数。
- **事件源**：哪个元素触发事件。
- **事件类型**：什么动作，比如 `click` 点击。
- **事件处理函数**：触发之后执行的代码。

JavaScript 一共有三种事件绑定方式。

1. **addEventListener 事件监听**

使用 `addEventListener` 绑定事件，可以给同一个元素的同一个事件绑定多个处理函数，不会覆盖。格式：`addEventListener('click', 函数)`。

```html
<body>
  <input type="button" id="btn1" value="点我一下试试1">
  <script>
    let btn = document.querySelector('#btn1');
    btn.addEventListener('click', function () {
      alert("使用addEventListener绑定点击事件");
    });
    // 同一个 click 事件可以绑定多个函数，全部都会执行
    btn.addEventListener('click', function () {
      console.log("第二个点击事件");
    });
  </script>
</body>
```

2. **onclick 属性**

在 HTML 标签上直接写 `onclick` 属性，属性值调用函数。

```html
<input type="button" id="btn1" value="点我一下试试1" onclick="on()">
<script>
  function on() {
    alert('试试就试试');
  }
</script>
```

3. **DOM 元素对象事件属性绑定**

先获取 DOM 元素，再给元素的 `onclick` 属性赋值函数。

```html
<body>
  <input type="button" id="btn1" value="点我一下试试1">
  <script>
    document.querySelector('#btn1').onclick = function () {
      alert("按钮2被点击了...");
    };
  </script>
</body>
```

# 二、Vue 基础

Vue 是一款用于构建用户界面的渐进式 JavaScript 框架。它基于标准 HTML、CSS 和 JavaScript 构建，并提供了一套声明式的、组件化的编程模型，帮助你高效地开发用户界面。无论是简单还是复杂的界面，Vue 都可以胜任。

![[Vue-渐进式.png]]

## 2.1 快速入门

```html
<!doctype html>
<html lang="zh-CN">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Vue-快速入门</title>
  </head>
  <body>
    <div id="app">
      <h1>{{ message }}</h1>
    </div>
   
    <script type="module">
      import { createApp } from "https://unpkg.com/vue@3/dist/vue.esm-browser.js";
      
      createApp({
        data() {
          return {
            message: "Hello,Vue!",
          };
        },
      }).mount("#app");
    </script>
  </body>
</html>
```

1. **HTML 部分**

```html
<div id="app">
  <h1>{{ message }}</h1>
</div>
```

- **挂载根容器**：Vue 应用会接管这个 DOM 节点，容器外的内容 Vue 不会处理。
- `{{ message }}`：**插值表达式**，把 Vue 实例里 `data` 中的 `message` 变量渲染到页面。

2. **Vue 引入部分**

通过 CDN 引入 Vue 3 的完整写法：

```html
<script type="module">
      import { createApp } from "https://unpkg.com/vue@3/dist/vue.esm-browser.js";
    </script>
```

- `type="module"`：开启浏览器原生 ESModule 能力。
- `https://unpkg.com/vue@3/dist/vue.esm-browser.js`：CDN 地址，直接从网络加载 Vue 3 的 ESM 浏览器版。
- `import { createApp }`：解构导入 Vue 提供的 `createApp` 函数，用来创建 Vue 应用实例。

3. **createApp 创建应用**

```js
createApp({
	data() {
	  return {
		message: "Hello,Vue!",
	  };
	},
}).mount("#app");
```

- `createApp(选项对象)`：接收一个组件配置对象，并返回应用实例。
- `data` 只是配置对象里的属性名：Vue 会读取选项对象上名为 `data` 的属性，拿到里面存的函数并调用它，函数的返回值就是页面渲染时使用的数据。

```js
// 这是 ES5 完整写法，`data() {}` 只是 ES6 对象方法简写语法糖，两者完全等价。
createApp({
	data: function () {
	  return {
	   message: "Hello,Vue!" 
	   };
	},
}).mount("#app");
```

`.mount(选择器/DOM元素)`：挂载，把 Vue 应用关联到页面上的 DOM 节点，挂载后 Vue 才开始接管该容器内部的内容。

## 2.2 常见指令

指令是带有 `v-` 前缀的特殊 attribute。Vue 提供了许多[内置指令](https://cn.vuejs.org/api/built-in-directives.html)，不同指令具有不同含义，可以实现不同的功能。

| 常见指令                  | 作用                             |
| --------------------- | ------------------------------ |
| v-for                 | 列表渲染，遍历容器的元素或者对象的属性            |
| v-bind                | 为HTML标签绑定属性值，如设置 href , css样式等 |
| v-if/v-else-if/v-else | 条件性的渲染某元素，判定为true时渲染,否则不渲染     |
| v-show                | 根据条件展示某元素，区别在于切换的是display属性的值  |
| v-model               | 在表单元素上创建双向数据绑定                 |
| v-on                  | 为HTML标签绑定事件                    |

在这里你可以直观地看到完整的指令语法：

![[Vue-语法格式.png]]

### 2.2.1 v-for

`v-for` **用于列表渲染，可以遍历数组元素或对象属性**。指令值必须遵循 `alias in expression` 语法，为迭代元素定义别名：

```html
<tr v-for="item in items" :key="item.id">{{item}}</tr>
```

也可以额外指定索引别名（遍历对象时，第二个参数为属性键名）：

```html
<tr v-for="(item,index) in items" :key="item.id">{{item}}</tr>
```

`:key`：给每一项元素绑定唯一标识，帮助 Vue 正确复用、排序 DOM 节点，提升列表渲染性能。

> 推荐使用 id 作为 key（唯一），不推荐使用 index 作为 key（会变化，不对应）

```html
<!doctype html>
<html lang="zh-CN">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>v-for 示例</title>
  </head>
  <body>
    <div id="app">
      <h3>遍历数组，获取每一项 item</h3>
      <table>
        <tr>
          <th>名称</th>
        </tr>
        <tr v-for="item in list">
          <td>{{ item.name }}</td>
        </tr>
      </table>
      
      <h3>遍历数组，同时获取 item 与索引 index</h3>
      <table>
        <tr>
          <th>索引</th>
          <th>名称</th>
        </tr>
        <tr v-for="(item, index) in list">
          <td>{{ index }}</td>
          <td>{{ item.name }}</td>
        </tr>
      </table>
      
      <h3>遍历对象，(值,键)</h3>
      <div v-for="(value, key) in userInfo">{{ key }}：{{ value }}</div>
    </div>

    <script type="module">
      import { createApp } from "https://unpkg.com/vue@3/dist/vue.esm-browser.js";
      createApp({
        data() {
          return {
            list: [
              { id: 1, name: "Vue学习" },
              { id: 2, name: "HTML" },
              { id: 3, name: "JavaScript" },
            ],
            userInfo: {
              name: "小明",
              age: 18,
              gender: "男",
            },
          };
        },
      }).mount("#app");
    </script>
  </body>
</html>
```

### 2.2.2 v-bind

`v-bind` 用于**动态绑定 HTML 标签属性**，将 Vue 中的响应式数据关联到标签属性上，常用于 `src`、`href`、class、style 等属性。`v-bind:属性名` 可简写为 `:属性名`。

```html
<!-- 基础属性绑定 -->
<img v-bind:src="imageSrc" />
<!-- 简写（最常用） -->
<img :src="imageSrc" />

<!-- 动态属性名（属性名本身是变量） -->
<button :[key]="value"></button>

<!-- class绑定：对象语法，控制类名是否生效 -->
<div :class="{ red: isRed }"></div>
<!-- class绑定：数组语法，应用多个类名 -->
<div :class="[classA, classB]"></div>

<!-- style绑定，动态设置行内样式 -->
<div :style="{ fontSize: size + 'px' }"></div>

<!-- 一次性绑定多个属性 -->
<div v-bind="{ id: someProp }"></div>

<!-- 组件传参，向子组件传递prop数据 -->
<MyComponent :prop="someThing" />
```

### 2.2.3 v-if & v-show

`v-if` 和 `v-show` 都可以控制元素**显示 / 隐藏**，但底层实现原理不同。

- **v-if**：基于表达式值的真假性，直接对 DOM 节点进行创建或销毁。可以配合 v-else-if / v-else 进行链式条件判断。
- **v-else-if**：必须出现在 v-if 之后，可以出现多个。
- **v-else**：必须出现在 v-if / v-else-if 之后。
- **v-show**：基于表达式值的真假性，切换 CSS 的 display 属性，DOM 始终存在页面中。

> 运行时条件很少改变，使用 `v-if`；需要频繁切换显示隐藏，使用 `v-show`。

```html
<!doctype html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>v-if 示例</title>
  <style>
    [v-cloak] { display: none; }
  </style>
</head>
<body>
  <div id="app" v-cloak>
    <h3>基础 v-if</h3>
    <div v-if="flag">条件成立</div>

    <h3>v-if / v-else 分支</h3>
    <div v-if="isLogin">欢迎回来！</div>
    <div v-else>请登录</div>

    <h3>v-if / v-else-if / v-else 多分支</h3>
    <div v-if="score >= 90">优秀</div>
    <div v-else-if="score >= 60">及格</div>
    <div v-else>不及格</div>
  </div>

  <script type="module">
    import { createApp } from "https://unpkg.com/vue@3/dist/vue.esm-browser.js";
    createApp({
      data() {
        return {
          flag: true,
          isLogin: false,
          score: 85
        }
      }
    }).mount("#app")
  </script>
</body>
</html>
```

### 2.2.4 v-model

在表单输入元素或组件上创建双向绑定，可以方便地**获取**或**设置**表单项数据。

> 本质语法糖：v-model = :value + @input

```html
<!-- 原生输入框 -->
<input v-model="msg" />
<!-- 等价写法 -->
<input :value="msg" @input="msg = $event.target.value" />
```

**修饰符**

- `.lazy`：失去焦点后更新数据
- `.number`：自动转为数字类型
- `.trim`：移除输入内容两端空格

### 2.2.5 v-on

**绑定事件监听**，给元素绑定 DOM 事件，触发后执行 JS 代码，可使用 @ 简写。

**事件修饰符**

- `.prevent`：阻止默认行为
- `.stop`：阻止事件冒泡
- `.once`：事件只触发一次
- `.self`：仅点击元素自身才触发

以下为 `v-model` 拿表单数据，`v-on` 绑定提交事件的示例：

```html
<template>
  <div id="app">
    <h3>登录表单</h3>
    <!-- v-on绑定表单提交事件 -->
    <form @submit.prevent="onLogin">
      <div>
        账号：<input v-model="account" placeholder="请输入账号">
      </div>
      <div>
        密码：<input v-model="password" type="password">
      </div>
      <div>
        <input type="checkbox" v-model="remember">记住密码
      </div>
      <button type="submit">登录</button>
    </form>
  </div>
</template>

<script>
export default {
  data() {
    return {
      // v-model绑定的响应式数据
      account: '',
      password: '',
      remember: false
    }
  },
  methods: {
    // v-on触发的登录方法
    onLogin() {
      console.log('账号', this.account)
      console.log('密码', this.password)
      console.log('记住密码', this.remember)
      if (!this.account || !this.password) {
        alert('账号密码不能为空')
        return
      }
      alert('登录成功！')
    }
  }
}
</script>
```

## 2.3 生命周期

Vue 的生命周期包含 8 个阶段：每触发一个生命周期事件，会自动执行一个生命周期方法，这些生命周期方法也被称为钩子方法。其完整的生命周期如下图所示：

| 状态            | 阶段周期 |
| ------------- | ---- |
| beforeCreate  | 创建前  |
| created       | 创建后  |
| beforeMount   | 挂载前  |
| mounted       | 挂载完成 |
| beforeUpdate  | 更新前  |
| updated       | 更新后  |
| beforeDestroy | 销毁前  |
| destroyed     | 销毁后  |

![[Vue-生命周期.png]]

对于 JavaWeb，只重点掌握 `mounted`，其余生命周期了解即可。

**mounted**：挂载完成，Vue 实例初始化完毕，HTML 页面渲染成功。

> 页面初始化时自动发送 ajax 请求，从后台加载数据

```js
methods: {
  // 查询员工列表
  async search() {
    // 基于axios发送GET异步请求，带查询条件获取员工列表
    const result = await axios.get(`https://web-server.itheima.net/emps/list?name=${this.searchForm.name}&gender=${this.searchForm.gender}&job=${this.searchForm.job}`);
    // 将返回的数据赋值给响应式变量，渲染到页面
    this.empList = result.data.data;
  },
  // 清空查询条件，并重新查询
  clear() {
    this.searchForm = {
      name: '',
      gender: '',
      job: ''
    }
    this.search();
  }
},
mounted() {
  // 页面挂载完成，自动执行查询，页面一打开就加载员工数据
  this.search();
}
}).mount('#container')
```

# 三、Vue 工程化
## 3.1 Vue 项目创建

### 3.1.1 工程化介绍

在之前的学习中，我们直接在项目里引入 `vue.js`、`axios.js` 这类资源文件就能使用，一个 HTML 文件写完所有代码。这种方式上手快、无需配置，但到了企业级项目，规模大、参与人员多、交付周期短，如果还靠"一个文件写到底"，代码会迅速变得难以维护。因此，企业开发更偏向采用**前端工程化**的方式，它主要有以下四个特点：

![[Pasted image 20260918155600.png]]

- **模块化**：将 JS、CSS 等拆分成一个个职责单一、可复用的模块，模块之间互不干扰。
- **组件化**：把 UI 组件、CSS 样式、JS 行为封装成一个个组件，方便复用与统一管理。
- **规范化**：提供一套统一的项目目录结构、编码规范与开发流程，所有开发人员共同遵守。
- **自动化**：项目的构建、测试、部署等环节全部自动完成，减少人工操作。

简单来说，前端工程化就是把企业级前端开发所需的工具、技术、流程、经验进行规范化和标准化：人人遵循同一套规范，项目才能持续演进、长期维护。

要快速搭建一个工程化项目，最省事的方式是使用 Vue 官方提供的**脚手架（create-vue）**。脚手架会自动生成标准目录结构、配置文件，并预装开发服务器与构建工具（Vite），我们只需专注于业务代码的编写。

### 3.1.2 项目构建过程

创建工程化项目前，先确保电脑上安装了 **Node.js**（内置 npm 包管理工具，建议安装长期支持版 LTS）。使用 npm 命令即可构建一个 Vue 工程化项目：

```bash
npm create vue@latest
```

执行过程中会出现一系列交互式提问，例如项目名称、是否安装 TypeScript / Router / Pinia 等，初学者全部选择默认值（No）即可。项目创建完成后，进入项目目录并安装依赖：

```bash
cd 项目名称
npm install
```

依赖安装完成后，就可以启动项目了，启动方式有两种：

**（1）命令行启动**

在项目根目录执行命令：

```bash
npm run dev
```

**（2）VSCode 图形化启动**

用 VSCode 打开项目文件夹，定位到根目录的 `package.json` 文件，在 **NPM 脚本**面板中找到 `dev` 脚本，运行脚本即可启动项目。

> `npm run dev` 是开发环境启动命令，启动后默认访问地址为 `http://localhost:5173` ；项目上线前还需执行 ` npm run build ` 打包生产版本。

### 3.1.3 项目目录结构

创建好的项目目录结构如下（具体内容会因创建时的选项略有差异）：

| 路径 / 文件           | 作用                             |
| ----------------- | ------------------------------ |
| `index.html`      | 项目入口页面，Vue 应用最终挂载到此页面上的节点      |
| `package.json`    | 项目配置文件，记录依赖包与脚本命令（dev / build） |
| `vite.config.js`  | Vite 构建工具的配置文件（开发服务器端口、代理等）    |
| `src/main.js`     | 项目入口 JS，创建 Vue 应用实例并挂载到页面      |
| `src/App.vue`     | 根组件，相当于应用的整体骨架，内部组装其他组件        |
| `src/components/` | 存放公共子组件（弹窗、表格、分页等）             |
| `src/assets/`     | 存放静态资源（图片、图标、公共样式等）            |
| `src/router/`     | 路由配置（创建项目时选择了 Router 才会生成）     |

日常开发主要工作在 `src` 目录下。以入口文件 `main.js` 为例：

```js
// src/main.js
import { createApp } from 'vue'
import App from './App.vue'

createApp(App).mount('#app')
```

`createApp(App)` 以根组件 `App.vue` 创建应用实例，`.mount('#app')` 将其挂载到 `index.html` 中 `id="app"` 的节点上。整个过程与第二部分学习的 CDN 用法完全一致，只是引入方式从"线上加载"变成了"从本地文件导入"。

### 3.1.4 项目开发流程
![[Pasted image 20260918160749.png]]

其中 `*.vue` 是 Vue 项目中的组件文件，也叫**单文件组件**（SFC，Single-File Components，详见[官方文档](https://cn.vuejs.org/guide/scaling-up/sfc.html)）。SFC 会将一个组件的逻辑（JS）、模板（HTML）和样式（CSS）封装在同一个 `*.vue` 文件中：

![[Pasted image 20260918160832.png]]

一个 SFC 文件通常由三个部分组成：

- `<template>`：组件的模板（HTML 结构），决定页面长什么样。
- `<script setup>`：组件的逻辑（JS），定义响应式数据与事件处理函数。
- `<style scoped>`：组件的样式（CSS），`scoped` 表示样式只对当前组件生效，不会污染其他组件。

组件定义好后，通过 `import` 引入即可像普通标签一样直接使用：

```html
<template>
  <div>
    <Header />
    <EmployeeTable />
  </div>
</template>

<script setup>
import Header from './components/Header.vue'
import EmployeeTable from './components/EmployeeTable.vue'
</script>
```

整个项目的运行流程：浏览器加载 `index.html` → `main.js` 创建应用并挂载到页面 → 渲染根组件 `App.vue` → 逐层渲染嵌套的子组件。

## 3.2 API 风格

Vue 组件的编写有两种不同的风格：**组合式 API（Composition API）** 和 **选项式 API（Options API）**。两种风格实现的是同一套功能，选择哪种取决于个人偏好与项目场景。

**（1）组合式 API**

组合式 API 是 Vue 3 提供的一种基于函数的组件编写方式，通过使用函数来组织和复用组件的逻辑，代码更灵活、更易组合，是 Vue 3 新项目的推荐写法。与选项式 API 不同的是，组合式 API 中**没有 `this`**，数据与函数都是直接声明、直接使用：

```html
<script setup>
import { ref, onMounted } from 'vue';
const count = ref(0); //声明响应式变量

function increment(){ //声明函数
   count.value++;
}

onMounted(() => { //声明钩子函数
  console.log('Vue Mounted....'); 
})
</script>

<template>
   <input type="button" @click="increment"> Api Demo1 Count : {{ count }}
</template>

<style scoped>
   
</style>
```

- `<script setup>`：`setup` 是一个编译标识，告诉 Vue 需要对这段代码做特殊处理，让我们可以在 `<script>` 中直接声明变量、函数并使用，这是组合式 API 的推荐写法。
- `ref()`：接收一个内部值，返回一个响应式的 **ref 对象**，该对象只有一个指向内部值的属性 `value`。在模板中使用时会**自动解包**，直接写 `count` 即可；在 JS 中操作时必须通过 `.value`**。
- `onMounted()`：组合式 API 中的生命周期钩子，注册一个回调函数，在组件挂载完成后执行。

**（2）选项式 API**

选项式 API 用一个包含多个选项的对象来描述组件的逻辑，如 `data`、`methods`、`mounted` 等。选项对象中定义的属性都会暴露在函数内部的 `this` 上，`this` 指向当前的组件实例，因此访问数据与方法都要通过 `this`：

```html
<script>
export default{
   data() {
      return {
         count: 0
      }
   },
   methods: {
      increment: function(){
         this.count++
      }
   },
   mounted() {
      console.log('vue mounted.....');
   }
}
</script>

<template>
  <input type="button" @click="increment">Api Demo1 Count :  {{ count }}
</template>

<style scoped>

</style>
```

> 记住：组合式 API 中没有 `this` 对象，数据与函数直接声明、直接使用；选项式 API 中 `this` 指向当前组件实例，通过 `this.xxx` 访问。新项目开发统一推荐使用**组合式 API**。

**（3）两种风格对比**

| 对比项 | 组合式 API | 选项式 API |
| --- | --- | --- |
| 代码组织 | 按逻辑功能组织，相关代码聚在一起 | 按 data / methods 等选项分类分区 |
| this 对象 | 没有 `this`，直接使用变量和函数 | 依赖 `this` 访问数据与方法 |
| 逻辑复用 | 容易，可封装为组合式函数 | 困难，只能借助 mixin |
| 适用场景 | Vue 3 新项目推荐使用 | 旧项目迁移、简单组件 |

## 3.3 Element Plus 组件库

### 3.3.1 什么是 Element Plus

实际开发中，页面上的表单、表格、弹窗、消息提示等 UI 组件如果全部手写，工作量巨大且风格难以统一。**Element Plus 是一套基于 Vue 3 的桌面端组件库**，将常用组件统一封装好，拿来即用，是 Vue 3 生态中最流行的组件库之一。

官方文档：https://element-plus.org

### 3.3.2 安装与引入

在项目根目录安装 Element Plus：

```bash
npm install element-plus
```

安装完成后，在入口文件 `main.js` 中**完整引入**：

```js
// src/main.js
import { createApp } from 'vue'
import ElementPlus from 'element-plus'
import 'element-plus/dist/index.css'
import App from './App.vue'

const app = createApp(App)
app.use(ElementPlus)   // 注册全部 Element Plus 组件
app.mount('#app')
```

> 完整引入会把所有组件一次性注册，简单方便，适合学习阶段；企业项目为减小打包体积，常用 unplugin-vue-components 实现按需自动导入，这里暂不展开。

引入完成后，就可以在任意组件中像使用普通标签一样使用 `el-` 开头的组件了。

### 3.3.3 常用组件

1. **按钮 el-button**

`type` 属性控制按钮的颜色风格：`primary`（主要）、`success`（成功）、`warning`（警告）、`danger`（危险）等。

```html
<el-button type="primary">主要按钮</el-button>
<el-button type="success">成功按钮</el-button>
<el-button type="danger">删除</el-button>
<el-button>默认按钮</el-button>
```

2. **表单 el-form + el-input**

`el-form` 是表单容器，`el-form-item` 表示一行表单项，`el-input` 是输入框，配合 `v-model` 完成数据的收集：

```html
<el-form :model="loginForm" label-width="80px">
  <el-form-item label="用户名">
    <el-input v-model="loginForm.username" placeholder="请输入用户名" />
  </el-form-item>
  <el-form-item label="密码">
    <el-input v-model="loginForm.password" type="password" show-password />
  </el-form-item>
  <el-form-item>
    <el-button type="primary" @click="onLogin">登录</el-button>
  </el-form-item>
</el-form>
```

表单数据用 `ref` 声明，提交时取 `loginForm.value` 即可。

3. **表格 el-table**

```html
<el-table :data="empList" border stripe>
  <el-table-column prop="name" label="姓名" width="120" />
  <el-table-column prop="age" label="年龄" width="120" />
  <el-table-column prop="job" label="职位" />
</el-table>
```

- `:data`：绑定表格数据源（数组）。
- `prop`：对应数据对象中的属性名；`label`：表头显示的文字；`width`：列宽。
- `border` 显示边框线，`stripe` 开启斑马纹。

4. **对话框 el-dialog**

```html
<el-dialog v-model="dialogVisible" title="新增员工" width="500px">
  <!-- 弹窗内部内容 -->
</el-dialog>
```

`v-model` 绑定一个布尔值：为 `true` 时显示弹窗，`false` 时关闭。

5. **分页 el-pagination**

```html
<el-pagination
  v-model:current-page="page"
  v-model:page-size="pageSize"
  :total="total"
  layout="prev, pager, next"
  @current-change="search"
/>
```

- `total`：总数据条数；`current-page`：当前页码；`page-size`：每页条数。
- `@current-change`：页码变化时触发的事件，一般在这里重新发起查询。

6. **消息提示 ElMessage**

```js
import { ElMessage } from 'element-plus'

ElMessage.success('操作成功')
ElMessage.error('操作失败，请重试')
```

> 注意：`ElMessage` 是函数调用方式（不是标签），需要先 `import` 再使用。

## 3.4 axios 请求库

### 3.4.1 什么是 axios

工程化项目中，页面数据几乎都来自后端接口。**axios 是一个基于 Promise 的 HTTP 请求库**，用于在浏览器中发送异步请求，是 Vue 项目中最常用的请求方案。

在项目根目录安装：

```bash
npm install axios
```

### 3.4.2 基本用法

在组件中引入后即可发起请求：

```js
import axios from 'axios'

// GET 请求：参数拼接在 URL 上
const res = await axios.get('https://web-server.itheima.net/emps/list?name=张&gender=1')

// POST 请求：数据放在请求体中
const res2 = await axios.post('https://web-server.itheima.net/emps/save', {
  name: '张三',
  gender: 1
})

console.log(res.data)   // res.data 是后端返回的响应数据
```

> 工程项目的接口返回数据通常封装为 `{ code, message, data }` 结构，后端返回的业务数据在 `res.data.data` 中（与 2.3 生命周期章节里的写法一致）。

> 前后端联调时，前端（5173 端口）与后端（如 8080 端口）端口不同会产生**跨域**问题，需要在 `vite.config.js` 中配置代理转发，等后面做完整项目案例时再展开。

### 3.4.3 综合示例：员工列表查询

把 axios 与 Element Plus 组合使用——页面加载后自动请求接口，将数据渲染到表格中：

```html
<script setup>
import { ref, onMounted } from 'vue'
import axios from 'axios'

const empList = ref([])   // 员工列表数据

// 查询员工列表
async function search() {
  const res = await axios.get('https://web-server.itheima.net/emps/list')
  empList.value = res.data.data
}

onMounted(() => {
  search()   // 页面挂载完成，自动加载数据
})
</script>

<template>
  <el-table :data="empList" border stripe>
    <el-table-column prop="name" label="姓名" />
    <el-table-column prop="gender" label="性别" />
    <el-table-column prop="job" label="职位" />
  </el-table>
</template>
```

至此，Vue 工程化的完整开发流程就打通了：**脚手架创建项目 → 编写 SFC 组件 → 引入 Element Plus 搭建界面 → 用 axios 请求后端数据并渲染展示**。