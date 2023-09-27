#include<bits/stdc++.h>
using namespace std;
#include<jsoncpp/json/json.h>

std::string sonolus_server_version = "1.4.7";
std::string Maximum_Sonolus_Version = "0.7.3";
std::string Sonolus_Version = Maximum_Sonolus_Version;
Json::Value appConfig, studiosConfig;
Json::Value i18n, i18n_raw;
Json::Value enableListJson;

const int exportLevelId = 1;
const int exportSkinId = 2;
const int exportBackgroundId = 3;
const int exportEffectId = 4;
const int exportParticleId = 5;
const int exportEngineId = 6;
string fileHeader = ".srp"; // Sonolus Resources Package

int levelVersion = 1;
int skinVersion = 3;
int backgroundVersion = 2;
int effectVersion = 5;
int particleVersion = 2;
int engineVersion = 8;

vector<string> levelVersionList = {"0.0.0"};
vector<string> skinVersionList = {"0.0.0", "0.5.8", "0.7.0", "0.7.3"};
vector<string> backgroundVersionList = {"0.0.0", "0.5.8"};
vector<string> effectVersionList = {"0.0.0", "0.5.8", "0.6.0", "0.6.4", "0.7.0"};
vector<string> particleVersionList = {"0.0.0", "0.7.0"};
vector<string> engineVersionList = {"0.0.0", "0.0.0", "0.0.0", "0.5.8", "0.5.13", "0.6.0", "0.6.4", "0.7.0", "0.7.2", "0.7.3"};

#include"modules/modules.h"
#include"items/Items.h"
#include"web/import.h"

pluma::Pluma plugins;
vector<SonolusServerPluginProvider*> providers;
stringstream usage;

