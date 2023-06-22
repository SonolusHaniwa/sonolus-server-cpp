<p align="center"><img src="http://pic.littleyang.ml/sonolus-server-screenshot/favicon.png"/></p>

<h1 align="center">Sonolus Server for C++</h1>

<p align="center"><a href="./README_en.md">English</a>&nbsp; <a href="./README.md">简体中文</a></p>

A sample api server for Sonolus, use C++ & MySQL to develop. You can use it to test your own Sonolus engine or build a Sonolus server for yourself.

It can be deployed in any Linux devices and any Windows devices, even if a moblie phone also can compile the server successfully.

Use httpd core developed by myself, which also support you to build HTTPS server.

## Catalog

- [Catalog](#Catalog)
- [Links](#Links)
- [Screenshots](#Screenshots)
- [Install](#Install)
- [Building](#Building)
- [Configuration](#Configuration)
- [Endpoints](#Endpoints)
- [Custom Search](#Custom%20Search)
- [Tips](#Tips)
- [Third-party Repo](#Third-party%Repo)
- [Plugins](#Plugins)
- [Upload Log](#Upload%20Log)

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

We provide executable file compression packages for each version, with SQLite database selected by default.

For `Windows` users, there's no need to configure the environment, just enter `./main serve`.

For `Linux` users, you need to prepare dependencies by yourself. The installation instructions can be found in [Install Dependencies](#Install%20Dependencies).

## Building

### Install Dependencies

```bash
g++ main.cpp -o main -lpthread -lcrypto -lssl -ljsoncpp -lmysqlclient -lz -std=c++14 -O2 -g
```

### Import Databases

Just enter MySQL Server and import `data.sql`.

### Customize

- Edit `./config/*_search.json` for all search configuration.
- Edit `./config/config.json` for application configuration.

### Fetch Source

```bash
git clone --recursive https://github.com/SonolusHaniwa/sonolus-server-cpp
```

### Compile

Basic Compile Command:

```bash
g++ main.cpp -o main -lpthread -lcrypto -lssl -ljsoncpp -lmysqlclient -g
```

**Extra Compilation Options List**

- `-DENABLE_SONOLUS` Load main routes of Sonolus Server.
- `-DENABLE_STUDIOS` Load main routes of Sonolus Studios Server (developing...).
- `-DENABLE_MYSQL` allow program interactive with MySQL/MariaDB database.
- `-DENABLE_SQLITE` allow program interactive with SQLite database.

Compile Plugins:

```bash
g++ plugins/libsonolush/libsonolush.cpp -o plugins/libsonolush.so -fPIC -shared -std=c++17 -DENABLE_MYSQL -DENABLE_SQLITE
g++ plugins/libsonolusjs/libsonolusjs.cpp -o plugins/libsonolusjs.so -fPIC -shared -std=c++17 -DENABLE_MYSQL -DENABLE_SQLITE
```

### Import Data

Take Bandori's data package `bandori.bin` as an example.

```bash
./main import bandori.bin
```

We provide data package download of all official engines: [Data Packages for v1.2.1+](https://github.com/LittleYang0531/sonolus-server-cpp/releases/tag/v1.2.1-data)

### Run

```bash
./main serve
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

- `database`: Database type. You can choose `mysql` or `sqlite`。
- `mysql.hostname`: MySQL Server listen host.
- `mysql.port`: MySQL Server listen port.
- `mysql.username`: MySQL user's name.
- `mysql.password`: MySQL user's password.
- `mysql.database`: MySQL Server database's name.
- `sqlite.dbfile`: SQLite Database File path。
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
- `POST /sonolus/skins/create`: Put a request to create a skin.
- `POST /sonolus/backgrounds/create`: Put a request to create a background.
- `POST /sonolus/effects/create`: Put a request to create an effect.
- `POST /sonolus/particles/create`: Put a request to create a particle.
- `POST /sonolus/engines/create`: Put a request to create an engine.
- `GET /sonolus/levels/{name}`: Fetch the information of level named {name}.
- `GET /sonolus/skins/{name}`: Fetch the information of skin named {name}.
- `GET /sonolus/backgrounds/{name}`: Fetch the information of background named {name}.
- `GET /sonolus/effects/{name}`: Fetch the information of effect named {name}.
- `GET /sonolus/particles/{name}`: Fetch the information of particle named {name}.
- `GET /sonolus/engines/{name}`: Fetch the information of engine named {name}.

### GUI

- `GET /`: Website's index.
- `GET /index`: Website's index.
- `GET /levels/list`: List levels according to the filter.
- `GET /skins/list`: List skins according to the filter.
- `GET /backgrounds/list`: List backgrounds according to the filter.
- `GET /effects/list`: List effects according to the filter.
- `GET /particles/list`: List particles according to the filter.
- `GET /engines/list`: List engines according to the filter.
- `GET /levels/search`: Search levels page.
- `GET /skins/search`: Search skins page.
- `GET /backgrounds/search`: Search backgrounds page.
- `GET /effects/search`: Search effects page.
- `GET /particles/search`: Search particles page.
- `GET /engines/search`: Search engines page.
- `GET /levels/create`: Create level page.
- `GET /skins/create`: Create skin page.
- `GET /backgrounds/create`: Create background page.
- `GET /effects/create`: Create effect page.
- `GET /particles/create`: Create particle page.
- `GET /engines/create`: Create engine page.
- `GET /levels/jump/{page}`: Jump to level list page.
- `GET /skins/jump/{page}`: Jump to skin list page.
- `GET /backgrounds/jump/{page}`: Jump to background list page.
- `GET /effects/jump/{page}`: Jump to effect list page.
- `GET /particles/jump/{page}`: Jump to particle list page.
- `GET /engines/jump/{page}`: Jump to engine list page.
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

## Tips

### 2023.6.5

Accroding to the implementation of [sevenc-nanashi/chart_cyanvas](https://github.com/sevenc-nanashi/chart_cyanvas), we implemented the interface for logging in using Sonolus users.At present, the upper right part of the webpage is the login entry for the webpage.

We have rewritten the automatic compilation scripts for each environment, adding pre-compiled version of the executable file for `Linux` users. And for `Windows` users, we have changed the installation package to a compressed package containing executable files and runtime environment.

### 2023.1.21

~~Windows users need to execute program "Setup Sonolus Database" as administrator firstly, and then execute "Sonolus Server for Windows" to open the service. ~~

### 2023.1.19

If you want set port to `80/443`, you must have the administrator privilege.

### 2023.1.18

~~These days, the official wiki website has added an endpoint `/sonolus/authenticate` and an object `UserProfile`, but I couldn't find the interactive way with the Sonolus app. So I didn't add this function into the application.~~

## Third-party Repo

- [open-source-parsers/jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- libmysqlclient
- [openssl/openssl](https://github.com/openssl/openssl)
- [jquery/jquery](https://github.com/jquery/jquery)
- [dankogai/js-base64](https://github.com/dankogai/js-base64)
- [lyoj-dev/webserver](https://github.com/lyoj-dev/webserver)
- [pluma](https://github.com/aelzenaar/pluma)

## Plugins

- [libsonolush](https://github.com/SonolusHaniwa/sonolus-server-plugin-libsonolush)
- [libsonolusjs](https://github.com/SonolusHaniwa/sonolus-server-plugin-libsonolusjs)
- [libonedrive](https://github.com/SonolusHaniwa/sonolus-server-plugin-libonedrive) (Default: Disable)

## Upload Log

### v1.4.1 2023.6.9

1. Add a OneDrive upload and download interface.
2. Update workflows script

### v1.4.0 2023.6.5

1. Add a Sonolus user login interface and discard the old login interface.
2. Fixed some Windows compilation issues.
3. Update workflows scripts.

### v1.3.0 2023.5.21

1. Add plugin functionality.
2. Allow custom Sonolus Server version.
3. Update workflows scripts.

### v1.2.3 2023.4.16

1. Add package & test engine features.
2. Add create items button.
3. Add admin login interface.

### v1.2.2 2023.3.12

1. Add each component creation page.
2. Add a `file` type search component, which can be used to create pages.
3. Add `Windows` automatic packaging script.

### v1.2.1 2023.3.6

1. New support for SQLite database.
2. The data package format has been modified to provide support for SQLite.
3. Re-package the official engine with a new download link: [Data Packages for v1.2.1+](https://github.com/LittleYang0531/sonolus-server-cpp/releases/tag/v1.2.1-data)。
4. New component creation interfaces are added to support the creation of components in the next version.
5. Temporarily delete the `/levels/create` interface to facilitate GUI rewriting in the next version.

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