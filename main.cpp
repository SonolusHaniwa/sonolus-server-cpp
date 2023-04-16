#include<bits/stdc++.h>
#include<jsoncpp/json/json.h>

std::string sonolus_server_version = "1.2.3";
Json::Value appConfig, studiosConfig;
Json::Value i18n, i18n_raw;
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
#include"modules/custom_engine.h"
using namespace std;

void invalidUsage(char** argv) {
    cerr << "Usage: " << argv[0] << " [command]" << endl;
    cerr << "commands: " << endl;
    cerr << "    help: " << argv[0] << " help" << endl;
    cerr << "    serve: " << argv[0] << " serve" << endl;
    cerr << "    import: " << argv[0] << " import [file]" << endl;
    cerr << "    export: " << argv[0] << " export [level/engine] [name] [file]" << endl;
    cerr << "    init: " << argv[0] << " init [dir]" << endl;
    exit(0);
}

int main(int argc, char** argv) {
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
    loadConfig();
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
    } else if (string(argv[1]) == "init") {
        if (argc < 3) invalidUsage(argv);
        initCustomEngine(argv);
        return 0;
    } else if (string(argv[1]) == "help") invalidUsage(argv);
    if (string(argv[1]) != "serve") invalidUsage(argv);
    
    if (argc > 2) initBuild(argv[2]);
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

    app.addRoute("/data/%s", downloader);
    app.addRoute("/js/%s", js_import);
    app.addRoute("/css/%s", css_import);

    #ifdef ENABLE_SONOLUS
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
    #endif

    #ifdef ENABLE_STUDIOS
    // app.addRoute("/sonolus/studios/skins/import", sonolus_studios_skins_import);
    // app.addRoute("/sonolus/studios/backgrounds/import", sonolus_studios_backgrounds_import);
    // app.addRoute("/sonolus/studios/effects/import", sonolus_studios_effects_import);
    // app.addRoute("/sonolus/studios/skins/create", sonolus_studios_skins_create);
    // app.addRoute("/sonolus/studios/backgrounds/create", sonolus_studios_backgrounds_create);
    // app.addRoute("/sonolus/studios/effects/create", sonolus_studios_effects_create);
    // app.addRoute("/sonolus/studios/skins/save", sonolus_studios_skins_save);
    // app.addRoute("/sonolus/studios/backgrounds/save", sonolus_studios_backgrounds_save);
    // app.addRoute("/sonolus/studios/effects/save", sonolus_studios_effects_save);
    // app.addRoute("/sonolus/studios/skins/export", sonolus_studios_skins_export);
    // app.addRoute("/sonolus/studios/backgrounds/export", sonolus_studios_backgrounds_export);
    // app.addRoute("/sonolus/studios/effects/export", sonolus_studios_effects_export);
    // app.addRoute("/sonolus/studios/skins/%s", sonolus_studios_skins_edit);
    // app.addRoute("/sonolus/studios/backgrounds/%s", sonolus_studios_backgrounds_edit);
    // app.addRoute("/sonolus/studios/effects/%s", sonolus_studios_effects_edit);

    // app.addRoute("/studios/", studios_index);
    // app.addRoute("/studios/index", studios_index);
    // app.addRoute("/studios/skins/list", studios_skins_list);
    // app.addRoute("/studios/backgrounds/list", studios_backgrounds_list);
    // app.addRoute("/studios/effects/list", studios_effects_list);
    // app.addRoute("/studios/skins/search", studios_skins_search);
    // app.addRoute("/studios/backgrounds/search", studios_backgrounds_search);
    // app.addRoute("/studios/effects/search", studios_effects_search);
    // app.addRoute("/studios/skins/jump", studios_skins_jump);
    // app.addRoute("/studios/backgrounds/jump", studios_backgrounds_jump);
    // app.addRoute("/studios/effects/jump", studios_effects_jump);
    // app.addRoute("/studios/skins/%s", studios_skins_edit);
    // app.addRoute("/studios/backgrounds/%s", studios_backgrounds_edit);
    // app.addRoute("/studios/effects/%s", studios_effects_edit);
    #endif

    app.addRoute("/uploader", uploader);

    app.run();
}