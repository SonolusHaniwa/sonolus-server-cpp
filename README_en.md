<p align="center"><img src="http://pic.littleyang.ml/sonolus-server-screenshot/favicon.png"/></p>

<h1 align="center">Sonolus Server for C++</h1>

<p align="center"><a href="./README_en.md">English</a>&nbsp; <a href="./README.md">简体中文</a></p>

A sample api server for Sonolus, use C++ & MySQL to develop. You can use it to test your own Sonolus engine or build a Sonolus server for yourself.

It can be deployed in any Linux devices and any Windows devices, even if a moblie phone also can compile the server successfully.

Use httpd core developed by myself, which also support you to build HTTPS server.

## Links

- [Sonolus Website](https://sonolus.com/)
- [Sonolus Wiki](https://wiki.sonolus.com/)

## Screenshots

![](http://pic.littleyang.ml/sonolus-server-screenshot/pic1.png)
![](http://pic.littleyang.ml/sonolus-server-screenshot/pic2.png)
![](http://pic.littleyang.ml/sonolus-server-screenshot/pic3.png)
![](http://pic.littleyang.ml/sonolus-server-screenshot/pic4.png)
![](http://pic.littleyang.ml/sonolus-server-screenshot/pic5.png)

## Install

We provide an installation package for Windows users, including MySQL v5.7.37. Download link: [Latest Release](https://github.com/LittleYang0531/sonolus-server-cpp/releases/latest).

The first use after installation requires to run the `Setup Sonolus Database` program in the startup menu **as an administrator**, and then you can run the `Sonolus Server for Windows` to start the service.

To import data, you need to run the `Import Sonolus Data` program and enter relevant information according to the prompts.

We have not provided the installation package for Linux users. Please refer to the following tutorial to build it.

## Building

### Install Dependencies

```bash
sudo apt install g++ libjsoncpp-dev libmysqlclient-dev libssl-dev -y
```

### Import Databases

Just enter MySQL Server and import `data.sql`.

### Customize

- Edit `./config/*_search.json` for all search configuration.
- Edit `./config/config.json` for application configuration.

### Compile

```bash
g++ main.cpp -o main -lpthread -lcrypto -lssl -ljsoncpp -lmysqlclient -g
```

### Import Data

Take Bandori's data package `bandori.bin` as an example.

```bash
./main import bandori.bin
```

We provide data package download of all official engines: [Data Packages for v1.1.1+](https://github.com/LittleYang0531/sonolus-server-cpp/releases/tag/data)

### Run

```bash
./main
```

### Export Data

Take the export engine named `bandori` as an example

```bash
./main export engine bandori bandori.bin
```

Take the export level named `bandori-#1` as an example

```bash
./main export level "bandori-#1" bandori1.bin
```

## Configuration

- `mysql.hostname`: MySQL Server listen host.
- `mysql.port`: MySQL Server listen port.
- `mysql.username`: MySQL user's name.
- `mysql.password`: MySQL user's password.
- `mysql.database`: MySQL Server database's name.
- `server.name`: Default is `sonolus`.
- `server.title`: Sonolus server's title.
- `server.description`: Sonolus server's description.
- `server.logo`: Sonolus server's logo.
- `server.bannerUrl`: The banner address that will displayed in the app.
- `server.bannerHash`: The SHA1 code of the banner.
- `server.listenHost`: Listening address for WebServer Core.
- `server.listenPort`: Listening port for WebServer Core.
- `server.enableSSL`: Whether the WebServer Core use HTTPS.
- `server.httpsCacert`: Server's CA Certificate.
- `server.httpsPrivate`: Server's Private Certificate.
- `server.threadNumber`: The number of multithread.
- `server.rootUrl`: Website's root directory(without protocol).
- `logSystem.debug`: Whether the WebServer Core output DEBUG info.
- `logSystem.target`: Where the log info was output. `0` for none, `1` for Console, `2` for Log File, `3` for Console & Log File. 
- `logSystem.targetFile`: Log file path.
- `language.default`: Default language name.

## Endpoints

### API

- `POST /uploader`: Upload a file.
- `GET /data/{hash}`: Download a file whoes SHA-1 is {hash}.
- `GET /sonolus/info`: Fetch the information of the server.
- `GET /sonolus/levels/list`: Fetch a list of levels.
- `GET /sonolus/skins/list`: Fetch a list of skins.
- `GET /sonolus/backgrounds/list`: Fetch a list of backgrounds.
- `GET /sonolus/effects/list`: Fecth a list of effects.
- `GET /sonolus/particles/list`: Fetch a list of particles.
- `GET /sonolus/engines/list`: Fecth a list of engines.
- `POST /sonolus/levels/create`: Put a request to create a level.
- `GET /sonolus/levels/{name}`: Fetch the information of level named {name}.
- `GET /sonolus/skins/{name}`: Fetch the information of skin named {name}.
- `GET /sonolus/backgrounds/{name}`: Fetch the information of background named {name}.
- `GET /sonolus/effects/{name}`: Fetch the information of effect named {name}.
- `GET /sonolus/particles/{name}`: Fetch the information of particle named {name}.
- `GET /sonolus/engines/{name}`: Fetch the information of engine named {name}.

### GUI

- `GET /`: Website's index.
- `GET /index`: Website's index.
- `GET /levels/create`: Create a level.
- `GET /levels/{name}`: Show information of level named {name}.
- `GET /skins/{name}`: Show information of skin named {name}.
- `GET /backgrounds/{name}`: Show information of background named {name}.
- `GET /effects/{name}`: Show information of effect named {name}.
- `GET /particles/{name}`: Show information of particle named {name}.
- `GET /engines/{name}`: Show information of engine named {name}.

## Custom Search

⚠️The following content needs to have a certain C++ & MySQL foundation. If the C++ & MySQL foundation is not available, the current configuration is sufficient for normal use.

### Syntax

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

### Example Configuration

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

### Request Process

By default, this code deals with all the requirements of `?keywords={{keywords}}`.

Take processing the search level as an example, modify the `levelSearch()` function in `items/LevelItem.h`.

Function defination: `string levelSearch(map<string, string> $_GET) {}`.

Of which `$_GET` is the requested `GET` parameter, and the return value is the parameter after MySQL `SELECT` function `WHERE`.

Specific examples:

```cpp
/**
 * @brief Level search request processing function
 * @param $_GET requested GET parameter
 * @return MySQL search filter code
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

## Tips

### 2023.1.21

Windows users need to execute program "Setup Sonolus Database" as administrator firstly, and then execute "Sonolus Server for Windows" to open the service. 

### 2023.1.19

If you want set port to `80/443`, you must have the administrator privilege.

### 2023.1.18

These days, the official wiki website has added an endpoint `/sonolus/authenticate` and an object `UserProfile`, but I couldn't find the interactive way with the Sonolus app. So I didn't add this function into the application.

## Extensions

- [open-source-parsers/jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- libmysqlclient
- [openssl/openssl](https://github.com/openssl/openssl)
- [jquery/jquery](https://github.com/jquery/jquery)
- [dankogai/js-base64](https://github.com/dankogai/js-base64)
- [lyoj-dev/webserver](https://github.com/lyoj-dev/webserver)

## Upload Log

### v1.2.0 2023.2.14

🎁 Happy Valentine's Day 💝

All basic web interfaces have been completed. At present, all pages and hyperlinks have been consistent with the official ones.

1. Add the jump page of all components.
2. Update the level search configuration.
3. Add the feature to save the status of all search interfaces.
4. Fix the problem that language modification only takes effect on the current page.

### v1.1.3 2023.2.11

1. Fix the error of displaying `{{html.xxx}}` when the list page has no entries.
2. Completely repair the memory leak caused by the client's long connection without sending information.
3. Add the display of search criteria on the list page of each component.
4. Fix the problem of stiff animation when the link jumps.
5. Fix the problem of reading error when reading the search configuration file.
6. Fix the problem that the search link on the list page of each component cannot be used.
7. Fix the problem that the server title cannot be displayed.
8. Add a search page for each component.

### v1.1.2 2023.2.5

1. Add WebServer Core to display listening address.
2. Add the database result quantity feedback function of each component.
3. Add the function of user-defined search function for each component.
4. Fix the problem that the `/sonolus/*/list` endpoint does not support paging.
5. Add a list page for each component.
6. Modify the loading mode of all pictures as lazy loading.
7. Add link jump animation.
8. Fix the memory leak error caused by improper memory release.

### v1.1.1 2023.1.26

1. Fixed the error that the main page displays `{{html.xxx}}` when there are no entries.
2. Fixed the program error caused by NULL data in the database.
3. Added data import function and provided data packages of bandori, pjsekai, llsif, taiko, voez and deemo.
4. Added data export function and provided exporting engines and levels.
5. Modified the server startup mode to `./main serve`
6. Added application help information.

### v1.1.0 2023.1.24

1. Fixed Windows version will compiled error caused by missing __int64_t.
2. Fixed the error that the feedbacks of all list apis is the same.
3. Add the recommended display function, which displays 5 works of the same author by default.
4. Add a new homepage and use native javascript to write it after the official project.
5. Add the detail page of each component, and use native javascript to write after the official project.
6. Add the website internationalization function.

### v1.0.2-2 2023.1.21

1. Fixed the issues occurred by `MYSQL` struct by @LittleYang0531 in #1.

### v1.0.2 2023.1.19

1. Add the endpoints to create levels.
2. Add a sample page to create levels.
3. Provide configuration options to set the website title and banner.

### v1.0.1 2023.1.18

1. Merge all the configuration into `./config/config.json`
2. Optimize the CPU usage of WebServer Core

### v1.0.0 2023.1.10

1. Release the first version