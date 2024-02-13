#define ENABLE_SQLITE
#include<iostream>
#include<vector>
#include<set>
#include<map>
#include<algorithm>
#include<functional>
#include<fstream>
#include<sstream>
#include<setjmp.h>
#include<jsoncpp/json/json.h>
using namespace std;

std::string sonolus_server_version = "1.4.8";
std::string Maximum_Sonolus_Version = "0.8.0";
std::string Minimum_Sonolus_Version = "0.8.0";
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
vector<string> particleVersionList = {"0.0.0", "0.7.0"};
vector<string> engineVersionList = {"0.0.0", "0.0.0", "0.0.0", "0.5.8", "0.5.13", "0.6.0", "0.6.4", "0.7.0", "0.7.2", "0.7.3", "0.7.4", "0.7.5"};
vector<string> replayVersionList = {"0.0.0"};
vector<string> postVersionList = {"0.0.0"};
vector<string> playlistVersionList = {"0.0.0"};

#include"modules/cgihttpd.h"
#include"modules/html.h"
#include"modules/mysqli.h"
#include"modules/json.h"
#include"modules/encrypt.h"
DB_Controller db;
#include"items/Items.h"
#include"web/import.h"

void routerRegister() {
    app.route.clear();

    app.addRoute("/data/%s", downloader);
    app.addRoute("/js/%s", js_import);
    app.addRoute("/css/%s", css_import);

    app.addRoute("/sonolus/info", ServerInfo);
    app.addRoute("/sonolus/%s/info", SonolusInfo);
    app.addRoute("/sonolus/%s/list", SonolusList);
    app.addRoute("/sonolus/%s/%s", SonolusDetails);
    app.addRoute("/sonolus/authenticate", Authentication);
    // app.addRoute("/sonolus/levels/create", sonolus_levels_create);
    // app.addRoute("/sonolus/skins/create", sonolus_skins_create);
    // app.addRoute("/sonolus/backgrounds/create", sonolus_backgrounds_create);
    // app.addRoute("/sonolus/effects/create", sonolus_effects_create);
    // app.addRoute("/sonolus/particles/create", sonolus_particles_create);
    // app.addRoute("/sonolus/engines/create", sonolus_engines_create);
    // app.addRoute("/sonolus/replays/create", sonolus_replays_create);

    app.addRoute("/", GUIIndex);
    app.addRoute("/index", GUIIndex);
    app.addRoute("/%s/info", GUIInfo);
    app.addRoute("/%s/list", GUIList);
    app.addRoute("/%s/search", GUISearch);
    app.addRoute("/%s/jump/%d", GUIJump);
    app.addRoute("/%s/%s", GUIDetails);
    // app.addRoute("/levels/create", web_levels_create);
    // app.addRoute("/skins/create", web_skins_create);
    // app.addRoute("/backgrounds/create", web_backgrounds_create);
    // app.addRoute("/effects/create", web_effects_create);
    // app.addRoute("/particles/create", web_particles_create);
    // app.addRoute("/engines/create", web_engines_create);
    // app.addRoute("/replays/create", web_replays_create);
    // app.addRoute("/levels/search", web_levels_search);
    // app.addRoute("/skins/search", web_skins_search);
    // app.addRoute("/backgrounds/search", web_backgrounds_search);
    // app.addRoute("/effects/search", web_effects_search);
    // app.addRoute("/particles/search", web_particles_search);
    // app.addRoute("/engines/search", web_engines_search);
    // app.addRoute("/replays/search", web_replays_search);
    // app.addRoute("/levels/jump/%d", web_levels_jump);
    // app.addRoute("/skins/jump/%d", web_skins_jump);
    // app.addRoute("/backgrounds/jump/%d", web_backgrounds_jump);
    // app.addRoute("/effects/jump/%d", web_effects_jump);
    // app.addRoute("/particles/jump/%d", web_particles_jump);
    // app.addRoute("/engines/jump/%d", web_engines_jump);
    // app.addRoute("/replays/jump/%d", web_replays_jump);
    // app.addRoute("/levels/%s/edit", web_levels_edit);
    // app.addRoute("/skins/%s/edit", web_skins_edit);
    // app.addRoute("/backgrounds/%s/edit", web_backgrounds_edit);
    // app.addRoute("/effects/%s/edit", web_effects_edit);
    // app.addRoute("/particles/%s/edit", web_particles_edit);
    // app.addRoute("/engines/%s/edit", web_engines_edit);
    // app.addRoute("/replays/%s/edit", web_replays_edit);
    // app.addRoute("/levels/%s", web_levels);
    // app.addRoute("/skins/%s", web_skins);
    // app.addRoute("/backgrounds/%s", web_backgrounds);
    // app.addRoute("/effects/%s", web_effects);
    // app.addRoute("/particles/%s", web_particles);
    // app.addRoute("/engines/%s", web_engines);
    // app.addRoute("/replays/%s", web_replays);
    // app.addRoute("/login", web_login);

    // app.addRoute("/uploader", uploader);
}

string cgiRunner(string request) {
	db.query("SELECT COUNT(*) FROM Level");

	// 适配 Resource Version
	levelVersion = upper_bound(levelVersionList.begin(), levelVersionList.end(), Sonolus_Version) - levelVersionList.begin();
	skinVersion = upper_bound(skinVersionList.begin(), skinVersionList.end(), Sonolus_Version) - skinVersionList.begin();
	backgroundVersion = upper_bound(backgroundVersionList.begin(), backgroundVersionList.end(), Sonolus_Version) - backgroundVersionList.begin();
	effectVersion = upper_bound(effectVersionList.begin(), effectVersionList.end(), Sonolus_Version) - effectVersionList.begin();
	particleVersion = upper_bound(particleVersionList.begin(), particleVersionList.end(), Sonolus_Version) - particleVersionList.begin();
	engineVersion = upper_bound(engineVersionList.begin(), engineVersionList.end(), Sonolus_Version) - engineVersionList.begin();
    replayVersion = upper_bound(replayVersionList.begin(), replayVersionList.end(), Sonolus_Version) - replayVersionList.begin();

	routerRegister();

	cgiRequest = request;
	app.cgiRun();
	return cgiResponse;
}

void preload() {
    // 先初始化一遍日志系统
    log_init(log_target_type);

    string sonolusJson = readFile("./config/sonolus_config.json");
    string configJson = readFile("./config/config.json");
    json_decode(sonolusJson, appConfig);
    Json::Value tmpConfig; json_decode(configJson, tmpConfig);
    json_merge(appConfig, tmpConfig);
    Sonolus_Version = appConfig["sonolus.version"].asString();
    if (appConfig["server.data.prefix"].asString() == "") appConfig["server.data.prefix"] = "/data/";
    dataPrefix = appConfig["server.data.prefix"].asString();
    appConfig["server.logo"] = dataPrefix + appConfig["server.logo"].asString();
    appConfig["server.bannerHash"] = appConfig["server.banner"].asString();
    appConfig["server.bannerUrl"] = dataPrefix + appConfig["server.banner"].asString();
    log_init(log_target_type);
    db = DB_Controller(true);
    http_init();
    loadConfig();

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