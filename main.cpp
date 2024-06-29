#include<bits/stdc++.h>
#include<jsoncpp/json/json.h>
using namespace std;
#define defineToString(str) #str

std::string sonolus_server_version = "1.6.2";
std::string Maximum_Sonolus_Version = "0.8.3";
std::string Sonolus_Version = Maximum_Sonolus_Version;
Json::Value appConfig, studiosConfig;
Json::Value i18n, i18n_raw;
Json::Value enableListJson;
std::string dataPrefix = "/data/";

const int exportLevelId = 1;
const int exportSkinId = 2;
const int exportBackgroundId = 3;
const int exportEffectId = 4;
const int exportParticleId = 5;
const int exportEngineId = 6;
const int exportReplayId = 7;
string fileHeader = ".srp"; // Sonolus Resources Package

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

#include"modules/modules.h"
#include"core/items.h"
#include"web/import.h"

pluma::Pluma plugins;
vector<SonolusServerPluginProvider*> providers;
stringstream usage;

void initUsage(char** argv) {
    usage << "Sonolus Server v" << sonolus_server_version << " (WebServer Core v" << httpd_version << ")" << endl;
    usage << "The highest supported version of Sonolus: " << Maximum_Sonolus_Version << endl;
    usage << "Copyright (c) 2023 LittleYang0531, all rights reserved." << endl;
    usage << "Compiled at " << __DATE__ << " " << __TIME__ << "." << endl;
    usage << endl;
    usage << "Usage: " << argv[0] << " [command]" << endl;
    usage << "Basic commands: " << endl;
    usage << "    help: " << argv[0] << " help" << endl;
    usage << "    serve: " << argv[0] << " serve" << endl;
	usage << "    cgi: " << argv[0] << " cgi [request] [response]" << endl;
    usage << "Import & Export commands: " << endl;
    usage << "    import: " << argv[0] << " import [file]" << endl;
    usage << "    export: " << argv[0] << " export [type] [name] [file] (name)" << endl;
    usage << "    export remote: " << argv[0] << " export remote [url] [file] (name)" << endl;
    usage << "    export all: " << argv[0] << " export all [file] (name)" << endl;
    usage << "    export remote all: " << argv[0] << " export remote-all [url] [file] (name)" << endl;
    usage << "    export filelist: " << argv[0] << " export filelist [source] [target] (name)" << endl;
    usage << "    list tasks: " << argv[0] << " task list" << endl;
    usage << "    manage task: " << argv[0] << " task <confinue/terminate/info> [name]" << endl;
    usage << "    test package: " << argv[0] << " test [file]" << endl;
    usage << "Plugin commands: " << endl;
    usage << "    list plugin: " << argv[0] << " plugin list" << endl;
    usage << "    manage plugin: " << argv[0] << " plugin <info/enable/disable> [plugin]" << endl;
    for (int i = 0; i < providers.size(); i++) {
        SonolusServerPlugin* plugin = providers[i]->create();
        if (plugin->onPluginPlatformVersion() != sonolus_server_version) {
            delete plugin;
            continue;
        } vector<string> res = plugin->onPluginHelp(argv);
        for (int j = 0; j < res.size(); j++) usage << "    " << res[j] << endl;
        delete plugin;
    }
    usage << "If you have any question about command, please visit Sonolus Server wiki to see the full defination." << endl;
}

void invalidUsage() {
    cerr << usage.str();
    exit(0);
}

#include"modules/export.h"
#include"modules/import.h"

void routerRegister(int argc, char** argv) {
    app.route.clear();

    app.addRoute("/data/%s", downloader);
    app.addRoute("/js/%s", js_import);
    app.addRoute("/css/%s", css_import);

    app.addRoute("/sonolus/info", ServerInfo);
    app.addRoute("/sonolus/%s/info", SonolusInfo);
    app.addRoute("/sonolus/%s/list", SonolusList);
    app.addRoute("/sonolus/%s/create", SonolusCreate);
    app.addRoute("/sonolus/%s/upload", SonolusUpload);
    app.addRoute("/sonolus/rooms/%s", SonolusRoomJoin);
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

    app.addRoute("/uploader", uploader);

    app.ws_addRoute("/rooms/%s", SonolusRoomConnection);

    for (int i = 0; i < providers.size(); i++) {
        SonolusServerPlugin* plugin = providers[i]->create();
        if (plugin->onPluginPlatformVersion() != sonolus_server_version) {
            delete plugin;
            continue;
        } plugin->onPluginRouter(argc, argv, &app);
        delete plugin;
    }
}

