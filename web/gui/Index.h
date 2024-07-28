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
    bool hasSession = db.query("SELECT uid FROM UserSession WHERE session = \"" + cookie["sessionId"] + 
        "\" AND expire <= " + to_string(time(NULL)) + " LIMIT 1", "UserSession").size() > 0;
    if (!isLogin && !hasSession) {
        session = generateSession();
        response["Set-Cookie"] = "sessionId=" + session + "; expires=" + toGMTDate(time(NULL) + appConfig["session.expireTime"].asInt64() * 24 * 60 * 60) + "; path=/",
        db.execute("INSERT INTO UserSession (uid, session, expire) VALUES (\"\", \"" + session + "\", " + to_string(time(NULL) + appConfig["session.expireTime"].asInt64() * 24 * 60 * 60) + ")", "UserSession");
    } else session = cookie["sessionId"];

    // TODO: add the argList here
    bool hasAuthentication = false;
    argList["page.title"] = appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(appConfig["server.title"].asString()).output();
    argList["html.open_in_sonolus"] = fetchOpenInSonolus("sonolus://" + appConfig["server.rootUrl"].asString()).output();
    argList["search.sourceOptions"] = "";
    for (int i = 0; i < appConfig["server.data.prefix"].size(); i++) {
        auto source = appConfig["server.data.prefix"][i];
        argList["search.sourceOptions"] += 
            "<option class=\"bg-sonolus-ui-surface\" value=\"" + to_string(i) + "\" id=\"search-source-" + to_string(i) + "\">" + 
                source["name"].asString() + 
            "</option>";
    }
    argList["search.bannerOptions"] = "";
    for (int i = 0; i < appConfig["server.banner"].size(); i++) {
        auto banner = appConfig["server.banner"][i];
        argList["search.bannerOptions"] += 
            "<option class=\"bg-sonolus-ui-surface\" value=\"" + to_string(i) + "\" id=\"search-banner-" + to_string(i) + "\">" + 
                banner["name"].asString() + 
            "</option>";
    }
    set<string> allowButton = {"authentication", "multiplayer", "post", "playlist", "level", "skin", "background", "effect", "particle", "engine", "replay"};
    string buttons = "";
    for (int i = 0; i < appConfig["server.buttons"].size(); i++) {
        auto button = appConfig["server.buttons"][i]["type"].asString();
        if (button == "authentication") {
            hasAuthentication = true;
            if (!isLogin) buttons += indexButton("{{icon.login}}", "{{language.signIn}}", "javascript:displayLogin()").output();
            else buttons += indexButton("{{icon.logout}}", "{{language.signOut}}", "javascript:logout()").output();
        } else if (button == "configuration") {
            buttons += indexButton("{{icon.configuration}}", "{{language.configuration}}", "javascript: displayConfiguration()").output();
        } else if (allowButton.find(button) != allowButton.end() && button != "multiplayer") 
            buttons += indexButton("{{icon." + button + "}}", "{{language." + button + "s}}", "/" + button + "s/info").output();
    }
    argList["html.buttons"] = buttons;
    argList["url"] = "sonolus://external-login/" + appConfig["server.rootUrl"].asString() + "/sonolus/authenticate?sessionId=" + session;
    argList["isLogin"] = to_string(hasAuthentication && !isLogin);
    argList["server.bannerUrl"] = dataPrefix + appConfig["server.banner"][atoi(cookieParam(request)["banner"].c_str())]["hash"].asString();

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
    string res = root.output();
    res = str_replace(dataPrefix.c_str(), appConfig["server.data.prefix"][atoi(cookieParam(request)["source"].c_str())]["url"].asCString(), res);
    response["Content-Length"] = to_string(res.size());
    putRequest(conn, 200, response);
    send(conn, res);
    exitRequest(conn);
};