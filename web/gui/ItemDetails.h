#define quickGUIDetailsSection(type) {\
    detailsSection += "<a style=\"height:0px;margin:0px;\" name=\"" + section["title"].asString() + "\"></a>"; \
    detailsSection += "<div class=\"flex flex-col space-y-2 sm:space-y-3\">" + fetchSectionTitle(section["title"].asString(), searchUrl, listUrl).output(); \
    auto list = type##List(section["filter"].asString(), section["order"].asString(), 1, appConfig[string(argv[0]) + ".pageSize.recommends"].asInt()); \
    for (int j = 0; j < list.size(); j++) detailsSection += list[j].toHTMLObject().output(); \
    detailsSection += fetchSectionBottom(searchUrl, listUrl).output(); \
    detailsSection += "</div>"; \
}

#define quickGUIDetails(name) {\
    auto items = name##List( \
    	"name = \"" + argv[1] + "\" AND (localization = \"" + $_GET["localization"] + "\" OR localization = \"default\")", \
    	"CASE WHEN localization = \"default\" THEN 1 WHEN localization != \"default\" THEN 0 END ASC"); \
    if (items.size() == 0) { quickSendMsg(404); } \
    auto item = items[0]; argList = merge(argList, item.fetchParamList()); \
    argList["html.open_in_sonolus"] = fetchOpenInSonolus(item.fetchParamList()["sonolus.url"]).output(); \
    argList["html.itemDetails"] = str_replace(ItemDetails, argList); \
    argList["page.title"] = item.title + " | " + appConfig["server.title"].asString(); \
    argList["html.navbar"] = fetchNavBar(item.title).output(); \
    argList["editUrl"] = "/" + argv[0] + "/" + argv[1] + "/edit"; \
    quickGUICommunity(request, argv[0], argv[1], argList, detailsIcon); \
    playlistValue = item.toJsonObject(); \
    args = item.fetchParamList(); \
}

