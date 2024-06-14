auto SonolusRoomJoin = [](client_conn conn, http_request request, param argv){
	auto items = roomsList("name = \"" + argv[0] + "\"", "");
	if (items.size() == 0) quickSendMsg(404);
	string creatorId = db.query("SELECT creatorId FROM Room where name = \"" + argv[0] + "\"", "Room")[0]["creatorId"];
	
	string id = argv[0];
	string key = request.argv["sonolus-room-key"];
	Json::Value JoinRoomRequest, JoinRoomResponse;
    string json = request.postdata;
    json_decode(json, JoinRoomRequest);
    string type = JoinRoomRequest["type"].asString();
    string realAddress = (appConfig["server.enableSSL"].asBool() ? "https://" : "http://") + appConfig["server.rootUrl"].asString();
    string address = JoinRoomRequest["address"].asString();
    time_t times = JoinRoomRequest["time"].asInt64();
    auto userProfile = JoinRoomRequest["userProfile"];
    if (type != "authenticateMultiplayer") quickSendMsg(401);
    if (!ecdsa_sha256_verify(json, base64_decode(request.argv["sonolus-signature"]), SonolusPublicKey)) quickSendMsg(401);
    if (abs(times / 1000 - time(0)) > appConfig["session.expireTime"].asInt() * 24 * 60 * 60) quickSendMsg(401);

    if (userProfile["id"].asString() == creatorId) {
    	auto $_GET = getParam(request);
    	auto newItem = items[0];
    	if ($_GET["title"] != "") newItem.title = urldecode($_GET["title"]);
    	if ($_GET["subtitle"] != "") newItem.subtitle = urldecode($_GET["subtitle"]);
    	newItem.master = userProfile["name"].asString() + "#" + userProfile["handle"].asString();
    	roomsCreate(newItem);
    }
	string session = generateSession();
	JoinRoomResponse["url"] = appConfig["server.multiplayer.address"].asString() + "/rooms/" + argv[0];
	JoinRoomResponse["type"] = "round";
	JoinRoomResponse["session"] = session;
	string json_base64 = base64_encode(const_cast<char*>(json.c_str()), json.size());
    db.execute("INSERT INTO UserSession (uid, session, expire, body, signature) VALUES (\"" + 
    	userProfile["id"].asString() + "\", \"" + session + "\", " + 
    	to_string(time(NULL) + appConfig["session.expireTime"].asInt64() * 24 * 60 * 60) + ", \"" +
    	json_base64 + "\", \"" + request.argv["sonolus-signature"] + "\")", "UserSession");
    usersCreate(UserProfile(userProfile));
    quickSendObj(JoinRoomResponse);
};
