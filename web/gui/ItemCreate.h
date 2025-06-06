auto GUICreate = [](client_conn conn, http_request request, param argv){
    if (argv[0] != "levels" && 
        argv[0] != "skins" && 
        argv[0] != "backgrounds" && 
        argv[0] != "effects" && 
        argv[0] != "particles" && 
        argv[0] != "engines" && 
        argv[0] != "replays" && 
        argv[0] != "posts" && 
        argv[0] != "playlists") { quickSendMsg(404, "Item type not found."); }
        
    if (!checkLogin(request)) { quickSendMsg(401, "Unauthorized."); }
    
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/ItemCreate.html");
    auto cookie = cookieParam(request);
    argvar argList = argvar();
    string localization = cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"];

    // TODO: add the argList here
    // Doesn't support MultiSearch & CollectionItemSearch
    argList["page.title"] = "{{language." + argv[0].substr(0, argv[0].size() - 1) + "Create}} | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar("{{language." + argv[0].substr(0, argv[0].size() - 1) + "Create}}").output();
    argList["html.createOption"] = "";
    argList["item.type"] = argv[0];
    argList["item.id"] = "-1";
    Json::Value Searches = appConfig[argv[0] + ".creates"];
    string searchOptions = "";
    for (int i = 0; i < Searches.size(); i++) {
        searchOptions += "<h2 class=\"flex-grow py-1 text-xl font-bold sm:py-1.5 sm:text-3xl\">" + Searches[i]["title"].asString() + "</h2>";
        string sections = ""; string type = Searches[i]["type"].asString();
        for (int j = 0; j < Searches[i]["options"].size(); j++) {
            auto item = Searches[i]["options"][j]; string query = item["query"].asString();
            if (item["type"].asString() == "text") sections += fetchSearchText(type + "_" + query, item["name"].asString(), item["placeholder"].asString(), "\"\"", "\"\"", 0, item["required"].asBool()).output();
            if (item["type"].asString() == "toggle") sections += fetchSearchToggle(type + "_" + query, item["name"].asString(), item["def"].asString(), item["def"].asString(), 0, item["required"].asBool()).output();
            if (item["type"].asString() == "slider") sections += fetchSearchSlider(type + "_" + query, item["name"].asString(), item["def"].asString(), item["def"].asString(), item["min"].asInt(), item["max"].asInt(), item["step"].asInt(), 0, item["required"].asBool()).output();
            if (item["type"].asString() == "select") {
                vector<pair<string, string> > values = {};
                for (int k = 0; k < item["values"].size(); k++) values.push_back({
                    item["values"][k]["name"].asString(),
                    item["values"][k]["title"].asString()
                });
                sections += fetchSearchSelect(type + "_" + query, item["name"].asString(), values, item["def"].asString(), item["def"].asString(), 0, item["required"].asBool()).output();
            }
            if (item["type"].asString() == "file") sections += fetchSearchFile(type + "_" + query, item["name"].asString(), "\"\"", "\"\"", 0, item["required"].asBool()).output();
            if (item["type"].asString() == "title") sections += fetchSearchTitle(item["name"].asString(), item["level"].asInt(), 0).output();
            if (item["type"].asString() == "textArea") sections += fetchSearchTextArea(type + "_" + query, item["name"].asString(), item["placeholder"].asString(), "\"\"", "\"\"", 0, item["required"].asBool()).output();
            if (item["type"].asString() == "serverItem") sections += fetchSearchServerItem(type + "_" + query, item["name"].asString(), item["itemType"].asString(), "\"\"", "\"\"", localization, 0, item["required"].asBool()).output();
            if (item["type"].asString() == "serverItems") sections += fetchSearchServerItems(type + "_" + query, item["name"].asString(), item["itemType"].asString(), {}, {}, localization, 0, item["required"].asBool()).output();
            if (item["type"].asString() == "localizationItem") sections += fetchSearchLocalizationItem(type + "_" + query, item["name"].asString(), "\"default\"", "\"default\"", 0, item["required"].asBool()).output();
        } searchOptions += fetchSectionCreate(sections, "/sonolus/" + argv[0] + "/create", "/" + argv[0] + "/", type).output();
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