/**
 * @file cgihttpd.h
 * @author LittleYang0531 (dev@lyoj.ml)
 * @brief CGI核心头文件
 * @version 1.0.5
 * @date 2022-09-18
 * 
 * @copyright Copyright (c) 2023 LittleYang0531
 * 
 */
const std::string httpd_version = "1.0.5";

#ifndef _HTTPD_H_
#define _HTTPD_H_
#define __windows__ (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))

#include<openssl/ssl.h>
#include<openssl/aes.h>
#include<openssl/err.h>
#include<openssl/ec.h>
#include<openssl/ecdsa.h>
#include<openssl/obj_mac.h>
using namespace std;

/** 日志输出库 */
enum LOG_LEVEL {
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO
};

/** 日志文件路径 */
string log_file_target = "./log.txt";
/** 日志输出类型 */
int target_id = 0;
/** 是否开启Debug模式 */
bool isDebug = false; 
ofstream log_fout;
pthread_mutex_t g_mutex_lock;

/** 日志目标枚举类型 */
enum LOG_TARGET {
    LOG_TARGER_NONE = 0,
    LOG_TARGET_CONSOLE = 1,
    LOG_TARGET_FILE = 2
};

/**
 * @brief 初始化日志结构
 * 
 * @param log_target 日志目标类型
 */
void log_init(int log_target) {
    target_id = log_target;
    if (log_fout.is_open()) log_fout.close();
    if (target_id & LOG_TARGET_FILE) log_fout.open(log_file_target.c_str(), ios::app);
}

#define writeLog(loglevel, dat) __writeLog(loglevel, __FILE__, __LINE__, dat)

/**
 * @brief 写入日志
 * 
 * @param loglevel 日志等级 
 * @param fileName 文件名
 * @param lineNumber 行号
 * @param dat 数据
 */
void __writeLog(LOG_LEVEL loglevel, string fileName, int lineNumber, string dat) {
    if (!isDebug && loglevel == LOG_LEVEL_DEBUG) return;

    /** 获取当前时间 */
    stringstream buffer, buffer2;
	time_t timep = time(&timep);
    char tmp[1024] = "";
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));

    /** 获取日志等级对应字符串 */
    string level_str = "";
    switch(loglevel) {
        case LOG_LEVEL_NONE: level_str = "NONE"; break;
        case LOG_LEVEL_INFO: level_str = "INFO"; break;
        case LOG_LEVEL_WARNING: level_str = "WARNING"; break;
        case LOG_LEVEL_ERROR: level_str = "ERROR"; break;
        case LOG_LEVEL_DEBUG: level_str = "DEBUG"; break;
        default: level_str = "UNKNOWN"; break;		      
    }

    /** 获取线程id */
    int tid = -1;

    /** 输出日志 */
    if (loglevel == LOG_LEVEL_ERROR) buffer << "\e[31m";
    else if (loglevel == LOG_LEVEL_WARNING) buffer << "\e[33m";
    else buffer << "\e[97m";
    buffer << "[" << tmp << "] [" << level_str << "] [tid:" << tid << "] [" << fileName << ":" << lineNumber << "] " << dat;
    buffer2 << "[" << tmp << "] [" << level_str << "] [tid:" << tid << "] [" << fileName << ":" << lineNumber << "] " << dat;
    buffer << "\e[0m";
    pthread_mutex_lock(&g_mutex_lock);
    if (target_id & LOG_TARGET_CONSOLE) cout << buffer.str() << endl;
    if (target_id & LOG_TARGET_FILE) log_fout << buffer2.str() << endl;
    pthread_mutex_unlock(&g_mutex_lock);
}

typedef map<string, string> argvar;
argvar _e, __default_response, __api_default_response;
string _endl = "<br/>";
map<string, argvar> http_mime;

