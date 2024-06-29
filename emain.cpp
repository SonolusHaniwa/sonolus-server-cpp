#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
#include<functional>
#include<fstream>
#include<sstream>
#include<setjmp.h>
#include<chrono>
#include<assert.h>
#include<jsoncpp/json/json.h>
using namespace std;
#define defineToString(str) #str

std::string sonolus_server_version = "1.6.2";
std::string Maximum_Sonolus_Version = "0.8.3";
std::string Sonolus_Version = Maximum_Sonolus_Version;
Json::Value appConfig;
Json::Value i18n, i18n_raw;
std::string dataPrefix = "/data/";

int levelVersion = 1;
int skinVersion = 3;
int backgroundVersion = 2;
int effectVersion = 5;
int particleVersion = 2;
int engineVersion = 8;
int replayVersion = 1;
int postVersion = 1;
int playlistVersion = 1;

vector<string> levelVersionList = {"0.0.0"};
vector<string> skinVersionList = {"0.0.0", "0.5.8", "0.7.0", "0.7.3"};
vector<string> backgroundVersionList = {"0.0.0", "0.5.8"};
vector<string> effectVersionList = {"0.0.0", "0.5.8", "0.6.0", "0.6.4", "0.7.0"};
vector<string> particleVersionList = {"0.0.0", "0.7.0", "0.8.0"};
vector<string> engineVersionList = {"0.0.0", "0.0.0", "0.0.0", "0.5.8", "0.5.13", "0.6.0", "0.6.4", "0.7.0", "0.7.2", "0.7.3", "0.7.4", "0.8.0"};
vector<string> replayVersionList = {"0.0.0"};
vector<string> postVersionList = {"0.0.0"};
vector<string> playlistVersionList = {"0.0.0"};

#include"modules/cgihttpd.h"
#include"modules/json.h"
#include"modules/html.h"
#include"modules/mysqli.h"
#include"modules/encrypt.h"
#include"core/items.h"
#include"web/import.h"

void routerRegister() {
    app.route.clear();

    app.addRoute("/data/%s", downloader);
    app.addRoute("/js/%s", js_import);
    app.addRoute("/css/%s", css_import);

    app.addRoute("/sonolus/info", ServerInfo);
    app.addRoute("/sonolus/%s/info", SonolusInfo);
    app.addRoute("/sonolus/%s/list", SonolusList);
    app.addRoute("/sonolus/%s/create", SonolusCreate);
    // app.addRoute("/sonolus/%s/upload", SonolusUpload);
    // app.addRoute("/sonolus/rooms/%s", SonolusRoomJoin);
    app.addRoute("/sonolus/%s/%s", SonolusDetails);
    app.addRoute("/sonolus/%s/%s/community", SonolusCommunity);
    app.addRoute("/sonolus/%s/%s/community/info", SonolusCommunityInfo);
    app.addRoute("/sonolus/%s/%s/community/submit", SonolusCommunitySubmit);
    app.addRoute("/sonolus/%s/%s/community/comments/list", SonolusCommunityCommentList);
    app.addRoute("/sonolus/%s/%s/leaderboards/%s", SonolusLeaderboardDetails);
    app.addRoute("/sonolus/%s/%s/leaderboards/%s/records/list", SonolusLeaderboardRecordList);
    app.addRoute("/sonolus/%s/%s/leaderboards/%s/records/%s", SonolusLeaderboardRecordDetails);
    app.addRoute("/sonolus/authenticate", Authentication);
    app.addRoute("/sonolus/checklogin", CheckLogin);

    app.addRoute("/", GUIIndex);
    app.addRoute("/index", GUIIndex);
    app.addRoute("/%s/info", GUIInfo);
    app.addRoute("/%s/list", GUIList);
    app.addRoute("/%s/search", GUISearch);
    app.addRoute("/%s/jump/%d", GUIJump);
    app.addRoute("/%s/create", GUICreate);
    app.addRoute("/%s/%s", GUIDetails);
    app.addRoute("/%s/%s/community/comments/list", GUICommunityCommentList);
    app.addRoute("/%s/%s/community/comments/jump/%d", GUICommunityCommentJump);
    app.addRoute("/%s/%s/edit", GUIEdit);

    // app.addRoute("/uploader", uploader);
}

string cgiRunner(string request) {
	// 适配 Resource Version
	levelVersion = upper_bound(levelVersionList.begin(), levelVersionList.end(), Sonolus_Version) - levelVersionList.begin();
	skinVersion = upper_bound(skinVersionList.begin(), skinVersionList.end(), Sonolus_Version) - skinVersionList.begin();
	backgroundVersion = upper_bound(backgroundVersionList.begin(), backgroundVersionList.end(), Sonolus_Version) - backgroundVersionList.begin();
	effectVersion = upper_bound(effectVersionList.begin(), effectVersionList.end(), Sonolus_Version) - effectVersionList.begin();
	particleVersion = upper_bound(particleVersionList.begin(), particleVersionList.end(), Sonolus_Version) - particleVersionList.begin();
	engineVersion = upper_bound(engineVersionList.begin(), engineVersionList.end(), Sonolus_Version) - engineVersionList.begin();
    replayVersion = upper_bound(replayVersionList.begin(), replayVersionList.end(), Sonolus_Version) - replayVersionList.begin();
    postVersion = upper_bound(postVersionList.begin(), postVersionList.end(), Sonolus_Version) - postVersionList.begin();
    playlistVersion = upper_bound(playlistVersionList.begin(), playlistVersionList.end(), Sonolus_Version) - playlistVersionList.begin();

	routerRegister();

	cgiRequest = request;
	app.cgiRun();
	return cgiResponse;
}

