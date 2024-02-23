#define quickSonolusDetails(name1, name2) {\
	if (name2##Number("name = \"" + argv[1] + "\"") == 0) quickSendMsg(404); \
    auto item = name2##List("name = \"" + argv[1] + "\"", "", 1, 1)[0]; \
    ItemDetails["item"] = item.toJsonObject(); \
    ItemDetails["description"] = item.description; \
    ItemDetails["sections"].resize(0); \
    argvar args = item.fetchParamList(); \
    for (auto v : args) args[v.first] = str_replace("\"", "\\\"", v.second); \
    for (int i = 0; i < appConfig[defineToString(name2)".details.sections"].size(); i++) { \
        auto obj = appConfig[defineToString(name2)".details.sections"][i]; \
        string filter = obj["filter"].asString(), \
               order = obj["order"].asString(); \
        filter = str_replace(filter, args); \
        order = str_replace(order, args); \
        ItemDetails["sections"].append(ItemSection<name1##Item>( \
            obj["title"].asString(), obj["icon"].asString(), \
            name2##List(filter, order, 1, 5) \
        ).toJsonObject()); \
    } \
}

auto SonolusDetails = [](client_conn conn, http_request request, param argv){
    auto $_GET = getParam(request);
    Json::Value ItemDetails;
    if (argv[0] == "levels") { quickSonolusDetails(Level, levels); }
    else if (argv[0] == "skins") { quickSonolusDetails(Skin, skins); }
    else if (argv[0] == "backgrounds") { quickSonolusDetails(Background, backgrounds); }
    else if (argv[0] == "effects") { quickSonolusDetails(Effect, effects); }
    else if (argv[0] == "particles") { quickSonolusDetails(Particle, particles); }
    else if (argv[0] == "engines") { quickSonolusDetails(Engine, engines); }
    else if (argv[0] == "replays") { quickSonolusDetails(Replay, replays); }
    else if (argv[0] == "posts") { quickSonolusDetails(Post, posts); }
    else if (argv[0] == "playlists") { quickSonolusDetails(Playlist, playlists); }
    else quickSendMsg(404);
    quickSendObj(ItemDetails);
};
