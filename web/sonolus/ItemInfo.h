#define quickSonolusInfoSection(name1, name2) { \
    ItemInfo["sections"].append(ItemSection<name1##Item>({ \
        title: item["title"].asString(), \
        icon: item["icon"].asString(), \
        itemType: item["itemType"].asString(), \
        items: name2##List( \
            item["filter"].asString() == "" ? "1" : item["filter"].asString(), \
            item["order"].asString() == "" ? "1" : item["order"].asString(), \
            1, appConfig[defineToString(name2)".pageSize.info"].asInt() \
        ), \
        search: constructDefaultSearchOption(name1##Search, item["searchValues"].asString()), \
        searchValues: item["searchValues"].asString() \
    }).toJsonObject()); \
}

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
    int banner = $_GET["banner"] == "" ? 0 : atoi($_GET["banner"].c_str());
    ItemInfo["banner"] = SRL<ServerBanner>(appConfig["server.banner"][banner]["hash"].asString(),
        dataPrefix + appConfig["server.banner"][banner]["hash"].asString()).toJsonObject();
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
    for (int i = 0; i < appConfig[argv[0] + ".info.sections"].size(); i++) {
        auto item = appConfig[argv[0] + ".info.sections"][i];
        if (item["itemType"].asString() == "level") { quickSonolusInfoSection(Level, levels); }
        else if (item["itemType"].asString() == "skin") { quickSonolusInfoSection(Skin, skins); }
        else if (item["itemType"].asString() == "background") { quickSonolusInfoSection(Background, backgrounds); }
        else if (item["itemType"].asString() == "effect") { quickSonolusInfoSection(Effect, effects); }
        else if (item["itemType"].asString() == "particle") { quickSonolusInfoSection(Particle, particles); }
        else if (item["itemType"].asString() == "engine") { quickSonolusInfoSection(Engine, engines); }
        else if (item["itemType"].asString() == "replay") { quickSonolusInfoSection(Replay, replays); }
        else if (item["itemType"].asString() == "post") { quickSonolusInfoSection(Post, posts); }
        else if (item["itemType"].asString() == "playlist") { quickSonolusInfoSection(Playlist, playlists); }
    }
    quickSendObj(ItemInfo);
};