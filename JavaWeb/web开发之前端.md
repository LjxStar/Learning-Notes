Web 标准也称为网页标准，由一系列的标准组成，大部分由 W3C（ World Wide Web Consortium，万维网联盟）负责制定。由三个组成部分：
- **结构层（HTML）**：负责网页的内容结构，定义页面有什么（标题、段落、图片、表格等）。
- **表现层（CSS）**：负责网页的样式展示，控制外观（颜色、字体、布局、间距等）。
- **行为层（JavaScript）**：负责网页的交互行为，实现动态效果、响应用户操作。

# 一、前端三件套
## 1.1 html 基本骨架与标签
```html
<html>
     <head>
          <title>我是标题</title>
     </head>
     <body>
                
     </body>
</html>
```
- `<head>` : 定义网页的头部，存放给浏览器看的信息，如：CSS 样式、网页的标题。
- `<body>` : 定义网页的主体，存放给用户看的信息，也是网页的主体内容，如：文字、图片、视频、音频、表格等。
- `<title>`中定义标题显示在浏览器的标题位置

**常见的 html 标签**
1. 标题标签 `<h1></h1> <h2></h2> <h3></h3>` 用来定义标题
2. 段落标签 `<p></p>` 定义文本段落，自带上下间距
3. 换行标签 `<br>` 强制文字换行，单标签
4. 加粗标签 `<b></b>` / `<strong></strong>` 文字加粗
5. 倾斜标签 `<i></i>` / `<em></em>` 文字倾斜
6. 下划线标签 `<u></u>` / `<ins></ins>`  文字添加下划线
7. 删除线标签 `<s></s>` / `<del></del>` 文字添加删除线
8. 图片标签 `<img src="" alt="" width="" height="" title="">` 
	插入图片 src：图片路径；alt：图片加载失败提示；width/height：宽高；title：鼠标悬浮提示
9. 视频标签 `<video src="" controls autoplay muted loop width=""></video>` 
	播放视频 src：视频地址；controls：播放控件；autoplay：自动播放；muted：静音；loop：循环播放
10. 超链接标签 `<a href="" target="" title=""></a>` 
	超链接 href: 指定资源访问的 url target: 打开方式 _self 当前页，_blank 新页面 title：鼠标悬浮提示
11. 盒子标签 div `<div></div>` 块级容器，用来布局
12. 行内容器 span `<span></span>` 行内容器，局部修饰文字
13. 列表标签 `<ul><li></li></ul>` 无序列表；
	type：列表符号 `<ol><li></li></ol>` 有序列表；type：序号类型，start：起始序号
14. 输入框标签 `<input type="" name="" value="" placeholder="">`
	type：输入框类型 (text/password/radio/checkbox)；name：名称；value：默认值；placeholder：提示文字
15. 注释 `<!-- 注释内容 -->` 浏览器不显示

```html
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    HTML标签示例
</head>
<body>
    <h1>一级标题</h1>
    <p>这是段落。<br>换行后文字</p>
    <p><strong>加粗</strong> <em>倾斜</em> <u>下划线</u> <del>删除线</del></p>
    <a href="https://www.baidu.com" target="_blank">新窗口打开链接</a>
    <img src="test.jpg" alt="测试图片" width="200" title="鼠标悬浮文字">
    <video src="test.mp4" controls width="300"></video>

    <div>块容器div <span>行内span</span></div>
    <ul>
        <li>无序列表1</li>
        <li>无序列表2</li>
    </ul>
    <ol>
        <li>有序列表1</li>
        <li>有序列表2</li>
    </ol>
    <input type="text" placeholder="请输入内容">
    <!-- 这是注释 -->
</body>
</html>
```

在 HTML 页面中，我们在代码中输入空格、<、> 这些符号的时候，是没有对应的效果的，因为浏览器并不能准确的识别，此时，我们就需要通过字符实体来表示空格，<, > 。常见符号的字符实体如下：
	①nbsp → 非换行空格（Non-Breaking Space）
	②lt → 左尖括号（小于号 Less Than）
	③gt → 右尖括号（大于号 Greater Than）

