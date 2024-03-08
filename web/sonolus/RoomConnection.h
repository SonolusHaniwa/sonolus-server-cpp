struct RoomEvent {
	string name;
	wsarg connArg;
};

map<string, vector<RoomEvent> > roomEvents;

// 房间主线程负责处理每一轮的指令并发送事件给客户端
void* RoomWorkThread(void* roomId) {
	string id = *(string*)roomId;
	UpdateEvent data;
	vector<wsarg> conns;
	while(true) {
		for (int i = 0; i < roomEvents[id].size(); i++) {
			auto item = roomEvents[id][i];
			if (item.name == "AddUser") {
				data.users.push_back(RoomUser("", item.connArg.request.argv["sonolus-room-session"]));
				ws_send(item.connArg.conn, json_encode(data.toJsonObject()));
				conns.push_back(item.connArg);
			}
		}
		roomEvents[id].clear();
		usleep(10000);
	}
	return NULL;
}

// 子线程负责获取用户指令并转换成对应指令给房间主线程
auto SonolusRoomConnection = [](client_conn conn, http_request request, param argv){
	// string info = json_encode(UpdateEvent().toJsonObject());
	// ws_send(conn, info);
	string roomId = argv[0];
	if (roomEvents.find(argv[0]) == roomEvents.end()) {
		pthread_t pt;
		roomEvents[argv[0]] = {};
		pthread_create(&pt, NULL, RoomWorkThread, (void*)&roomId);
	}
	roomEvents[argv[0]].push_back({ name: "AddUser", connArg: { conn: conn, request: request, argv: argv }});
	
	while(true) {
		string command = ws_recv(conn);
		cout << "Received from client: " << command << endl;
		usleep(10000);
	}
};