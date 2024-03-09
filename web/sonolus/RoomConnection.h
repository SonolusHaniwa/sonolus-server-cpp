struct RoomEvent {
	string name;
	wsarg connArg;
};

map<string, vector<RoomEvent> > roomEvents;

// 房间主线程负责处理每一轮的指令并发送事件给客户端
void* RoomWorkThread(void* roomId) {
	string id = *(string*)roomId;
	auto items = roomsList("name = \"" + id + "\"", "");
	if (items.size() == 0) pthread_exit(NULL);
	auto room = items[0];
	string creatorId = db.query("SELECT creatorId FROM Room where name = \"" + id + "\"")[0]["creatorId"];
	
	UpdateEvent data; data.options = { quickSearchObject };
	data.optionValues = "type=quick";
	data.allowOtherServers = appConfig["server.multiplayer.allowOtherServers"].asBool();
	vector<wsarg> conns;
	while(true) {
		while(roomEvents[id].size()) {
			auto item = roomEvents[id].front();

			if (item.name == "AddUser") {
				// 数据修改
				auto res = db.query("SELECT * FROM UserSession where session = \"" + item.connArg.request.argv["sonolus-room-session"] + "\"");
				data.users.push_back(RoomUser(res[0]["body"], res[0]["signature"]));
				data.userStatuses.push_back(UserStatusEntry(res[0]["uid"], UserStatus.waiting));
				if (res[0]["uid"] == creatorId) {
					if (data.master == "room") data.master = res[0]["uid"];
					if (data.lead == "room") data.lead = res[0]["uid"];
				}
				// 推送给其他用户
				for (int i = 0; i < conns.size(); i++)
					ws_send(conns[i].conn, json_encode(AddUserEvent(RoomUser(res[0]["body"], res[0]["signature"])).toJsonObject()));
				// 发送当前数据给新用户
				ws_send(item.connArg.conn, json_encode(data.toJsonObject()));
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
				auto res = db.query("SELECT * FROM UserSession where session = \"" + item.connArg.request.argv["sonolus-room-session"] + "\"");
				for (int i = 0; i < data.users.size(); i++) {
					if (data.users[i].authentication == res[0]["body"] && data.users[i].signature == res[0]["signature"]) {
						data.users.erase(data.users.begin() + i);
						data.userStatuses.erase(data.userStatuses.begin() + i);
						break;
					}
				}
				if (data.master == res[0]["uid"]) data.master = "room";
				if (data.lead == res[0]["uid"]) data.lead = "room";
				// 推送给其他用户
				for (int i = 0; i < conns.size(); i++)
					ws_send(conns[i].conn, json_encode(RemoveUserEvent(RoomUser(res[0]["body"], res[0]["signature"])).toJsonObject()));
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
	
	srand(time(0));
	string roomId = argv[0]; int userSessionId = rand() * 1e8 + rand() * 1e4 + rand();
	if (roomEvents.find(argv[0]) == roomEvents.end()) {
		pthread_t pt;
		roomEvents[argv[0]] = {};
		pthread_create(&pt, NULL, RoomWorkThread, (void*)&roomId);
	}
	roomEvents[argv[0]].push_back({ name: "AddUser", connArg: { routeId: userSessionId, conn: conn, request: request, argv: argv }});
	
	while(true) {
		string command = ws_recv(conn);
		if (command == ws_recv_error) {
			roomEvents[argv[0]].push_back({ name: "RemoveUser", connArg: { routeId: userSessionId, conn: conn, request: request, argv: argv }});
			pthread_exit(NULL);
		}
		usleep(10000);
	}
};