#include<bits/stdc++.h>
#include<jsoncpp/json/json.h>
#include<mysql/mysql.h>

Json::Value appConfig;

#include"httpd.h"
#include"html.h"
#include"mysqli.h"
#include"json.h"
#include"encrypt.h"
#include"items/Items.h"
#include"sonolus/sonolus.h"
#include"api/import.h"
#include"web/import.h"
using namespace std;

int main() {
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

    app.addRoute("/levels/create", web_levels_create);
    // app.addRoute("/backgrounds/create", web_backgrounds_create);

    app.addRoute("/uploader", uploader);

    app.run();
}