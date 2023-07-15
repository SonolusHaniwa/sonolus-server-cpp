using namespace std;

auto web_skins = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/skins.html");
    auto cookie = cookieParam(request);

    Section<SkinItem> skin = skinList("name = \"" + str_replace("\"", "\\\"", argv[0]) + "\" AND localization = \"" + cookie["lang"] + "\"", 1, 1);
    if (skin.items.size() == 0) skin = skinList("name = \"" + str_replace("\"", "\\\"", argv[0]) + "\"", 1, 1);
    if (skin.items.size() == 0) {
        putRequest(conn, 404, __api_default_response);
        send(conn, json_encode(msg[404]));
        exitRequest(conn);
    }
    SkinItem item = skin.items[0];
    Section<SkinItem> recommended = skinList("author = \"" + str_replace("\"", "\\\"", item.author) + "\" AND (localization = \"" + cookie["lang"] + "\" OR localization = \"default\")", 1, 5);
    argvar argList = item.fetchParamList();
    argList = merge(argList, transfer(appConfig));
    argList = merge(argList, transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));
    argList["page.title"] = item.title + " | " + appConfig["server.title"].asString();
    argList["recommended"] = "";
    for (int i = 0; i < recommended.items.size(); i++) 
        argList["recommended"] += recommended.items[i].toHTMLObject().output();
    argList["html.navbar"] = fetchNavBar(item.title, checkLogin(request)).output();
    argList["html.open_in_sonolus"] = fetchOpenInSonolus(argList["sonolus.url"]).output();

    header = str_replace(header, argList);
    body = str_replace(body, argList);
    H root = H(true, "html");
    root.append(header);
    root.append(body);
    putRequest(conn, 200, __default_response);
    send(conn, root.output());
    exitRequest(conn);
};
