using namespace std;

auto web_particles_search = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/particles_search.html");
    auto cookie = cookieParam(request);
    argvar argList = merge(transfer(appConfig), transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));

    // TODO: add the argList here
    argList["page.title"] = argList["language.search"] + " | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar("{{language.search}}").output();
    argList["html.particlesSearchOption"] = "";
    for (int i = 0; i < ParticleSearch.options.size(); i++) {
        auto v = ParticleSearch.options[i];
        if (v.type == "text") argList["html.particlesSearchOption"] += fetchSearchText(v.text.query, v.text.name, v.text.placeholder, i != 0).output();
        if (v.type == "toggle") argList["html.particlesSearchOption"] += fetchSearchToggle(v.toggle.query, v.toggle.name, v.toggle.def, i != 0).output();
        if (v.type == "select") argList["html.particlesSearchOption"] += fetchSearchSelect(v.select.query, v.select.name, v.select.values, v.select.def, i != 0).output();
        if (v.type == "slider") argList["html.particlesSearchOption"] += fetchSearchSlider(v.slider.query, v.slider.name, v.slider.def, v.slider.min, v.slider.max, v.slider.step, i != 0).output();
    }
    argList["json.searchConfig"] = readFile("./config/particle_search.json");

    header = str_replace(header, argList);
    body = str_replace(body, argList);
    H root = H(true, "html");
    root.append(header);
    root.append(body);
    putRequest(conn, 200, __default_response);
    send(conn, root.output());
    exitRequest(conn);
};