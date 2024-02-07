#define quickSonolusList1(name) \
    for (int i = 0; i < name##Search.size(); i++) if (name##Search[i].type == $_GET["type"]) { \
        searchId = i, filter = name##Search[i].filter, order = name##Search[i].order; \
        for (int j = 0; j < Searches[i]["options"].size(); j++) { \
            if (args.find(Searches[i]["options"][j]["query"].asString()) == args.end()) \
                args[Searches[i]["options"][j]["query"].asString()] = Searches[i]["options"][j]["def"].asString(); \
        } \
    }

#define quickSonolusList2(name, name2) {\
    ItemList["pageCount"] = (name2##Number(sqlFilter) - 1) / itemsPerPage + 1; \
    vector<name##Item> list = name2##List(sqlFilter, order, l, r); \
    ItemList["items"].resize(0); for (int i = 0; i < list.size(); i++) ItemList["items"].append(list[i].toJsonObject()); \
    ItemList["searches"].resize(0); for (int i = 0; i < name##Search.size(); i++) ItemList["searches"].append(name##Search[i].toJsonObject()); \
}

auto SonolusList = [](client_conn conn, http_request request, param argv){
    auto $_GET = getParam(request);
    Json::Value ItemList;

    // 准备 Filter
    string sqlFilter = "(localization = \"" + $_GET["localization"] + "\" OR localization = \"default\") AND (";
    string order = "id DESC";
    if ($_GET["type"] == "quick") sqlFilter += "title like \"%" + str_replace("\"", "\\\"", urldecode($_GET["keywords"])) + "%\"";
    else {
        argvar args = argvar();
        for (auto v : $_GET) args[v.first] = str_replace("\"", "\\\"", urldecode(v.second));
        int searchId = -1; string filter = "";

        string searchJson = readFile("./config/" + argv[0].substr(0, argv[0].size() - 1) + "_search.json"); Json::Value Searches;
        json_decode(searchJson, Searches);
        if (argv[0] == "levels") { quickSonolusList1(Level); }
        else if (argv[0] == "skins") { quickSonolusList1(Skin); }
        else if (argv[0] == "backgrounds") { quickSonolusList1(Background); }
        else if (argv[0] == "effects") { quickSonolusList1(Effect); }
        else if (argv[0] == "particles") { quickSonolusList1(Particle); }
        else if (argv[0] == "engines") { quickSonolusList1(Engine); }
        else if (argv[0] == "replays") { quickSonolusList1(Replay); }
        else if (argv[0] == "posts") { quickSonolusList1(Post); }
        else if (argv[0] == "playlists") { quickSonolusList1(Playlist); }
        else quickSendMsg(404);
        if (searchId == -1) quickSendMsg(404);
        sqlFilter += str_replace(filter, args);
        if (filter == "") sqlFilter += "1";
    } sqlFilter += ")";

    int page = $_GET["page"] == "" ? 0 : atoi($_GET["page"].c_str());
    int itemsPerPage = appConfig["sonolus.itemsPerPage"].asInt();
    int l = page * itemsPerPage + 1, r = (page + 1) * itemsPerPage;
    if (argv[0] == "levels") { quickSonolusList2(Level, levels); }
    else if (argv[0] == "skins") { quickSonolusList2(Skin, skins); }
    else if (argv[0] == "backgrounds") { quickSonolusList2(Background, backgrounds); }
    else if (argv[0] == "effects") { quickSonolusList2(Effect, effects); }
    else if (argv[0] == "particles") { quickSonolusList2(Particle, particles); }
    else if (argv[0] == "engines") { quickSonolusList2(Engine, engines); }
    else if (argv[0] == "replays") { quickSonolusList2(Replay, replays); }
    else if (argv[0] == "posts") { quickSonolusList2(Post, posts); }
    else if (argv[0] == "playlists") { quickSonolusList2(Playlist, playlists); }
    else quickSendMsg(404);
    quickSendObj(ItemList);
};