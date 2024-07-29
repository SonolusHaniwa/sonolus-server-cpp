#define quickSonolusDetailsSection(name1, name2) { \
    string filter = obj["filter"].asString() == "" ? "1" : obj["filter"].asString(), \
           order = obj["order"].asString() == "" ? "1" : obj["order"].asString(); \
    filter = str_replace(filter, args); \
    order = str_replace(order, args); \
    string searchValues = str_replace(obj["searchValues"].asString(), args); \
    ItemDetails["sections"].append(ItemSection<name1##Item>({ \
        title: obj["title"].asString(), \
        icon: obj["icon"].asString(), \
        itemType: obj["itemType"].asString(), \
        items: name2##List(filter, order, 1, appConfig[defineToString(name2)".pageSize.recommends"].asInt()), \
        search: constructDefaultSearchOption(name1##Search, obj["searchValues"].asString()), \
        searchValues: searchValues \
    }).toJsonObject()); \
}

#define quickSonolusDetails(name1, name2) {\
    auto items = name2##List( \
    	"name = \"" + argv[1] + "\" AND (localization = \"" + $_GET["localization"] + "\" OR localization = \"default\")", \
    	"CASE WHEN localization = \"default\" THEN 1 WHEN localization != \"default\" THEN 0 END ASC"); \
    if (items.size() == 0) { quickSendMsg(404); } \
    auto item = items[0]; \
    ItemDetails["item"] = item.toJsonObject(); \
    ItemDetails["description"] = item.description; \
    ItemDetails["sections"].resize(0); \
    ItemDetails["hasCommunity"] = \
        appConfig[defineToString(name2)".enableLike"].asBool() || \
        appConfig[defineToString(name2)".enableComment"].asBool() || \
        appConfig[defineToString(name2)".enableRating"].asBool(); \
    ItemDetails["leaderboards"].resize(0); \
    ItemDetails["actions"].resize(0); \
    if (appConfig[string(argv[0]) + ".enableLike"].asBool()) ItemDetails["actions"].append((isLike ? unlikeCommunityObject : likeCommunityObject).toJsonObject()); \
    if (appConfig[string(argv[0]) + ".enableRating"].asBool()) { \
        int defaultRating = 5; \
        if (isLogin) { \
            auto myRating = db.query("SELECT rating FROM Rating WHERE targetType = \"" + string(argv[0]) + "\" AND targetName = \"" + string(argv[1]) + "\" AND uid = \"" + getUserProfile(request).id + "\"", "Rating"); \
            if (myRating.size() != 0) defaultRating = atoi(myRating[0]["rating"].c_str()); \
        } Search ratingSearch = ratingCommunityObject; \
        ratingSearch.options[0].slider.def = defaultRating; \
        ItemDetails["actions"].append(ratingSearch.toJsonObject()); \
    } \
    args = item.fetchParamList(); \
    auto recommended = name2##List("author = \"" + quote_encode(item.author) + "\"", "id DESC", 1, 5); \
    ItemDetails["recommended"].resize(0); \
    for (int i = 0; i < recommended.size(); i++) ItemDetails["recommended"].append(recommended[i].toJsonObject()); \
}

auto SonolusDetails = [](client_conn conn, http_request request, param argv){
    auto $_GET = getParam(request);
    Json::Value ItemDetails;
    bool isLogin = checkLogin(request);
    bool isLike = isLogin && (db.query("SELECT uid FROM LikeTable WHERE targetType = \"" + string(argv[0]) + "\" AND targetName = \"" + string(argv[1]) + "\" AND uid = \"" + getUserProfile(request).id + "\"", "LikeTable").size());
    argvar args;
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
    for (auto v : args) args[v.first] = quote_encode(v.second);
    for (int i = 0; i < appConfig[argv[0] + ".details.sections"].size(); i++) {
        auto obj = appConfig[argv[0] + ".details.sections"][i];
        if (obj["itemType"].asString() == "level") { quickSonolusDetailsSection(Level, levels); }
        else if (obj["itemType"].asString() == "skin") { quickSonolusDetailsSection(Skin, skins); }
        else if (obj["itemType"].asString() == "background") { quickSonolusDetailsSection(Background, backgrounds); }
        else if (obj["itemType"].asString() == "effect") { quickSonolusDetailsSection(Effect, effects); }
        else if (obj["itemType"].asString() == "particle") { quickSonolusDetailsSection(Particle, particles); }
        else if (obj["itemType"].asString() == "engine") { quickSonolusDetailsSection(Engine, engines); }
        else if (obj["itemType"].asString() == "replay") { quickSonolusDetailsSection(Replay, replays); }
        else if (obj["itemType"].asString() == "post") { quickSonolusDetailsSection(Post, posts); }
        else if (obj["itemType"].asString() == "playlist") { quickSonolusDetailsSection(Playlist, playlists); }
    }
    quickSendObj(ItemDetails);
};
