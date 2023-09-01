using namespace std;

string toGMTDate(time_t t) {
    struct tm *tm = gmtime(&t);
    char buf[1024];
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", tm);
    return buf;
}

auto web_login = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/levels.html");

    if (checkLogin(request)) {
        __default_response["Content-Length"] = "75";
        putRequest(conn, 302, __default_response);
        send(conn, "<script>alert(\"Login Success!\"); location.href=document.referrer;</script>");
        exitRequest(conn);
    }

    auto cookie = cookieParam(request);

    srand(time(0));
    string code = ""; for (int i = 0; i < 8; i++) code += rand() % 10 + '0';
    // 防止 code 冲突
    while ((new DB_Controller)->query("SELECT * FROM LoginRequest WHERE code=\"" + code + "\" AND userId=0").size()) {
        code.clear();
        for (int i = 0; i < 8; i++) code += rand() % 10 + '0';
    } string session = "";
    for (int i = 0; i < 32; i++) session += rand() % 2 ? rand() % 10 + '0' : rand() % 6 + 'A';
    (new DB_Controller)->execute("INSERT INTO LoginRequest (code, session, time, ip, userId) VALUES (\"" + 
        code + "\", \"" + session + "\", " + to_string(time(NULL)) + ", \"" + string(inet_ntoa(conn.client_addr.sin_addr)) + "\", 0)");
    argvar argList = transfer(appConfig);
    argList = merge(argList, transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));
    LevelItem item = LevelItem(
        0, "sonolus-server-auth", 0,
        str_replace("{{language.loginTitle}}", argList),
        str_replace("{{language.loginSubtitle}}", argList),
        "System", EngineItem(0, "sonolus-server-auth", "(Message)", "", "", {}, {}, {}, {}, {}, {}, {}, {}, {}),
        UseItem<SkinItem>(), UseItem<BackgroundItem>(), UseItem<EffectItem>(), UseItem<ParticleItem>(),
        SRL<LevelCover>("", appConfig["server.auth.logo"].asString()), SRL<LevelBgm>(), SRL<LevelData>(), SRL<LevelPreview>()
    );
    string description = "{{language.loginTipTitle}}<br/><br/>{{language.loginTipStep1}}<br/>{{language.loginTipStep2}}<br/><br/>{{language.loginTipAlternative}}";
    argList["server.auth.url"] = appConfig["server.rootUrl"].asString() + "/auth";
    argList["login.code"] = code;
    description = str_replace(description, argList);
    argList = merge(argList, item.fetchParamList());
    argList["page.title"] = item.title + " | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(item.title, checkLogin(request)).output();
    argList["html.open_in_sonolus"] = fetchOpenInSonolus("sonolus://" + appConfig["server.rootUrl"].asString() + "/auth/levels/" + code).output();
    argList["recommended"] = "";
    argList["description"] = description;

    header = str_replace(header, argList);
    body = str_replace(body, argList);
    H root = H(true, "html");
    root.append(header);
    root.append(body);
    auto response = __default_response;
    response["Set-Cookie"] = "sessionId=" + session + "; Path=/; Expires=" + toGMTDate(time(NULL) + appConfig["session.expireTime"].asInt64() * 24 * 60 * 60);
    response["Content-Length"] = to_string(root.output().size());
    putRequest(conn, 200, response);
    send(conn, root.output());
    exitRequest(conn);
};
