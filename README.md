<p align="center"><img src="http://pic.littleyang.ml/sonolus-server-screenshot/favicon.png"/></p>

<h1 align="center">Sonolus Server for C++</h1>

<p align="center"><a href="./README_en.md">English</a>&nbsp; <a href="./README.md">简体中文</a></p>

使用 C++ 与 MySQL/SQLite 开发的一个适用于 Sonolus 的 api 服务器，您可以用于测试您自己的引擎或自己构建一个 Sonolus 服务。

该项目可以被部署在任何 Linux 设备以及 Windows 设备，即使是一台手机也能轻松完成服务器的编译。

使用自主研发的 WebServer Core 进行开发，同时支持您构建 HTTPS 服务器。

## 目录

- [目录](#目录)
- [链接](#链接)
- [屏幕截图](#屏幕截图)
- [安装](#安装)
- [构建](#构建)
- [配置信息](#配置信息)
- [终节点](#终节点)
- [自定义搜索](#自定义搜索)
- [提示](#提示)
- [第三方仓库](#第三方仓库)
- [插件](#插件)
- [更新日志](#更新日志) 

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

我们提供各版本的可执行文件压缩包，默认选择 SQLite 数据库。

对于 `Windows` 用户，无需配置环境，直接命令行输入 `./main serve` 即可运行。

对于 `Linux` 用户，您需要自行准备依赖文件，安装指令见 [下载依赖](#下载依赖)。

## 构建

### 下载依赖

```bash
sudo apt install g++ libjsoncpp-dev libmysqlclient-dev libssl-dev libsqlite3-dev zliblg-dev -y
```

### 导入数据库

仅需要进入 MySQL 数据库导入 `data.sql` 即可。

### 网站配置

- 在 `./config/*_search.json` 中编辑所有的搜索配置。
- 在 `./config/config.json` 中编辑应用配置。

### 获取源码

```bash
git clone --recursive https://github.com/SonolusHaniwa/sonolus-server-cpp
```

### 编译

基础编译指令: 

```bash
g++ main.cpp -o main -lpthread -lcrypto -lssl -ljsoncpp -lmysqlclient -lz -std=c++14 -O2 -g
```

**附加编译选项列表**

- `-DENABLE_SONOLUS` 加载 Sonolus 服务器主要路由。
- `-DENABLE_STUDIOS` 加载 Sonolus Studio (暂未开放) 服务器主要路由。
- `-DENABLE_MYSQL` 允许程序与 MySQL/MariaDB 数据库进行交互。
- `-DENABLE_SQLITE` 允许程序与 SQLite 数据库进行交互。

扩展编译指令:

```bash
g++ plugins/libsonolush/libsonolush.cpp -o plugins/libsonolush.so -fPIC -shared -std=c++17 -DENABLE_MYSQL -DENABLE_SQLITE
g++ plugins/libsonolusjs/libsonolusjs.cpp -o plugins/libsonolusjs.so -fPIC -shared -std=c++17 -DENABLE_MYSQL -DENABLE_SQLITE
```

### 导入数据

以 bandori 的数据包 `bandori.bin` 为例。

```bash
./main import bandori.bin
```

我们提供各官方引擎的数据包下载: [Data Packages for v1.2.1+](https://github.com/LittleYang0531/sonolus-server-cpp/releases/tag/v1.2.1-data)

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

- `database`: 数据库类型，可选 `mysql` 或 `sqlite`。
- `mysql.hostname`: MySQL 服务器监听地址。
- `mysql.port`: MySQL 服务器监听端口。
- `mysql.username`: MySQL 用户名。
- `mysql.password`: MySQL 用户密码。
- `mysql.database`: MySQL 目标数据库名。
- `sqlite.dbfile`: SQLite 数据库文件位置。
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
- `POST /sonolus/skins/create`: 创建请求以新增皮肤。
- `POST /sonolus/backgrounds/create`: 创建请求以新增背景。
- `POST /sonolus/effects/create`: 创建请求以新增音效。
- `POST /sonolus/particles/create`: 创建请求以新增粒子效果。
- `POST /sonolus/engines/create`: 创建请求以新增引擎。
- `GET /sonolus/levels/{name}`: 获取名为 {name} 的关卡信息。
- `GET /sonolus/skins/{name}`: 获取名为 {name} 的皮肤信息。
- `GET /sonolus/backgrounds/{name}`: 获取名为 {name} 的背景信息。
- `GET /sonolus/effects/{name}`: 获取名为 {name} 的音效信息。
- `GET /sonolus/particles/{name}`: 获取名为 {name} 的粒子效果信息。
- `GET /sonolus/engines/{name}`: 获取名为 {name} 的引擎信息。

### GUI

- `GET /`: 网站主页面。
- `GET /index`: 网站主页面。
- `GET /levels/list`: 根据筛选信息列举关卡。
- `GET /skins/list`: 根据筛选信息列举皮肤。
- `GET /backgrounds/list`: 根据筛选信息列举背景。
- `GET /effects/list`: 根据筛选信息列举音效。
- `GET /particles/list`: 根据筛选信息列举粒子效果。
- `GET /engines/list`: 根据筛选信息列举引擎。
- `GET /levels/search`: 搜索关卡页面。
- `GET /skins/search`: 搜索皮肤页面。
- `GET /backgrounds/search`: 搜索背景页面。
- `GET /effects/search`: 搜索音效页面。
- `GET /particles/search`: 搜索粒子效果页面。
- `GET /engines/search`: 搜索引擎页面。
- `GET /levels/create`: 创建关卡页面。
- `GET /skins/create`: 创建皮肤页面。
- `GET /backgrounds/create`: 创建背景页面。
- `GET /effects/create`: 创建音效页面。
- `GET /particles/create`: 创建粒子效果页面。
- `GET /engines/create`: 创建引擎页面。
- `GET /levels/jump/{page}`: 跳转关卡列表页面。
- `GET /skins/jump/{page}`: 跳转皮肤列表页面。
- `GET /backgrounds/jump/{page}`: 跳转背景列表页面。
- `GET /effects/jump/{page}`: 跳转音效列表页面。
- `GET /particles/jump/{page}`: 跳转粒子效果列表页面。
- `GET /engines/jump/{page}`: 跳转引擎列表页面。
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

## 提示

### 2023.6.5

我们依照了 [sevenc-nanashi/chart_cyanvas](https://github.com/sevenc-nanashi/chart_cyanvas) 的实现原理，实现了使用 Sonolus 用户登录的接口。目前网页的右上部分即为网页的登录入口。

我们重新编写了各环境的自动编译脚本，对于 `Linux` 用户来说新增了预编译版本的可执行文件，而对于 `Windows` 用户来说，我们将安装包变为了包含可执行文件及运行时环境的压缩包。

### 2023.1.21

~~对于 Windows 用户，需要首先以管理员身份运行 "Setup Sonolus Database" 程序，然后运行 "Sonolus Server for Windows" 开启服务。~~

### 2023.1.19

如果您想监听 `80/443` 端口，您必须以管理员身份运行程序。

### 2023.1.18

~~最近官方 wiki 网站新增了终节点 `/sonolus/authenticate` 以及对象 `UserProfile`，但我不知道该终节点与 Sonolus 应用交互的方法，故本应用中暂时无 `/sonolus/authenticate` 终节点。~~

## 第三方仓库

- [open-source-parsers/jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- libmysqlclient
- [openssl/openssl](https://github.com/openssl/openssl)
- [jquery/jquery](https://github.com/jquery/jquery)
- [dankogai/js-base64](https://github.com/dankogai/js-base64)
- [lyoj-dev/webserver](https://github.com/lyoj-dev/webserver)
- [pluma](https://github.com/aelzenaar/pluma)

## 插件

- [libsonolush](https://github.com/SonolusHaniwa/sonolus-server-plugin-libsonolush)
- [libsonolusjs](https://github.com/SonolusHaniwa/sonolus-server-plugin-libsonolusjs)
- [libonedrive](https://github.com/SonolusHaniwa/sonolus-server-plugin-libonedrive) (Default: Disable)

## 更新日志

### v1.4.1 2023.6.9

1. 新增 OneDrive 上传与下载接口。
2. 更新 workflows 脚本

### v1.4.0 2023.6.5

1. 新增 Sonolus 用户登录接口，并摒弃旧的登录接口。
2. 修复了部分 Windows 编译问题。
3. 更新 workflows 脚本。

### v1.3.0 2023.5.21

1. 新增插件功能。
2. 允许自定义服务器 Sonolus 版本。
3. 更新 workflows 脚本。

### v1.2.3 2023.4.16

1. 新增引擎一键打包 & 测试功能。
2. 新增创建组件按钮。
3. 新增用户登录接口。

### v1.2.2 2023.3.12

1. 新增各组件创建页面。
2. 新增 `file` 类型的搜索组件，可用于创建页面使用。
3. 新增 `Windows` 自动打包脚本。

### v1.2.1 2023.3.6

1. 新增对 SQLite 数据库的支持。
2. 修改了数据包格式，以为 SQLite 提供支持。
3. 重新对官方引擎进行打包，新的下载链接: [Data Packages for v1.2.1+](https://github.com/LittleYang0531/sonolus-server-cpp/releases/tag/v1.2.1-data)。
4. 新增了各组件创建接口，为下一版本创建组件提供支持。
5. 临时删除 `/levels/create` 接口，便于下一版本对 GUI 的重写。

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