/** 全局参数列表 */
#define HTTP_ENABLE_SSL 1
bool https = false; // 是否开启https
#define HTTP_LISTEN_HOST 2
string http_host = "0.0.0.0"; // 运行主机名
#define HTTP_LISTEN_PORT 3
int http_port = 8888; // 监听端口
#define HTTP_SSL_CACERT 4
string http_cacert = "cert.pem"; // 证书路径
#define HTTP_SSL_PRIVKEY 5
string http_privkey = "privkey.pem"; // 私钥路径
#define HTTP_MULTI_THREAD 6
int http_thread_num = 8; // 运行线程数
#define LOG_FILE_PATH 7 // 日志文件路径
#define LOG_TARGET_TYPE 8 // 日志输出类型
int log_target_type = LOG_TARGET_CONSOLE;
#define OPEN_DEBUG 9 // 是否开启Debug模式
/** 全局参数结束 */

struct client_conn {
    int thread_id;
};

void exitRequest(client_conn&);
void putRequest(client_conn&, int, argvar);
bool isCgi = true;
string cgiRequest, cgiResponse;
jmp_buf buf;

/**
 * @brief 发送信息
 * 
 * @param __fd 客户端连接符
 * @param __buf 信息主体
 * @return ssize_t 
 */
ssize_t send(client_conn __fd, string __buf) {
	return cgiResponse += __buf, __buf.size();
}

/**
 * @brief 以二进制形式发送信息
 * 
 * @param __fd 客户端连接符
 * @param __buf 信息主体
 * @param len 要发送的信息长度
 * @return ssize_t 
 */
ssize_t send(client_conn __fd, char* __buf, int len) {
    for (int i = 0; i < len; i++) cgiResponse.push_back(__buf[i]);
    return len;
}

/**
 * @brief 接收单个字符
 * 
 * @param __fd 客户端连接符
 * @return char
*/
char recvchar(client_conn __fd) {
    if (cgiRequest.size() == 0) return 0;
    char ch = cgiRequest.front();
    cgiRequest = cgiRequest.substr(1); return ch;
}

/**
 * @brief 接收信息
 * 
 * @param __fd 客户端连接符
 * @return string 
 */
string recv(client_conn __fd, int siz = -1) {
    string __buf = "";
    const int lim = 1e6;
    if (siz == -1) {
        int times = 0;
        while (__buf.size() < 4 || __buf.substr(__buf.size() - 4, 4) != "\r\n\r\n") {
            char ch = recvchar(__fd);
            if (ch == -1) {
                if (times <= lim) times++;
                else {
                    writeLog(LOG_LEVEL_WARNING, "Failed to recieve data!");
                    return "";    
                }
            }
            __buf.push_back(ch);
        } 
        writeLog(LOG_LEVEL_DEBUG, "Recieve " + to_string(__buf.size()) + " bytes from client.");
        return __buf;
    } else {
        while (__buf.size() != siz) {
            char ch = recvchar(__fd);
            if (ch == -1) {
                writeLog(LOG_LEVEL_WARNING, "Failed to recieve data!");
                return "";
            }
            __buf.push_back(ch);
        } 
        writeLog(LOG_LEVEL_DEBUG, "Recieve " + to_string(__buf.size()) + " bytes from client.");
        return __buf;
    }
}

struct http_request {
    string method = "";
    string path = "";
    string protocol = "";
    argvar argv;
    string postdata;
};

int sock;
string http_code[1024];

/**
 * @brief 初始化HTTP服务器
 * 
 */
void http_init() {
    writeLog(LOG_LEVEL_DEBUG, "Initializing WebServer Core...");

    /** 设置默认响应头 */
    __default_response["Server"] = "Web Server Version " + httpd_version;
    __default_response["Access-Control-Allow-Origin"] = "*";
    __default_response["Connection"] = "Keep-Alive";
    __default_response["Content-Type"] = "text/html; charset=utf-8";
    __default_response["Sonolus-Version"] = Sonolus_Version;
    __api_default_response = __default_response;
    __api_default_response["Content-Type"] = "application/json; charset=utf-8";
    writeLog(LOG_LEVEL_DEBUG, "Successfully make default Response Header!");
}