void initUsage(char** argv) {
    usage << "Sonolus Server v" << sonolus_server_version << " (WebServer Core v" << httpd_version << ")" << endl;
    usage << "The highest supported version of Sonolus: " << Maximum_Sonolus_Version << endl;
    usage << "Copyright (c) 2023 LittleYang0531, all rights reserved." << endl;
    usage << endl;
    usage << "Usage: " << argv[0] << " [command]" << endl;
    usage << "Basic commands: " << endl;
    usage << "    help: " << argv[0] << " help" << endl;
    usage << "    serve: " << argv[0] << " serve" << endl;
	usage << "    cgi: " << argv[0] << " cgi [request] [response]" << endl;
    usage << "Import & Export commands: " << endl;
    usage << "    import: " << argv[0] << " import [file]" << endl;
    usage << "    export: " << argv[0] << " export <level/skin/background/effect/particle/engine> [name] [file] (name)" << endl;
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

    app.addRoute("/sonolus/info", sonolus_info);
    app.addRoute("/sonolus/levels/create", sonolus_levels_create);
    app.addRoute("/sonolus/skins/create", sonolus_skins_create);
    app.addRoute("/sonolus/backgrounds/create", sonolus_backgrounds_create);
    app.addRoute("/sonolus/effects/create", sonolus_effects_create);
    app.addRoute("/sonolus/particles/create", sonolus_particles_create);
    app.addRoute("/sonolus/engines/create", sonolus_engines_create);
    app.addRoute("/sonolus/levels/list", sonolus_levels_list);
    app.addRoute("/sonolus/skins/list", sonolus_skins_list);
    app.addRoute("/sonolus/backgrounds/list", sonolus_backgrounds_list);
    app.addRoute("/sonolus/effects/list", sonolus_effects_list);
    app.addRoute("/sonolus/particles/list", sonolus_particles_list);
    app.addRoute("/sonolus/engines/list", sonolus_engines_list);
    app.addRoute("/sonolus/levels/%s", sonolus_levels);
    app.addRoute("/sonolus/skins/%s", sonolus_skins);
    app.addRoute("/sonolus/backgrounds/%s", sonolus_backgrounds);
    app.addRoute("/sonolus/effects/%s", sonolus_effects);
    app.addRoute("/sonolus/particles/%s", sonolus_particles);
    app.addRoute("/sonolus/engines/%s", sonolus_engines);

    app.addRoute("/", web_index);
    app.addRoute("/index", web_index);
    app.addRoute("/levels/create", web_levels_create);
    app.addRoute("/skins/create", web_skins_create);
    app.addRoute("/backgrounds/create", web_backgrounds_create);
    app.addRoute("/effects/create", web_effects_create);
    app.addRoute("/particles/create", web_particles_create);
    app.addRoute("/engines/create", web_engines_create);
    app.addRoute("/levels/list", web_levels_list);
    app.addRoute("/skins/list", web_skins_list);
    app.addRoute("/backgrounds/list", web_backgrounds_list);
    app.addRoute("/effects/list", web_effects_list);
    app.addRoute("/particles/list", web_particles_list);
    app.addRoute("/engines/list", web_engines_list);
    app.addRoute("/levels/search", web_levels_search);
    app.addRoute("/skins/search", web_skins_search);
    app.addRoute("/backgrounds/search", web_backgrounds_search);
    app.addRoute("/effects/search", web_effects_search);
    app.addRoute("/particles/search", web_particles_search);
    app.addRoute("/engines/search", web_engines_search);
    app.addRoute("/levels/jump/%d", web_levels_jump);
    app.addRoute("/skins/jump/%d", web_skins_jump);
    app.addRoute("/backgrounds/jump/%d", web_backgrounds_jump);
    app.addRoute("/effects/jump/%d", web_effects_jump);
    app.addRoute("/particles/jump/%d", web_particles_jump);
    app.addRoute("/engines/jump/%d", web_engines_jump);
    app.addRoute("/levels/%s/edit", web_levels_edit);
    app.addRoute("/skins/%s/edit", web_skins_edit);
    app.addRoute("/backgrounds/%s/edit", web_backgrounds_edit);
    app.addRoute("/effects/%s/edit", web_effects_edit);
    app.addRoute("/particles/%s/edit", web_particles_edit);
    app.addRoute("/engines/%s/edit", web_engines_edit);
    app.addRoute("/levels/%s", web_levels);
    app.addRoute("/skins/%s", web_skins);
    app.addRoute("/backgrounds/%s", web_backgrounds);
    app.addRoute("/effects/%s", web_effects);
    app.addRoute("/particles/%s", web_particles);
    app.addRoute("/engines/%s", web_engines);
    app.addRoute("/login", web_login);

    app.addRoute("/auth/data/%s", downloader);
    app.addRoute("/auth/sonolus/info", auth_sonolus_info);
    app.addRoute("/auth/sonolus/levels/list", auth_sonolus_list);
    app.addRoute("/auth/sonolus/levels/%s", auth_sonolus_levels);
    app.addRoute("/auth/sonolus/authenticate", auth_sonolus_authenticate);

    app.addRoute("/uploader", uploader);

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
    (new DB_Controller)->query("SELECT COUNT(*) FROM Level");

    // 适配 Resource Version
    levelVersion = upper_bound(levelVersionList.begin(), levelVersionList.end(), Sonolus_Version) - levelVersionList.begin();
    skinVersion = upper_bound(skinVersionList.begin(), skinVersionList.end(), Sonolus_Version) - skinVersionList.begin();
    backgroundVersion = upper_bound(backgroundVersionList.begin(), backgroundVersionList.end(), Sonolus_Version) - backgroundVersionList.begin();
    effectVersion = upper_bound(effectVersionList.begin(), effectVersionList.end(), Sonolus_Version) - effectVersionList.begin();
    particleVersion = upper_bound(particleVersionList.begin(), particleVersionList.end(), Sonolus_Version) - particleVersionList.begin();
    engineVersion = upper_bound(engineVersionList.begin(), engineVersionList.end(), Sonolus_Version) - engineVersionList.begin();

    setConfiguration();
    routerRegister(argc, argv);

    app.run();
}

void cgiRunner(int argc, char** argv) {
	(new DB_Controller)->query("SELECT COUNT(*) FROM Level");

	// 适配 Resource Version
	levelVersion = upper_bound(levelVersionList.begin(), levelVersionList.end(), Sonolus_Version) - levelVersionList.begin();
	skinVersion = upper_bound(skinVersionList.begin(), skinVersionList.end(), Sonolus_Version) - skinVersionList.begin();
	backgroundVersion = upper_bound(backgroundVersionList.begin(), backgroundVersionList.end(), Sonolus_Version) - backgroundVersionList.begin();
	effectVersion = upper_bound(effectVersionList.begin(), effectVersionList.end(), Sonolus_Version) - effectVersionList.begin();
	particleVersion = upper_bound(particleVersionList.begin(), particleVersionList.end(), Sonolus_Version) - particleVersionList.begin();
	engineVersion = upper_bound(engineVersionList.begin(), engineVersionList.end(), Sonolus_Version) - engineVersionList.begin();

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

    string sonolusJson = readFile("./config/sonolus_config.json");
    string studiosJson = readFile("./config/studios_config.json");
    string configJson = readFile("./config/config.json");
    json_decode(sonolusJson, appConfig); json_decode(studiosJson, studiosConfig);
    Json::Value tmpConfig; json_decode(configJson, tmpConfig);
    json_merge(appConfig, tmpConfig); json_merge(studiosConfig, tmpConfig);
    Sonolus_Version = appConfig["sonolus.version"].asString();
    setConfiguration();
    log_init(log_target_type);
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
