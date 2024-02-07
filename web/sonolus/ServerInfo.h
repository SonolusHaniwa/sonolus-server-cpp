auto ServerInfo = [](client_conn conn, http_request request, param argv){
    Json::Value ServerInfo;
    auto $_GET = getParam(request);
    SRL<ServerBanner> banner = SRL<ServerBanner>(
        appConfig["server.bannerHash"].asString(), appConfig["server.bannerUrl"].asString());
    ServerInfo["title"] = appConfig["server.title"].asString();
    ServerInfo["description"] = appConfig["server.description"].asString();
    ServerInfo["hasAuthentication"] = appConfig["server.hasAuthentication"].asBool();
    ServerInfo["banner"] = banner.toJsonObject();
    quickSendObj(ServerInfo);
};
