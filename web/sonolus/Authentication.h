const string SonolusPublicKey = "-----BEGIN PUBLIC KEY-----\n"
"MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEd2B14ZAn+zDsqY42rHofst8rw3XB\n"
"90+a5lT80NFdXo0fHOL0MeuUngJVIlVGfJ3EVYFkCCQXFldvLE9JrxIlDA==\n"
"-----END PUBLIC KEY-----";

auto Authentication = [](client_conn conn, http_request request, param argv){
    Json::Value AuthenticateServerRequest;
    json_decode(request.postdata, AuthenticateServerRequest);
    string type = AuthenticateServerRequest["type"].asString();
    string realAddress = (appConfig["server.enableSSL"].asBool() ? "https://" : "http://") + appConfig["server.rootUrl"].asString();
    if (type == "authenticateServer") {
        string address = AuthenticateServerRequest["address"].asString();
        time_t times = AuthenticateServerRequest["time"].asInt64();
        auto userProfile = AuthenticateServerRequest["userProfile"];
        Json::Value AuthenticateServerResponse;
        string json = request.postdata;
        if (!ecdsa_sha256_verify(json, base64_decode(request.argv["sonolus-signature"]), SonolusPublicKey)) quickSendMsg(401);
        if (abs(times / 1000 - time(0)) > appConfig["session.expireTime"].asInt() * 24 * 60 * 60) quickSendMsg(401);
        // if (address.size() < realAddress.size() || address.substr(0, realAddress.size()) != realAddress) quickSendMsg(401);
		string session = generateSession();
        AuthenticateServerResponse["session"] = session;
        AuthenticateServerResponse["expiration"] = (Json::Int64)(times += appConfig["session.expireTime"].asInt64() * 24 * 60 * 60 * 1000);
        // 保存用户信息
        db.execute("INSERT INTO UserSession (uid, session, expire) VALUES (\"" + userProfile["id"].asString() + "\", \"" + session + "\", " + to_string(time(NULL) + appConfig["session.expireTime"].asInt64() * 24 * 60 * 60) + ")", "UserSession");
        usersCreate(UserProfile(userProfile));
        quickSendObj(AuthenticateServerResponse);
    } else if (type == "authenticateExternal") {
        string address = AuthenticateServerRequest["address"].asString();
        time_t times = AuthenticateServerRequest["time"].asInt64();
        auto userProfile = AuthenticateServerRequest["userProfile"];
        Json::Value AuthenticateServerResponse;
        string json = request.postdata;
        if (!ecdsa_sha256_verify(json, base64_decode(request.argv["sonolus-signature"]), SonolusPublicKey)) quickSendMsg(401);
        if (abs(times / 1000 - time(0)) > appConfig["session.expireTime"].asInt() * 24 * 60 * 60) quickSendMsg(401);
        // if (address.size() < realAddress.size() || address.substr(0, realAddress.size()) != realAddress) quickSendMsg(401);
        AuthenticateServerResponse["session"] = getParam(request)["sessionId"];
        AuthenticateServerResponse["expiration"] = (Json::Int64)(times += appConfig["session.expireTime"].asInt64() * 24 * 60 * 60 * 1000);
        // 保存用户信息
        db.execute("UPDATE UserSession SET uid = \"" + userProfile["id"].asString() + "\" WHERE session = \"" + getParam(request)["sessionId"] + "\"", "UserSession");
        usersCreate(UserProfile(userProfile));
        quickSendObj(AuthenticateServerResponse);
    } else quickSendMsg(404);
};