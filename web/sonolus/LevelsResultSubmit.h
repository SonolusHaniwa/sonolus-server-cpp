auto SonolusLevelsResultSubmit = [](client_conn conn, http_request request, param argv){
    if (!checkLogin(request)) quickSendMsg(401, "Unauthorized.");
    int uid = atoi(getUserProfile(request).handle.c_str());
    bool allowCreate = appConfig["levels.enableResultUpload"].asBool();
    bool isExcept = false;
    for (int i = 0; i < appConfig["levels.exceptResultUpload"].size(); i++)
        if (appConfig["levels.exceptResultUpload"][i].asInt() == uid) isExcept = true;
    bool allowUserCreate = allowCreate ^ isExcept;
    if (!allowUserCreate) quickSendMsg(401, "Unauthorized.");
    ReplayItem replay = ReplayItem(-1, json_decode(request.postdata)["replay"]);
    auto $_POST = (request.postdata = json_decode(request.postdata)["values"].asString(), postParam(request));
    LevelItem level = replay.level;
    if (!levelsNumber("name = \"" + replay.level.name + "\"")) 
        quickSendMsg(404, "Level not found.");
    level = levelsList("name = \"" + replay.level.name + "\"", "")[0];
    if (replaysNumber("data = \"" + replay.data.hash + "\" AND configuration = \"" + replay.configuration.hash + "\""))
        quickSendMsg(409, "Conflict.");

    replay.name = "replay-" + generateSession();
    replay.isPrivate = $_POST.find("private") != $_POST.end();
    replay.isRank = $_POST.find("rank") == $_POST.end();
	replay.allowRank = false;
    replaysCreate(replay);
    replay = replaysList("name = \"" + replay.name + "\"", "")[0];
    Record<LevelItem> record = Record<LevelItem>(
        -1, generateSession(), "levels", level, replay, getUserProfile(request),
        0, 0, 0, 0, 0, 0, "", 0, 0, 0, 0, 0, 0, 0, 0, $_POST.find("private") != $_POST.end(), true, $_POST.find("rank") == $_POST.end()
    );
    recordsCreate(record);
    Json::Value ServerSubmitLevelResultResponse;
    ServerSubmitLevelResultResponse["key"] = record.name;
    ServerSubmitLevelResultResponse["hashes"].resize(0);
    ServerSubmitLevelResultResponse["hashes"].append(replay.configuration.hash);
    ServerSubmitLevelResultResponse["hashes"].append(replay.data.hash);
    quickSendObj(ServerSubmitLevelResultResponse);
};