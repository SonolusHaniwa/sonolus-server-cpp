using namespace std;

auto web_index = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/index.html");
    auto cookie = cookieParam(request);
    argvar argList = merge(transfer(appConfig), transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));

    // TODO: add the argList here
    argList["page.title"] = appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(appConfig["server.title"].asString()).output();
    argList["html.open_in_sonolus"] = fetchOpenInSonolus("sonolus:" + appConfig["server.rootUrl"].asString()).output();

    argList["html.levelsTitle"] = fetchComponentsTitle("/levels/search/0", "/levels/list", "levels").output();
    Section<LevelItem> levels = levelList("", 1, 5);
    argList["html.levelsList"] = "";
    for (int i = 0; i < levels.items.size(); i++) 
        argList["html.levelsList"] += levels.items[i].toHTMLObject().output();
    argList["html.levelsBottom"] = fetchComponentsBottom("/levels/search/0", "/levels/list").output();

    argList["html.skinsTitle"] = fetchComponentsTitle("/skins/search/0", "/skins/list", "skins").output();
    Section<SkinItem> skins = skinList("", 1, 5);
    argList["html.skinsList"] = "";
    for (int i = 0; i < skins.items.size(); i++) 
        argList["html.skinsList"] += skins.items[i].toHTMLObject().output();
    argList["html.skinsBottom"] = fetchComponentsBottom("/skins/search/0", "/skins/list").output();

    argList["html.backgroundsTitle"] = fetchComponentsTitle("/backgrounds/search/0", "/backgrounds/list", "backgrounds").output();
    Section<BackgroundItem> backgrounds = backgroundList("", 1, 5);
    argList["html.backgroundsList"] = "";
    for (int i = 0; i < backgrounds.items.size(); i++) 
        argList["html.backgroundsList"] += backgrounds.items[i].toHTMLObject().output();
    argList["html.backgroundsBottom"] = fetchComponentsBottom("/backgrounds/search/0", "/backgrounds/list").output();
    
    argList["html.effectsTitle"] = fetchComponentsTitle("/effects/search/0", "/effect/list", "effects").output();
    Section<EffectItem> effects = effectList("", 1, 5);
    argList["html.effectsList"] = "";
    for (int i = 0; i < effects.items.size(); i++) 
        argList["html.effectsList"] += effects.items[i].toHTMLObject().output();
    argList["html.effectsBottom"] = fetchComponentsBottom("/effects/search/0", "/effects/list").output();

    argList["html.particlesTitle"] = fetchComponentsTitle("/particles/search/0", "/particles/list", "particles").output();
    Section<ParticleItem> particles = particleList("", 1, 5);
    argList["html.particlesList"] = "";
    for (int i = 0; i < particles.items.size(); i++) 
        argList["html.particlesList"] += particles.items[i].toHTMLObject().output();
    argList["html.particlesBottom"] = fetchComponentsBottom("/particles/search/0", "/particles/list").output();\

    argList["html.enginesTitle"] = fetchComponentsTitle("/engines/search/0", "/engines/list", "engines").output();
    Section<EngineItem> engines = engineList("", 1, 5);
    argList["html.enginesList"] = "";
    for (int i = 0; i < engines.items.size(); i++) 
        argList["html.enginesList"] += engines.items[i].toHTMLObject().output();
    argList["html.enginesBottom"] = fetchComponentsBottom("/engines/search/0", "/engines/list").output();

    header = str_replace(header, argList);
    body = str_replace(body, argList);
    H root = H(true, "html");
    root.append(header);
    root.append(body);
    putRequest(conn, 200, __default_response);
    send(conn, root.output());
    exitRequest(conn);
};