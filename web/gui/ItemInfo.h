#define quickGUIInfo(itemname) {\
    auto infos = itemname##List(item["filter"].asString(), item["order"].asString(), 1, appConfig[string(argv[0]) + ".pageSize.info"].asInt());\
    map<string, bool> used; int number = 0; \
    for (int i = 0; i < infos.size(); i++) { \
    	if (used[infos[i].name]) continue; \
    	used[infos[i].name] = 1; sections += infos[i].toHTMLObject().output(); \
    } \
}

auto GUIInfo = [](client_conn conn, http_request request, param argv) { 
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
    string body = readFile("./web/html/pages/ItemInfo.html");
    auto $_GET = getParam(request);
    auto cookie = cookieParam(request);
    $_GET["localization"] = cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"];
    argvar argList = argvar();

    // TODO: add the argList here
    argList["page.title"] = "{{language." + argv[0] + "}} | " + appConfig["server.title"].asString(); 
    argList["html.navbar"] = fetchNavBar("{{language." + argv[0] + "}}").output();
    argList["html.open_in_sonolus"] = fetchOpenInSonolus("sonolus://" + appConfig["server.rootUrl"].asString() + "/" + argv[0] + "/info").output();
    argList["search.placeholder"] = "{{language.KEYWORDS_PLACEHOLDER}}";
    argList["searchUrl"] = "/" + argv[0] + "/search";
    argList["listUrl"] = "/" + argv[0] + "/list";
    argList["createUrl"] = "/" + argv[0] + "/create";

    bool isLogin = checkLogin(request);
    int uid = !isLogin ? -1 : atoi(getUserProfile(request).handle.c_str());
    bool allowCreate = appConfig[argv[0] + ".enableGUICreate"].asBool();
    bool isExcept = false;
    for (int i = 0; i < appConfig[argv[0] + ".exceptGUICreate"].size(); i++)
        if (appConfig[argv[0] + ".exceptGUICreate"][i].asInt() == uid) isExcept = true;
    bool allowUserCreate = isLogin ? allowCreate ^ isExcept : 0;
    argList["allowUserCreate"] = allowUserCreate ? "" : "style=\"display:none\"";

    string sections = ""; string icons = fetchIconButton("#SearchText", "{{icon.search}}").output();
    for (int i = 0; i < appConfig[argv[0] + ".info.sections"].size(); i++) {
        auto item = appConfig[argv[0] + ".info.sections"][i];
        sections += "<a style=\"height:0px;\" name=\"" + item["title"].asString() + "\"></a>";
        string searchUrl = "", listUrl = "";
        auto fakeGet = getParam(item["searchValues"].asString());
        if (item["searchValues"].asString() == "") ;
        else if (fakeGet["type"] == "quick") 
            listUrl = "/" + item["itemType"].asString() + "s/list?" + item["searchValues"].asString();
        else searchUrl = "/" + item["itemType"].asString() + "s/search?" + item["searchValues"].asString(),
            listUrl = "/" + item["itemType"].asString() + "s/list?" + item["searchValues"].asString();
        sections += "<div class=\"flex flex-col space-y-2 sm:space-y-3\">" + fetchSectionTitle(item["title"].asString(), searchUrl, listUrl).output();
        icons += fetchIconButton("#" + item["title"].asString(), "{{icon." + item["icon"].asString() + "}}").output();
        item["filter"] = "(localization = \"" + $_GET["localization"] + "\" OR localization = \"default\") AND (" + 
        	(item["filter"].asString() == "" ? "1" : item["filter"].asString()) + ")";
        item["order"] = "CASE WHEN localization = \"default\" THEN 1 WHEN localization != \"default\" THEN 0 END ASC, " + 
        	(item["order"].asString() == "" ? "id DESC" : item["order"].asString());
        if (item["itemType"].asString() == "level") { quickGUIInfo(levels); }
        else if (item["itemType"].asString() == "skin") { quickGUIInfo(skins); }
        else if (item["itemType"].asString() == "background") { quickGUIInfo(backgrounds); }
        else if (item["itemType"].asString() == "effect") { quickGUIInfo(effects); }
        else if (item["itemType"].asString() == "particle") { quickGUIInfo(particles); }
        else if (item["itemType"].asString() == "engine") { quickGUIInfo(engines); }
        else if (item["itemType"].asString() == "replay") { quickGUIInfo(replays); }
        else if (item["itemType"].asString() == "post") { quickGUIInfo(posts); }
        else if (item["itemType"].asString() == "playlist") { quickGUIInfo(playlists); }
        sections += fetchSectionBottom(searchUrl, listUrl).output();
        sections += "</div>";
    }
    argList["html.icons"] = icons;
    argList["html.sections"] = sections;
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