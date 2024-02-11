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

    argList["page.title"] = "{{language.search}} | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar("{{language.search}}").output();
    string searchJson = readFile("./config/" + argv[0].substr(0, argv[0].size() - 1) + "_search.json"); Json::Value Searches;
    json_decode(searchJson, Searches); string searchOptions = "";
    searchOptions += "<h2 class=\"flex-grow py-1 text-xl font-bold sm:py-1.5 sm:text-3xl\">{{language.quick}}</h2>";
    searchOptions += fetchSectionSearch(fetchSearchText("quick_keywords", "{{language.KEYWORDS}}", "{{language.KEYWORDS_PLACEHOLDER}}", "", 0).output(), "/" + argv[0] + "/list", "quick").output();
    for (int i = 0; i < Searches.size(); i++) {
        searchOptions += "<h2 class=\"flex-grow py-1 text-xl font-bold sm:py-1.5 sm:text-3xl\">" + Searches[i]["title"].asString() + "</h2>";
        string sections = ""; string type = Searches[i]["type"].asString();
        for (int j = 0; j < Searches[i]["options"].size(); j++) {
            auto item = Searches[i]["options"][j];
            if (item["type"].asString() == "text") sections += fetchSearchText(
                type + "_" + item["query"].asString(), item["name"].asString(), item["placeholder"].asString(), "", 0).output();
            if (item["type"].asString() == "toggle") sections += fetchSearchToggle(
                type + "_" + item["query"].asString(), item["name"].asString(), item["def"].asInt(), 0).output();
            if (item["type"].asString() == "slider") sections += fetchSearchSlider(
                type + "_" + item["query"].asString(), item["name"].asString(), item["def"].asInt(), item["min"].asInt(), item["max"].asInt(), item["step"].asInt(), 0).output();
            if (item["type"].asString() == "select") {
                vector<string> values = {};
                for (int k = 0; k < item["values"].size(); k++) values.push_back(item["values"][k].asString());
                sections += fetchSearchSelect(
                    type + "_" + item["query"].asString(), item["name"].asString(), values, item["def"].asInt(), 0).output();
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