/**
 * @brief 拆散字符串
 * 
 * @param seperator 分隔符
 * @param source 源字符串
 * @return vector<string> 
 */
vector<string> explode(const char* seperator, const char* source) {
	string src = source; vector<string> res;
	while (src.find(seperator) != string::npos) {
		int wh = src.find(seperator);
		res.push_back(src.substr(0, src.find(seperator)));
		src = src.substr(wh + string(seperator).size());
	} res.push_back(src);
	return res;
}

/**
 * @brief 结束请求
 * 
 * @param conn 客户端连接符
 */
void exitRequest(client_conn& conn) {
	longjmp(buf, 1);
}

/**
 * @brief 获取HTTP请求头
 * 
 * @param conn 客户端连接符
 * @return http_request 
 */
http_request getRequest(client_conn& conn) {
    /** 获取请求头 */
    string s = recv(conn);
    if (s == "") {
        writeLog(LOG_LEVEL_WARNING, "Empty Request Header!");
        exitRequest(conn);
    }
    writeLog(LOG_LEVEL_DEBUG, "Recieved Request Header from client!");

    /** 判断请求方式 */
    vector<string> __arg = explode("\r\n", s.c_str());
    if (__arg.size() < 1) {
        writeLog(LOG_LEVEL_WARNING, "Invalid HTTP Request in line 1: eof!");
        stringstream buffer;
        buffer << "<html>" << endl;
        buffer << "<head><title>500 Internal Server Error</title></head>" << endl;
        buffer << "<body>" << endl;
        buffer << "<center><h1>500 Internal Server Error</h1></center>" << endl;
        buffer << "<hr><center>Made by <a href='https://github.com/lyoj-dev/webserver'>webserver</a> v" << httpd_version << "</center>" << endl;
        buffer << "</body>" << endl;
        buffer << "</html>" << endl;
        putRequest(conn, 500, __default_response);
        send(conn, buffer.str());
        exitRequest(conn);
    }
    writeLog(LOG_LEVEL_DEBUG, "Request Header lines: " + to_string(__arg.size()));


    /** 读取请求头的第一行 */
    vector<string> header = explode(" ", __arg[0].c_str());
    if (header.size() < 3 || (
        header[0] != "GET" && header[0] != "HEAD" && header[0] != "POST" &&
        header[0] != "PUT" && header[0] != "DELETE" && header[0] != "CONNECT" && 
        header[0] != "OPTIONS" && header[0] != "TRACE" && header[0] != "PATCH"
    )) {
        writeLog(LOG_LEVEL_WARNING, "Invalid HTTP Request in line 1: Invalid Request Method!");
        stringstream buffer;
        buffer << "<html>" << endl;
        buffer << "<head><title>500 Internal Server Error</title></head>" << endl;
        buffer << "<body>" << endl;
        buffer << "<center><h1>500 Internal Server Error</h1></center>" << endl;
        buffer << "<hr><center>Made by <a href='https://github.com/lyoj-dev/webserver'>webserver</a> v" << httpd_version << "</center>" << endl;
        buffer << "</body>" << endl;
        buffer << "</html>" << endl;
        putRequest(conn, 500, __default_response);
        send(conn, buffer.str());
        exitRequest(conn);
    }
    http_request request;
    request.method = header[0];
    request.path = header[1];
    request.protocol = header[2];
    writeLog(LOG_LEVEL_DEBUG, "Request Info: " + header[0] + " " + header[1] + " " + header[2]);

    /** 读取请求头参数 */
    int pt = 1;
    for (; pt < __arg.size(); pt++) {
        if (__arg[pt].find(": ") == string::npos) break;
        int __wh = __arg[pt].find(": ");
        string key = __arg[pt].substr(0, __wh);
        for (int i = 0; i < key.size(); i++) if (key[i] >= 'A' && key[i] <= 'Z') key[i] -= 'A', key[i] += 'a';
        string value = __arg[pt].substr(__wh + 2);
        request.argv.insert(make_pair(key, value));
    }
    if (request.argv["content-length"] != "0") request.postdata = recv(conn, atoi(request.argv["content-length"].c_str()));
    writeLog(LOG_LEVEL_DEBUG, "Request Parameters: " + to_string(request.argv.size()));

    /** 返回请求头信息 */
    return request;
}

