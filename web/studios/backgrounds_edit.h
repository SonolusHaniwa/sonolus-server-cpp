using namespace std;

auto studios_backgrounds_edit = [](client_conn conn, http_request request, param argv){
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/studios/backgrounds_edit.html");
    auto cookie = cookieParam(request);
    argvar argList = merge(transfer(appConfig), transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));

    // TODO: add the argList here
    argList["page.title"] = argList["language.backgroundCreate"] + " | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(argList["language.backgroundCreate"], checkLogin(request)).output();
    argList["html.backgroundsCreateOption"] = "";
    for (int i = 0; i < BackgroundStudios.options.size(); i++) {
        auto v = BackgroundStudios.options[i];
        if (v.type == "text") argList["html.backgroundsCreateOption"] += fetchSearchText(v.text.query, v.text.name, v.text.placeholder, "", i != 0).output();
        if (v.type == "toggle") argList["html.backgroundsCreateOption"] += fetchSearchToggle(v.toggle.query, v.toggle.name, v.toggle.def, i != 0).output();
        if (v.type == "select") argList["html.backgroundsCreateOption"] += fetchSearchSelect(v.select.query, v.select.name, v.select.values, v.select.def, i != 0).output();
        if (v.type == "slider") argList["html.backgroundsCreateOption"] += fetchSearchSlider(v.slider.query, v.slider.name, v.slider.def, v.slider.min, v.slider.max, v.slider.step, i != 0).output();
        if (v.type == "file") argList["html.backgroundsCreateOption"] += fetchSearchFile(v.file.query, v.file.name, i != 0).output();
        if (v.type == "title") argList["html.backgroundsCreateOption"] += fetchSearchTitle(v.title.name, v.title.level, i != 0).output();
        if (v.type == "color") argList["html.backgroundsCreateOption"] += fetchSearchColor(v.color.query, v.color.name, "", i != 0).output();
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