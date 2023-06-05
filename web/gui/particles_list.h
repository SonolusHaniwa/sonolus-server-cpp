using namespace std;

auto web_particles_list = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/particles_list.html");
    auto cookie = cookieParam(request);
    argvar argList = merge(transfer(appConfig), transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));

    // TODO: add the argList here
    argvar $_GET = getParam(request); int page = atoi($_GET["page"].c_str());
    if (page < 0) page = 0;
    auto section = particleList(particleFilter($_GET), page * 20 + 1, (page + 1) * 20);
    argList["page.title"] = argList["language.particles"] + " | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar(argList["language.particles"], checkLogin(request)).output();
    string sonolusUrl = "sonolus://" + appConfig["server.rootUrl"].asString() + "/particles/list?" + getStringfy($_GET);
    $_GET["page"] = "0"; string topUrl = "/particles/list?" + getStringfy($_GET);
    $_GET["page"] = to_string(page - 1); string previousUrl = "/particles/list?" + getStringfy($_GET);
    $_GET["page"] = to_string(page + 1); string nextUrl = "/particles/list?" + getStringfy($_GET);
    $_GET["page"] = to_string(section.pageCount - 1); string bottomUrl = "/particles/list?" + getStringfy($_GET);
    $_GET.erase("page"); string jumpUrl = "/particles/jump/" + to_string(page) + "?" + getStringfy($_GET);
    string searchUrl = "/particles/search?" + getStringfy($_GET);
    argList["html.particlesBottom"] = fetchBottomBar(sonolusUrl, topUrl, previousUrl, nextUrl, bottomUrl, searchUrl, jumpUrl, page, section.pageCount).output();
    argList["html.particlesList"] = "";
    argList["url.list"] = "/particles/list";
    argList["search.display"] = $_GET.size() == 0 ? "style=\"display: none\"" : "";
    argList["search.filterWords"] = "";
    for (auto v : $_GET) {
        if (v.first == "page") continue;
        argList["search.filterWords"] += v.first + ": " + urldecode(v.second) + ", ";
    } if (argList["search.filterWords"].size() >= 2) {
        argList["search.filterWords"].pop_back();
        argList["search.filterWords"].pop_back();
    } for (int i = 0; i < section.items.size(); i++) argList["html.particlesList"] += section.items[i].toHTMLObject().output();

    header = str_replace(header, argList);
    body = str_replace(body, argList);
    H root = H(true, "html");
    root.append(header);
    root.append(body);
    putRequest(conn, 200, __default_response);
    send(conn, root.output());
    exitRequest(conn);
};