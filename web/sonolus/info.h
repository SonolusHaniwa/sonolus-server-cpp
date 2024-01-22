using namespace std;

auto sonolus_info = [](client_conn conn, http_request request, param argv){
    Json::Value ServerInfo;
    auto $_GET = getParam(request);
    Section<LevelItem> levels = levelList(levelFilter($_GET), 1, 5);
    Section<SkinItem> skins = skinList(levelFilter($_GET), 1, 5);
    Section<BackgroundItem> backgrounds = backgroundList(levelFilter($_GET), 1, 5);
    Section<EffectItem> effects = effectList(levelFilter($_GET), 1, 5);
    Section<ParticleItem> particles = particleList(levelFilter($_GET), 1, 5);
    Section<EngineItem> engines = engineList(levelFilter($_GET), 1, 5);
    Section<ReplayItem> replays = replayList(replayFilter($_GET), 1, 5);
    SRL<ServerBanner> banner = SRL<ServerBanner>(
        appConfig["server.bannerHash"].asString(), appConfig["server.bannerUrl"].asString());
    ServerInfo["title"] = appConfig["server.title"].asString();
    ServerInfo["banner"] = banner.toJsonObject();
    ServerInfo["levels"] = levels.toJsonObject();
    ServerInfo["skins"] = skins.toJsonObject();
    ServerInfo["backgrounds"] = backgrounds.toJsonObject();
    ServerInfo["effects"] = effects.toJsonObject();
    ServerInfo["particles"] = particles.toJsonObject();
    ServerInfo["engines"] = engines.toJsonObject();
    ServerInfo["replays"] = replays.toJsonObject();
    __api_default_response["Content-Length"] = to_string(json_encode(ServerInfo).size());
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(ServerInfo));
    exitRequest(conn);
};
