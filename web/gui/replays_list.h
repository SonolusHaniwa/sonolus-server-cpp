using namespace std;

auto web_replays_list = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/replays_list.html");
    auto cookie = cookieParam(request);
    argvar argList = merge(transfer(appConfig), transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));

    // TODO: add the argList here
    argvar $_GET = getParam(request); int page = atoi($_GET["page"].c_str());
    $_GET["localization"] = cookie["lang"];
    if (page < 0) page = 0;
    auto section = replaysList(replayFilter($_GET), page * 20 + 1, (page + 1) * 20);
    argList["page.title"] = argList["language.replays"] + " | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(argList["language.replays"], checkLogin(request)).output();
    string sonolusUrl = "sonolus://" + appConfig["server.rootUrl"].asString() + "/replays/list?" + getStringfy($_GET);
    $_GET["page"] = "0"; string topUrl = "/replays/list?" + getStringfy($_GET);
    $_GET["page"] = to_string(page - 1); string previousUrl = "/replays/list?" + getStringfy($_GET);
    $_GET["page"] = to_string(page + 1); string nextUrl = "/replays/list?" + getStringfy($_GET);
    $_GET["page"] = to_string(section.pageCount - 1); string bottomUrl = "/replays/list?" + getStringfy($_GET);
    $_GET.erase("page"); string jumpUrl = "/replays/jump/" + to_string(page) + "?" + getStringfy($_GET);
    $_GET.erase("localization");
    string searchUrl = "/replays/search?" + getStringfy($_GET);
    argList["html.replaysBottom"] = fetchBottomBar(sonolusUrl, topUrl, previousUrl, nextUrl, bottomUrl, searchUrl, jumpUrl, page, section.pageCount).output();
    argList["html.replaysList"] = "";
    argList["url.list"] = "/replays/list";
    argList["search.display"] = $_GET.size() == 0 ? "style=\"display: none\"" : "";
    argList["search.filterWords"] = "";
    for (auto v : $_GET) {
        if (v.first == "page") continue;
        if (v.first == "localization") continue;
        argList["search.filterWords"] += v.first + ": " + urldecode(v.second) + ", ";
    } if (argList["search.filterWords"].size() >= 2) {
        argList["search.filterWords"].pop_back();
        argList["search.filterWords"].pop_back();
    } for (int i = 0; i < section.items.size(); i++) argList["html.replaysList"] += section.items[i].toHTMLObject().output();

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
