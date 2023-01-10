#include<bits/stdc++.h>
#include<jsoncpp/json/json.h>
#include<mysql/mysql.h>
#include"httpd.h"
#include"mysqli.h"
#include"json.h"
#include"encrypt.h"
#include"items/Items.h"
#include"sonolus/sonolus.h"
#include"api/import.h"
using namespace std;
int main() {
    app.setopt(HTTP_LISTEN_PORT, 8080);
    app.setopt(LOG_TARGET_TYPE, LOG_TARGET_CONSOLE);
    app.setopt(OPEN_DEBUG, true);

    // 先初始化一遍日志系统
    log_init(log_target_type);

    loadConfig();
    mysql = mysqli_connect("127.0.0.1", "", "", "", 3306);

    app.addRoute("/data/%s", downloader);

    app.addRoute("/sonolus/info", sonolus_info);
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

    app.addRoute("/uploader", uploader);

    app.run();
}