/**
 * @brief 放置响应头
 * 
 * @param conn 客户端连接符
 * @param code 响应代码
 * @param argv 响应参数
 * @param content 输出内容
 */
void putRequest(client_conn& conn, int code, argvar argv) {

    /** 判断响应代码 */
    if (code <= 0 || code >= 1000 || http_code[code] == "") {
        writeLog(LOG_LEVEL_WARNING, "Invalid Response Code!");
        stringstream buffer;
        buffer << "<html>" << endl;
        buffer << "<head><title>500 Internal Server Error</title></head>" << endl;
        buffer << "<body>" << endl;
        buffer << "<center><h1>500 Internal Server Error</h1></center>" << endl;
        buffer << "<hr><center>Made by <a href='https://github.com/lyoj-dev/webserver'>webserver</a> v" << httpd_version << "</center>" << endl;
        buffer << "</body>" << endl;
        buffer << "</html>" << endl;
        putRequest(conn, 500, __default_response);
        send(conn, buffer.str());
        exitRequest(conn);
    } 
    writeLog(LOG_LEVEL_DEBUG, "Valid Response Code!");

    /** 构造响应头 */
    stringstream __buf;
    __buf << "HTTP/1.1 " << code << " " << http_code[code] << "\r\n";
    writeLog(LOG_LEVEL_DEBUG, "Response Info: HTTP/1.1 " + to_string(code) + " " + http_code[code]);
    for (auto it = argv.begin(); it != argv.end(); it++)
        __buf << (*it).first << ": " << (*it).second << "\r\n";
    __buf << "\r\n";

    /** 发送响应头 */
    writeLog(LOG_LEVEL_DEBUG, "Send Response Header to client");
    int s = send(conn, __buf.str());
}

/**
 * @brief 获取GET参数
 * 
 * @param request 请求头信息
 * @return argvar
 */
argvar getParam(http_request request) {
    writeLog(LOG_LEVEL_DEBUG, "Analysing GET parameters...");

    /** 读取路径信息 */
    string path = request.path;
    if (path.find("?") == string::npos) {
        writeLog(LOG_LEVEL_DEBUG, "Empty GET parameters!");
        return _e;
    }

    /** 提取参数信息 */
    string param = path.substr(path.find("?") + 1);
    vector<string> __arg = explode("&", param.c_str());
    writeLog(LOG_LEVEL_DEBUG, "GET parameter length: " + to_string(__arg.size()));

    /** 逐个处理 */
    argvar $_GET;
    for (int i = 0; i < __arg.size(); i++) {
        if (__arg[i].find("=") == string::npos) 
            writeLog(LOG_LEVEL_DEBUG, "Could find value of key \"" + __arg[i] + "\""),
            $_GET.insert(make_pair(__arg[i], ""));
        else {
            string key = __arg[i].substr(0, __arg[i].find("="));
            string val = __arg[i].substr(__arg[i].find("=") + 1);
            writeLog(LOG_LEVEL_DEBUG, "Add key \"" + key + "\" $_GET");
            $_GET.insert(make_pair(key, val));
        }
    } 
    
    /** 返回 */
    return $_GET;
}

/**
 * @brief 获取GET参数
 * 
 * @param param 参数
 * @return argvar
 */
argvar getParam(string param) {
    vector<string> __arg = explode("&", param.c_str());
    writeLog(LOG_LEVEL_DEBUG, "GET parameter length: " + to_string(__arg.size()));

    /** 逐个处理 */
    argvar $_GET;
    for (int i = 0; i < __arg.size(); i++) {
        if (__arg[i].find("=") == string::npos) 
            writeLog(LOG_LEVEL_DEBUG, "Could find value of key \"" + __arg[i] + "\""),
            $_GET.insert(make_pair(__arg[i], ""));
        else {
            string key = __arg[i].substr(0, __arg[i].find("="));
            string val = __arg[i].substr(__arg[i].find("=") + 1);
            writeLog(LOG_LEVEL_DEBUG, "Add key \"" + key + "\" $_GET");
            $_GET.insert(make_pair(key, val));
        }
    } 
    
    /** 返回 */
    return $_GET;
}

