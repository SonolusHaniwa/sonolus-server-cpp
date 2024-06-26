#define quickSonolusInfo(name1, name2) {\
    if (allowUserCreate) { \
        ItemInfo["creates"].resize(0); \
        for (int i = 0; i < name1##Create.size(); i++) \
            ItemInfo["creates"].append(name1##Create[i].toJsonObject()); \
    } \
    ItemInfo["searches"].resize(0); \
    for (int i = 0; i < name1##Search.size(); i++) \
        ItemInfo["searches"].append(name1##Search[i].toJsonObject()); \
    ItemInfo["sections"].resize(0); \
    for (int i = 0; i < appConfig[defineToString(name2)".info.sections"].size(); i++) { \
        auto item = appConfig[defineToString(name2)".info.sections"][i]; \
        item["filter"] = "(localization = \"" + $_GET["localization"] + "\" OR localization = \"default\") AND (" + \
            (item["filter"].asString() == "" ? "1" : item["filter"].asString()) + ")"; \
        item["order"] = "CASE WHEN localization = \"default\" THEN 1 WHEN localization != \"default\" THEN 0 END ASC, " + \
            (item["order"].asString() == "" ? "id DESC" : item["order"].asString()); \
        ItemInfo["sections"].append(ItemSection<name1##Item>( \
            item["title"].asString(), item["icon"].asString(), \
            name2##List(item["filter"].asString(), item["order"].asString(), 1, appConfig[defineToString(name2)".pageSize.info"].asInt()) \
        ).toJsonObject()); \
    } \
}

auto SonolusInfo = [](client_conn conn, http_request request, param argv){
    auto $_GET = getParam(request);
    bool isLogin = checkLogin(request);
    int uid = !isLogin ? -1 : atoi(getUserProfile(request).handle.c_str());
    bool allowCreate = appConfig[argv[0] + ".enableSonolusCreate"].asBool();
    bool isExcept = false;
    for (int i = 0; i < appConfig[argv[0] + ".exceptSonolusCreate"].size(); i++)
        if (appConfig[argv[0] + ".exceptSonolusCreate"][i].asInt() == uid) isExcept = true;
    bool allowUserCreate = isLogin ? allowCreate ^ isExcept : 0;
    Json::Value ItemInfo;
    ItemInfo["banner"] = SRL<ServerBanner>(appConfig["server.banner"].asString(), 
        appConfig["server.data.prefix"].asString() + appConfig["server.banner"].asString()).toJsonObject();
    if (argv[0] == "levels") { quickSonolusInfo(Level, levels); }
    else if (argv[0] == "skins") { quickSonolusInfo(Skin, skins); }
    else if (argv[0] == "backgrounds") { quickSonolusInfo(Background, backgrounds); }
    else if (argv[0] == "effects") { quickSonolusInfo(Effect, effects); }
    else if (argv[0] == "particles") { quickSonolusInfo(Particle, particles); }
    else if (argv[0] == "engines") { quickSonolusInfo(Engine, engines); }
    else if (argv[0] == "replays") { quickSonolusInfo(Replay, replays); }
    else if (argv[0] == "posts") { quickSonolusInfo(Post, posts); }
    else if (argv[0] == "playlists") { quickSonolusInfo(Playlist, playlists); }
    else if (argv[0] == "rooms") { quickSonolusInfo(Room, rooms); }
    else quickSendMsg(404);
    quickSendObj(ItemInfo);
};