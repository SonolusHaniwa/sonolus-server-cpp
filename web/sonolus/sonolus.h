using namespace std;

Json::Value msg[1024];
void loadDefaultVariable() {
    for (int i = 1; i <= 1000; i++) msg[i]["code"] = i;
    msg[100]["msg"] = "100 Continue";
    msg[101]["msg"] = "101 Switching Protocols";

    msg[200]["msg"] = "200 OK";
    msg[201]["msg"] = "201 Created";
    msg[202]["msg"] = "202 Accepted";
    msg[203]["msg"] = "203 Non-Authoritative Information";
    msg[204]["msg"] = "204 No Content";
    msg[205]["msg"] = "205 Reset Content";
    msg[206]["msg"] = "206 Partial Content";

    msg[300]["msg"] = "300 Multiple Choices";
    msg[301]["msg"] = "301 Moved Permanently";
    msg[302]["msg"] = "302 Found";
    msg[303]["msg"] = "303 See Other";
    msg[304]["msg"] = "304 Not Modified";
    msg[305]["msg"] = "305 Use Proxy";
    msg[306]["msg"] = "306 Unused";
    msg[307]["msg"] = "307 Temporary Redirect";

    msg[400]["msg"] = "400 Bad Request";
    msg[401]["msg"] = "401 Unauthorized";
    msg[402]["msg"] = "402 Payment Required";
    msg[403]["msg"] = "403 Forbidden";
    msg[404]["msg"] = "404 Not Found";
    msg[405]["msg"] = "405 Method Not Allowed";
    msg[406]["msg"] = "406 Not Acceptable";
    msg[407]["msg"] = "407 Proxy Authentication Required";
    msg[408]["msg"] = "408 Request Time-out";
    msg[409]["msg"] = "409 Conflict";
    msg[410]["msg"] = "410 Gone";
    msg[411]["msg"] = "411 Length Required";
    msg[412]["msg"] = "412 Precondition Failed";
    msg[413]["msg"] = "413 Request Entity Too Large";
    msg[414]["msg"] = "414 Request-URI Too Large";
    msg[415]["msg"] = "415 Unsupported Media Type";
    msg[416]["msg"] = "416 Requested range not satisfiable";
    msg[417]["msg"] = "417 Expectation Failed";

    msg[500]["msg"] = "500 Internal Server Error";
    msg[501]["msg"] = "501 Not Implemented";
    msg[502]["msg"] = "502 Bad Gateway";
    msg[503]["msg"] = "503 Service Unavailable";
    msg[504]["msg"] = "504 Gateway Time-out";
    msg[505]["msg"] = "505 HTTP Version not supported";
}

#define defineToString(str) #str

#define quickSendMsg(code) {\
    __api_default_response["Content-Length"] = to_string(json_encode(msg[code]).size()); \
    putRequest(conn, code, __api_default_response); \
    send(conn, json_encode(msg[code])); \
    exitRequest(conn); \
}

#define quickSendObj(obj) {\
    __api_default_response["Content-Length"] = to_string(json_encode(obj).size()); \
    putRequest(conn, 200, __api_default_response); \
    send(conn, json_encode(obj)); \
    exitRequest(conn); \
}

void checkSecret(string token, client_conn conn) {
    if (token != appConfig["admin.password"].asString()) quickSendMsg(401);
}

#include"ServerInfo.h"
#include"ItemInfo.h"
// #include"levels_list.h"
// #include"skins_list.h"
// #include"backgrounds_list.h"
// #include"effects_list.h"
// #include"particles_list.h"
// #include"engines_list.h"
// #include"replays_list.h"
// #include"levels.h"
// #include"skins.h"
// #include"backgrounds.h"
// #include"effects.h"
// #include"particles.h"
// #include"engines.h"
// #include"replays.h"
// #include"levels_create.h"
// #include"skins_create.h"
// #include"backgrounds_create.h"
// #include"effects_create.h"
// #include"particles_create.h"
// #include"engines_create.h"
// #include"replays_create.h"
// #include"./studios/skins_create.h"
// #include"./studios/backgrounds_create.h"
// #include"./studios/effects_create.h"