void setConfiguration() {
    app.setopt(HTTP_ENABLE_SSL, appConfig["server.enableSSL"].asBool());
    app.setopt(HTTP_LISTEN_HOST, appConfig["server.listenHost"].asString().c_str());
    app.setopt(HTTP_LISTEN_PORT, appConfig["server.listenPort"].asInt());
    app.setopt(HTTP_SSL_CACERT, appConfig["server.httpsCacert"].asString().c_str());
    app.setopt(HTTP_SSL_PRIVKEY, appConfig["server.httpsPrivkey"].asString().c_str());
    app.setopt(HTTP_MULTI_THREAD, appConfig["server.threadNumber"].asInt());
    app.setopt(LOG_FILE_PATH, appConfig["logSystem.targetFile"].asString().c_str());
    app.setopt(LOG_TARGET_TYPE, appConfig["logSystem.target"].asInt());
    app.setopt(OPEN_DEBUG, appConfig["logSystem.debug"].asInt());
}

void serverRunner(int argc, char** argv) {
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

    setConfiguration();
    routerRegister(argc, argv);

    app.run();
}

void cgiRunner(int argc, char** argv) {
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

	setConfiguration();
	routerRegister(argc, argv);

	app.cgiRun(argv[2], argv[3]);
}

#if __windows__
string pluginExt = ".dll";
#else
string pluginExt = ".so";
#endif

void getSoFileList(string path, vector<string>& fileList) {
    DIR* dir;
    struct dirent* ptr;
    if ((dir = opendir(path.c_str())) == NULL) return;
    while ((ptr = readdir(dir)) != NULL) {
        if (ptr->d_name[0] == '.') continue;
        string fileName = ptr->d_name;
        if (fileName.size() < pluginExt.size()) continue;
        if (fileName.substr(fileName.length() - pluginExt.size(), pluginExt.size()) == pluginExt) {
            fileList.push_back(fileName);
        }
    }
    closedir(dir);
}

void saveEnableList(Json::Value enableList) {
    string enableListJson = json_encode(enableList);
    ofstream fout("./plugins/enableList.json");
    fout << enableListJson;
    fout.close();
}

void loadPlugins() {
    plugins.acceptProviderType<SonolusServerPluginProvider>();
    vector<string> fileList;
    getSoFileList("./plugins", fileList);
    string enableList = readFile("./plugins/enableList.json");
    json_decode(enableList, enableListJson);
    for (int i = 0; i < fileList.size(); i++) {
        string fileName = fileList[i];
        if (!enableListJson.isMember(fileName)) enableListJson[fileName] = true;
        if (enableListJson[fileName].asBool()) plugins.load("./plugins/" + fileName);
    } plugins.getProviders(providers);
    saveEnableList(enableListJson);
}

void listPlugin() {
    vector<string> fileList;
    getSoFileList("./plugins", fileList);
    cout << "Loaded Plugins:" << endl;
    for (int i = 0; i < fileList.size(); i++) {
        string fileName = fileList[i];
        if (!enableListJson[fileName].asBool()) continue;
        pluma::Pluma pluma;
        pluma.acceptProviderType<SonolusServerPluginProvider>();
        pluma.load("./plugins/" + fileName);
        vector<SonolusServerPluginProvider*> providers;
        pluma.getProviders(providers);
        if (providers.size() == 0) continue;
        SonolusServerPlugin* plugin = providers[0]->create();
        string platformVersion = plugin->onPluginPlatformVersion();
        if (platformVersion != sonolus_server_version) {
            delete plugin;
            continue;
        } cout << "    " << plugin->onPluginName() << " v" << plugin->onPluginVersion() << " (Loaded from \"./plugins/" << fileName << "\")" << endl;
        delete plugin;
    }
    cout << "Disabled Plugins:" << endl;
    for (int i = 0; i < fileList.size(); i++) {
        string fileName = fileList[i];
        if (enableListJson[fileName].asBool()) continue;
        pluma::Pluma pluma;
        pluma.acceptProviderType<SonolusServerPluginProvider>();
        pluma.load("./plugins/" + fileName);
        vector<SonolusServerPluginProvider*> providers;
        pluma.getProviders(providers);
        if (providers.size() == 0) continue;
        SonolusServerPlugin* plugin = providers[0]->create();
        string platformVersion = plugin->onPluginPlatformVersion();
        if (platformVersion != sonolus_server_version) {
            delete plugin;
            continue;
        } cout << "    " << plugin->onPluginName() << " v" << plugin->onPluginVersion() << " (Source from \"./plugins/" << fileName << "\")" << endl;
        delete plugin;
    }
    cout << "Unsuitable Plugins:" << endl;
    for (int i = 0; i < fileList.size(); i++) {
        string fileName = fileList[i];
        pluma::Pluma pluma;
        pluma.acceptProviderType<SonolusServerPluginProvider>();
        pluma.load("./plugins/" + fileName);
        vector<SonolusServerPluginProvider*> providers;
        pluma.getProviders(providers);
        if (providers.size() == 0) continue;
        SonolusServerPlugin* plugin = providers[0]->create();
        string platformVersion = plugin->onPluginPlatformVersion();
        if (platformVersion == sonolus_server_version) {
            delete plugin;
            continue;
        } cout << "    ./plugins/" << fileName << "(Expected platform version v" << platformVersion << ")" << endl;
        delete plugin;
    }
}

