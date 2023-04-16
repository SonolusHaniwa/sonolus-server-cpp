using namespace std;

auto studios_index = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/studios/index.html");
    auto cookie = cookieParam(request);
    argvar argList = merge(transfer(studiosConfig), transfer(i18n[cookie["lang"] == "" ? studiosConfig["language.default"].asString() : cookie["lang"]], "language."));

    // TODO: add the argList here
    argList["page.title"] = studiosConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(studiosConfig["server.title"].asString()).output();

    argList["html.skinsTitle"] = fetchIndexTitle("javascript: displayStudiosSkin();", "/studios/skins/search", "/studios/skins/list", "skins").output();
    Section<StudiosSkinItem> skins = studios_skinList("", 1, 5);
    argList["html.skinsList"] = "";
    for (int i = 0; i < skins.items.size(); i++) 
        argList["html.skinsList"] += skins.items[i].toHTMLObject().output();
    argList["html.skinsBottom"] = fetchIndexBottom("/studios/skins/search", "/studios/skins/list").output();

    argList["html.backgroundsTitle"] = fetchIndexTitle("javascript: displayStudiosBackground();", "/studios/backgrounds/search", "/studios/backgrounds/list", "backgrounds").output();
    Section<StudiosBackgroundItem> backgrounds = studios_backgroundList("", 1, 5);
    argList["html.backgroundsList"] = "";
    for (int i = 0; i < backgrounds.items.size(); i++) 
        argList["html.backgroundsList"] += backgrounds.items[i].toHTMLObject().output();
    argList["html.backgroundsBottom"] = fetchIndexBottom("/studios/backgrounds/search", "/studios/backgrounds/list").output();
    
    argList["html.effectsTitle"] = fetchIndexTitle("javascript: displayStudiosEffect();", "/studios/effects/search", "/studios/effect/list", "effects").output();
    Section<StudiosEffectItem> effects = studios_effectList("", 1, 5);
    argList["html.effectsList"] = "";
    for (int i = 0; i < effects.items.size(); i++) 
        argList["html.effectsList"] += effects.items[i].toHTMLObject().output();
    argList["html.effectsBottom"] = fetchIndexBottom("/studios/effects/search", "/studios/effects/list").output();

    argList["html.createStudiosSkin"] = fetchSearchText("skins-name", "Name", "Enter skin name...", "", false).output();
    argList["html.createStudiosBackground"] = fetchSearchText("backgrounds-name", "Name", "Enter background name...", "", false).output();
    argList["html.createStudiosEffect"] = fetchSearchText("effects-name", "Name", "Enter effect name...", "", false).output();

    header = str_replace(header, argList);
    body = str_replace(body, argList);
    H root = H(true, "html");
    root.append(header);
    root.append(body);
    putRequest(conn, 200, __default_response);
    send(conn, root.output());
    exitRequest(conn);
};