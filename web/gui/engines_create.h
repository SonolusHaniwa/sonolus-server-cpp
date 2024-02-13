using namespace std;

auto web_engines_create = [](client_conn conn, http_request request, param argv){
    if (!checkLogin(request)) {
        __default_response["Content-Length"] = "88";
        putRequest(conn, 401, __default_response);
        send(conn, "<script>alert('Permission Denied: 401 Unauthorized'); window.history.back(-1);</script>");
        exitRequest(conn);
    }
    
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/engines_create.html");
    auto cookie = cookieParam(request);
    argvar argList = merge(transfer(appConfig), transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));

    // TODO: add the argList here
    argList["page.title"] = argList["language.engineCreate"] + " | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(argList["language.engineCreate"], checkLogin(request)).output();
    argList["html.enginesCreateOption"] = "";
    for (int i = 0; i < EngineCreate.options.size(); i++) {
        auto v = EngineCreate.options[i];
        if (v.type == "select") {
            vector<string> values;
            for (auto i = 0; i < v.select.values.size(); i++) {
                if (v.select.values[i] == "{{jsonArray.skinList}}") {
                    auto arr = db.query("SELECT title FROM Skin ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.backgroundList}}") {
                    auto arr = db.query("SELECT title FROM Background ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.effectList}}") {
                    auto arr = db.query("SELECT title FROM Effect ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.particleList}}") {
                    auto arr = db.query("SELECT title FROM Particle ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.engineList}}") {
                    auto arr = db.query("SELECT title FROM Engine ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.languageList}}") {
                    for (int j = 0; j < i18n_index.size(); j++) values.push_back(i18n_index[j]);
                } else values.push_back(v.select.values[i]);
            } v.select.values = values;
        }
        if (v.type == "text") argList["html.enginesCreateOption"] += fetchSearchText(v.text.query, v.text.name, v.text.placeholder, "", i != 0).output();
        if (v.type == "toggle") argList["html.enginesCreateOption"] += fetchSearchToggle(v.toggle.query, v.toggle.name, v.toggle.def, i != 0).output();
        if (v.type == "select") argList["html.enginesCreateOption"] += fetchSearchSelect(v.select.query, v.select.name, v.select.values, v.select.def, i != 0).output();
        if (v.type == "slider") argList["html.enginesCreateOption"] += fetchSearchSlider(v.slider.query, v.slider.name, v.slider.def, v.slider.min, v.slider.max, v.slider.step, i != 0).output();
        if (v.type == "file") argList["html.enginesCreateOption"] += fetchSearchFile(v.file.query, v.file.name, "", i != 0).output();
    }

    header = str_replace(header, argList);
    body = str_replace(body, argList);
    H root = H(true, "html");
    root.append(header);
    root.append(body);
    __default_response["Content-Length"] = to_string(root.output().size());
    putRequest(conn, 200, __default_response);
    send(conn, root.output());
    exitRequest(conn);
};