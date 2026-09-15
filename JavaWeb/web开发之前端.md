Web 标准也称为网页标准，由一系列的标准组成，大部分由 W3C（ World Wide Web Consortium，万维网联盟）负责制定。由三个组成部分：
- **结构层（HTML）**：负责网页的内容结构，定义页面有什么（标题、段落、图片、表格等）。
- **表现层（CSS）**：负责网页的样式展示，控制外观（颜色、字体、布局、间距等）。
- **行为层（JavaScript）**：负责网页的交互行为，实现动态效果、响应用户操作。


# 一、前端三件套
## 1.1基本骨架
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

##  1.2 html 标签
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

CSS 引入方式 + 指定 CSS 选择器（Markdown 笔记）

## 1.3 CSS 三种引入方式

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

3. **外部样式**：独立 `.css` 文件，link 标签引入，多页面复用

```html
<link rel="stylesheet" href="style.css">
```

## 1.4 CSS 选择器

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
	    <link rel="stylesheet" href="style.css">
	    <!--内部样式-->
	    <style>
	        /*元素选择器*/
	        p {
	            text-indent: 2em; /* 首行缩进2em */
				line-height: 2; /* 行高2倍 */
	        }
	        /*类选择器*/
	        .title {
	            color:green;
	        }
	        /*id选择器*/
	        #main {
	            border: 1px solid #333;
	        }
	        /*并集选择器*/
	        h1,.title {
	            font-weight:bold;
	        }
	        /*元素[属性]*/
	        a[href] {
	            text-decoration: none;
	        }
	        /*元素[属性="值"]*/
	        input[type="password"] {
	            background:#f5f5f5;
	        }
	        /*后代选择器 元素1 元素2*/
	        #main p {
	            line-height:1.5;
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

## 1.5