/**
 * @brief 将GET参数字符串化
 * 
 * @param $_GET GET参数
 * @return string
 */
string getStringfy(argvar $_GET) {
    string res = "";
    for (auto v : $_GET) {
        if (v.first == "" && v.second == "") continue;
        res += v.first + "=" + v.second + "&";
    } if (res.size()) res.pop_back();
    return res;
}

/**
 * @brief 获取POST参数
 * 
 * @param request 请求头信息
 * @return argvar 
 */
argvar postParam(http_request request) {
    writeLog(LOG_LEVEL_DEBUG, "Analysing POST parameters...");


    /** 提取参数信息 */
    vector<string> __arg = explode("&", request.postdata.c_str());
    writeLog(LOG_LEVEL_DEBUG, "POST parameter length: " + to_string(__arg.size()));

    /** 逐个处理 */
    argvar $_POST;
    for (int i = 0; i < __arg.size(); i++) {
        if (__arg[i] == "") continue;
        if (__arg[i].find("=") == string::npos) 
            writeLog(LOG_LEVEL_DEBUG, "Could find value of key \"" + __arg[i] + "\""),
            $_POST.insert(make_pair(__arg[i], ""));
        else {
            string key = __arg[i].substr(0, __arg[i].find("="));
            string val = __arg[i].substr(__arg[i].find("=") + 1);
            writeLog(LOG_LEVEL_DEBUG, "Add key \"" + key + "\" to $_POST");
            $_POST.insert(make_pair(key, val));
        }
    } 
    
    /** 返回 */
    return $_POST;
}

/**
 * @brief 获取COOKIE参数
 * 
 * @param request 请求头信息
 * @return argvar 
 */
argvar cookieParam(http_request request) {
    writeLog(LOG_LEVEL_DEBUG, "Analysing COOKIE parameters...");
    
    /**  获取Cookie字符串 */
    if (request.argv.find("cookie") == request.argv.end()) {
        writeLog(LOG_LEVEL_DEBUG, "Empty COOKIE parameters!");
        return _e;
    }
    string s = request.argv["cookie"];

    /** 拆散字符串 */
    vector<string> arr = explode("; ", s.c_str());
    writeLog(LOG_LEVEL_DEBUG, "COOKIE parameter length: " + to_string(arr.size()));
    argvar $_COOKIE;
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i].find("=") != string::npos) {
            string key = arr[i].substr(0, arr[i].find("="));
            string val = arr[i].substr(arr[i].find("=") + 1);
            writeLog(LOG_LEVEL_DEBUG, "Add key \"" + key + "\" to $_COOKIE");
            $_COOKIE.insert(make_pair(key, val));
        } else writeLog(LOG_LEVEL_WARNING, "Invalid COOKIE parameter!");
    }

    /** 返回 */
    return $_COOKIE;
}

/**
 * @brief 合并两个数组
 * 
 * @param a 
 * @param b 
 * @return argvar 
 */
argvar merge(argvar a, argvar b) {
    for (auto it : b) a[it.first] = it.second;
    return a;
}

/**
 * @brief 获取mime类型
 * 
 * @param ext 文件后缀名
 * @return argvar 
 */
