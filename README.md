<p align="center"><img src="http://pic.littleyang.ml/sonolus-server-screenshot/favicon.png"/></p>

<h1 align="center">Sonolus Server for C++</h1>

<p align="center"><a href="./README_en.md">English</a>&nbsp; <a href="./README.md">简体中文</a></p>

使用 C++ 与 MySQL 开发的一个适用于 Sonolus 的 api 服务器，您可以用于测试您自己的引擎或自己构建一个 Sonolus 服务。

该项目可以被部署在任何 Linux 设备以及 Windows 设备，即使是一台手机也能轻松完成服务器的编译。

使用自主研发的 WebServer Core 进行开发，同时支持您构建 HTTPS 服务器。

## 链接

- [Sonolus Website](https://sonolus.com/)
- [Sonolus Wiki](https://wiki.sonolus.com/)

## 屏幕截图

![](http://pic.littleyang.ml/sonolus-server-screenshot/pic1.png)
![](http://pic.littleyang.ml/sonolus-server-screenshot/pic2.png)
![](http://pic.littleyang.ml/sonolus-server-screenshot/pic3.png)
![](http://pic.littleyang.ml/sonolus-server-screenshot/pic4.png)
![](http://pic.littleyang.ml/sonolus-server-screenshot/pic5.png)

## 安装

我们提供对 Windows 用户的安装包，内含 MySQL v5.7.37，下载地址: [Latest Release](https://github.com/LittleYang0531/sonolus-server-cpp/releases/latest)

安装后第一次使用需要**以管理员身份运行**启动菜单里的 `Setup Sonolus Database` 程序，之后就可以启动 `Sonolus Server for Windows` 来启动服务。

导入数据需要运行 `Import Sonolus Data` 程序，按照提示输入相关信息即可。

我们并未提供对 Linux 用户的安装包 ~~(都用 Linux 了相信或多或少还是有一点使用终端的基础了吧)~~，请自行参照下方教程进行构建。

## 构建

### 下载依赖

```bash
sudo apt install g++ libjsoncpp-dev libmysqlclient-dev libssl-dev -y
```

### 导入数据库

仅需要进入 MySQL 数据库导入 `data.sql` 即可。

### 网站配置

- 在 `./config/*_search.json` 中编辑所有的搜索配置。
- 在 `./config/config.json` 中编辑应用配置。

### 编译

```bash
g++ main.cpp -o main -lpthread -lcrypto -lssl -ljsoncpp -lmysqlclient -g
```

### 导入数据

以 bandori 的数据包 `bandori.bin` 为例。

```bash
./main import bandori.bin
```

我们提供各官方引擎的数据包下载: [Data Packages for v1.1.1+](https://github.com/LittleYang0531/sonolus-server-cpp/releases/tag/data)

### 运行

```bash
./main serve
```

### 导出数据

以导出名为 `bandori` 的引擎为例

```bash
./main export engine bandori bandori.bin
```

以导出名为 `bandori-#1` 的关卡为例

```bash
./main export level "bandori-#1" bandori1.bin
```

## 配置信息

- `mysql.hostname`: MySQL 服务器监听地址。
- `mysql.port`: MySQL 服务器监听端口。
- `mysql.username`: MySQL 用户名。
- `mysql.password`: MySQL 用户密码。
- `mysql.database`: MySQL 目标数据库名。
- `server.name`: 默认 `sonolus` 就行。
- `server.title`: Sonolus 服务器标题。
- `server.description`: Sonolus 服务器描述。
- `server.logo`: Sonolus 服务器 logo。
- `server.bannerUrl`: 网站导航图地址。
- `server.bannerHash`: 网站导航图 SHA-1 代码。
- `server.listenHost`: WebServer Core 监听地址。
- `server.listenPort`: WebServer Core 监听端口。
- `server.enableSSL`: 指定服务器是否启用 HTTPS。
- `server.httpsCacert`: 服务器 CA 证书。
- `server.httpsPrivate`: 服务器私钥。
- `server.threadNumber`: 线程数。
- `server.rootUrl`: 服务器根节点(不含协议信息)。
- `logSystem.debug`: 指定是否输出 DEBUG 信息。
- `logSystem.target`: 指定日志输出位置。`0` 为不输出，`1` 为输出在控制台，`2` 为文件输出，`3` 为二者皆有。 
- `logSystem.targetFile`: 日志文件地址。
- `language.default`: 网站默认语言。

## 终节点

### API

- `POST /uploader`: 上传文件接口。
- `GET /data/{hash}`: 下载 SHA-1 为 {hash} 的文件。
- `GET /sonolus/info`: 获取服务器信息。
- `GET /sonolus/levels/list`: 获取部分关卡信息。
- `GET /sonolus/skins/list`: 获取部分皮肤信息。
- `GET /sonolus/backgrounds/list`: 获取部分背景信息。
- `GET /sonolus/effects/list`: 获取部分音效信息。
- `GET /sonolus/particles/list`: 获取部分粒子效果信息。
- `GET /sonolus/engines/list`: 获取部分引擎信息。
- `POST /sonolus/levels/create`: 创建请求以新增关卡
- `GET /sonolus/levels/{name}`: 获取名为 {name} 的关卡信息。
- `GET /sonolus/skins/{name}`: 获取名为 {name} 的皮肤信息。
- `GET /sonolus/backgrounds/{name}`: 获取名为 {name} 的背景信息。
- `GET /sonolus/effects/{name}`: 获取名为 {name} 的音效信息。
- `GET /sonolus/particles/{name}`: 获取名为 {name} 的粒子效果信息。
- `GET /sonolus/engines/{name}`: 获取名为 {name} 的引擎信息。

### GUI

- `GET /`: 网站主页面。
- `GET /index`: 网站主页面。
- `GET /levels/create`: 创建一个关卡。
- `GET /levels/{name}`: 显示名为 {name} 的关卡信息。
- `GET /skins/{name}`: 显示名为 {name} 的皮肤信息。
- `GET /backgrounds/{name}`: 显示名为 {name} 的背景信息。
- `GET /effects/{name}`: 显示名为 {name} 的音效信息。
- `GET /particles/{name}`: 显示名为 {name} 的粒子效果信息。
- `GET /engines/{name}`: 显示名为 {name} 的引擎信息。

## 自定义搜索

⚠️接下来的内容需要有一定的 C++ & MySQL 基础，如果不具备 C++ & MySQL 基础目前的配置已足够正常使用了。

### 语法 

```C++
class Search {
    vector<SearchOption> options;
}

class SearchOption =
    SearchTextOption text ||
    SearchSliderOption slider ||
    SearchToggleOption toggle ||
    SearchSelectOption select;

class SearchTextOption {
    string query;
    string name;
    string type = "text";
    string placeholder;
}

class SearchSliderOption {
    string query;
    string name;
    string type = "slider";
    int def;
    int min;
    int max;
    int step;
    string unit;
}

class SearchToggleOption {
    string query;
    string name;
    string type = "toggle";
    bool def = 0;
}

class SearchSelectOption {
    string query;
    string name;
    string type = "select";
    int def;
    vector<string> values;
}
```

### 配置示例

```json
{
    "options": [
        {
            "query": "keywords",
            "name": "#KEYWORDS",
            "type": "text",
            "placeholder": "#KEYWORDS"
        }
    ]
}
```

### 请求处理

本代码默认处理所有满足 `?keywords={{keywords}}` 的请求。

以处理搜索关卡为例，修改 `items/LevelItem.h` 中的 `levelSearch()` 函数。

函数定义: `string levelSearch(map<string, string> $_GET) {}`。

其中 `$_GET` 为请求的 `GET` 参数，返回值为 MySQL `SELECT` 函数 `WHERE` 后面的参数。

具体示例:

```cpp
/**
 * @brief 关卡搜索请求处理函数
 * @param $_GET 请求GET参数
 * @return MySQL搜索过滤代码
 */
string levelSearch(map<string, string> $_GET) {
    string filter = "";
    if ($_GET["keywords"] != "") filter = "title like \"%" + str_replace("\"", "\\\"", $_GET["keywords"]) + "%\"";
    return filter;
}
```

<!-- ## 数据包格式

**数据格式**

| 域 | 长度 | 内容说明 |
|:-:|:-:|:-:|
| FileNumber | $8$ bytes | 存储数据包文件数 |
| FileChunk | $x$ bytes | 存储各文件数据 |
| SQLCode | $y$ bytes | 存储更新数据库代码 |

**FileChunk 格式**

| 域 | 长度 | 内容说明 |
|:-:|:-:|:-:|
| FileSHA1 | $20$ bytes | 文件 SHA1 码，用于校验文件 |
| FileSize | $8$ bytes | 存储文件大小 |
| FileBuffer | $z$ bytes | 存储文件内容 | -->

## 提示

### 2023.1.21

对于 Windows 用户，需要首先以管理员身份运行 "Setup Sonolus Database" 程序，然后运行 "Sonolus Server for Windows" 开启服务。

### 2023.1.19

如果您想监听 `80/443` 端口，您必须以管理员身份运行程序。

### 2023.1.18

最近官方 wiki 网站新增了终节点 `/sonolus/authenticate` 以及对象 `UserProfile`，但我不知道该终节点与 Sonolus 应用交互的方法，故本应用中暂时无 `/sonolus/authenticate` 终节点。

## 扩展

- [open-source-parsers/jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- libmysqlclient
- [openssl/openssl](https://github.com/openssl/openssl)
- [jquery/jquery](https://github.com/jquery/jquery)
- [dankogai/js-base64](https://github.com/dankogai/js-base64)
- [lyoj-dev/webserver](https://github.com/lyoj-dev/webserver)

## 更新日志

### v1.2.0 2023.2.14

🎁 情人节特供 💝

所有基础 web 界面已全部完工，目前所有页面及超链接已保持和官方一致。

1. 新增所有组件的 jump 页面。
2. 更新关卡搜索配置。
3. 新增对所有搜索界面状态保存。
4. 修复语言修改只在当前页面生效的问题。

### v1.1.3 2023.2.11

1. 修复当 list 页面没有条目时显示 `{{html.xxx}}` 的错误。
2. 彻底修复由于客户端长连接不发送信息导致的内存爆炸。
3. 新增各组件 list 页面对搜索条件的显示。
4. 修复链接跳转时动画生硬的问题。
5. 修复读取搜索配置文件时读取出错的问题。
6. 修复各组件 list 页面搜索链接无法使用的问题。
7. 修复服务器标题无法显示的问题。
8. 新增各组件搜索页面。

### v1.1.2 2023.2.5

1. 新增 WebServer Core 对监听地址的显示。
2. 新增各组件的数据库结果数量反馈函数。
3. 新增各组件自定义搜索函数功能。
4. 修复 `/sonolus/*/list` 终结点不支持翻页的问题。
5. 新增各组件 list 页面。
6. 修改所有图片加载形式为延迟加载。
7. 新增链接跳转动画。
8. 修复由于内存释放不当导致的内存泄漏错误。

### v1.1.1 2023.1.26

1. 修复当没有条目时主页面显示 `{{html.xxx}}` 的错误。
2. 修复当数据库中存在 NULL 数据时引起程序退出错误。
3. 新增数据导入功能，并提供 bandori, pjsekai, llsif, taiko, voez, deemo 的数据包。
4. 新增数据导出功能，支持引擎和关卡的导出。
5. 修改服务器启动方式为 `./main serve`。
6. 新增应用帮助信息。

### v1.1.0 2023.1.24

1. 修复 Windows 由于缺少 __int64_t 导致的编译错误。
2. 修复所有 list api 的反馈均为一个条目的错误。
3. 新增推荐显示功能，默认显示同一作者的 5 个作品。
4. 新增网页主页，仿照官方项目使用原生 js 进行编写。
5. 新增各组件详情页，仿照官方项目使用原生 js 进行编写。
6. 新增网站国际化功能，即 i18n。

### v1.0.2-2 2023.1.21

1. 修复由于 `MYSQL` 引起的 Windows 版程序闪退 by @LittleYang0531 in #1。

### v1.0.2 2023.1.19

1. 新增用于创建关卡的终节点。
2. 新增页面以添加关卡。
3. 提供设置网站标题以及网站导航图的选项。

### v1.0.1 2023.1.18

1. 合并所有的设置选项于 `./config/config.json`。
2. 优化 WebServer Core 对 CPU 的使用。

### v1.0.0 2023.1.10

1. 第一个版本发布