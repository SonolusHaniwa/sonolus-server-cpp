auto GUICommunityCommentJump = [](client_conn conn, http_request request, param argv) {
    if (argv[0] != "levels" && 
        argv[0] != "skins" && 
        argv[0] != "backgrounds" && 
        argv[0] != "effects" && 
        argv[0] != "particles" && 
        argv[0] != "engines" && 
        argv[0] != "replays" && 
        argv[0] != "posts" && 
        argv[0] != "playlists") { quickSendMsg(404); }

    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/ItemJump.html");
    auto cookie = cookieParam(request);
    argvar argList = argvar();

    // TODO: add the argList here
    argList["page.title"] = "#jumpTitle | " + appConfig["server.title"].asString();
    argList["html.navbar"] = fetchNavBar("{{language.jumpTitle}}").output();
    argList["pages.current"] = to_string(atoi(argv[2].c_str()) + 1);
    argList["url.base"] = "/" + argv[0] + "/" + argv[1] + "/community/comments/list?";
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
    __default_response["Content-Length"] = to_string(res.size());
    putRequest(conn, 200, __default_response);
    send(conn, res);
    exitRequest(conn);
};