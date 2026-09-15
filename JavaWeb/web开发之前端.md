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

## 1.4 元素选择器

| 选择器                 | 写法                         | 示例                                 |
| ------------------- | -------------------------- | ---------------------------------- |
| 元素选择器               | 元素名称 {...}                 | `h1{...}`                          |
| 类选择器                | .class 属性值 {...}           | `.cls{...}`                        |
| id 选择器              | `#id属性值 {...}`             | `#hid{...}`                        |
| 分组选择器               | 选择器1, 选择器2{...}            | `h1, h2{...}`                      |
| 属性选择器               | 元素名称[属性] {...}             | `input[type]{...}`                 |
| 元素名称[属性名="值"] {...} | `input[type="text"] {...}` | 选择页面上 type 属性为 text 的 `<input>` 标签 |
| 后代选择器               | 元素1 元素2{...}               | form input {...}                   |
