# 前端部分

Web 标准（也称网页标准）是由一系列标准组成的规范体系，大部分由 W3C（World Wide Web Consortium，万维网联盟）负责制定，主要由以下三个部分组成：

- **结构层（HTML）**：负责网页的内容结构，定义页面上有什么（标题、段落、图片、表格等）。
- **表现层（CSS）**：负责网页的样式展示，控制外观（颜色、字体、布局、间距等）。
- **行为层（JavaScript）**：负责网页的交互行为，实现动态效果、响应用户操作。

## 一、前端三件套

### 1.1 HTML 页面结构与常用标签

#### 1.1.1 HTML 标准骨架

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

- `<head>`：网页头部，存放浏览器解析所需的元信息（CSS 样式、页面标题、字符编码等），对用户不可见。
- `<body>`：网页主体，存放展示给用户的所有内容（文字、图片、视频、表单等）。
- `<title>`：定义浏览器标签栏显示的页面标题。

#### 1.1.2 常用核心标签

##### 1. 文本类标签

| 标签                 | 作用   | 说明                     |
| ------------------ | ---- | ---------------------- |
| `<h1>~<h6>`        | 标题标签 | 一级到六级，权重依次降低           |
| `<p>`              | 段落标签 | 自带上下外边距                |
| `<br>`             | 换行标签 | 单标签，强制文本换行             |
| `<strong>` / `<b>` | 文本加粗 | `<strong>` 语义权重更高，推荐使用 |
| `<em>` / `<i>`     | 文本倾斜 | `<em>` 语义权重更高，推荐使用     |
| `<ins>` / `<u>`    | 下划线  | `<ins>` 语义权重更高         |
| `<del>` / `<s>`    | 删除线  | `<del>` 语义权重更高         |

##### 2. 媒体与链接标签

- **图片标签 `<img>`**

```html
<img src="图片路径" alt="加载失败提示" width="宽度" height="高度" title="鼠标悬浮提示">
```

- **视频标签 `<video>`**

```html
<video src="视频地址" controls autoplay muted loop width="宽度"></video>
```

`controls`：显示播放控件；`autoplay`：自动播放；`muted`：静音；`loop`：循环播放。

- **超链接标签 `<a>`**

```html
<a href="链接地址" target="_blank" title="悬浮提示">链接文本</a>
```

`target="_blank"`：在新窗口打开链接；`target="_self"`：在当前页面打开（默认）。

##### 3. 容器与列表标签

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

##### 4. 表格与注释

```html
<!-- 这是注释，浏览器不渲染 -->
<input type="text" name="名称" value="默认值" placeholder="提示文字">
```

#### 1.1.3 HTML 字符实体

特殊符号无法被浏览器直接解析，需要使用字符实体来替代：

| 符号  | 字符实体     | 说明         |
| --- | -------- | ---------- |
| 空格  | `&nbsp;` | 不换行空格      |
| <   | `&lt;`   | 小于号 / 左尖括号 |
| >   | `&gt;`   | 大于号 / 右尖括号 |

### 1.2 CSS 的引入与选择器

#### 1.2.1 三种引入方式

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

#### 1.2.2 CSS 选择器

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

5. **属性选择器（有该属性）**：格式 `元素名称[属性] { 样式 }`，选中拥有该属性的对应元素。

```css
/* 元素[属性] */
a[href] {
    text-decoration: none; /* 取消超链接下划线 */
}
```

6. **属性选择器（属性等于指定值）**：格式 `元素名称[属性名="值"] { 样式 }`，选中属性值严格等于指定内容的元素。

```css
/* 元素[属性="值"] */
input[type="password"] {
    background: #f5f5f5; /* 背景颜色 */
}
```

7. **后代选择器**：格式 `元素1 元素2 { 样式 }`，选中元素 1 内部所有的元素 2 后代。

