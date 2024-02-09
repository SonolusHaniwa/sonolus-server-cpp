auto GUIIndex = [](client_conn conn, http_request request, param argv) { 
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/Index.html");
    auto cookie = cookieParam(request);
    argvar argList = argvar();

    // TODO: add the argList here
    argList["page.title"] = appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(appConfig["server.title"].asString()).output();
    argList["html.open_in_sonolus"] = fetchOpenInSonolus("sonolus://" + appConfig["server.rootUrl"].asString()).output();
    string buttons = "";
    if (appConfig["server.hasAuthentication"].asBool()) buttons += indexButton("{{icon.login}}", "Sign In", "/login").output();
    buttons += indexButton("{{icon.post}}", "Posts", "/posts/info").output();
    buttons += indexButton("{{icon.playlist}}", "Playlist", "/playlists/info").output();
    buttons += indexButton("{{icon.level}}", "Levels", "/levels/info").output();
    buttons += indexButton("{{icon.skin}}", "Skins", "/skins/info").output();
    buttons += indexButton("{{icon.background}}", "Backgrounds", "/backgrounds/info").output();
    buttons += indexButton("{{icon.effect}}", "Effects", "/effects/info").output();
    buttons += indexButton("{{icon.particle}}", "Particles", "/particles/info").output();
    buttons += indexButton("{{icon.engine}}", "Engines", "/engines/info").output();
    buttons += indexButton("{{icon.replay}}", "Replays", "/replays/info").output();
    argList["html.buttons"] = buttons;

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