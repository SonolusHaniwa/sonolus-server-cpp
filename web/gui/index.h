using namespace std;

auto web_index = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/index.html");
    auto cookie = cookieParam(request);
    argvar argList = merge(transfer(appConfig), transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));

    // TODO: add the argList here
    argList["page.title"] = appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(appConfig["server.title"].asString(), checkLogin(request)).output();
    argList["html.open_in_sonolus"] = fetchOpenInSonolus("sonolus://" + appConfig["server.rootUrl"].asString()).output();
    argvar filter; filter["localization"] = cookie["lang"];

    argList["html.levelsTitle"] = fetchIndexTitle("/levels/create", "/levels/search", "/levels/list", "levels").output();
    Section<LevelItem> levels = levelsList(levelFilter(filter), 1, 5);
    argList["html.levelsList"] = "";
    for (int i = 0; i < levels.items.size(); i++) 
        argList["html.levelsList"] += levels.items[i].toHTMLObject().output();
    argList["html.levelsBottom"] = fetchIndexBottom("/levels/search", "/levels/list").output();

    argList["html.skinsTitle"] = fetchIndexTitle("/skins/create", "/skins/search", "/skins/list", "skins").output();
    Section<SkinItem> skins = skinsList(skinFilter(filter), 1, 5);
    argList["html.skinsList"] = "";
    for (int i = 0; i < skins.items.size(); i++) 
        argList["html.skinsList"] += skins.items[i].toHTMLObject().output();
    argList["html.skinsBottom"] = fetchIndexBottom("/skins/search", "/skins/list").output();

    argList["html.backgroundsTitle"] = fetchIndexTitle("/backgrounds/create", "/backgrounds/search", "/backgrounds/list", "backgrounds").output();
    Section<BackgroundItem> backgrounds = backgroundsList(backgroundFilter(filter), 1, 5);
    argList["html.backgroundsList"] = "";
    for (int i = 0; i < backgrounds.items.size(); i++) 
        argList["html.backgroundsList"] += backgrounds.items[i].toHTMLObject().output();
    argList["html.backgroundsBottom"] = fetchIndexBottom("/backgrounds/search", "/backgrounds/list").output();
    
    argList["html.effectsTitle"] = fetchIndexTitle("/effects/create", "/effects/search", "/effects/list", "effects").output();
    Section<EffectItem> effects = effectsList(effectFilter(filter), 1, 5);
    argList["html.effectsList"] = "";
    for (int i = 0; i < effects.items.size(); i++) 
        argList["html.effectsList"] += effects.items[i].toHTMLObject().output();
    argList["html.effectsBottom"] = fetchIndexBottom("/effects/search", "/effects/list").output();

    argList["html.particlesTitle"] = fetchIndexTitle("/particles/create", "/particles/search", "/particles/list", "particles").output();
    Section<ParticleItem> particles = particlesList(particleFilter(filter), 1, 5);
    argList["html.particlesList"] = "";
    for (int i = 0; i < particles.items.size(); i++) 
        argList["html.particlesList"] += particles.items[i].toHTMLObject().output();
    argList["html.particlesBottom"] = fetchIndexBottom("/particles/search", "/particles/list").output();

    argList["html.enginesTitle"] = fetchIndexTitle("/engines/create", "/engines/search", "/engines/list", "engines").output();
    Section<EngineItem> engines = enginesList(engineFilter(filter), 1, 5);
    argList["html.enginesList"] = "";
    for (int i = 0; i < engines.items.size(); i++) 
        argList["html.enginesList"] += engines.items[i].toHTMLObject().output();
    argList["html.enginesBottom"] = fetchIndexBottom("/engines/search", "/engines/list").output();
    
    argList["html.replaysTitle"] = fetchIndexTitle("/replays/create", "/replays/search", "/replays/list", "replays").output();
    Section<ReplayItem> replays = replaysList(replayFilter(filter), 1, 5);
    argList["html.replaysList"] = "";
    for (int i = 0; i < replays.items.size(); i++) 
        argList["html.replaysList"] += replays.items[i].toHTMLObject().output();
    argList["html.replaysBottom"] = fetchIndexBottom("/replays/search", "/replays/list").output();

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
