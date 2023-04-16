using namespace std;

auto web_particles_jump = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/jump.html");
    auto cookie = cookieParam(request);
    argvar argList = merge(transfer(appConfig), transfer(i18n[cookie["lang"] == "" ? appConfig["language.default"].asString() : cookie["lang"]], "language."));

    // TODO: add the argList here
    argList["page.title"] = argList["language.jumpTitle"] + " | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar("{{language.jumpTitle}}").output();
    argList["pages.current"] = to_string(atoi(argv[0].c_str()) + 1);
    argList["url.base"] = "/particles/list?" + getStringfy(getParam(request));

    header = str_replace(header, argList);
    body = str_replace(body, argList);
    H root = H(true, "html");
    root.append(header);
    root.append(body);
    putRequest(conn, 200, __default_response);
    send(conn, root.output());
    exitRequest(conn);
};