## 1.2 CSS 的引入与选择器
### 1.2.1 三种引入方式

1. **行内样式**：写在标签 `style` 属性，优先级最高

```html
<div style="color:red;">行内样式</div>
```

2. **内部样式**：`head` 内 `<style>` 标签，当前页面生效

```html
<head>
  <style>
    /* css写在这里 */
  </style>
</head>
```

3. **外部样式**：新建`.css` 文件，link 标签引入，多页面复用

```html
<link rel="stylesheet" href="style.css">
```

### 1.2.2 CSS 选择器

1. **元素名称选择器**   格式：`元素名称 { 样式 }` 作用：选中页面所有该标签元素

```css
div {
  width:100px;
}
```

2. **class 类选择器** 格式：`.class属性值 { 样式 }` 作用：选中 class 等于该值的元素，`.` 开头

```css
.box {
  color: blue;
}
```

```html
<div class="box"></div>
```

3. **id 选择器** 格式：`#id属性值 { 样式 }` 作用：选中 id 等于该值的元素，`#`开头，id 页面唯一

```css
#wrap {
  background: #000;
}
```

```html
<div id="wrap"></div>
```

4. **并集选择器** 格式：`选择器1,选择器2 { 样式 }` 作用：多个选择器，统一设置样式，逗号分隔

```css
div,.box {
  font-size:16px;
}
```

5. **属性选择器（有该属性）** 格式：`元素名称[属性] { 样式 }` 作用：选中**拥有这个属性**的对应元素

```css
input[placeholder] {
  border:1px solid #ccc;
}
```

6. **属性选择器（属性等于指定值）** 格式：`元素名称[属性名="值"] { 样式 }` 作用：选中属性值严格等于指定内容的元素

```css
input[type="text"] {
  height:30px;
}
```

7. **后代选择器** 格式：`元素1 元素2 {样式}` 作用：选中元素1内部所有后代元素2

```css
ul li {
  list-style:none;
}
```

```html
<!DOCTYPE html>
<html lang="zh-CN">
	<head>
	    <meta charset="UTF-8">
	    <title>css选择器示例</title>
	    <!--外部样式引入-->
	    <link rel="stylesheet" href="css/style.css"> 
	    	<!-- ./当前目录（可省略） ../上级目录 -->
	    <!--内部样式-->
	    <style>
	        /*元素选择器*/
	        p {
	            text-indent: 2em; /* 首行缩进2em */
				line-height: 2; /* 行高2倍 */
	        }
	        /*类选择器*/
	        .title {
	            color:green; /* 常见有四种颜色表示方式：关键字、rgb、ragb、十六进制 */
	        }
	        /*id选择器*/
	        #main {
	            border: 1px solid #333;
	        }
	        /*并集选择器*/
	        h1,.title {
	            font-weight:bold; /* 设置文字字体粗细 */
	        }
	        /*元素[属性]*/
	        a[href] {
	            text-decoration: none; /* 设置超链接取消下划线效果 */
	        }
	        /*元素[属性="值"]*/
	        input[type="password"] {
	            background:#f5f5f5; /* 背景颜色 */
	        }
	        /*后代选择器 元素1 元素2*/
	        #main p {
	            margin: 0 auto; /* 横向居中 4个值表示上右下左 2个值表示上下、左右 */
	        }
	    </style>
	</head>
	<body>
	    <!--行内样式-->
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

网页中所有元素都可以看作一个**矩形盒子**，由 4 部分组成：`内容区(content)`、`内边距(padding)`、`边框(border)`、`外边距(margin)`。

> 盒子总宽度 / 高度 = content + padding + border + margin

### 1.3.2 盒子模型的四大组成

1. **content 内容区** 元素实际存放文字、图片的区域，由 `width / height` 控制大小。
2. **padding 内边距** 内容和边框之间的距离，**背景会延伸到 padding 区域**。

```css
padding: 上 右 下 左;
padding: 10px; /* 四个方向统一 */
padding: 10px 20px; /* 上下、左右 */
```

3. **border 边框** 盒子的边线，包裹 padding。

```css
border: 宽度 样式 颜色;
border: 1px solid #000;
```

4. **margin 外边距** 盒子与其他盒子之间的距离，**背景不会显示在 margin**。

> **margin 塌陷（合并）**：垂直方向相邻块元素 margin 会取最大值，不是相加。

### 1.3.3 两种盒子模式 box-sizing

1. **content-box**

`width/height` 只代表**content**大小盒子实际宽 = width + padding + border

2. **border-box**

`width/height` 包含 content + padding + border 盒子实际宽 = width（padding 和 border 向内压缩内容，不会撑大盒子）

```css
box-sizing: border-box;
```

内容在内，padding 垫里面，边框围一圈，margin 隔外面。 content-box 向外撑，border-box 向内挤。

### 1.3.4 布局标签 div/span

- div 会独占一行，默认宽度为父元素 body 的宽度。可以设置宽高（width、height）
- span 一行会显示多个，用来组合行内元素，默认宽度为内容撑开的宽度。不可以设置宽高（width、height）

```html
<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>盒子模型</title>
    <style>
      div {
        width: 200px; /* 宽度 */
        height: 200px; /* 高度 */
        box-sizing: border-box; /* 指定width height为盒子的高宽 */
        background-color: aquamarine; /* 背景色 */

        padding: 20px 20px 20px 20px; /* 内边距, 上 右 下 左 , 边距都一行, 可以简写: padding: 20px;*/
        border: 10px solid red; /* 边框, 宽度 线条类型 颜色 */
        margin: 30px 30px 30px 30px; /* 外边距, 上 右 下 左 , 边距都一行, 可以简写: margin: 30px; */
      }
    </style>
  </head>

  <body>
    <div>A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A</div>
  </body>
