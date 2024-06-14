#define quickGUIEdit(name1, name2) {\
    auto items = name2##List( \
    	"name == \"" + argv[1] + "\" AND (localization = \"" + $_GET["localization"] + "\" OR localization = \"default\")", \
    	"CASE WHEN localization = \"default\" THEN 1 WHEN localization != \"default\" THEN 0 END ASC"); \
    if (items.size() == 0) { quickSendMsg(404); } \
    auto item = items[0]; dataSet = item.fetchParamList(); \
    dataSet["localization"] = name2##Number("name = \"" + argv[1] + "\" AND localization = \"" + $_GET["localization"] + "\"") == 0 ? "default" : $_GET["localization"]; \
    argList["item.id"] = to_string(item.id); \
    SearchItem = name1##Create[0]; \
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

    // TODO: add the argList here
    argList["page.title"] = "{{language." + argv[0].substr(0, argv[0].size() - 1) + "Edit}} | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar("{{language." + argv[0].substr(0, argv[0].size() - 1) + "Edit}}").output();
    argList["html.createOption"] = "";
    argList["item.type"] = argv[0];
    Search SearchItem; argvar dataSet;
    if (argv[0] == "levels") { quickGUIEdit(Level, levels); }
    else if (argv[0] == "skins") { quickGUIEdit(Skin, skins); }
    else if (argv[0] == "backgrounds") { quickGUIEdit(Background, backgrounds); }
    else if (argv[0] == "effects") { quickGUIEdit(Effect, effects); }
    else if (argv[0] == "particles") { quickGUIEdit(Particle, particles); }
    else if (argv[0] == "engines") { quickGUIEdit(Engine, engines); }
    else if (argv[0] == "replays") { quickGUIEdit(Replay, replays); }
    else if (argv[0] == "posts") { quickGUIEdit(Post, posts); }
    else if (argv[0] == "playlists") { quickGUIEdit(Playlist, playlists); }
    for (auto v : dataSet) dataSet[v.first] = str_replace("\n", "\\n", v.second.c_str());
    for (int i = 0; i < SearchItem.options.size(); i++) {
        auto v = SearchItem.options[i];
        if (v.type == "select") {
            vector<string> values;
            for (auto i = 0; i < v.select.values.size(); i++) {
                if (v.select.values[i] == "{{jsonArray.skinList}}") {
                    auto arr = db.query("SELECT title FROM Skin ORDER BY id ASC", "Skin");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.backgroundList}}") {
                    auto arr = db.query("SELECT title FROM Background ORDER BY id ASC", "Background");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.effectList}}") {
                    auto arr = db.query("SELECT title FROM Effect ORDER BY id ASC", "Effect");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.particleList}}") {
                    auto arr = db.query("SELECT title FROM Particle ORDER BY id ASC", "Particle");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.engineList}}") {
                    auto arr = db.query("SELECT title FROM Engine ORDER BY id ASC", "Engine");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.languageList}}") {
                    for (int j = 0; j < i18n_index.size(); j++) values.push_back(i18n_index[j]);
                } else values.push_back(v.select.values[i]);
            } v.select.values = values;
        }
        if (v.type == "text") argList["html.createOption"] += fetchSearchText(v.text.query, v.text.name, v.text.placeholder, dataSet[v.text.query], i != 0).output();
        if (v.type == "toggle") argList["html.createOption"] += fetchSearchToggle(v.toggle.query, v.toggle.name, atoi(dataSet[v.toggle.query].c_str()), i != 0).output();
        if (v.type == "select") {
            int def = atoi(dataSet[v.select.query].c_str());
            if (def == 0) for (int j = 0; j < v.select.values.size(); j++) if (dataSet[v.select.query] == v.select.values[j]) def = j;
        	argList["html.createOption"] += fetchSearchSelect(v.select.query, v.select.name, v.select.values, def, i != 0).output();
        }
        if (v.type == "slider") argList["html.createOption"] += fetchSearchSlider(v.slider.query, v.slider.name, atoi(dataSet[v.slider.query].c_str()), v.slider.min, v.slider.max, v.slider.step, i != 0).output();
        if (v.type == "file") argList["html.createOption"] += fetchSearchFile(v.file.query, v.file.name, dataSet[v.file.query].size() < 6 ? dataSet[v.file.query] : dataSet[v.file.query].substr(6), i != 0).output();
    }

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