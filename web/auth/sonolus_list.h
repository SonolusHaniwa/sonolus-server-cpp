using namespace std;

auto auth_sonolus_list = [](client_conn conn, http_request request, param argv){
    const string warn_sha1 = "18d418a036faf35bbd17c9d265a782e32723356c";

    Search authLevelSearch;
    authLevelSearch.append(SearchTextOption("code", "Code", "Please enter 8 digits code here"));
    string code = getParam(request)["code"];
    LevelItem level = LevelItem(
        0, code, 0, "Login with [" + code + "]?",  
        "Do not use code by other people!", "System",
        EngineItem(0, "sonolus-server-auth", "(Message)", "", "", {}, {}, {}, {}, {}, {}, {}, {}, {}, {}),
        UseItem<SkinItem>(), UseItem<BackgroundItem>(), UseItem<EffectItem>(), UseItem<ParticleItem>(),
        SRL<LevelCover>(warn_sha1, dataPrefix + warn_sha1), SRL<LevelBgm>(), SRL<LevelData>(), SRL<LevelPreview>()
    );
    ItemList<LevelItem> levels = ItemList<LevelItem>(-1, authLevelSearch);
    levels.append(level);
    __api_default_response["Content-Length"] = to_string(json_encode(levels.toJsonObject()).size());
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(levels.toJsonObject()));
    exitRequest(conn);
};