void preload() {
    // 先初始化一遍日志系统
    log_init(log_target_type);

    log_init(log_target_type);
    vector<string> config_lists = {
        "./config/config.json",
        "./config/singleplayer_config.json",
        "./config/multiplayer_config.json",
        "./config/level_config.json",
        "./config/skin_config.json",
        "./config/background_config.json",
        "./config/effect_config.json",
        "./config/particle_config.json",
        "./config/engine_config.json",
        "./config/replay_config.json",
        "./config/post_config.json",
        "./config/playlist_config.json",
        "./config/room_config.json"
    };
    for (int i = 0; i < config_lists.size(); i++) {
        string json = readFile(config_lists[i]);
        Json::Value obj;
        json_decode(json, obj);
        json_merge(appConfig, obj);
    }
    Json::Value dbConfig = json_decode(readFile("./config/database.json"));
    db = DB_Total_Controller(dbConfig);

    Sonolus_Version = appConfig["sonolus.version"].asString();
    if (appConfig["server.data.prefix"].asString() == "") appConfig["server.data.prefix"] = "/data/";
    dataPrefix = appConfig["server.data.prefix"].asString();
    appConfig["server.logo"] = dataPrefix + appConfig["server.logo"].asString();
    appConfig["server.bannerHash"] = appConfig["server.banner"].asString();
    appConfig["server.bannerUrl"] = dataPrefix + appConfig["server.banner"].asString();
    appConfig["database"] = "emscripten";
    log_init(log_target_type);
    http_init();
    loadConfig();
    loadDefaultVariable();

    /** 设置HTTP代码解释 */
    http_code[100] = "Continue";
    http_code[101] = "Switching Protocols";

    http_code[200] = "OK";
    http_code[201] = "Created";
    http_code[202] = "Accepted";
    http_code[203] = "Non-Authoritative Information";
    http_code[204] = "No Content";
    http_code[205] = "Reset Content";
    http_code[206] = "Partial Content";

    http_code[300] = "Multiple Choices";
    http_code[301] = "Moved Permanently";
    http_code[302] = "Found";
    http_code[303] = "See Other";
    http_code[304] = "Not Modified";
    http_code[305] = "Use Proxy";
    http_code[306] = "Unused";
    http_code[307] = "Temporary Redirect";

    http_code[400] = "Bad Request";
    http_code[401] = "Unauthorized";
    http_code[402] = "Payment Required";
    http_code[403] = "Forbidden";
    http_code[404] = "Not Found";
    http_code[405] = "Method Not Allowed";
    http_code[406] = "Not Acceptable";
    http_code[407] = "Proxy Authentication Required";
    http_code[408] = "Request Time-out";
    http_code[409] = "Conflict";
    http_code[410] = "Gone";
    http_code[411] = "Length Required";
    http_code[412] = "Precondition Failed";
    http_code[413] = "Request Entity Too Large";
    http_code[414] = "Request-URI Too Large";
    http_code[415] = "Unsupported Media Type";
    http_code[416] = "Requested range not satisfiable";
    http_code[417] = "Expectation Failed";

    http_code[500] = "Internal Server Error";
    http_code[501] = "Not Implemented";
    http_code[502] = "Bad Gateway";
    http_code[503] = "Service Unavailable";
    http_code[504] = "Gateway Time-out";
    http_code[505] = "HTTP Version not supported";
}

#ifndef EM_PORT_API
#	if defined(__EMSCRIPTEN__)
#		include <emscripten.h>
#		if defined(__cplusplus)
#			define EM_PORT_API(rettype) extern "C" rettype EMSCRIPTEN_KEEPALIVE
#		else
#			define EM_PORT_API(rettype) rettype EMSCRIPTEN_KEEPALIVE
#		endif
#	else
#		if defined(__cplusplus)
#			define EM_PORT_API(rettype) extern "C" rettype
#		else
#			define EM_PORT_API(rettype) rettype
#		endif
#	endif
#endif

extern "C" EMSCRIPTEN_KEEPALIVE void cgi(char* requestId, int len) {
	preload();
	string data = "";
	for (int i = 0; i < len; i++) data.push_back(requestId[i]);
	string res = cgiRunner(readFile("./request_" + data));
    ofstream fout("./response_" + data);
    fout.write(res.c_str(), res.size());
    fout.close();
}
