using namespace std;

auto auth_sonolus_info = [](client_conn conn, http_request request, param argv){
    Search authLevelSearch;
    authLevelSearch.append(SearchTextOption("code", "Code", "Please enter 8 digits code here"));
    Json::Value res;
    res["title"] = "auth";
    res["banner"] = SRL<ServerBanner>(appConfig["server.banner"].asString(), 
        appConfig["server.data.prefix"].asString() + appConfig["server.banner"].asString()).toJsonObject();
    res["levels"] = Section<LevelItem>(-1, {
        LevelItem(0, "sonolus-server-auth", 0, "Welcome to " + appConfig["server.title"].asString(), 
        "Please enter the [Search] button to enter the code", "System", 
        EngineItem(0, "sonolus-server-auth", "(Message)", "", "", {}, {}, {}, {}, {}, {}, {}, {}, {}, {}),
        UseItem<SkinItem>(), UseItem<BackgroundItem>(), UseItem<EffectItem>(), UseItem<ParticleItem>(),
        SRL<LevelCover>("", appConfig["server.auth.logo"].asString()), SRL<LevelBgm>(), SRL<LevelData>(), SRL<LevelPreview>())}, authLevelSearch).toJsonObject();
    res["skins"] = Section<SkinItem>(0, {}, Search()).toJsonObject();
    res["backgrounds"] = Section<BackgroundItem>(0, {}, Search()).toJsonObject();
    res["effects"] = Section<EffectItem>(0, {}, Search()).toJsonObject();
    res["particles"] = Section<ParticleItem>(0, {}, Search()).toJsonObject();
    res["engines"] = Section<EngineItem>(0, {}, Search()).toJsonObject();
    __api_default_response["Content-Length"] = to_string(json_encode(res).size());
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(res));
    exitRequest(conn);
};
