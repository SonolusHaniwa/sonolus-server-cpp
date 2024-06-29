#define quickGUIEdit(name1, name2) {\
    auto items = name2##List( \
    	"name == \"" + argv[1] + "\" AND (localization = \"" + $_GET["localization"] + "\" OR localization = \"default\")", \
    	"CASE WHEN localization = \"default\" THEN 1 WHEN localization != \"default\" THEN 0 END ASC"); \
    if (items.size() == 0) { quickSendMsg(404); } \
    auto item = items[0]; dataSet = item.toJsonObject(); \
    dataSet["localization"] = name2##Number("name = \"" + argv[1] + "\" AND localization = \"" + $_GET["localization"] + "\"") == 0 ? "default" : $_GET["localization"]; \
    argList["item.id"] = to_string(item.id); \
}

void jsonToArgvar(Json::Value json, argvar& argList, string prefix = "") {
    if (json.isObject()) {
        Json::Value::Members members = json.getMemberNames();
        for (int i = 0; i < members.size(); i++) jsonToArgvar(json[members[i]], argList, prefix + members[i] + ".");
    } else if (json.isArray()) {
        for (int i = 0; i < json.size(); i++) jsonToArgvar(json[i], argList, prefix + to_string(i) + ".");
    } else argList[prefix.size() ? prefix.substr(0, prefix.size() - 1) : prefix] = json.asString();
}

auto GUIEdit = [](client_conn conn, http_request request, param argv){
    if (argv[0] != "levels" && 
        argv[0] != "skins" && 
        argv[0] != "backgrounds" && 
        argv[0] != "effects" && 
        argv[0] != "particles" && 
        argv[0] != "engines" && 
        argv[0] != "replays" && 
        argv[0] != "posts" && 
        argv[0] != "playlists") { quickSendMsg(404); }
        
    if (!checkLogin(request)) { quickSendMsg(401); }
    
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/ItemCreate.html");
    auto cookie = cookieParam(request);
    auto $_GET = getParam(request);
    $_GET["localization"] = cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"];
    argvar argList = argvar();
    string localization = $_GET["localization"];

    // TODO: add the argList here
    argList["page.title"] = "{{language." + argv[0].substr(0, argv[0].size() - 1) + "Edit}} | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar("{{language." + argv[0].substr(0, argv[0].size() - 1) + "Edit}}").output();
    argList["html.searchOptions"] = "";
    argList["item.type"] = argv[0];
    Json::Value dataSet;
    if (argv[0] == "levels") { quickGUIEdit(Level, levels); }
    else if (argv[0] == "skins") { quickGUIEdit(Skin, skins); }
    else if (argv[0] == "backgrounds") { quickGUIEdit(Background, backgrounds); }
    else if (argv[0] == "effects") { quickGUIEdit(Effect, effects); }
    else if (argv[0] == "particles") { quickGUIEdit(Particle, particles); }
    else if (argv[0] == "engines") { quickGUIEdit(Engine, engines); }
    else if (argv[0] == "replays") { quickGUIEdit(Replay, replays); }
    else if (argv[0] == "posts") { quickGUIEdit(Post, posts); }
    else if (argv[0] == "playlists") { quickGUIEdit(Playlist, playlists); }
    argvar argListData = argvar();
    jsonToArgvar(dataSet, argListData);
    vector<Tag> tags;
    for (int i = 0; i < dataSet["tags"].size(); i++) tags.push_back(Tag(dataSet["tags"][i]));
    argListData["tags"] = serializeTagString(tags);
    for (auto v : argListData) argListData[v.first] = quote_encode2(v.second);

    Json::Value Searches = appConfig[argv[0] + ".creates"];
    string searchOptions = "";
    for (int i = 0; i < Searches.size(); i++) {
        searchOptions += "<h2 class=\"flex-grow py-1 text-xl font-bold sm:py-1.5 sm:text-3xl\">" + Searches[i]["title"].asString() + "</h2>";
        string sections = ""; string type = Searches[i]["type"].asString();
        for (int j = 0; j < Searches[i]["options"].size(); j++) {
            auto item = Searches[i]["options"][j]; string query = item["query"].asString();
            string defValue = Searches[i]["constructs"][query].asString();
            defValue = str_replace(defValue, argListData);
            if (item["type"].asString() == "text") sections += fetchSearchText(type + "_" + query, item["name"].asString(), item["placeholder"].asString(), defValue, "\"\"", 0, item["required"].asBool()).output();
            if (item["type"].asString() == "toggle") sections += fetchSearchToggle(type + "_" + query, item["name"].asString(), defValue, item["def"].asString(), 0, item["required"].asBool()).output();
            if (item["type"].asString() == "slider") sections += fetchSearchSlider(type + "_" + query, item["name"].asString(), defValue, item["def"].asString(), item["min"].asInt(), item["max"].asInt(), item["step"].asInt(), 0, item["required"].asBool()).output();
            if (item["type"].asString() == "select") {
                vector<string> values = {};
                for (int k = 0; k < item["values"].size(); k++) values.push_back(item["values"][k].asString());
                sections += fetchSearchSelect(type + "_" + query, item["name"].asString(), values, defValue, item["def"].asString(), 0, item["required"].asBool()).output();
            }
            if (item["type"].asString() == "file") sections += fetchSearchFile(type + "_" + query, item["name"].asString(), defValue, "\"\"", 0, item["required"].asBool()).output();
            if (item["type"].asString() == "title") sections += fetchSearchTitle(item["name"].asString(), item["level"].asInt(), 0).output();
            if (item["type"].asString() == "textArea") sections += fetchSearchTextArea(type + "_" + query, item["name"].asString(), item["placeholder"].asString(), defValue, "\"\"", 0, item["required"].asBool()).output();
            if (item["type"].asString() == "serverItem") sections += fetchSearchServerItem(type + "_" + query, item["name"].asString(), item["itemType"].asString(), defValue, "\"\"", localization, 0, item["required"].asBool()).output();
            if (item["type"].asString() == "localizationItem") sections += fetchSearchLocalizationItem(type + "_" + query, item["name"].asString(), defValue, "\"default\"", 0, item["required"].asBool()).output();
        } searchOptions += fetchSectionCreate(sections, "/sonolus/" + argv[0] + "/create", "/" + argv[0] + "/", type).output();
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