</html>
```
![[Pasted image 20260915180003.png]] 
## 1.4 Flex 布局
Flex 布局（又称弹性布局）是网页主流布局方案，**可以轻松实现居中、等分、自适应排列**，完美替代传统浮动、margin 布局，解决传统布局高度塌陷、对齐困难等问题。给父元素设置 `display: flex`，即可开启弹性布局，子元素自动成为弹性项。

所有属性作用于开启 `display: flex` 的父盒子，控制子元素整体排列规则。
### 1.4.1 布局方向 flex-direction

设置主轴方向，决定子元素排列方向

```css
flex-direction: row; /* 默认值，水平从左到右 */
flex-direction: column; /* 垂直从上到下 */
flex-direction: row-reverse; /* 水平从右到左 */
flex-direction: column-reverse; /* 垂直从下到上 */
```

### 1.4.2 换行规则 flex-wrap

控制子元素溢出容器时是否换行，默认不换行挤压元素

```css
flex-wrap: nowrap; /* 默认，不换行，子元素自动压缩 */
flex-wrap: wrap; /* 自动换行，超出容器宽度自动换行 */
flex-wrap: wrap-reverse; /* 反向换行 */
```

### 1.4.3 主轴对齐 justify-content

控制子元素在**主轴方向**的对齐方式（最常用居中属性）

```css
justify-content: flex-start; /* 默认，主轴起点对齐 */
justify-content: flex-end; /* 主轴终点对齐 */
justify-content: center; /* 主轴居中对齐（水平居中） */
justify-content: space-between; /* 两端对齐，子元素间距均分，左右无间距 */
justify-content: space-around; /* 子元素两侧间距均分，左右有间距 */
justify-content: space-evenly; /* 所有间距完全均等 */
```

### 1.4.4 *侧轴对齐 align-items

控制单行子元素在**侧轴方向**的对齐方式

```css
align-items: stretch; /* 默认，子元素拉伸铺满容器高度 */
align-items: flex-start; /* 侧轴起点对齐 */
align-items: flex-end; /* 侧轴终点对齐 */
align-items: center; /* 侧轴居中对齐（垂直居中） */
align-items: baseline; /* 按文字基线对齐 */
```

## 1.5 表单 form
### 1.5.1 表单作用

表单用于**收集用户信息**，提交数据给后端服务器。
核心标签：`<form>` 作为表单容器。

```html
<form action="提交地址" method="提交方式">
  <!-- 表单控件：输入框、单选、复选、下拉、按钮等 -->
