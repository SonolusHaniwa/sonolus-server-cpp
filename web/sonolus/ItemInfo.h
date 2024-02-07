#define quickItemInfo(name1, name2) {\
    ItemInfo["searches"].resize(0); \
    for (int i = 0; i < name1##Search.size(); i++) \
        ItemInfo["searches"].append(name1##Search[i].toJsonObject()); \
    ItemInfo["sections"].resize(0); \
    for (int i = 0; i < appConfig[defineToString(name2)".sections"].size(); i++) \
        ItemInfo["sections"].append(ItemSection<name1##Item>( \
            appConfig[defineToString(name2)".sections"][i]["title"].asString(), appConfig[defineToString(name2)".sections"][i]["icon"].asString(), \
            name2##List(appConfig[defineToString(name2)".sections"][i]["filter"].asString(), appConfig[defineToString(name2)".sections"][i]["order"].asString(), 1, 5) \
        ).toJsonObject()); \
    }

auto ItemInfo = [](client_conn conn, http_request request, param argv){
    auto $_GET = getParam(request);
    Json::Value ItemInfo;
    SRL<ServerBanner> banner = SRL<ServerBanner>(
        appConfig["server.bannerHash"].asString(), appConfig["server.bannerUrl"].asString());
    ItemInfo["banner"] = banner.toJsonObject();
    if (argv[0] == "levels") { quickItemInfo(Level, levels); }
    else if (argv[0] == "skins") { quickItemInfo(Skin, skins); }
    else if (argv[0] == "backgrounds") { quickItemInfo(Background, backgrounds); }
    else if (argv[0] == "effects") { quickItemInfo(Effect, effects); }
    else if (argv[0] == "particles") { quickItemInfo(Particle, particles); }
    else if (argv[0] == "engines") { quickItemInfo(Engine, engines); }
    else if (argv[0] == "replays") { quickItemInfo(Replay, replays); }
    else quickSendMsg(404);
    quickSendObj(ItemInfo);
};