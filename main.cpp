#include<bits/stdc++.h>
#include<jsoncpp/json/json.h>

std::string sonolus_server_version = "1.3.0";
std::string Sonolus_Version = "0.6.5";
Json::Value appConfig, studiosConfig;
Json::Value i18n, i18n_raw;
Json::Value enableListJson;
int exportLevelId[] = {};
int exportSkinId[] = {};
int exportBackgroundId[] = {};
int exportEffectId[] = {};
int exportParticleId[] = {};
int exportEngineId[] = {};

#include"modules/modules.h"
#include"items/Items.h"
#include"sonolus/sonolus.h"
#include"api/import.h"
#include"web/import.h"
#include"modules/import.h"
#include"modules/export.h"
using namespace std;

pluma::Pluma plugins;
vector<SonolusServerPluginProvider*> providers;
stringstream usage;

void initUsage(char** argv) {
    usage << "Usage: " << argv[0] << " [command]" << endl;
    usage << "commands: " << endl;
    usage << "    help: " << argv[0] << " help" << endl;
    usage << "    serve: " << argv[0] << " serve" << endl;
    usage << "    import: " << argv[0] << " import [file]" << endl;
    usage << "    export: " << argv[0] << " export <level/engine> [name] [file]" << endl;
    usage << "    list plugin: " << argv[0] << " plugin list" << endl;
    usage << "    manage plugin: " << argv[0] << " plugin <info/enable/disable> [plugin]" << endl;
    usage << "Plugin commands: " << endl;
    for (int i = 0; i < providers.size(); i++) {
        SonolusServerPlugin* plugin = providers[i]->create();
        vector<string> res = plugin->onPluginHelp(argv);
        for (int j = 0; j < res.size(); j++) usage << "    " << res[j] << endl;
        delete plugin;
    }
}

void invalidUsage() {
    cerr << usage.str();
    exit(0);
}

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
    app.addRoute("/levels/%s", web_levels);
    app.addRoute("/skins/%s", web_skins);
    app.addRoute("/backgrounds/%s", web_backgrounds);
    app.addRoute("/effects/%s", web_effects);
    app.addRoute("/particles/%s", web_particles);
    app.addRoute("/engines/%s", web_engines);

    app.addRoute("/uploader", uploader);

    for (int i = 0; i < providers.size(); i++) {
        SonolusServerPlugin* plugin = providers[i]->create();
        plugin->onPluginRouter(argc, argv);
        delete plugin;
    }
}

void serverRunner(int argc, char** argv) {
    (new DB_Controller)->query("SELECT COUNT(*) FROM Level");

    app.setopt(HTTP_ENABLE_SSL, appConfig["server.enableSSL"].asBool());
    app.setopt(HTTP_LISTEN_HOST, appConfig["server.listenHost"].asString().c_str());
    app.setopt(HTTP_LISTEN_PORT, appConfig["server.listenPort"].asInt());
    app.setopt(HTTP_SSL_CACERT, appConfig["server.httpsCacert"].asString().c_str());
    app.setopt(HTTP_SSL_PRIVKEY, appConfig["server.httpsPrivkey"].asString().c_str());
    app.setopt(HTTP_MULTI_THREAD, appConfig["server.threadNumber"].asInt());
    app.setopt(LOG_FILE_PATH, appConfig["logSystem.targetFile"].asString().c_str());
    app.setopt(LOG_TARGET_TYPE, appConfig["logSystem.target"].asInt());
    app.setopt(OPEN_DEBUG, appConfig["logSystem.debug"].asInt());

    routerRegister(argc, argv);

    app.run();
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
        if (ptr->d_type == 8) {
            string fileName = ptr->d_name;
            if (fileName.substr(fileName.length() - pluginExt.size(), pluginExt.size()) == pluginExt) {
                fileList.push_back(fileName);
            }
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
        cout << "    " << plugin->onPluginName() << " v" << plugin->onPluginVersion() << " (Loaded from \"./plugins/" << fileName << "\")" << endl;
        delete plugin;
    }
    cout << "Other Plugins:" << endl;
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
        cout << "    " << plugin->onPluginName() << " v" << plugin->onPluginVersion() << " (Source from \"./plugins/" << fileName << "\")" << endl;
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
    loadConfig();
    loadDefaultVariable();
}

int main(int argc, char** argv) {
    preload();
    loadPlugins();
    initUsage(argv);

    if (argc < 2) invalidUsage();
    if (string(argv[1]) == "import") {
        if (argc < 3) invalidUsage();
        import(argv[2]);
        return 0;
    }
    else if (string(argv[1]) == "export") {
        if (argc < 5) invalidUsage();
        if (string(argv[2]) == "level") {
            exportLevel(argv[3]);
            exportData(argv[4]);
            return 0;
        } else if (string(argv[2]) == "engine") {
            exportEngine(argv[3]);
            exportData(argv[4]);
            return 0;
        } else invalidUsage();
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
    } 
    for (int i = 0; i < providers.size(); i++) {
        SonolusServerPlugin* plugin = providers[i]->create();
        plugin->onPluginRunner(argc, argv);
        delete plugin;
    }
    invalidUsage();
}