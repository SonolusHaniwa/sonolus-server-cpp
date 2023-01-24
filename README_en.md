# Sonolus Server for C++

[English](./README_en.md) [简体中文](./README.md)

A sample api server for Sonolus, use C++ & MySQL to develop. You can use it to test your own Sonolus engine or build a Sonolus server for yourself.

It can be deployed in any Linux devices and any Windows devices, even if a moblie phone also can compile the server successfully.

Use httpd core developed by myself, which also support you to build HTTPS server.

## Links

- [Sonolus Website](https://sonolus.com/)
- [Sonolus Wiki](https://wiki.sonolus.com/)

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

### Run

```bash
./main
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

## Upload Log

### v1.1.0 2023.1.24

1. Fixed Windows version will compiled error caused by missing __int64_t.
2. Fixed the error that the feedbacks of all list apis is the same.
3. Add the recommended display function, which displays 5 works of the same author by default.
4. Add a new homepage and use native js to write it after the official project.
5. Add the detail page of each component, and use native js to write after the official project.
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