void infoPlugin(string name) {
    string enableList = readFile("./plugins/enableList.json");
    Json::Value enableListJson;
    json_decode(enableList, enableListJson);
    pluma::Pluma pluma;
    pluma.acceptProviderType<SonolusServerPluginProvider>();
    pluma.load("./plugins/" + name + pluginExt);
    vector<SonolusServerPluginProvider*> providers;
    pluma.getProviders(providers);
    if (providers.size() == 0) {
        cout << "Plugin \"" << name << "\" not found." << endl;
        return;
    }
    SonolusServerPlugin* plugin = providers[0]->create();
    cout << "Plugin \"" << plugin->onPluginName() << "\" v" << plugin->onPluginVersion() << endl;
    cout << "Description: " << plugin->onPluginDescription() << endl;
    cout << "Author: " << plugin->onPluginAuthor() << endl;
    cout << "Website: " << plugin->onPluginWebsite() << endl;
    cout << "License: " << plugin->onPluginLicense() << endl;
    cout << "Status: " << (enableListJson[name + pluginExt].asBool() ? "Enabled" : "Disabled") << endl;
}

void enablePlugin(string name) {
    string enableList = readFile("./plugins/enableList.json");
    Json::Value enableListJson;
    json_decode(enableList, enableListJson);
    if (enableListJson.isMember(name + pluginExt)) {
        enableListJson[name + pluginExt] = true;
        saveEnableList(enableListJson);
        cout << "Plugin \"" << name << "\" enabled." << endl;
    } else {
        cout << "Plugin \"" << name << "\" not found." << endl;
    }
}

void disablePlugin(string name) {
    string enableList = readFile("./plugins/enableList.json");
    Json::Value enableListJson;
    json_decode(enableList, enableListJson);
    if (enableListJson.isMember(name + pluginExt)) {
        enableListJson[name + pluginExt] = false;
        saveEnableList(enableListJson);
        cout << "Plugin \"" << name << "\" disabled." << endl;
    } else {
        cout << "Plugin \"" << name << "\" not found." << endl;
    }
}

void preload() {
    app.setopt(LOG_TARGET_TYPE, LOG_TARGET_CONSOLE);
    app.setopt(OPEN_DEBUG, true);

    // 先初始化一遍日志系统
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
    log_init(log_target_type);
    setConfiguration();
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

void MKDIR(string path, int mode = 0777) {
    #ifdef __linux__
    mkdir(path.c_str(), mode);
    #else
    _mkdir(path.c_str());
    #endif
}

int main(int argc, char** argv) {
    MKDIR("./data");
    preload();
    loadPlugins();
    initUsage(argv);

    if (argc < 2) invalidUsage();
    if (string(argv[1]) == "import") {
        if (argc < 3) invalidUsage();
        import(argv[2]);
        return 0;
    }
	else if(string(argv[1]) == "cgi") {
		if (argc < 4) invalidUsage();
		cgiRunner(argc, argv);
		return 0;
	}
    else if (string(argv[1]) == "export") {
        MKDIR("./.tmp");
        exportData(argc, argv);
        return 0;
    } else if (string(argv[1]) == "task") {
        MKDIR("./.tmp");
        taskManage(argc, argv);
        return 0;
    } else if (string(argv[1]) == "help") invalidUsage();
    else if (string(argv[1]) == "serve") {
        serverRunner(argc, argv);
        return 0;
    } else if (string(argv[1]) == "plugin") {
        if (argc < 3) invalidUsage();
        if (string(argv[2]) == "list") {
            listPlugin();
            return 0;
        } else if (string(argv[2]) == "info") {
            if (argc < 4) invalidUsage();
            infoPlugin(argv[3]);
            return 0;
        } else if (string(argv[2]) == "enable") {
            if (argc < 4) invalidUsage();
            enablePlugin(argv[3]);
            return 0;
        } else if (string(argv[2]) == "disable") {
            if (argc < 4) invalidUsage();
            disablePlugin(argv[3]);
            return 0;
        } else invalidUsage();
    } else if (string(argv[1]) == "test") {
        if (argc < 3) invalidUsage();
        testPackage(argv[2]);
        return 0;
    } for (int i = 0; i < providers.size(); i++) {
        SonolusServerPlugin* plugin = providers[i]->create();
        if (plugin->onPluginPlatformVersion() != sonolus_server_version) {
            delete plugin;
            continue;
        } plugin->onPluginRunner(argc, argv);
        delete plugin;
    }
    invalidUsage();
}
