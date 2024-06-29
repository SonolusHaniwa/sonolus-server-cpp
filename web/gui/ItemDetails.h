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
    string detailsSection = ""; \
    argvar args = item.fetchParamList(); \
    for (auto v : args) args[v.first] = quote_encode(v.second); \
    for (int i = 0; i < appConfig[argv[0] + ".details.sections"].size(); i++) { \
        auto section = appConfig[argv[0] + ".details.sections"][i]; \
        detailsSection += "<a style=\"height:0px;margin:0px;\" name=\"" + section["title"].asString() + "\"></a>"; \
        detailsSection += "<div class=\"flex flex-col space-y-2 sm:space-y-3\"><h2 class=\"py-1 text-xl font-bold sm:py-1.5 sm:text-3xl\">" + section["title"].asString() + "</h2>"; \
        auto list = name##List(str_replace(section["filter"].asString(), args), str_replace(section["order"].asString(), args), 1, appConfig[string(argv[0]) + ".pageSize.recommends"].asInt()); \
        for (int j = 0; j < list.size(); j++) detailsSection += list[j].toHTMLObject().output(); \
        detailsSection += "</div>"; \
        detailsIcon += fetchIconButton("#" + section["title"].asString(), "{{icon." + section["icon"].asString() + "}}").output(); \
    } argList["html.detailsSection"] = detailsSection; \
    argList["html.icons"] += detailsIcon; \
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
    argvar argList = argvar();

    // TODO: add the argList here
    string detailsIcon = fetchIconButton("#ItemDetails", "{{icon." + argv[0].substr(0, argv[0].size() - 1) + "}}").output();
    detailsIcon += fetchIconButton("##DESCRIPTION", "{{icon.description}}").output();
    detailsIcon += fetchIconButton("##TAGS", "{{icon.tags}}").output();
    if (argv[0] == "playlists") detailsIcon += fetchIconButton("#Levels", "{{icon.level}}").output();
    if (argv[0] == "levels") { quickGUIDetails(levels); }
    else if (argv[0] == "skins") { quickGUIDetails(skins); }
    else if (argv[0] == "backgrounds") { quickGUIDetails(backgrounds); }
    else if (argv[0] == "effects") { quickGUIDetails(effects); }
    else if (argv[0] == "particles") { quickGUIDetails(particles); }
    else if (argv[0] == "engines") { quickGUIDetails(engines); }
    else if (argv[0] == "replays") { quickGUIDetails(replays); }
    else if (argv[0] == "posts") { quickGUIDetails(posts); }
    else if (argv[0] == "playlists") { quickGUIDetails(playlists); }

    bool isLogin = checkLogin(request);
    int uid = !isLogin ? -1 : atoi(getUserProfile(request).handle.c_str());
    bool allowCreate = appConfig[argv[0] + ".enableGUICreate"].asBool();
    bool isExcept = false;
    for (int i = 0; i < appConfig[argv[0] + ".exceptGUICreate"].size(); i++)
        if (appConfig[argv[0] + ".exceptGUICreate"][i].asInt() == uid) isExcept = true;
    bool allowUserCreate = isLogin ? allowCreate ^ isExcept : 0;
    argList["allowUserCreate"] = allowUserCreate ? "" : "style=\"display:none\"";

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
    __default_response["Content-Length"] = to_string(root.output().size());
    putRequest(conn, 200, __default_response);
    send(conn, root.output());
    exitRequest(conn);
};