auto SonolusLevelsResultInfo = [](client_conn conn, http_request request, param argv){
    if (!checkLogin(request)) quickSendMsg(401, "Unauthorized.");
    int uid = atoi(getUserProfile(request).handle.c_str());
    bool allowCreate = appConfig["levels.enableResultUpload"].asBool();
    bool isExcept = false;
    for (int i = 0; i < appConfig["levels.exceptResultUpload"].size(); i++)
        if (appConfig["levels.exceptResultUpload"][i].asInt() == uid) isExcept = true;
    bool allowUserCreate = allowCreate ^ isExcept;
    if (!allowUserCreate) quickSendMsg(401, "Unauthorized.");
    Json::Value ServerLevelResultInfo = Json::Value();
    ServerLevelResultInfo["submits"].append(levelResultObject.toJsonObject());
    quickSendObj(ServerLevelResultInfo);
};