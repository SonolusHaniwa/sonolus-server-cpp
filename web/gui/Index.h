string toGMTDate(time_t t) {
    struct tm *tm = gmtime(&t);
    char buf[1024];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", tm);
    return buf;
}

auto GUIIndex = [](client_conn conn, http_request request, param argv) { 
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/Index.html");
    auto cookie = cookieParam(request);
    argvar argList = argvar(); string session = "";
    auto response = __default_response;
    bool isLogin = checkLogin(request);
    if (!isLogin && cookie["sessionId"] == "") {
        session = generateSession();
        response["Set-Cookie"] = "sessionId=" + session + "; expires=" + toGMTDate(time(NULL) + appConfig["session.expireTime"].asInt64() * 24 * 60 * 60) + "; path=/",
        db.execute("INSERT INTO UserSession (uid, session, expire) VALUES (\"\", \"" + session + "\", " + to_string(time(NULL) + appConfig["session.expireTime"].asInt64() * 24 * 60 * 60) + ")", "UserSession");
    } else session = cookie["sessionId"];

    // TODO: add the argList here
    argList["page.title"] = appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(appConfig["server.title"].asString()).output();
    argList["html.open_in_sonolus"] = fetchOpenInSonolus("sonolus://" + appConfig["server.rootUrl"].asString()).output();
    string buttons = "";
    if (appConfig["server.hasAuthentication"].asBool()) {
        if (!isLogin) buttons += indexButton("{{icon.login}}", "Sign In", "javascript:displayLogin()").output();
        else buttons += indexButton("{{icon.logout}}", "Sign Out", "javascript:logout()").output();
    } buttons += indexButton("{{icon.post}}", "{{language.posts}}", "/posts/info").output();
    buttons += indexButton("{{icon.playlist}}", "{{language.playlists}}", "/playlists/info").output();
    buttons += indexButton("{{icon.level}}", "{{language.levels}}", "/levels/info").output();
    buttons += indexButton("{{icon.skin}}", "{{language.skins}}", "/skins/info").output();
    buttons += indexButton("{{icon.background}}", "{{language.backgrounds}}", "/backgrounds/info").output();
    buttons += indexButton("{{icon.effect}}", "{{language.effects}}", "/effects/info").output();
    buttons += indexButton("{{icon.particle}}", "{{language.particles}}", "/particles/info").output();
    buttons += indexButton("{{icon.engine}}", "{{language.engines}}", "/engines/info").output();
    buttons += indexButton("{{icon.replay}}", "{{language.replays}}", "/replays/info").output();
    argList["html.buttons"] = buttons;
    argList["url"] = "sonolus://external-login/" + appConfig["server.rootUrl"].asString() + "/sonolus/authenticate?sessionId=" + session;
    argList["isLogin"] = to_string(appConfig["server.hasAuthentication"].asBool() && !isLogin);

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
    response["Content-Length"] = to_string(root.output().size());
    putRequest(conn, 200, response);
    send(conn, root.output());
    exitRequest(conn);
};