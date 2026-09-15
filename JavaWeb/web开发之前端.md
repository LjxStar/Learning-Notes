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
 1. `<h1></h1>` `<h2></h2>` `<h3></h3>` 用来定义标题
 2. `<a href="" target=""></a>` 超链接
	 href: 指定资源访问的url
	 target: 指定在何处打开资源链接
     `_self`: 默认值，在当前页面打开
     `_blank`: 在空白页面打开

## 1.3 css 引入
1. 行内样式
写在标签 `style` 属性上，**只作用当前标签**，优先级最高
```html
<div style="color:red; font-size:16px;">行内样式</div>
```
2. 内部样式
写在 `<head>` 的 `<style>` 标签内，**当前页面生效**
```html
<head>
  <style>
    div {
      color: blue;
    }
  </style>
</head>
```
3. 外部样式
单独 `.css` 文件，用 `<link>` 标签引入，**多页面共用**
```html
<head>
  <!-- rel="stylesheet" 固定写法，href写css文件路径 -->
  <link rel="stylesheet" href="style.css">
</head>
```

设置颜色样式
```HTML
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>【新思想引领新征程】推进长江十年禁渔 谱写长江大保护新篇章</title>
  <!-- 2. 内部样式 -->
  <style>
    .publish-date {
      color: #b2b2b2;
    }
  </style>
  <!-- 3. 外部样式 -->
  <!-- <link rel="stylesheet" href="css/news.css"> -->
</head>
<body>

  <!-- 定义网页标题, 标题内容： 【新思想引领新征程】推进长江十年禁渔 谱写长江大保护新篇章 -->
  <h1 id="title">【新思想引领新征程】推进长江十年禁渔 谱写长江大保护新篇章</h1>
  
  <!-- 定义一个超链接, 链接地址：https://news.cctv.com/, 链接内容：央视网 -->
  <a href="https://news.cctv.com/" target="_blank">央视网</a>

  <!-- 1. 行内样式 -->
  <!-- <span style="color: #b2b2b2;">2024年05月15日 20:07</span> -->

  <span class="publish-date">2024年05月15日 20:07</span>

</body>
</html>
```

## 1 .4 元素选择器

| 选择器                 | 写法                         | 示例                                 |
| ------------------- | -------------------------- | ---------------------------------- |
| 元素选择器               | 元素名称 {...}                 | `h1{...}`                          |
| 类选择器                | .class 属性值 {...}           | `.cls{...}`                        |
| id 选择器              | `#id属性值 {...}`             | `#hid{...}`                        |
| 分组选择器               | 选择器1, 选择器2{...}            | `h1, h2{...}`                      |
| 属性选择器               | 元素名称[属性] {...}             | `input[type]{...}`                 |
| 元素名称[属性名="值"] {...} | `input[type="text"] {...}` | 选择页面上 type 属性为 text 的 `<input>` 标签 |
| 后代选择器               | 元素1 元素2{...}               | form input {...}                   |
