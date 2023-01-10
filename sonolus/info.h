using namespace std;

auto sonolus_info = [](client_conn conn, http_request request, param argv){
    Json::Value ServerInfo;
    Section<LevelItem> levels = levelList("", 1, 5);
    Section<SkinItem> skins = skinList("", 1, 5);
    Section<BackgroundItem> backgrounds = backgroundList("", 1, 5);
    Section<EffectItem> effects = effectList("", 1, 5);
    Section<ParticleItem> particles = particleList("", 1, 5);
    Section<EngineItem> engines = engineList("", 1, 5);
    SRL<ServerBanner> banner = SRL<ServerBanner>("a7f0a83e3929f70caf583140d4f6df14e0bd5e41", 
        "https://servers.sonolus.com/bestdorisonolus/repository/ServerBanner/a7f0a83e3929f70caf583140d4f6df14e0bd5e41");
    ServerInfo["title"] = "Local Sonolus Server";
    ServerInfo["banner"] = banner.toJsonObject();
    ServerInfo["levels"] = levels.toJsonObject();
    ServerInfo["skins"] = skins.toJsonObject();
    ServerInfo["backgrounds"] = backgrounds.toJsonObject();
    ServerInfo["effects"] = effects.toJsonObject();
    ServerInfo["particles"] = particles.toJsonObject();
    ServerInfo["engines"] = engines.toJsonObject();
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(ServerInfo));
    exitRequest(conn);
};