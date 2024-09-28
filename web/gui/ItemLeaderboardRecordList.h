#define quickGUILeaderboardRecordList(name1, name2) { \
    auto items = name2##List("name = \"" + argv[1] + "\"", "");\
    if (items.size() == 0) quickSendMsg(404, "Item not found."); \
    auto item = items[0]; \
    auto leaderboards = appConfig[defineToString(name2)".leaderboards"]; \
    int leaderboardId = -1; \
    for (int i = 0; i < leaderboards.size(); i++) \
        if (leaderboards[i]["name"] == argv[2]) leaderboardId = i; \
    if (leaderboardId == -1) quickSendMsg(404, "Leaderboard not found."); \
    auto leaderboard = leaderboards[leaderboardId]; \
    argvar args = item.fetchParamList(); \
    string filter = str_replace(leaderboard["filter"].asString(), args); \
    string order = str_replace(leaderboard["order"].asString(), args); \
    pageCount = recordsNumber(leaderboard["groupFunction"].asString(), leaderboard["groupVariable"].asString(), filter); \
    auto records = recordsList<name1##Item>( \
        leaderboard["groupFunction"].asString(), leaderboard["groupVariable"].asString(), \
        filter, order, 1, itemsPerPage \
    ); \
    for (int i = 0; i < records.size(); i++) { \
        argvar args = records[i].fetchParamList(); \
        args["order"] = to_string(i + 1); \
        argvar args2; \
        args2["name"] = records[i].name; \
        args2["rank"] = str_replace(leaderboard["constructor"]["rank"].asString(), args); \
        args2["player"] = str_replace(leaderboard["constructor"]["player"].asString(), args); \
        args2["value"] = str_replace(leaderboard["constructor"]["value"].asString(), args); \
        args2["replays"] = records[i].replay.toHTMLObject().output(); \
        args2["leaderboard"] = leaderboard["name"].asString(); \
        argList["html.itemsList"] += fetchRecord(args2).output(); \
    } \
}

auto GUILeaderboardRecordList = [](client_conn conn, http_request request, param argv) {
    string header = readFile("./web/html/components/header.html");
    string body = readFile("./web/html/pages/ItemLeaderboardRecordList.html");
    auto cookie = cookieParam(request);
    argvar argList = argvar();

    argvar $_GET = getParam(request);
    argList["html.navbar"] = fetchNavBar("{{language.leaderboard}}").output();
    argList["page.title"] = "{{language.leaderboard}} | " + appConfig["server.title"].asString();
    int page = $_GET["page"] == "" ? 0 : atoi($_GET["page"].c_str());
    int itemsPerPage = appConfig[string(argv[0]) + ".pageSize.leaderboard.record"].asInt();
    int l = page * itemsPerPage + 1, r = (page + 1) * itemsPerPage;

    int pageCount = 0;
    argList["html.itemsList"] = "";
    if (argv[0] == "levels") { quickGUILeaderboardRecordList(Level, levels); }
    else if (argv[0] == "skins") { quickGUILeaderboardRecordList(Skin, skins); }
    else if (argv[0] == "backgrounds") { quickGUILeaderboardRecordList(Background, backgrounds); }
    else if (argv[0] == "effects") { quickGUILeaderboardRecordList(Effect, effects); }
    else if (argv[0] == "particles") { quickGUILeaderboardRecordList(Particle, particles); }
    else if (argv[0] == "engines") { quickGUILeaderboardRecordList(Engine, engines); }
    else if (argv[0] == "replays") { quickGUILeaderboardRecordList(Replay, replays); }
    else if (argv[0] == "posts") { quickGUILeaderboardRecordList(Post, posts); }
    else if (argv[0] == "playlists") { quickGUILeaderboardRecordList(Playlist, playlists); }
    else if (argv[0] == "rooms") { quickGUILeaderboardRecordList(Room, rooms); }
    argList["url.top"] = "/" + argv[0] + "/" + argv[1] + "/leaderboard/" + argv[2] + "/list?page=0";
    argList["url.previous"] = "/" + argv[0] + "/" + argv[1] + "/leaderboard/" + argv[2] + "/list?page=" + to_string(page - 1);
    argList["url.next"] = "/" + argv[0] + "/" + argv[1] + "/leaderboard/" + argv[2] + "/list?page=" + to_string(page + 1);
    argList["url.bottom"] = "/" + argv[0] + "/" + argv[1] + "/leaderboard/" + argv[2] + "/list?page=" + to_string(pageCount - 1);
    argList["url.jump"] = "/" + argv[0] + "/" + argv[1] + "/leaderboard/" + argv[2] + "/jump/" + to_string(page);
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