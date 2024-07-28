#define quickServerInfo(name, name2) {\
    ServerInfo[defineToString(name2)]["items"].resize(0); \
    auto name2 = name2##List("(localization = \"" + $_GET["localization"] + "\" OR localization = \"default\")", "id DESC", 1, 5); \
    for (int i = 0; i < name2.size(); i++) ServerInfo[defineToString(name2)]["items"].append(name2[i].toJsonObject()); \
    ServerInfo[defineToString(name2)]["search"] = name##Search.size() ? name##Search[0].toJsonObject() : quickSearchObject.toJsonObject(); \
}
auto ServerInfo = [](client_conn conn, http_request request, param argv){
    Json::Value ServerInfo;
    auto $_GET = getParam(request);
    
    quickServerInfo(Level, levels);
    quickServerInfo(Skin, skins);
    quickServerInfo(Background, backgrounds);
    quickServerInfo(Effect, effects);
    quickServerInfo(Particle, particles);
    quickServerInfo(Engine, engines);
    quickServerInfo(Replay, replays);
    quickServerInfo(Post, posts);
    quickServerInfo(Playlist, playlists);
    ServerInfo["buttons"] = appConfig["server.buttons"];
    ServerInfo["title"] = appConfig["server.title"].asString();
    ServerInfo["description"] = appConfig["server.description"].asString();
    ServerInfo["hasAuthentication"] = appConfig["server.hasAuthentication"].asBool();
    ServerInfo["hasMultiplayer"] = appConfig["server.hasMultiplayer"].asBool();
    ServerInfo["configuration"]["options"].resize(0);
    ServerInfo["configuration"]["options"].append(SearchSelectOption({
        query: "source",
        name: "Data Source",
        def: 0,
        values: [&](){
            vector<string> values;
            for (int i = 0; i < appConfig["server.data.prefix"].size(); i++) values.push_back(appConfig["server.data.prefix"][i]["name"].asString());
            return values;
        }(),
    }).toJsonObject());
    ServerInfo["configuration"]["options"].append(SearchSelectOption({
        query: "banner",
        name: "Banner Image",
        def: 0,
        values: [&](){
            vector<string> values;
            for (int i = 0; i < appConfig["server.banner"].size(); i++) values.push_back(appConfig["server.banner"][i]["name"].asString());
            return values;
        }(),
    }).toJsonObject());
    int banner = $_GET["banner"] == "" ? 0 : atoi($_GET["banner"].c_str());
    ServerInfo["banner"] = SRL<ServerBanner>(appConfig["server.banner"][banner]["hash"].asString(),
        dataPrefix + appConfig["server.banner"][banner]["hash"].asString()).toJsonObject();
    quickSendObj(ServerInfo);
};
