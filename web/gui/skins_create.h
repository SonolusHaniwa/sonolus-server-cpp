using namespace std;

auto web_skins_create = [](client_conn conn, http_request request, param argv){
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/skins_create.html");
    auto cookie = cookieParam(request);
    argvar argList = merge(transfer(appConfig), transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));

    // TODO: add the argList here
    argList["page.title"] = argList["language.skinCreate"] + " | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(argList["language.skinCreate"]).output();
    argList["html.skinsCreateOption"] = "";
    for (int i = 0; i < SkinCreate.options.size(); i++) {
        auto v = SkinCreate.options[i];
        if (v.type == "select") {
            vector<string> values;
            for (auto i = 0; i < v.select.values.size(); i++) {
                if (v.select.values[i] == "{{jsonArray.skinList}}") {
                    auto arr = (new DB_Controller)->query("SELECT title FROM Skin ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.backgroundList}}") {
                    auto arr = (new DB_Controller)->query("SELECT title FROM Background ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.effectList}}") {
                    auto arr = (new DB_Controller)->query("SELECT title FROM Effect ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.particleList}}") {
                    auto arr = (new DB_Controller)->query("SELECT title FROM Particle ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else if (v.select.values[i] == "{{jsonArray.engineList}}") {
                    auto arr = (new DB_Controller)->query("SELECT title FROM Engine ORDER BY id ASC");
                    for (int j = 0; j < arr.size(); j++) values.push_back(arr[j]["title"]);
                } else values.push_back(v.select.values[i]);
            } v.select.values = values;
        }
        if (v.type == "text") argList["html.skinsCreateOption"] += fetchSearchText(v.text.query, v.text.name, v.text.placeholder, "", i != 0).output();
        if (v.type == "toggle") argList["html.skinsCreateOption"] += fetchSearchToggle(v.toggle.query, v.toggle.name, v.toggle.def, i != 0).output();
        if (v.type == "select") argList["html.skinsCreateOption"] += fetchSearchSelect(v.select.query, v.select.name, v.select.values, v.select.def, i != 0).output();
        if (v.type == "slider") argList["html.skinsCreateOption"] += fetchSearchSlider(v.slider.query, v.slider.name, v.slider.def, v.slider.min, v.slider.max, v.slider.step, i != 0).output();
        if (v.type == "file") argList["html.skinsCreateOption"] += fetchSearchFile(v.file.query, v.file.name, i != 0).output();
    }

    header = str_replace(header, argList);
    body = str_replace(body, argList);
    H root = H(true, "html");
    root.append(header);
    root.append(body);
    putRequest(conn, 200, __default_response);
    send(conn, root.output());
    exitRequest(conn);
};