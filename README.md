# Sonolus Server for C++

A sample api server for Sonolus, use C++ & MySQL to develop. You can use it to test your own Sonolus engine or build a Sonolus server for yourself.

It can be deployed in any Linux devices, even if a moblie phone also can compile the server successfully.

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
- `server.listenHost`: Listening address for WebServer Core.
- `server.listenPort`: Listening port for WebServer Core.
- `server.enableSSL`: Whether the WebServer Core use HTTPS.
- `server.httpsCacert`: Server's CA Certificate.
- `server.httpsPrivate`: Server's Private Certificate.
- `server.threadNumber`: The number of multithread.
- `logSystem.debug`: Whether the WebServer Core output DEBUG info.
- `logSystem.target`: Where the log info was output. `0` for none, `1` for Console, `2` for Log File, `3` for Console & Log File. 
- `logSystem.targetFile`: Log file path.

## Tips

### 2023.1.18

These days, the official wiki website has added an endpoint `/sonolus/authenticate` and an object `UserProject`, but I couldn't find the interactive way with the Sonolus app. So I didn't add this function into the application.

## Upload Log

### v1.0.1 2023.1.18

1. Merge all the configuration into `./config/config.json`
2. Optimize the CPU usage of WebServer Core

### v1.0.0 2023.1.10

1. Release the first version