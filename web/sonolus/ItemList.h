#define quickSonolusList(name, name2) {\
    ItemList["pageCount"] = (name2##Number(sqlFilter) - 1) / itemsPerPage + 1; \
    vector<name##Item> list = name2##List(sqlFilter, order, l, r); \
    ItemList["items"].resize(0); for (int i = 0; i < list.size(); i++) ItemList["items"].append(list[i].toJsonObject()); \
    ItemList["searches"].resize(0); for (int i = 0; i < name##Search.size(); i++) ItemList["searches"].append(name##Search[i].toJsonObject()); \
    ItemList["search"] = name##Search.size() ? name##Search[0].toJsonObject() : quickSearchObject.toJsonObject(); \
}

auto SonolusList = [](client_conn conn, http_request request, param argv){
    auto $_GET = getParam(request);
    Json::Value ItemList;
    bool isLogin = checkLogin(request);
    UserProfile user = !isLogin ? UserProfile() : getUserProfile(request);

    // 准备 Filter
    string sqlFilter = "(localization = \"" + $_GET["localization"] + "\" OR localization = \"default\") AND (";
    string order = "id DESC";
    if ($_GET["type"] == "quick") {
        sqlFilter += "title like \"%" + str_replace("\"", "\\\"", urldecode($_GET["keywords"])) + "%\"";
        // 特判 itemType = replay, searchType = quick 的情况
        if (argv[0] == "replays") sqlFilter += " AND (isPrivate = false OR author LIKE \"%#" + user.handle + "\")";
    } else {
        argvar args = argvar(); string filter = "";
        Json::Value Searches = appConfig[argv[0] + ".searches"];
        for (auto v : $_GET) $_GET[v.first] = str_replace("\"", "\\\"", urldecode(v.second));
        int searchId = -1;
        
        if ($_GET["type"] == "") { // 向下兼容 Sonolus 0.7.5-
        	if (Searches.size()) $_GET["type"] = Searches[0]["type"].asString();
        	else filter = "title LIKE \"%" + $_GET["keywords"] + "%\"", searchId = 114514;
        }
        
        for (int i = 0; i < Searches.size(); i++) if (Searches[i]["type"].asString() == $_GET["type"]) {
            searchId = i; filter = Searches[i]["filter"].asString(); order = Searches[i]["order"].asString();
            args = argResolver($_GET, Searches[i]["options"], $_GET["localization"]);
        }
        args["user.id"] = user.id;
        args["user.name"] = user.name;
        args["user.handle"] = user.handle;

        if (searchId == -1) quickSendMsg(404, "Search type not found.");
        sqlFilter += str_replace(filter, args);
        order = str_replace(order, args);
        if (filter == "") sqlFilter += "1";
    } sqlFilter += ")";
    order = "CASE WHEN localization = \"default\" THEN 1 WHEN localization != \"default\" THEN 0 END ASC, " + order;

    int page = $_GET["page"] == "" ? 0 : atoi($_GET["page"].c_str());
    int itemsPerPage = appConfig[string(argv[0]) + ".pageSize.list"].asInt();
    int l = page * itemsPerPage + 1, r = (page + 1) * itemsPerPage;
    if (argv[0] == "levels") { quickSonolusList(Level, levels); }
    else if (argv[0] == "skins") { quickSonolusList(Skin, skins); }
    else if (argv[0] == "backgrounds") { quickSonolusList(Background, backgrounds); }
    else if (argv[0] == "effects") { quickSonolusList(Effect, effects); }
    else if (argv[0] == "particles") { quickSonolusList(Particle, particles); }
    else if (argv[0] == "engines") { quickSonolusList(Engine, engines); }
    else if (argv[0] == "replays") { quickSonolusList(Replay, replays); }
    else if (argv[0] == "posts") { quickSonolusList(Post, posts); }
    else if (argv[0] == "playlists") { quickSonolusList(Playlist, playlists); }
    else if (argv[0] == "rooms") { quickSonolusList(Room, rooms); }
    else quickSendMsg(404, "Item type not found.");
    quickSendObj(ItemList);
};