#define quickGUIList(itemname) {\
    pageCount = (itemname##Number(sqlFilter) - 1) / itemsPerPage + 1; \
    auto section = itemname##List(sqlFilter, order, l, r); \
    for (int i = 0; i < section.size(); i++) argList["html.itemsList"] += section[i].toHTMLObject().output(); \
}

auto GUIList = [](client_conn conn, http_request request, param argv) { 
    if (argv[0] != "levels" && 
        argv[0] != "skins" && 
        argv[0] != "backgrounds" && 
        argv[0] != "effects" && 
        argv[0] != "particles" && 
        argv[0] != "engines" && 
        argv[0] != "replays" && 
        argv[0] != "posts" && 
        argv[0] != "playlists") { quickSendMsg(404, "Item type not found."); }

    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/ItemList.html");
    auto cookie = cookieParam(request);
    argvar argList = argvar();
    bool isLogin = checkLogin(request);
    UserProfile user = !isLogin ? UserProfile() : getUserProfile(request);

    // TODO: add the argList here
    argvar $_GET = getParam(request);
    $_GET["localization"] = cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"];
    int page = $_GET["page"] == "" ? 0 : atoi($_GET["page"].c_str());
    int itemsPerPage = appConfig[string(argv[0]) + ".pageSize.list"].asInt();
    int l = page * itemsPerPage + 1, r = (page + 1) * itemsPerPage;
    auto it = $_GET.begin(); 

    // 准备 Filter
    string sqlFilter = "(localization = \"" + $_GET["localization"] + "\" OR localization = \"default\") AND (";
    string order = "id DESC"; string type = "", filterWords = "";
    if ($_GET["type"] == "quick") {
        type = "{{language.quick}}";
        if ($_GET["keywords"] != "") filterWords = "{{language.KEYWORDS}}: " + urldecode($_GET["keywords"]) + "、";
        sqlFilter += "title like \"%" + str_replace("\"", "\\\"", urldecode($_GET["keywords"])) + "%\"";
        // 特判 itemType = replay, searchType = quick 的情况
        if (argv[0] == "replays") sqlFilter += " AND (isPrivate = false OR author LIKE \"%#" + user.handle + "\")";
    } else {
        argvar args = argvar(); string filter = "";
        Json::Value Searches = appConfig[argv[0] + ".searches"];
        for (auto v : $_GET) $_GET[v.first] = str_replace("\"", "\\\"", urldecode(v.second));
        int searchId = -1;
        for (int i = 0; i < Searches.size(); i++) if (Searches[i]["type"].asString() == $_GET["type"]) {
            searchId = i; filter = Searches[i]["filter"].asString(); order = Searches[i]["order"].asString();
            type = Searches[i]["title"].asString();
            for (int j = 0; j < Searches[i]["options"].size(); j++) {
                auto item = Searches[i]["options"][j]; string query = item["query"].asString();
                args = argResolver($_GET, Searches[i]["options"], $_GET["localization"]);
                if (args[query] != "") filterWords += item["name"].asString() + ": " + urldecode(args[query]) + "、";
            }
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

    int pageCount = 0; argList["html.itemsList"] = "";
    if (argv[0] == "levels") { quickGUIList(levels); }
    else if (argv[0] == "skins") { quickGUIList(skins); }
    else if (argv[0] == "backgrounds") { quickGUIList(backgrounds); }
    else if (argv[0] == "effects") { quickGUIList(effects); }
    else if (argv[0] == "particles") { quickGUIList(particles); }
    else if (argv[0] == "engines") { quickGUIList(engines); }
    else if (argv[0] == "replays") { quickGUIList(replays); }
    else if (argv[0] == "posts") { quickGUIList(posts); }
    else if (argv[0] == "playlists") { quickGUIList(playlists); }
    else quickSendMsg(404, "Iten type not found.");

    argList["page.title"] = "{{language." + argv[0] + "}} | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar("{{language." + argv[0] + "}}").output();
    string sonolusUrl = "sonolus://" + appConfig["server.rootUrl"].asString() + "/" + argv[0] + "/list?" + getStringfy($_GET);
    $_GET["page"] = "0"; string topUrl = "/" + argv[0] + "/list?" + getStringfy($_GET);
    $_GET["page"] = to_string(page - 1); string previousUrl = "/" + argv[0] + "/list?" + getStringfy($_GET);
    $_GET["page"] = to_string(page + 1); string nextUrl = "/" + argv[0] + "/list?" + getStringfy($_GET);
    $_GET["page"] = to_string(pageCount - 1); string bottomUrl = "/" + argv[0] + "/list?" + getStringfy($_GET);
    $_GET.erase("page"); string jumpUrl = "/" + argv[0] + "/jump/" + to_string(page) + "?" + getStringfy($_GET);
    $_GET.erase("localization");
    string searchUrl = "/" + argv[0] + "/search?" + getStringfy($_GET);
    argList["html.itemsBottom"] = fetchBottomBar(sonolusUrl, topUrl, previousUrl, nextUrl, bottomUrl, searchUrl, jumpUrl, page, pageCount).output();
    argList["url.list"] = "/" + argv[0] + "/list?type=" + $_GET["type"];
    argList["search.display"] = $_GET["keywords"] == "" && $_GET["type"] == "quick" ? "style=\"display: none\"" : "";
    argList["search.filterWords"] = type + "、" + filterWords;
    argList["search.filterWords"] = argList["search.filterWords"].substr(0, argList["search.filterWords"].size() - 3);
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