```css
/* 后代选择器 元素1 元素2 */
#main p {
    margin: 0 auto; /* 横向居中；4 个值表示上右下左，2 个值表示上下、左右 */
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
        /* 元素[属性] */
        a[href] {
            text-decoration: none; /* 取消超链接下划线 */
        }
        /* 元素[属性="值"] */
        input[type="password"] {
            background: #f5f5f5; /* 背景颜色 */
        }
        /* 后代选择器 元素1 元素2 */
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

### 1.3 CSS 盒子模型

#### 1.3.1 什么是盒子模型

网页中所有元素都可以看作一个**矩形盒子**，由 4 部分组成：`内容区（content）`、`内边距（padding）`、`边框（border）`、`外边距（margin）`。

> 盒子总宽度 / 高度 = content + padding + border + margin

#### 1.3.2 盒子模型的四大组成

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

#### 1.3.3 两种盒子模式 box-sizing

| 模式 | 说明 | 实际宽度计算 |
| --- | --- | --- |
| `content-box`（默认） | `width / height` 仅代表 content 大小 | 实际宽 = width + padding + border |
| `border-box` | `width / height` 包含 content + padding + border | 实际宽 = width（内边距和边框向内压缩） |

> 记忆口诀：content-box 向外撑，border-box 向内挤。开发中推荐全局设置 `box-sizing: border-box;`，便于布局计算。

#### 1.3.4 布局标签 div / span

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

### 1.4 Flex 布局

Flex 布局（又称弹性布局）是网页的主流布局方案，**可以轻松实现居中、等分、自适应排列**，完美替代传统的浮动与 margin 布局，解决了传统布局高度塌陷、对齐困难等问题。只需给父元素设置 `display: flex;` 即可开启弹性布局，子元素会自动成为弹性项。

Flex 的所有属性都作用于开启 `display: flex` 的父盒子，用来控制子元素的整体排列规则。

#### 1.4.1 布局方向 flex-direction

设置主轴方向，决定子元素的排列方向：

```css
flex-direction: row;            /* 默认值，水平方向从左到右 */
flex-direction: column;         /* 垂直方向从上到下 */
flex-direction: row-reverse;    /* 水平方向从右到左 */
flex-direction: column-reverse; /* 垂直方向从下到上 */
```

#### 1.4.2 换行规则 flex-wrap

控制子元素溢出容器时是否换行，默认不换行、压缩元素：

```css
flex-wrap: nowrap;        /* 默认，不换行，子元素自动压缩 */
flex-wrap: wrap;          /* 自动换行，超出容器宽度自动换行 */
flex-wrap: wrap-reverse;  /* 反向换行 */
```

#### 1.4.3 主轴对齐 justify-content

控制子元素在**主轴方向**的对齐方式（最常用的居中属性）：

```css
justify-content: flex-start;    /* 默认，主轴起点对齐 */
justify-content: flex-end;      /* 主轴终点对齐 */
justify-content: center;        /* 主轴居中对齐（水平居中） */
justify-content: space-between; /* 两端对齐，子元素间距均分，左右无间距 */
justify-content: space-around;  /* 子元素两侧间距均分，左右有间距 */
justify-content: space-evenly;  /* 所有间距完全均等 */
```

#### 1.4.4 侧轴对齐 align-items

控制单行子元素在**侧轴方向**的对齐方式：

```css
align-items: stretch;    /* 默认，子元素拉伸铺满容器高度 */
align-items: flex-start; /* 侧轴起点对齐 */
align-items: flex-end;   /* 侧轴终点对齐 */
align-items: center;     /* 侧轴居中对齐（垂直居中） */
align-items: baseline;   /* 按文字基线对齐 */
```

### 1.5 表单 form

#### 1.5.1 表单作用

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

#### 1.5.2 表单常用控件 input

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
- **name 相同为同一组**：同组单选框**互斥，只能选中一个**。
- **name 不同为不同组**：多组单选**相互独立**，可以同时选中多个。
- `value`：提交给后端的真实数值。
- `checked`：默认选中当前选项。

```html
<!-- 性别单选：同 name，只能三选一 -->
<input type="radio" name="gender" value="man" id="man" checked>
<label for="man">男</label>

<input type="radio" name="gender" value="woman" id="woman">
<label for="woman">女</label>

<input type="radio" name="gender" value="secret" id="secret">
<label for="secret">保密</label>
```

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

#### 1.5.3 label 标签

点击文字即可自动选中对应的输入框，扩大点击区域，提升用户体验。

**两种标准写法**

方式 1：`for` + `id` 绑定

```html
<input type="radio" name="gender" value="male" id="male">
<label for="male">男生</label>
```

方式 2：嵌套写法

```html
<label>
  <input type="checkbox" name="read"> 已阅读协议
</label>
```

#### 1.5.4 下拉选择框 select + option

```html
<select name="city">
  <option value="gz">广州</option>
  <option value="sz">深圳</option>
  <option value="sh" selected>上海</option>
</select>
```

`selected`：默认选中当前下拉选项。

#### 1.5.5 文本域 textarea

```html
<textarea name="msg" rows="4" cols="30" placeholder="请输入留言"></textarea>
```

`rows`：显示行数；`cols`：显示字符宽度；默认支持拖动右下角缩放大小。

### 1.6 表格 table

#### 1.6.1 基础标签

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

#### 1.6.2 表格分区标签

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

#### 1.6.3 合并单元格

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

### 1.7 JavaScript 的引入

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

4. **ES6 Module 模块化**
`type="module"`，默认延迟执行，支持 `import`

```js
// 此处以Vue3 引入为例
<div id="app">{{ message }}</div>

