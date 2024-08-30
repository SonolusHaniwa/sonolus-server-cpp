auto GUISearch = [](client_conn conn, http_request request, param argv) { 
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
            if (item["type"].asString() == "text") args[type + "_" + query] = $_GET.find(query) == $_GET.end() || type != $_GET["type"] ? item["def"].asString() : $_GET[query];
            if (item["type"].asString() == "slider") args[type + "_" + query] = $_GET.find(query) == $_GET.end() || type != $_GET["type"] ? item["def"].asString() : $_GET[query];
            if (item["type"].asString() == "toggle") args[type + "_" + query] = $_GET.find(query) == $_GET.end() || type != $_GET["type"] ? item["def"].asString() : $_GET[query];
            if (item["type"].asString() == "select") args[type + "_" + query] = $_GET.find(query) == $_GET.end() || type != $_GET["type"] ? item["def"].asString() : $_GET[query];
            if (item["type"].asString() == "multi") args[type + "_" + query] = $_GET.find(query) == $_GET.end() || type != $_GET["type"] ? [&](){
                string res;
                for (int i = 0; i < item["def"].size(); i++) res += item["def"][i].asString();
                return res;
            }() : $_GET[query];
        }
    }

    argList["page.title"] = "{{language.search}} | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar("{{language.search}}").output(); string searchOptions = "";
    searchOptions += "<h2 class=\"flex-grow py-1 text-xl font-bold sm:py-1.5 sm:text-3xl\">{{language.quick}}</h2>";
    searchOptions += fetchSectionSearch(
        fetchSearchText(
            "quick_keywords", 
            "{{language.KEYWORDS}}", 
            "{{language.KEYWORDS_PLACEHOLDER}}", 
            "\"" + args["quick_keywords"] + "\"", 
            "\"\"", 
            0, 0).output(), 
        "/" + argv[0] + "/list", 
        "quick",
        false,
        "quick"
    ).output();
    for (int i = 0; i < Searches.size(); i++) {
        searchOptions += "<div class=\"flex\" style=\"align-items: between\">"
                             "<h2 class=\"flex-grow py-1 text-xl font-bold sm:py-1.5 sm:text-3xl\">" + 
                                 Searches[i]["title"].asString() + 
                             "</h2>";
        if (Searches[i]["help"].asString() != "") {
            searchOptions += "<a href='javascript: showHelp(\"" + quote_encode2(Searches[i]["help"].asString()) + "\")' class=\"flex select-none space-x-2 p-2 transition-colors sm:space-x-3 sm:p-3 cursor-pointer bg-sonolus-ui-button-normal hover:bg-sonolus-ui-button-highlighted active:bg-sonolus-ui-button-pressed\">";
                searchOptions += "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 512 512\" class=\"h-5 w-5 fill-current sm:h-6 sm:w-6\"><path d=\"M80 160c0-35.3 28.7-64 64-64l32 0c35.3 0 64 28.7 64 64l0 3.6c0 21.8-11.1 42.1-29.4 53.8l-42.2 27.1c-25.2 16.2-40.4 44.1-40.4 74l0 1.4c0 17.7 14.3 32 32 32s32-14.3 32-32l0-1.4c0-8.2 4.2-15.8 11-20.2l42.2-27.1c36.6-23.6 58.8-64.1 58.8-107.7l0-3.6c0-70.7-57.3-128-128-128l-32 0C73.3 32 16 89.3 16 160c0 17.7 14.3 32 32 32s32-14.3 32-32zm80 320a40 40 0 1 0 0-80 40 40 0 1 0 0 80z\" transform=\"translate(96,0)\"/></svg>";
            searchOptions += "</a>";
        }
        searchOptions += "</div>";
        string sections = ""; string type = Searches[i]["type"].asString();
        for (int j = 0; j < Searches[i]["options"].size(); j++) {
            auto item = Searches[i]["options"][j]; string query = item["query"].asString();
            if (item["type"].asString() == "text") sections += fetchSearchText(
                type + "_" + query, item["name"].asString(), item["placeholder"].asString(), "\"" + args[type + "_" + query] + "\"", "\"\"", 0, 0).output();
            if (item["type"].asString() == "toggle") sections += fetchSearchToggle(
                type + "_" + query, item["name"].asString(), args[type + "_" + query].c_str(), "0", 0, 0).output();
            if (item["type"].asString() == "slider") sections += fetchSearchSlider(
                type + "_" + query, item["name"].asString(), args[type + "_" + query].c_str(), "0", item["min"].asInt(), item["max"].asInt(), item["step"].asInt(), 0, 0).output();
            if (item["type"].asString() == "select") {
                vector<string> values = {};
                for (int k = 0; k < item["values"].size(); k++) values.push_back(item["values"][k].asString());
                sections += fetchSearchSelect(type + "_" + query, item["name"].asString(), values, args[type + "_" + query].c_str(), "0", 0, 0).output();
            }
            if (item["type"].asString() == "multi") sections += fetchSearchMulti(
                type + "_" + query, item["name"].asString(), [&](){
                    vector<bool> defs;
                    for (int k = 0; k < args[type + "_" + query].size(); k++) defs.push_back(args[type + "_" + query][k] - '0');
                    return defs;
                }(), [&](){
                    vector<string> values;
                    for (int k = 0; k < item["values"].size(); k++) values.push_back(item["values"][k].asString());
                    return values;
                }(), [&](){
                    vector<bool> defs;
                    for (int k = 0; k < item["def"].size(); k++) defs.push_back(false);
                    return defs;
                }(), 0, 0).output();
        } searchOptions += fetchSectionSearch(
            sections, 
            "/" + argv[0] + "/list", 
            type,
            Searches[i]["help"].asString() != "",
            Searches[i]["help"].asString()
        ).output();
    } argList["html.searchOptions"] = searchOptions;
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