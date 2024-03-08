#define quickServerInfo(name, name2) {\
    ServerInfo[defineToString(name2)]["items"].resize(0); \
    auto name2 = name2##List("(localization = \"" + $_GET["localization"] + "\" OR localization = \"default\")", "id DESC", 1, 5); \
    for (int i = 0; i < name2.size(); i++) ServerInfo[defineToString(name2)]["items"].append(name2[i].toJsonObject()); \
    ServerInfo[defineToString(name2)]["search"] = name##Search.size() ? name##Search[0].toJsonObject() : quickSearchObject.toJsonObject(); \
}
auto ServerInfo = [](client_conn conn, http_request request, param argv){
    Json::Value ServerInfo;
    auto $_GET = getParam(request);
    
    // ServerInfo["levels"]["items"].resize(0);
    // auto levels = levelsList("", "", 1, 5);
    // for (int i = 0; i < levels.size(); i++) ServerInfo["levels"]["items"].append(levels[i].toJsonObject());
    // ServerInfo["levels"]["search"] = LevelSearch[0].toJsonObject();
    quickServerInfo(Level, levels);
    quickServerInfo(Skin, skins);
    quickServerInfo(Background, backgrounds);
    quickServerInfo(Effect, effects);
    quickServerInfo(Particle, particles);
    quickServerInfo(Engine, engines);
    quickServerInfo(Replay, replays);
    quickServerInfo(Post, posts);
    quickServerInfo(Playlist, playlists);
    ServerInfo["title"] = appConfig["server.title"].asString();
    ServerInfo["description"] = appConfig["server.description"].asString();
    ServerInfo["hasAuthentication"] = appConfig["server.hasAuthentication"].asBool();
    ServerInfo["hasMultiplayer"] = appConfig["server.hasMultiplayer"].asBool();
    ServerInfo["banner"] = SRL<ServerBanner>(appConfig["server.banner"].asString(), 
        appConfig["server.data.prefix"].asString() + appConfig["server.banner"].asString()).toJsonObject();
    quickSendObj(ServerInfo);
};
