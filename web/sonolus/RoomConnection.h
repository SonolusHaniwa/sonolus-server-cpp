struct RoomEvent {
	string name;
	wsarg connArg;
	LevelLocator level;
};

map<string, vector<RoomEvent> > roomEvents;

template<typename T>
void RoomConnectionSend(wsarg conn, string roomId, T msg) {
	string type = msg.type;
	type[0] -= 'a' - 'A';
	writeLog(LOG_LEVEL_INFO, "Send " + type + "Event to client. (roomId = " + roomId + ", userThread = " + to_string(conn.routeId) + ")");
	ws_send(conn.conn, json_encode(msg.toJsonObject()));
}

// 房间主线程负责处理每一轮的指令并发送事件给客户端
void* RoomWorkThread(void* roomId) {
	string id = *(string*)roomId;
	auto items = roomsList("name = \"" + id + "\"", "");
	if (items.size() == 0) pthread_exit(NULL);
	auto room = items[0];
	string creatorId = db.query("SELECT creatorId FROM Room where name = \"" + id + "\"")[0]["creatorId"];
	writeLog(LOG_LEVEL_INFO, "Room server started! (roomId = " + id + ")");
	
	UpdateEvent data; data.options = { quickSearchObject };
	data.optionValues = "type=quick";
	data.allowOtherServers = appConfig["server.multiplayer.allowOtherServers"].asBool();
	vector<wsarg> conns;
	while(true) {
		while(roomEvents[id].size()) {
			auto item = roomEvents[id].front();

			if (item.name == "AddUser") {
				// 数据修改
				auto res = db.query("SELECT * FROM UserSession where session = \"" + item.connArg.request.argv["sonolus-room-session"] + "\"")[0];
				data.users.push_back(RoomUser(res["body"], res["signature"]));
				data.userStatuses.push_back(UserStatusEntry(res["uid"], UserStatus.waiting));
				// 推送给其他用户
				for (int i = 0; i < conns.size(); i++) {
					RoomConnectionSend(conns[i], id, AddUserEvent(RoomUser(res["body"], res["signature"])));
					if (res["uid"] == creatorId) {
						if (data.master == "room") RoomConnectionSend(conns[i], id, UpdateMasterEvent(res["uid"]));
						if (data.lead == "room") RoomConnectionSend(conns[i], id, UpdateLeadEvent(res["uid"]));
					}
				}
				if (res["uid"] == creatorId) {
					if (data.master == "room") data.master = res["uid"];
					if (data.lead == "room") data.lead = res["uid"];
				}
				// 发送当前数据给新用户
				cout << data.toJsonObject() << endl;
				RoomConnectionSend(item.connArg, id, data);
				conns.push_back(item.connArg);
			}
			else if (item.name == "RemoveUser") {
				// 数据修改
				for (int i = 0; i < conns.size(); i++) {
					if (conns[i].routeId == item.connArg.routeId) {
						conns.erase(conns.begin() + i);
						break;
					}
				}
				auto res = db.query("SELECT * FROM UserSession where session = \"" + item.connArg.request.argv["sonolus-room-session"] + "\"")[0];
				for (int i = 0; i < data.users.size(); i++) {
					if (data.users[i].authentication == res["body"] && data.users[i].signature == res["signature"]) {
						data.users.erase(data.users.begin() + i);
						data.userStatuses.erase(data.userStatuses.begin() + i);
						break;
					}
				}
				// 推送给其他用户
				for (int i = 0; i < conns.size(); i++) {
					RoomConnectionSend(conns[i], id, RemoveUserEvent(RoomUser(res["body"], res["signature"])));
					if (data.master == res["uid"]) RoomConnectionSend(conns[i], id, UpdateMasterEvent(string("room")));
					if (data.lead == res["uid"]) RoomConnectionSend(conns[i], id, UpdateLeadEvent(string("room")));
				}
				if (data.master == res["uid"]) data.master = "room";
				if (data.lead == res["uid"]) data.lead = "room";
			}
			else if (item.name == "UpdateLevel") {
				data.level = item.level;
				for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, UpdateLevelEvent(item.level));
			}


			roomEvents.erase(roomEvents.begin());
		}
		usleep(10000);
	}
	return NULL;
}

// 子线程负责获取用户指令并转换成对应指令给房间主线程
auto SonolusRoomConnection = [](client_conn conn, http_request request, param argv){
	auto items = roomsList("name = \"" + argv[0] + "\"", "");
	if (items.size() == 0) ws_exitRequest(conn), pthread_exit(NULL);
	auto sessions = db.query("SELECT * FROM UserSession where session = \"" + request.argv["sonolus-room-session"] + "\"");
	if (items.size() == 0) ws_exitRequest(conn), pthread_exit(NULL);
	auto session = sessions[0];
	
	srand(time(0));
	string roomId = argv[0]; int userSessionId = getThreadId();
	if (roomEvents.find(argv[0]) == roomEvents.end()) {
		pthread_t pt;
		roomEvents[argv[0]] = {};
		pthread_create(&pt, NULL, RoomWorkThread, (void*)&roomId);
	}
	roomEvents[argv[0]].push_back({ name: "AddUser", connArg: { routeId: userSessionId, conn: conn, request: request, argv: argv }});
	writeLog(LOG_LEVEL_INFO, "Connected to room server! (thread = " + to_string(userSessionId) + ", roomId = " + argv[0] + ", userId = " + session["uid"] + ")");
	
	while(true) {
		string command = ws_recv(conn);
		if (command == ws_recv_error) {
			roomEvents[argv[0]].push_back({ name: "RemoveUser", connArg: { routeId: userSessionId, conn: conn, request: request, argv: argv }});
			writeLog(LOG_LEVEL_INFO, "Sonolus client was disconnected. (roomId = " + argv[0] + ", thread = " + to_string(userSessionId) + ")");
			ws_exitRequest(conn);
			pthread_exit(NULL);
		}
		Json::Value Command; json_decode(command, Command);
		string type = Command["type"].asString(); type[0] -= 'a' - 'A';
		writeLog(LOG_LEVEL_INFO, "Received " + type + "Command from client. (roomId = " + argv[0] + ", thread = " + to_string(userSessionId) + ")");

		if (type == "AddChatMessage") {}
		else if (type == "AddSuggestion") {}
		else if (type == "ClearSuggestions") {}
		else if (type == "AddChatMessage") {}
		else if (type == "AddSuggestion") {}
		else if (type == "ClearSuggestions") {}
		else if (type == "FinishGameplay") {}
		else if (type == "RemoveSuggestion") {}
		else if (type == "RemoveUser") {}
		else if (type == "ReportUser") {}
		else if (type == "ResetScoreboard") {}
		else if (type == "StartGameplay") {}
		else if (type == "SwapSuggestions") {}
		else if (type == "UpdateAutoExit") {}
		else if (type == "UpdateIsSuggestionsLocked") {}
		else if (type == "UpdateLead") {}
		else if (type == "UpdateLevel") roomEvents[argv[0]].push_back({ name: "UpdateLevel", level: UpdateLevelCommand(Command).level });
		else if (type == "UpdateLevelOption") {}
		else if (type == "UpdateMaster") {}
		else if (type == "UpdateOptionValues") {}
		else if (type == "UpdateStatus") {}
		else if (type == "UpdateUserStatus") {}
		else writeLog(LOG_LEVEL_INFO, "Unknown " + type + "Command. (roomId = " + argv[0] + ", thread = " + to_string(userSessionId) + ")");

		usleep(10000);
	}
};