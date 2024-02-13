auto ServerInfo = [](client_conn conn, http_request request, param argv){
    Json::Value ServerInfo;
    auto $_GET = getParam(request);
    ServerInfo["title"] = appConfig["server.title"].asString();
    ServerInfo["description"] = appConfig["server.description"].asString();
    ServerInfo["hasAuthentication"] = appConfig["server.hasAuthentication"].asBool();
    ServerInfo["banner"] = SRL<ServerBanner>(appConfig["server.banner"].asString(), 
        appConfig["server.data.prefix"].asString() + appConfig["server.banner"].asString()).toJsonObject();
    quickSendObj(ServerInfo);
};