auto GUIDetails = [](client_conn conn, http_request request, param argv) {
    if (argv[0] != "levels" && 
        argv[0] != "skins" && 
        argv[0] != "backgrounds" && 
        argv[0] != "effects" && 
        argv[0] != "particles" && 
        argv[0] != "engines" && 
        argv[0] != "replays" && 
        argv[0] != "posts" && 
        argv[0] != "playlists") { quickSendMsg(404); }

    string header = readFile("./web/html/components/header.html");
    string ItemDetails = readFile("./web/html/pages/ItemDetails/" + argv[0] + ".html");
    string body = readFile("./web/html/pages/ItemDetails.html");
    auto cookie = cookieParam(request);
    auto $_GET = getParam(request);
    $_GET["localization"] = cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"];
    argvar argList = argvar(), args = argvar();

    // TODO: add the argList here
    string detailsIcon = fetchIconButton("#ItemDetails", "{{icon." + argv[0].substr(0, argv[0].size() - 1) + "}}").output();
    detailsIcon += fetchIconButton("##DESCRIPTION", "{{icon.description}}").output();
    detailsIcon += fetchIconButton("##TAGS", "{{icon.tags}}").output();
    Json::Value playlistValue;
    if (argv[0] == "levels") { quickGUIDetails(levels); }
    else if (argv[0] == "skins") { quickGUIDetails(skins); }
    else if (argv[0] == "backgrounds") { quickGUIDetails(backgrounds); }
    else if (argv[0] == "effects") { quickGUIDetails(effects); }
    else if (argv[0] == "particles") { quickGUIDetails(particles); }
    else if (argv[0] == "engines") { quickGUIDetails(engines); }
    else if (argv[0] == "replays") { quickGUIDetails(replays); }
    else if (argv[0] == "posts") { quickGUIDetails(posts); }
    else if (argv[0] == "playlists") { quickGUIDetails(playlists); }

    string detailsSection = "";
    if (argv[0] == "playlists") {
        PlaylistItem playlist(playlistValue["id"].asInt(), playlistValue);
        detailsIcon += fetchIconButton("##LEVEL", "{{icon.level}}").output();
        detailsSection += "<a style=\"height:0px;margin:0px;\" name=\"#LEVEL\"></a>";
        detailsSection += "<div class=\"flex flex-col space-y-2 sm:space-y-3\"><h2 class=\"py-1 text-xl font-bold sm:py-1.5 sm:text-3xl\">{{language.LEVEL}}</h2>";
        for (int i = 0; i < playlist.levels.size(); i++) {
            auto level = playlist.levels[i];
            detailsSection += level.toHTMLObject().output();
        }
        detailsSection += "</div>";
    }
    for (auto v : args) args[v.first] = quote_encode(v.second);
    for (int i = 0; i < appConfig[argv[0] + ".details.sections"].size(); i++) {
        auto section = appConfig[argv[0] + ".details.sections"][i];
        string searchUrl = "", listUrl = "";
        section["searchValues"] = str_replace(section["searchValues"].asString(), args);
        section["order"] = str_replace(section["order"].asString(), args);
        section["filter"] = str_replace(section["filter"].asString(), args);
        auto fakeGet = getParam(section["searchValues"].asString());
        if (section["searchValues"].asString() == "") ;
        else if (fakeGet["type"] == "quick")
            listUrl = "/" + section["itemType"].asString() + "s/list?" + section["searchValues"].asString();
        else searchUrl = "/" + section["itemType"].asString() + "s/search?" + section["searchValues"].asString(),
            listUrl = "/" + section["itemType"].asString() + "s/list?" + section["searchValues"].asString();
        if (section["itemType"].asString() == "level") { quickGUIDetailsSection(levels); }
        else if (section["itemType"].asString() == "skin") { quickGUIDetailsSection(skins); }
        else if (section["itemType"].asString() == "background") { quickGUIDetailsSection(backgrounds); }
        else if (section["itemType"].asString() == "effect") { quickGUIDetailsSection(effects); }
        else if (section["itemType"].asString() == "particle") { quickGUIDetailsSection(particles); }
        else if (section["itemType"].asString() == "engine") { quickGUIDetailsSection(engines); }
        else if (section["itemType"].asString() == "replay") { quickGUIDetailsSection(replays); }
        else if (section["itemType"].asString() == "post") { quickGUIDetailsSection(posts); }
        else if (section["itemType"].asString() == "playlist") { quickGUIDetailsSection(playlists); }
        detailsIcon += fetchIconButton("#" + section["title"].asString(), "{{icon." + section["icon"].asString() + "}}").output();
    } argList["html.detailsSection"] = detailsSection;
    argList["html.icons"] += detailsIcon;

    bool isLogin = checkLogin(request);
    int uid = !isLogin ? -1 : atoi(getUserProfile(request).handle.c_str());
    bool allowCreate = appConfig[argv[0] + ".enableGUICreate"].asBool();
    bool isExcept = false;
    for (int i = 0; i < appConfig[argv[0] + ".exceptGUICreate"].size(); i++)
        if (appConfig[argv[0] + ".exceptGUICreate"][i].asInt() == uid) isExcept = true;
    bool allowUserCreate = isLogin ? allowCreate ^ isExcept : 0;
    argList["allowUserCreate"] = allowUserCreate ? "" : "style=\"display:none\"";

    argList["html.itemActions"] = "";
    string type = argv[0], name = argv[1];
    bool isLike = isLogin && (db.query("SELECT uid FROM LikeTable WHERE targetType = \"" + type + "\" AND targetName = \"" + name + "\" AND uid = \"" + getUserProfile(request).id + "\"", "LikeTable").size());
    if (appConfig[type + ".enableLike"].asBool()) {
        if (isLike) argList["html.itemActions"] += fetchIconTextButton("unlike('" + type + "', '" + name + "')", "heart", "#DISLIKE").output();
        else argList["html.itemActions"] += fetchIconTextButton("like('" + type + "', '" + name + "')", "heartHollow", "#LIKE").output();
    }
    if (appConfig[type + ".enableRating"].asBool())
        argList["html.itemActions"] += fetchIconTextButton("rating()", "star", "#RATING").output();
    argList["server.bannerUrl"] = dataPrefix + appConfig["server.banner"][atoi(cookieParam(request)["banner"].c_str())]["hash"].asString();

    argList = merge(argList, merge(
        transfer(appConfig), merge(
            transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."), 
            iconList
        )
    )); header = str_replace(header, argList);
    body = str_replace(body, argList);
    H root = H(true, "html");
    root.append(header);
    root.append(body);
    string res = root.output();
    res = str_replace(dataPrefix.c_str(), appConfig["server.data.prefix"][atoi(cookieParam(request)["source"].c_str())]["url"].asCString(), res);
    __default_response["Content-Length"] = to_string(res.size());
    putRequest(conn, 200, __default_response);
    send(conn, res);
    exitRequest(conn);
};