</form>
```

- `action`：提交到后端接口地址（后端接收数据的 url）
- `method`：提交方式，常用 `get` / `post`
    - get：参数拼在 url 上，长度有限，适合查询，不安全
    - post：请求体携带数据，适合提交隐私、大量数据

### 1.5.2 表单常用控件 input

`<input>` 单标签，**type 属性决定控件类型**

**重要核心规则**：表单提交，控件必须写 **name 属性**，后端才能拿到数据；id 仅用于 label 关联、JS 获取元素。
1. 文本输入框

```html
<input type="text" name="username" placeholder="请输入用户名">
```
`placeholder`：输入框提示文字，不属于输入值

2. 密码框

```html
<input type="password" name="pwd" placeholder="请输入密码">
```

3. 单选框 radio

name 属性控制分组互斥
- **name 相同为同一组**：同组单选框**互斥，只能选中一个**
- **name 不同为不同组**：多组单选**相互独立**，可以同时选中多个
- `value`：提交传给后端的真实数值
- `checked`：默认选中当前选项
```html
<!-- 性别单选：同name，只能二选一/三选一 -->
<input type="radio" name="gender" value="man" id="man" checked>
<label for="man">男</label>

<input type="radio" name="gender" value="woman" id="woman">
<label for="woman">女</label>

<input type="radio" name="gender" value="secret" id="secret">
<label for="secret">保密</label>
```

4. 复选框 checkbox

可多选，同一组选项 `name` 可以相同，后端会拿到数组形式的数据。
```html
<input type="checkbox" name="hobby" value="game" id="game">
<label for="game">游戏</label>
<input type="checkbox" name="hobby" value="read" id="read" checked>
<label for="read">阅读</label>
```

5. 数字框 number

```html
<input type="number" name="age">
```

6. 邮箱 email、手机号 tel

```html
<input type="email" name="email">
<input type="tel" name="phone">
```

7. 按钮系列

**提交按钮** submit：点击自动提交 form 表单
```html
<input type="submit" value="登录">
```

**重置按钮** reset：一键清空表单所有输入内容
```html
<input type="reset" value="重置">
```

**普通按钮** button：不会自动提交，配合 JS 使用
```html
<input type="button" value="点击" onclick="fn()">
```

补充：`<button>` 标签默认 type="submit"
```html
<button type="submit">提交</button>
<button type="button">普通按钮</button>
```

### 1.5.3 label 标签

点击文字，自动选中对应输入框，扩大点击区域，提升用户体验。

**两种标准写法**

方式 1：for + id 绑定
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

### 1.5.3 下拉选择框 select + option

```html
<select name="city">
  <option value="gz">广州</option>
  <option value="sz">深圳</option>
  <option value="sh" selected>上海</option>
</select>
```
`selected`：默认选中当前下拉选项

### 1.5.4 文本域 textarea

```html
<textarea name="msg" rows="4" cols="30" placeholder="请输入留言"></textarea>
```
rows：显示行数；cols：显示字符宽度；默认支持拖动右下角缩放大小


## 1.6 表格 table
### 1.6.1 基础标签

table 是表格最外层容器。 tr 代表表格中的一行。 td 是普通单元格，用来存放数据内容。 th 是表头单元格，文字默认加粗并且居中。

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
    <td>19</td>
    <td>女</td>
  </tr>
</table>
```

### 1.6.2 表格分区标签

可以把表格分成表头主体底部三个区域，提升代码可读性，浏览器会优先加载表格主体 tbody。 thead 存放表头内容。 tbody 存放表格主要数据。 tfoot 存放表格底部汇总信息。

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
      <td>java</td>
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

