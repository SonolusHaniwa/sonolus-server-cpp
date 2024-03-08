#define quickSonolusInfo(name1, name2) {\
    ItemInfo["searches"].resize(0); \
    for (int i = 0; i < name1##Search.size(); i++) \
        ItemInfo["searches"].append(name1##Search[i].toJsonObject()); \
    ItemInfo["sections"].resize(0); \
    for (int i = 0; i < appConfig[defineToString(name2)".info.sections"].size(); i++) { \
        auto item = appConfig[defineToString(name2)".info.sections"][i]; \
        item["filter"] = "(localization = \"" + $_GET["localization"] + "\" OR localization = \"default\") AND (" + \
            (item["filter"].asString() == "" ? "1" : item["filter"].asString()) + ")"; \
        item["order"] = "CASE WHEN localization == \"default\" THEN 1 WHEN localization != \"default\" THEN 0 END ASC, " + \
            (item["order"].asString() == "" ? "id DESC" : item["order"].asString()); \
        ItemInfo["sections"].append(ItemSection<name1##Item>( \
            item["title"].asString(), item["icon"].asString(), \
            name2##List(item["filter"].asString(), item["order"].asString(), 1, 5) \
        ).toJsonObject()); \
    } \
}

auto SonolusInfo = [](client_conn conn, http_request request, param argv){
    auto $_GET = getParam(request);
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