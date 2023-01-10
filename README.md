# Sonolus Server for C++

A sample api server for Sonolus, use C++ & MySQL to develop. You can use it to test your own Sonolus engine or build a Sonolus server for your self.

It can be deployed in any Linux devices, even if a moblie phone also can compile the server successfully.

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

- Edit `./config/*` for all search configuration.
- Edit `main.cpp` for MySQL Server Information.
- Edit `httpd.h` for HTTP Server Information.

### Compile

```bash
g++ main.cpp -o main -lpthread -lcrypto -lssl -ljsoncpp -lmysqlclient
```

### Run

```bash
./main
```