argvar mime(string ext) {
    argvar _r = _e;
    if (ext == ".aac") _r["Content-Type"] = "audio/aac";
    else if (ext == ".abw") _r["Content-Type"] = "application/x-abiword";
    else if (ext == ".arc") _r["Content-Type"] = "application/x-freearc";
    else if (ext == ".avi") _r["Content-Type"] = "video/x-msvideo";
    else if (ext == ".azw") _r["Content-Type"] = "application/vnd.amazon.ebook";
    else if (ext == ".bin") _r["Content-Type"] = "application/octet-stream";
    else if (ext == ".bmp") _r["Content-Type"] = "image/bmp";
    else if (ext == ".bz") _r["Content-Type"] = "application/x-bzip";
    else if (ext == ".bz2") _r["Content-Type"] = "application/x-bzip2";
    else if (ext == ".csh") _r["Content-Type"] = "application/x-csh";
    else if (ext == ".css") _r["Content-Type"] = "text/css";
    else if (ext == ".csv") _r["Content-Type"] = "text/csv";
    else if (ext == ".doc") _r["Content-Type"] = "application/msword";
    else if (ext == ".docx") _r["Content-Type"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    else if (ext == ".eot") _r["Content-Type"] = "application/vnd.ms-fontobject";
    else if (ext == ".epub") _r["Content-Type"] = "application/epub+zip";
    else if (ext == ".gif") _r["Content-Type"] = "image/gif";
    else if (ext == ".htm") _r["Content-Type"] = "text/html";
    else if (ext == ".html") _r["Content-Type"] = "text/html";
    else if (ext == ".ico") _r["Content-Type"] = "image/vnd.microsoft.icon";
    else if (ext == ".ics") _r["Content-Type"] = "text/calendar";
    else if (ext == ".jar") _r["Content-Type"] = "application/java-archive";
    else if (ext == ".jpeg") _r["Content-Type"] = "image/jpeg";
    else if (ext == ".jpg") _r["Content-Type"] = "image/jpeg";
    else if (ext == ".js") _r["Content-Type"] = "text/javascript";
    else if (ext == ".json") _r["Content-Type"] = "application/json";
    else if (ext == ".jsonld") _r["Content-Type"] = "application/ld+json";
    else if (ext == ".mid") _r["Content-Type"] = "audio/midi";
    else if (ext == ".midi") _r["Content-Type"] = "audio/midi";
    else if (ext == ".mjs") _r["Content-Type"] = "text/javascript";
    else if (ext == ".mp3") _r["Content-Type"] = "audio/mpeg";
    else if (ext == ".mp4") _r["Content-Type"] = "video/mp4";
    else if (ext == ".mpeg") _r["Content-Type"] = "video/mpeg";
    else if (ext == ".mpkg") _r["Content-Type"] = "application/vnd.apple.installer+xml";
    else if (ext == ".odp") _r["Content-Type"] = "application/vnd.oasis.opendocument.presentation";
    else if (ext == ".ods") _r["Content-Type"] = "application/vnd.oasis.opendocument.spreadsheet";
    else if (ext == ".odt") _r["Content-Type"] = "application/vnd.oasis.opendocument.text";
    else if (ext == ".oga") _r["Content-Type"] = "audio/ogg";
    else if (ext == ".ogv") _r["Content-Type"] = "video/ogg";
    else if (ext == ".ogx") _r["Content-Type"] = "application/ogg";
    else if (ext == ".otf") _r["Content-Type"] = "font/otf";
    else if (ext == ".png") _r["Content-Type"] = "image/png";
    else if (ext == ".pdf") _r["Content-Type"] = "application/pdf";
    else if (ext == ".ppt") _r["Content-Type"] = "application/vnd.ms-powerpoint";
    else if (ext == ".pptx") _r["Content-Type"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    else if (ext == ".rar") _r["Content-Type"] = "application/x-rar-compressed";
    else if (ext == ".rtf") _r["Content-Type"] = "application/rtf";
    else if (ext == ".sh") _r["Content-Type"] = "application/x-sh";
    else if (ext == ".svg") _r["Content-Type"] = "image/svg+xml";
    else if (ext == ".swf") _r["Content-Type"] = "application/x-shockwave-flash";
    else if (ext == ".tar") _r["Content-Type"] = "application/x-tar";
    else if (ext == ".tif") _r["Content-Type"] = "image/tiff";
    else if (ext == ".tiff") _r["Content-Type"] = "image/tiff";
    else if (ext == ".ttf") _r["Content-Type"] = "font/ttf";
    else if (ext == ".txt") _r["Content-Type"] = "text/plain";
    else if (ext == ".vsd") _r["Content-Type"] = "application/vnd.visio";
    else if (ext == ".wav") _r["Content-Type"] = "audio/wav";
    else if (ext == ".weba") _r["Content-Type"] = "audio/webm";
    else if (ext == ".webm") _r["Content-Type"] = "video/webm";
    else if (ext == ".webp") _r["Content-Type"] = "image/webp";
    else if (ext == ".woff") _r["Content-Type"] = "font/woff";
    else if (ext == ".woff2") _r["Content-Type"] = "font/woff2";
    else if (ext == ".xhtml") _r["Content-Type"] = "application/xhtml+xml";
    else if (ext == ".xls") _r["Content-Type"] = "application/vnd.ms-excel";
    else if (ext == ".xlsx") _r["Content-Type"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    else if (ext == ".xml") _r["Content-Type"] = "text/xml";
    else if (ext == ".xul") _r["Content-Type"] = "application/vnd.mozilla.xul+xml";
    else if (ext == ".zip") _r["Content-Type"] = "application/zip";
    else if (ext == ".3gp") _r["Content-Type"] = "video/3gpp";
    else if (ext == ".3g2") _r["Content-Type"] = "video/3gpp2";
    else if (ext == ".7z") _r["Content-Type"] = "application/x-7z-compressed";
    else _r["Content-Type"] = "text/plain";
    writeLog(LOG_LEVEL_DEBUG, "Get Content-Type for " + ext + ": " + _r["Content-Type"]);
    return _r;
}

typedef vector<string> param;

class application {
    public: 
        struct r {
            string path;
            function<void(client_conn, http_request, param)> main;
            r(){}
            r(string path, function<void(client_conn, http_request, param)> main):path(path),main(main){}
        }; 
        vector<r> route;

        /**
         * @brief 判断是否为整数
         * 
         * @param x 传入参数
         * @return true 
         * @return false 
         */
        bool isInt(string x) {
            if (x.size() == 0) return false;
            int st = 0; 
            if (x[0] == '-') st++;
            for (int i = st; i < x.size(); i++) 
                if (x[i] < '0' || x[i] > '9') return false;
            return true;
        }

        /**
         * @brief 判断是否为小数
         * 
         * @param x 传入参数
         * @return true 
         * @return false 
         */
        bool isDouble(string x) {
            if (x.size() == 0) return false;
            bool pointed = false; int st = 0;
            if (x[0] == '-') st++;
            for (int i = st; i < x.size(); i++) {
                if (x[i] == '.') {
                    if (!pointed) pointed = true;
                    else return false;
                } else if (x[i] < '0' || x[i] > '9') return false;
            } return true;
        }

        /**
         * @brief 匹配路径
         * 
         * @param __route 路由结构体
         * @param path 匹配串
         * @return true 匹配成功
         * @return false 匹配失败
         */
        bool matchPath(r __route, string path) {
            /** 拆散字符串 */
            vector<string> __goal = explode("/", __route.path.c_str());
            vector<string> __path = explode("/", path.c_str());
            while (__goal.size() && __goal.back() == "") __goal.pop_back();
            while (__path.size() && __path.back() == "") __path.pop_back();
            if (__goal.size() != __path.size()) return false;

            /** 逐个判断 */
            for (int i = 0; i < __goal.size(); i++) {
                if (__goal[i] == "%d" || __goal[i] == "%D") {
                    if (!isInt(__path[i])) return false;
                } else if (__goal[i] == "%f" || __goal[i] == "%F") {
                    if (!isDouble(__path[i])) return false;
                } else if (__goal[i] == "%s" || __goal[i] == "%S") {
                    
                } else {
                    if (__goal[i] != __path[i]) return false;
                }
            } return true;
        }

        /**
         * @brief 添加路由
         * 
         * @param path 路由路径
         * @param func 执行函数
         */
        void addRoute(string path, function<void(client_conn, http_request, param)> func) {
            route.push_back(r(path, func));
        }

        /**
         * @brief 程序运行主函数
         * 
         * @param host 主机名
         * @param port 运行端口
         */
		void cgiRun() {
			isCgi = true;

			int res = setjmp(buf);
			if (res == 0) {
				client_conn conn;
				conn.thread_id = 0;
				http_request request = getRequest(conn);

				/** 提取路径 */
				string rlpath = request.path;
				if (rlpath.find("?") != string::npos) rlpath = rlpath.substr(0, rlpath.find("?"));

				/** 分发路由 */
				for (int i = 0; i < route.size(); i++) {
					if (matchPath(route[i], rlpath)) {
						writeLog(LOG_LEVEL_DEBUG, "Matched route \"" + route[i].path + "\"");

						/** 参数提取 */
						param argv;
						string __goal = route[i].path;
						string __path = rlpath;
						vector<string> __a1 = explode("/", __goal.c_str());
						vector<string> __a2 = explode("/", __path.c_str());
						for (int j = 0; j < __a1.size(); j++)
							if (__a1[j] == "%d" || __a1[j] == "%D" ||
								__a1[j] == "%f" || __a1[j] == "%F" ||
								__a1[j] == "%s" || __a1[j] == "%S") argv.push_back(__a2[j]);

						/** 主函数执行 */
						route[i].main(conn, request, argv);
						putRequest(conn, 200, __default_response);
						send(conn, "");
						exitRequest(conn);
						break;
					}
				}
			} else {
				return;
			}
		}
}app;

/** url解码字表 */
static unsigned char dec_tab[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0,  0,  0,  0,  0,
    0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0, 10, 11, 12, 13, 14, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

/**
 * @brief url解码
 * 
 * @param str 源字符串
 * @return string
 */
string urldecode(string str){
    int len = str.size();
    char *tmp = (char *)malloc(len + 1);
    int i = 0, pos = 0;
    for (i = 0; i < len; i++) {
        if (str[i] != '%') tmp[pos] = str[i];
        else if (i + 2 >= len) {
            tmp[pos++] = '%';
            if (++i >= len) break;
            tmp[pos] = str[i];
            break;
        } else if (isalnum(str[i + 1]) && isalnum(str[i + 2])) {
            tmp[pos] = (dec_tab[(unsigned char) str[i + 1]] << 4) + dec_tab[(unsigned char) str[i + 2]];
            i += 2;
        } else tmp[pos] = str[i];
        pos++;
    }
    tmp[pos] = '\0';
    return tmp;
}

string str_replace(string, string, string, bool);
string str_replace(string source, argvar argv) {
    for (auto it : argv) {
        source = str_replace("{{" + it.first + "}}", it.second, source, true);
    } for (auto it : argv) {
        source = str_replace("{{" + it.first + "}}", it.second, source, true);
    } for (auto it = argv.rbegin(); it != argv.rend(); it++) {
        if ((*it).first.size() < 9) continue;
        if ((*it).first.substr(0, 9) != "language.") continue;
        source = str_replace("#" + (*it).first.substr(9), (*it).second, source, true);
    } return source;
}

string readFile(string path) {
    ifstream fin(path.c_str());
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    if (len == -1) return "";
    fin.seekg(0, ios::beg);
    char* ch = new char[len];
    fin.read(ch, len);
    string buffer;
    for (int i = 0; i < len; i++) buffer.push_back(ch[i]);
    delete[] ch;
    return buffer;
}

time_t getMilliSeconds() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

argvar transfer(Json::Value source, string prefix = "") {
    argvar result;
    Json::Value::Members mem = source.getMemberNames();
    for (auto it = mem.begin(); it != mem.end(); it++) {
        if (source[*it].type() == Json::objectValue || source[*it].type() == Json::arrayValue) continue;
        result[prefix + (*it)] = source[*it].asString();
    } return result;
}

#endif
