auto GUICommunityCommentList = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/ItemCommunityCommentList.html");
    auto cookie = cookieParam(request);
    argvar argList = argvar();

    argvar $_GET = getParam(request);
    argList["html.navbar"] = fetchNavBar("{{language.community}}").output();
    argList["page.title"] = "{{language.community}} | " + appConfig["server.title"].asString();
    int page = $_GET["page"] == "" ? 0 : atoi($_GET["page"].c_str());
    int itemsPerPage = appConfig[string(argv[0]) + ".pageSize.community"].asInt();
    int l = page * itemsPerPage + 1, r = (page + 1) * itemsPerPage;

    auto comments = commentsList("targetType = \"" + argv[0] + "\" AND targetName = \"" + argv[1] + "\"", "time ASC", l, r);
    int pageCount = (commentsNumber("targetType = \"" + argv[0] + "\" AND targetName = \"" + argv[1] + "\"") - 1) / itemsPerPage + 1;
    argList["html.itemsList"] = "";
    for (int i = 0; i < comments.size(); i++) argList["html.itemsList"] += comments[i].toHTMLObject().output();
    argList["url.top"] = "/" + argv[0] + "/" + argv[1] + "/community/comments/list?page=0";
    argList["url.previous"] = "/" + argv[0] + "/" + argv[1] + "/community/comments/list?page=" + to_string(page - 1);
    argList["url.next"] = "/" + argv[0] + "/" + argv[1] + "/community/comments/list?page=" + to_string(page + 1);
    argList["url.bottom"] = "/" + argv[0] + "/" + argv[1] + "/community/comments/list?page=" + to_string(pageCount - 1);
    argList["url.jump"] = "/" + argv[0] + "/" + argv[1] + "/community/comments/jump/" + to_string(page);
    argList["url.sonolus"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/" + argv[0] + "/" + argv[1];
    argList["url.last"] = "/" + argv[0] + "/" + argv[1];
    argList["pages.current"] = to_string(page + 1);
    argList["pages.all"] = to_string(pageCount);
    argList["class.previous"] = page > 0 ? enableClass : disableClass;
    argList["class.next"] = page < pageCount - 1 ? enableClass : disableClass;
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