using namespace std;

auto web_backgrounds_edit = [](client_conn conn, http_request request, param argv){
    if (!checkLogin(request)) {
        __default_response["Content-Length"] = "88";
        putRequest(conn, 401, __default_response);
        send(conn, "<script>alert('Permission Denied: 401 Unauthorized'); window.history.back(-1);</script>");
        exitRequest(conn);
    }

    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/backgrounds_edit.html");
    auto cookie = cookieParam(request);
    argvar argList = merge(transfer(appConfig), transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));

    // TODO: add the argList here
    argList["page.title"] = argList["language.backgroundEdit"] + " | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(argList["language.backgroundEdit"], checkLogin(request)).output();
    argList["html.backgroundsCreateOption"] = "";
    auto dataSet2 = (new DB_Controller)->query("SELECT * FROM Background WHERE name = \"" + argv[0] +
         "\" AND (localization = \"" + cookie["lang"] + "\" OR localization = \"default\")");
    sort(dataSet2.begin(), dataSet2.end(), [](argvar a, argvar b){
        if (a["name"] == b["name"]) return (a["localization"] == "default") < (b["localization"] == "default");
        else return atoi(a["id"].c_str()) > atoi(b["id"].c_str());
    }); 
    if (dataSet2.size() == 0) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[404]).size());
        putRequest(conn, 404, __api_default_response);
        send(conn, json_encode(msg[404]));
        exitRequest(conn);
    } auto dataSet = dataSet2[0];
    // for (auto v : dataSet) cout << v.first << " " << v.second << endl;
    for (int i = 0; i < BackgroundCreate.options.size(); i++) {
        auto v = BackgroundCreate.options[i];
        if (v.type == "select") {
            vector<string> values;
            for (auto k = 0; k < v.select.values.size(); k++) {
                if (v.select.values[k] == "{{jsonArray.skinList}}") {
                    auto arr = (new DB_Controller)->query("SELECT title FROM Skin ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[k] == "{{jsonArray.backgroundList}}") {
                    auto arr = (new DB_Controller)->query("SELECT title FROM Background ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[k] == "{{jsonArray.effectList}}") {
                    auto arr = (new DB_Controller)->query("SELECT title FROM Effect ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[k] == "{{jsonArray.particleList}}") {
                    auto arr = (new DB_Controller)->query("SELECT title FROM Particle ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[k] == "{{jsonArray.engineList}}") {
                    auto arr = (new DB_Controller)->query("SELECT title FROM Engine ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[k] == "{{jsonArray.languageList}}") {
                    for (int j = 0; j < i18n_index.size(); j++) values.push_back(i18n_index[j]);
                } else values.push_back(v.select.values[k]);
            } v.select.values = values;
        }
        if (v.type == "text") argList["html.backgroundsCreateOption"] += fetchSearchText(v.text.query, v.text.name, v.text.placeholder, dataSet[v.text.query], i != 0).output();
        if (v.type == "toggle") argList["html.backgroundsCreateOption"] += fetchSearchToggle(v.toggle.query, v.toggle.name, atoi(dataSet[v.toggle.query].c_str()), i != 0).output();
        if (v.type == "select") {
            int def = atoi(dataSet[v.select.query].c_str());
            if (def == 0) for (int j = 0; j < v.select.values.size(); j++) if (dataSet[v.select.query] == v.select.values[j]) def = j;
            argList["html.backgroundsCreateOption"] += fetchSearchSelect(v.select.query, v.select.name, v.select.values, def, i != 0).output();
        } if (v.type == "slider") argList["html.backgroundsCreateOption"] += fetchSearchSlider(v.slider.query, v.slider.name, atoi(dataSet[v.slider.query].c_str()), v.slider.min, v.slider.max, v.slider.step, i != 0).output();
        if (v.type == "file") argList["html.backgroundsCreateOption"] += fetchSearchFile(v.file.query, v.file.name, dataSet[v.file.query], i != 0).output();
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