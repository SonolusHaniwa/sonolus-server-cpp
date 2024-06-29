auto GUISearch = [](client_conn conn, http_request request, param argv) { 
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
    string body = readFile("./web/html/pages/ItemSearch.html");
    auto cookie = cookieParam(request);
    argvar argList = argvar();

    // 准备 Filter
    argvar $_GET = getParam(request);
    argvar args = argvar();
    if ($_GET["type"] == "quick") args["quick_keywords"] = urldecode($_GET["keywords"]);
    for (auto v : $_GET) $_GET[v.first] = str_replace("\"", "\\\"", urldecode(v.second));
    Json::Value Searches = appConfig[argv[0] + ".searches"];
    for (int i = 0; i < Searches.size(); i++) {
        string type = Searches[i]["type"].asString();
        for (int j = 0; j < Searches[i]["options"].size(); j++) {
            auto item = Searches[i]["options"][j]; string query = item["query"].asString();
            if (item["type"].asString() == "text") args[type + "_" + query] = $_GET.find(query) == $_GET.end() || type != $_GET["type"] ? "" : $_GET[query];
            if (item["type"].asString() == "slider") args[type + "_" + query] = $_GET.find(query) == $_GET.end() || type != $_GET["type"] ? item["def"].asString() : $_GET[query];
            if (item["type"].asString() == "toggle") args[type + "_" + query] = $_GET.find(query) == $_GET.end() || type != $_GET["type"] ? item["def"].asString() : $_GET[query];
            if (item["type"].asString() == "select") args[type + "_" + query] = $_GET.find(query) == $_GET.end() || type != $_GET["type"] ? item["def"].asString() : $_GET[query];
        }
    }

    argList["page.title"] = "{{language.search}} | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar("{{language.search}}").output(); string searchOptions = "";
    searchOptions += "<h2 class=\"flex-grow py-1 text-xl font-bold sm:py-1.5 sm:text-3xl\">{{language.quick}}</h2>";
    searchOptions += fetchSectionSearch(fetchSearchText("quick_keywords", "{{language.KEYWORDS}}", "{{language.KEYWORDS_PLACEHOLDER}}", args["quick_keywords"], "", 0, 0).output(), "/" + argv[0] + "/list", "quick").output();
    for (int i = 0; i < Searches.size(); i++) {
        searchOptions += "<h2 class=\"flex-grow py-1 text-xl font-bold sm:py-1.5 sm:text-3xl\">" + Searches[i]["title"].asString() + "</h2>";
        string sections = ""; string type = Searches[i]["type"].asString();
        for (int j = 0; j < Searches[i]["options"].size(); j++) {
            auto item = Searches[i]["options"][j]; string query = item["query"].asString();
            if (item["type"].asString() == "text") sections += fetchSearchText(
                type + "_" + query, item["name"].asString(), item["placeholder"].asString(), "\"" + args[type + "_" + query] + "\"", "", 0, 0).output();
            if (item["type"].asString() == "toggle") sections += fetchSearchToggle(
                type + "_" + query, item["name"].asString(), args[type + "_" + query].c_str(), "", 0, 0).output();
            if (item["type"].asString() == "slider") sections += fetchSearchSlider(
                type + "_" + query, item["name"].asString(), args[type + "_" + query].c_str(), "", item["min"].asInt(), item["max"].asInt(), item["step"].asInt(), 0, 0).output();
            if (item["type"].asString() == "select") {
                vector<string> values = {};
                for (int k = 0; k < item["values"].size(); k++) values.push_back(item["values"][k].asString());
                sections += fetchSearchSelect(type + "_" + query, item["name"].asString(), values, args[type + "_" + query].c_str(), "", 0, 0).output();
            }
        } searchOptions += fetchSectionSearch(sections, "/" + argv[0] + "/list", type).output();
    } argList["html.searchOptions"] = searchOptions;

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