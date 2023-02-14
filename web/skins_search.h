using namespace std;

auto web_skins_search = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/skins_search.html");
    auto cookie = cookieParam(request);
    argvar argList = merge(transfer(appConfig), transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));

    // TODO: add the argList here
    argList["page.title"] = argList["language.search"] + " | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar("{{language.search}}").output();
    argList["html.skinsSearchOption"] = ""; auto $_GET = getParam(request);
    for (int i = 0; i < SkinSearch.options.size(); i++) {
        auto v = SkinSearch.options[i];
        if (v.type == "text") argList["html.skinsSearchOption"] += fetchSearchText(v.text.query, v.text.name, v.text.placeholder, $_GET[v.text.query], i != 0).output();
        if (v.type == "toggle") argList["html.skinsSearchOption"] += fetchSearchToggle(v.toggle.query, v.toggle.name, $_GET[v.toggle.query] == "" ? v.toggle.def : atoi($_GET[v.toggle.query].c_str()), i != 0).output();
        if (v.type == "select") argList["html.skinsSearchOption"] += fetchSearchSelect(v.select.query, v.select.name, v.select.values, $_GET[v.select.query] == "" ? v.select.def : atoi($_GET[v.select.query].c_str()), i != 0).output();
        if (v.type == "slider") argList["html.skinsSearchOption"] += fetchSearchSlider(v.slider.query, v.slider.name, $_GET[v.slider.query] == "" ? v.slider.def : atoi($_GET[v.slider.query].c_str()), v.slider.min, v.slider.max, v.slider.step, i != 0).output();
    }
    argList["json.searchConfig"] = readFile("./config/skin_search.json");

    header = str_replace(header, argList);
    body = str_replace(body, argList);
    H root = H(true, "html");
    root.append(header);
    root.append(body);
    putRequest(conn, 200, __default_response);
    send(conn, root.output());
    exitRequest(conn);
};