colspan 用于横向跨列合并单元格。 rowspan 用于纵向跨行合并单元格。合并单元格之后，需要手动删除被合并的 td 标签，只保留写合并属性的单元格。

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
**JavaScript** 是一门跨平台、面向对象的脚本语言，是用来控制网页行为的，实现人机交互效果。其包括如下：
- ECMAScript: 规定了 JS 基础语法核心知识，包括变量、数据类型、流程控制、函数、对象等。
- BOM：浏览器对象模型，用于操作浏览器本身，如：页面弹窗、地址栏操作、关闭窗口等。
- DOM：文档对象模型，用于操作 HTML 文档，如：改变标签内的内容、字体样式等。

1. 行内引入

直接把 JS 代码写在标签的事件属性里面，点击等动作触发代码执行。不推荐大量使用，结构和代码混在一起，不方便维护。

```html
<!-- 点击按钮弹出提示 -->
<button onclick="alert('你好')">点击我</button>
```

2. 内部引入

使用 script 标签，把 JS 代码直接写在页面里面。 script 标签可以放在页面的 head 或者 body 中。推荐放在 body 末尾，页面元素加载完成后再执行 JS，避免找不到页面标签。

```html
<script>
// 这里写js代码
alert('内部js')
</script>
```

3. 外部引入

新建单独的 js 文件，在 html 中通过 script 的 src 属性引入文件。 script 标签写了 src 属性后，标签内部不能再写 js 代码，写了也不会执行。
	在外部 JS 文件中，只包含 JS 代码，不包含 `<script>` 标签
    引入外部 js 的 `<script>` 标签，必须是双标签

```html
<!-- 引入外部js文件 -->
<script src="test.js"></script>
```

## 1.8 JS核心语法
### 1.8.1 变量

变量用来存储数据，使用关键字 var let const 声明变量。
	var 旧版本变量，可以重复声明，存在变量提升，开发不推荐。 
	let 可以修改变量的值，不允许重复声明，存在块级作用域。 
	const 常量，声明之后不能修改值，必须在声明时赋值。

```js
let name = "张三";
let age = 18;
const PI = 3.14;
```

### 1.8.2 数据类型

分为简单数据类型和复杂数据类型。简单数据类型

1. string 字符串，文本内容，使用单引号或者双引号包裹
2. number 数字，整数和小数
3. boolean 布尔值，只有 true 和 false 两个值，代表真和假
4. undefined 未定义，变量声明没有赋值，默认就是 undefined
5. null 空值，表示一个空对象

复杂数据类型 object 对象，用来存放一组相关的数据。
```js
// 对象示例，存放一个人的信息
let person = {
  name:"李四",
  age:20,
  gender:"男"
  hello: function(){ console.log("你好啊！");
  } // 此处为函数，后面会提及
}
// 获取对象里面的数据
console.log(person.name)
console.log(person.age)
```

### 1.8.3 输出方式

1. console.log () 在浏览器控制台打印内容，调试代码最常用
```js
console.log("测试");
```

2. alert () 弹出提示框
```js
alert("弹出消息");
```

3. document.write () 在页面输出内容
```js
document.write("页面文字");
```

### 1.8.4 流程控制

1. if 判断语句
```js
if(条件){
  条件成立执行代码
}else{
  条件不成立执行代码
}
```

2. for 循环
```js
for(let i=0;i<10;i++){
  //循环执行代码
}
```

### 1.8.5 函数

函数是封装好的一段代码，可以重复调用，实现代码复用。

函数声明语法
```js
function 函数名(参数){
  // 函数体代码
  return 返回值
}
```

调用方式：函数名 ()，不调用函数不会执行里面代码。

参数：调用函数时传入的数据，多个参数用逗号隔开。 return 作用：将结果返回给调用位置，同时终止函数。没有写 return，函数默认返回 undefined。

无参函数示例

```js
function sayHi(){
  console.log("你好");
}
sayHi()
```

带参数函数示例

```js
function sum(a,b){
  return a + b
}
let result = sum(10,20)
console.log(result)
```