// 引入模块化的js时，需要设置 `type="module"
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

### 1.8 JS 核心语法

#### 1.8.1 变量

变量用来存储数据，使用关键字 `var`、`let`、`const` 声明变量：
- `var`：旧版本变量，可以重复声明，存在变量提升，开发中不推荐。
- `let`：可以修改变量的值，不允许重复声明，存在块级作用域。
- `const`：常量，声明之后不能修改值，必须在声明时赋值。

```js
let name = "张三";
let age = 18;
const PI = 3.14;
```

#### 1.8.2 数据类型

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

#### 1.8.3 输出方式

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

#### 1.8.4 流程控制

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

#### 1.8.5 函数

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

### 1.9 JS DOM 与事件监听

浏览器加载 HTML 页面后，会把页面上的所有标签转换成对象，DOM 就是用来操作这些页面元素的。通过 DOM 可以修改页面内容、修改标签属性、修改样式，还可以绑定事件。

HTML 文档被浏览器解析后，封装成的对象分为：
- **Document**：整个文档对象
- **Element**：元素对象
- **Attribute**：属性对象
- **Text**：文本对象
- **Comment**：注释对象

#### 1.9.1 操作节点

**(1) 获取元素**

```html
<div id="box">我是盒子</div>
<ul>
  <li>列表1</li>
  <li>列表2</li>
</ul>
<div class="item">项目</div>
<div class="box">第一个盒子</div>
```

1. 根据 id 获取元素

```js
// 获取页面上 id="box" 的 div
let box = document.getElementById("box");
```

2. 根据标签名获取元素

```js
// 获取页面上所有 li 标签，得到集合
let lis = document.getElementsByTagName("li");
```

3. 根据类名获取元素

```js
// 获取页面上 class 为 item 的元素集合
let items = document.getElementsByClassName("item");
```

4. querySelector 选择器获取

```js
// 获取页面上 class 为 box 的第一个元素
let one = document.querySelector(".box");
```

5. querySelectorAll

```js
// 获取页面上全部 li 元素，返回集合
let all = document.querySelectorAll("li");
```

**(2) 操作元素内容**

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

**(3) 操作元素属性**

1. **普通属性**

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

2. **自定义属性**

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

**(4) 操作元素样式**

```html
<div class="box"></div>
```

1. **通过 style 修改行内样式**

```js
// 获取 class 为 box 的 div
let box = document.querySelector(".box");
box.style.width = "200px";
box.style.backgroundColor = "red";
```

CSS 里面的短横线样式，在 JS 中要改为驼峰命名，例如 `background-color` 写成 `backgroundColor`。

2. **操作类名 className，修改多个样式**

```js
// 获取 class 为 box 的 div
let box = document.querySelector(".box");
box.className = "active";
```

#### 1.9.2 事件监听

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

## 二、Vue 3 基础

Vue 是一款用于构建用户界面的渐进式 JavaScript 框架。它基于标准 HTML、CSS 和 JavaScript 构建，并提供了一套声明式的、组件化的编程模型，帮助你高效地开发用户界面。无论是简单还是复杂的界面，Vue 都可以胜任。

![[Vue-渐进式.png]]

### 2.1 快速入门
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
**1.html 部分
```html
<div id="app">
  <h1>{{ message }}</h1>
</div>
```
- **挂载根容器**：Vue 应用会接管这个 DOM 节点，容器外的内容 Vue 不会处理。
- `{{ message }}`：**插值语法**，把 Vue 实例里 `data` 中的 `message` 变量渲染到页面

**2.Vue 引入部分**
```html
<script type="module">
      import { createApp } from "https://unpkg.com/vue@3/dist/vue.esm-browser.js";
    </script>
```
-  `type="module"`：开启浏览器原生 ESModule 能力。
-  `https://unpkg.com/vue@3/dist/vue.esm-browser.js`：CDN 地址，直接从网络加载 Vue 3 的ESM 浏览器版。
-  `import { createApp }`：解构导入 Vue 提供的 `createApp` 函数，用来创建 Vue 应用实例。

**3.createApp 创建应用**
```js
createApp({
	data() {
	  return {
		message: "Hello,Vue!",
	  };
	},
}).mount("#app");
```
- `createApp(选项对象)`，接收组件配置对象
- `data` 是组件配置项里的一个函数，Vue 在组件初始化时调用这个函数，拿到返回普通对象，然后把这个对象变成响应式对象，挂载到实例上供模板使用。