#include<bits/stdc++.h>
#include<jsoncpp/json/json.h>
#include<mysql/mysql.h>

Json::Value appConfig;
Json::Value i18n, i18n_raw;
int exportLevelId[] = {};
int exportSkinId[] = {};
int exportBackgroundId[] = {};
int exportEffectId[] = {};
int exportParticleId[] = {};
int exportEngineId[] = {};

#include"modules/import.h"
#include"items/Items.h"
#include"sonolus/sonolus.h"
#include"api/import.h"
#include"web/import.h"
#include"modules/export.h"
using namespace std;

void invalidUsage(char** argv) {
    cerr << "Usage: " << argv[0] << " [command]" << endl;
    cerr << "commands: " << endl;
    cerr << "    help: " << argv[0] << " help" << endl;
    cerr << "    serve: " << argv[0] << " serve" << endl;
    cerr << "    import: " << argv[0] << " import [file]" << endl;
    cerr << "    export: " << argv[0] << " export [level/engine] [name] [file]" << endl;
    exit(0);
}

int main(int argc, char** argv) {
    app.setopt(LOG_TARGET_TYPE, LOG_TARGET_CONSOLE);
    app.setopt(OPEN_DEBUG, true);

    // 先初始化一遍日志系统
    log_init(log_target_type);

    loadConfig();
    ifstream fin("./config/config.json");
    if (!fin) {
        writeLog(LOG_LEVEL_ERROR, "Failed to open config file \"./config/config.json\"");
        exit(3);
    }
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    fin.seekg(0, ios::beg);
    char* config = new char[len];
    fin.read(config, len);
    string configJson = "";
    for (int i = 0; i < len; i++) configJson.push_back(config[i]);
    if (!json_decode(configJson, appConfig)) {
        writeLog(LOG_LEVEL_ERROR, "Invalid json file in \"./config/config.json\""); 
        exit(3);
    }
    loadDefaultVariable();

    if (argc < 2) invalidUsage(argv);
    if (string(argv[1]) == "import") {
        if (argc < 3) invalidUsage(argv);
        import(argv[2]);
        return 0;
    }
    else if (string(argv[1]) == "export") {
        if (argc < 5) invalidUsage(argv);
        if (string(argv[2]) == "level") {
            exportLevel(argv[3]);
            exportData(argv[4]);
            return 0;
        } else if (string(argv[2]) == "engine") {
            exportEngine(argv[3]);
            exportData(argv[4]);
            return 0;
        } else invalidUsage(argv);
    } else if (string(argv[1]) == "help") invalidUsage(argv);
    else if (string(argv[1]) != "serve") invalidUsage(argv);
    
    mysql = mysqli_connect(appConfig["mysql.hostname"].asString().c_str(), appConfig["mysql.username"].asString().c_str(), 
        appConfig["mysql.password"].asString().c_str(), appConfig["mysql.database"].asString().c_str(), appConfig["mysql.port"].asInt());
    app.setopt(HTTP_ENABLE_SSL, appConfig["server.enableSSL"].asBool());
    app.setopt(HTTP_LISTEN_HOST, appConfig["server.listenHost"].asString().c_str());
    app.setopt(HTTP_LISTEN_PORT, appConfig["server.listenPort"].asInt());
    app.setopt(HTTP_SSL_CACERT, appConfig["server.httpsCacert"].asString().c_str());
    app.setopt(HTTP_SSL_PRIVKEY, appConfig["server.httpsPrivkey"].asString().c_str());
    app.setopt(HTTP_MULTI_THREAD, appConfig["server.threadNumber"].asInt());
    app.setopt(LOG_FILE_PATH, appConfig["logSystem.targetFile"].asString().c_str());
    app.setopt(LOG_TARGET_TYPE, appConfig["logSystem.target"].asInt());
    app.setopt(OPEN_DEBUG, appConfig["logSystem.debug"].asInt());

    app.addRoute("/data/%s", downloader);
    app.addRoute("/js/%s", js_import);
    app.addRoute("/css/%s", css_import);

    app.addRoute("/sonolus/info", sonolus_info);
    app.addRoute("/sonolus/levels/create", sonolus_levels_create);
    // app.addRoute("/sonolus/skins/create", sonolus_skins_create);
    // app.addRoute("/sonolus/backgrounds/create", sonolus_backgrounds_create);
    // app.addRoute("/sonolus/effects/create", sonolus_effects_create);
    // app.addRoute("/sonolus/particles/create", sonolus_particles_create);
    // app.addRoute("/sonolus/engines/create", sonolus_engines_create);
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
    // app.addRoute("/skins/create", web_skins_create);
    // app.addRoute("/backgrounds/create", web_backgrounds_create);
    // app.addRoute("/effects/create", web_effects_create);
    // app.addRoute("/particles/create", web_particles_create);
    // app.addRoute("/engines/create", web_engines_create);
    // app.addRoute("/levels/list", web_levels_list);
    // app.addRoute("/skins/list", web_skins_list);
    // app.addRoute("/backgrounds/list", web_backgrounds_list);
    // app.addRoute("/effects/list", web_effects_list);
    // app.addRoute("/particles/list", web_particles_list);
    // app.addRoute("/engines/list", web_engines_list);
    // app.addRoute("/levels/search", web_levels_search);
    // app.addRoute("/skins/search", web_skins_search);
    // app.addRoute("/backgrounds/search", web_backgrounds_search);
    // app.addRoute("/effects/search", web_effects_search);
    // app.addRoute("/particles/search", web_particles_search);
    // app.addRoute("/engines/search", web_engines_search);
    app.addRoute("/levels/%s", web_levels);
    app.addRoute("/skins/%s", web_skins);
    app.addRoute("/backgrounds/%s", web_backgrounds);
    app.addRoute("/effects/%s", web_effects);
    app.addRoute("/particles/%s", web_particles);
    app.addRoute("/engines/%s", web_engines);


    app.addRoute("/uploader", uploader);

    app.run();
}