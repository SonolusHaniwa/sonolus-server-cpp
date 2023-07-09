using namespace std;

auto web_engines = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/engines.html");
    auto cookie = cookieParam(request);

    Section<EngineItem> engine = engineList("name = \"" + str_replace("\"", "\\\"", argv[0]) + "\"", 1, 1);
    if (engine.items.size() == 0) {
        putRequest(conn, 404, __api_default_response);
        send(conn, json_encode(msg[404]));
        exitRequest(conn);
    }
    EngineItem item = engine.items[0];
    Section<EngineItem> recommended = engineList("author = \"" + str_replace("\"", "\\\"", item.author) + "\"", 1, 5);
    argvar argList = item.fetchParamList();
    argList = merge(argList, transfer(appConfig));
    argList = merge(argList, transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));
    argList["page.title"] = item.title + " | " + appConfig["server.title"].asString();
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
