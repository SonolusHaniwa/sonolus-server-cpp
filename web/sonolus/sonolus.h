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

#define quickSendMsg(code) {\
    __api_default_response["Content-Length"] = to_string(json_encode(msg[code]).size()); \
    putRequest(conn, code, __api_default_response); \
    send(conn, json_encode(msg[code])); \
    exitRequest(conn); \
}

#define quickSendObj(obj) { \
    string E2BAAF3B97DBEEF01C0043275F9A0E7 = str_replace(dataPrefix.c_str(), appConfig["server.data.prefix"][atoi(getParam(request)["source"].c_str())]["url"].asCString(), json_encode(obj).c_str()); \
    __api_default_response["Content-Length"] = to_string(E2BAAF3B97DBEEF01C0043275F9A0E7.size()); \
    putRequest(conn, 200, __api_default_response); \
    send(conn, E2BAAF3B97DBEEF01C0043275F9A0E7); \
    exitRequest(conn); \
}

bool checkLogin(http_request request) {
    string session = cookieParam(request)["sessionId"];
    if (session == "") session = request.argv["sonolus-session"];
    auto res = db.query("SELECT session FROM UserSession WHERE session=\"" + session + "\" AND expire >= " + to_string(time(NULL)) + " AND uid != \"\"", "UserSession");
    return res.size();
}

UserProfile getUserProfile(http_request request) {
	if (!checkLogin(request)) return UserProfile();
    string session = cookieParam(request)["sessionId"];
    if (session == "") session = request.argv["sonolus-session"];
    auto res = db.query("SELECT uid FROM UserSession WHERE session=\"" + session + "\" AND expire >= " + to_string(time(NULL)) + " AND uid != \"\"", "UserSession");
    string uid = res[0]["uid"];
    return usersList("id = \"" + uid + "\"", "")[0];
}

string generateSession() {
    srand(time(NULL));
    string session = "", key = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < 32; i++) session += key[rand() % 62];
    return session;
}

#include"ServerInfo.h"
#include"ItemInfo.h"
#include"ItemList.h"
#include"ItemUpload.h"
#include"ItemDetails.h"
#include"ItemDetailsSubmit.h"
#include"ItemDetailsUpload.h"
#include"ItemCreate.h"
#include"Authentication.h"
#include"CheckLogin.h"
#include"ItemCommunityInfo.h"
#include"ItemCommunitySubmit.h"
#include"ItemCommunityCommentList.h"
#include"ItemCommunityCommentSubmit.h"
#include"ItemCommunity.h"
#include"ItemLeaderboardDetails.h"
#include"ItemLeaderboardRecordList.h"
#include"ItemLeaderboardRecordDetails.h"
#ifndef __EMSCRIPTEN__
#include"RoomJoin.h"
#include"RoomConnection.h"
#endif