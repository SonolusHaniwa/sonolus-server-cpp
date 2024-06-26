struct RoomEvent {
	string name;
	wsarg connArg;
	bool bValue;
	int64_t iValue;
	double dValue;
	string sValue;
	LevelLocator level;
	GameplayResult result;
	LevelItem levelItem;
	ChatMessage msg;
	Suggestion suggestionA, suggestionB;
	LevelOptionEntry levelOption;
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
	string creatorId = db.query("SELECT creatorId FROM Room where name = \"" + id + "\"", "Room")[0]["creatorId"];
	writeLog(LOG_LEVEL_INFO, "Room server started! (roomId = " + id + ")");
	
	UpdateEvent data; data.options = { quickSearchObject };
	data.optionValues = "type=quick";
	data.allowOtherServers = appConfig["server.multiplayer.allowOtherServers"].asBool();
	data.scoreboardDescription = "测试";
	data.scoreboardSections = { { "Total Score", "medal", {} } };
	vector<wsarg> conns;
	vector<string> userState;
	bool createdScoreboard = false;
	while(true) {
		while(roomEvents[id].size()) {
			auto item = roomEvents[id].front();

			if (item.name == "AddUser") {
				// 数据修改
				auto res = db.query("SELECT * FROM UserSession where session = \"" + item.connArg.request.argv["sonolus-room-session"] + "\"", "UserSession")[0];
				data.users.push_back(RoomUser(res["body"], res["signature"]));
				data.userStatuses.push_back(UserStatusEntry(res["uid"], UserStatus.waiting));
				data.scoreboardSections[0].scores.push_back(ScoreEntry(res["uid"], "0"));
				userState.push_back(generateSession());
				// 推送给其他用户
				for (int i = 0; i < conns.size(); i++) {
					RoomConnectionSend(conns[i], id, AddUserEvent(RoomUser(res["body"], res["signature"])));
					RoomConnectionSend(conns[i], id, InsertScoreboardSectionScoreEvent(0, conns.size(), data.scoreboardSections[0].scores.back()));
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
				RoomConnectionSend(item.connArg, id, data);
				conns.push_back(item.connArg);
			}
			else if (item.name == "RemoveUser") {
				// 数据修改
				int id1 = -1;
				for (int i = 0; i < conns.size(); i++) {
					if (conns[i].routeId == item.connArg.routeId) {
						conns.erase(conns.begin() + i); id1 = i;
						break;
					}
				}
				if (id1 == -1) goto next;
				auto res = db.query("SELECT * FROM UserSession where session = \"" + item.connArg.request.argv["sonolus-room-session"] + "\"", "UserSession")[0];
				for (int i = 0; i < data.users.size(); i++)
					if (data.users[i].authentication == res["body"] && data.users[i].signature == res["signature"]) {
						data.users.erase(data.users.begin() + i);
						data.userStatuses.erase(data.userStatuses.begin() + i);
						userState.erase(userState.begin() + i);
						break;
					}
				for (int i = 0; i < data.scoreboardSections.size(); i++) {
					for (int j = 0; j < data.scoreboardSections[i].scores.size(); j++) {
						if (data.scoreboardSections[i].scores[j].userId != res["uid"]) continue;
						data.scoreboardSections[i].scores.erase(data.scoreboardSections[i].scores.begin() + j);
						for (int k = 0; k < conns.size(); k++)
							RoomConnectionSend(conns[k], id, RemoveScoreboardSectionScoreEvent(i, j));
						break; 
					}
				}
				// 推送给其他用户
				for (int i = 0; i < conns.size(); i++) {
					if (data.master == res["uid"]) RoomConnectionSend(conns[i], id, UpdateMasterEvent(string("room")));
					if (data.lead == res["uid"]) RoomConnectionSend(conns[i], id, UpdateLeadEvent(string("room")));
					RoomConnectionSend(conns[i], id, RemoveUserEvent(RoomUser(res["body"], res["signature"])));
				}
				if (data.master == res["uid"]) data.master = "room";
				if (data.lead == res["uid"]) data.lead = "room";
			}
			else if (item.name == "UpdateLevel") {
				data.level = item.level;
				for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, UpdateLevelEvent(item.level));
			}
			else if (item.name == "UpdateStatus") {
				data.status = item.sValue; srand(time(NULL));
				for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, UpdateStatusEvent(item.sValue));
				if (item.sValue == RoomStatus.playing) for (int i = 0; i < data.users.size(); i++) {
					if (data.userStatuses[i].status == UserStatus.ready) RoomConnectionSend(conns[i], id, StartRoundEvent(userState[i], 1.0 / rand()));
					else data.userStatuses[i].status = UserStatus.waiting;
				} else if (item.sValue == RoomStatus.selecting) {
					if (data.scoreboardSections.size() > 1) {
						auto tmp = data.scoreboardSections[1].scores;
						sort(tmp.begin(), tmp.end(), [](ScoreEntry a, ScoreEntry b){
							return atoi(a.value.c_str()) > atoi(b.value.c_str());
						});
						for (int i = 0; i < min(3, (int)tmp.size()); i++) {
							for (int j = 0; j < data.scoreboardSections[0].scores.size(); j++) {
								if (data.scoreboardSections[0].scores[j].userId != tmp[i].userId) continue;
								data.scoreboardSections[0].scores[j].value = to_string(
									atoi(data.scoreboardSections[0].scores[j].value.c_str()) + 3 - i
								);
							}
						}
						sort(data.scoreboardSections[0].scores.begin(), data.scoreboardSections[0].scores.end(), [](ScoreEntry a, ScoreEntry b){
							return atoi(a.value.c_str()) > atoi(b.value.c_str());
						});
						for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, UpdateScoreboardSectionScoresEvent(0, data.scoreboardSections[0].scores));
					}
					createdScoreboard = false;
					for (int i = 0; i < data.users.size(); i++) data.userStatuses[i].status = UserStatus.waiting;
				}
			}
			else if (item.name == "StartGameplay") {
				if (!createdScoreboard) {
					data.scoreboardSections.insert(data.scoreboardSections.begin() + 1, { item.levelItem.title, "", {} });
					for (int i = 0; i < conns.size(); i++) {
						auto res = db.query("SELECT * FROM UserSession where session = \"" + conns[i].request.argv["sonolus-room-session"] + "\"", "UserSession")[0];
						data.scoreboardSections[1].scores.push_back(ScoreEntry(res["uid"], data.userStatuses[i].status == UserStatus.ready ? "Playing" : "Skipped"));
					}
					for (int i = 0; i < data.users.size(); i++)
						RoomConnectionSend(conns[i], id, InsertScoreboardSectionEvent(1, data.scoreboardSections[1]));
					createdScoreboard = true;
				}
			}
			else if (item.name == "UpdateUserStatus") {
				auto res = db.query("SELECT * FROM UserSession where session = \"" + item.connArg.request.argv["sonolus-room-session"] + "\"", "UserSession")[0];
				for (int i = 0; i < data.users.size(); i++) 
					if (data.users[i].authentication == res["body"] && data.users[i].signature == res["signature"]) 
						data.userStatuses[i] = UserStatusEntry(res["uid"], item.sValue);
				auto event = UpdateUserStatusEvent(UserStatusEntry(res["uid"], item.sValue));
				for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, event);
			}
			else if (item.name == "FinishGameplay") {
				auto res = db.query("SELECT * FROM UserSession where session = \"" + item.connArg.request.argv["sonolus-room-session"] + "\"", "UserSession")[0];
				int uid = -1;
				for (int i = 0; i < data.users.size(); i++)
					if (data.users[i].authentication == res["body"] && data.users[i].signature == res["signature"]) 
						data.userStatuses[i].status = UserStatus.waiting, uid = i;
				if (data.scoreboardSections.size() > 1) {
					data.scoreboardSections[1].scores[uid].value = to_string(item.result.arcadeScore);
						// + " / " + to_string(item.result.accuracyScore)
						// + " / " + to_string(item.result.perfect) + " / " + to_string(item.result.great)
						// + " / " + to_string(item.result.great) + " / " + to_string(item.result.good);
					for (int i = 0; i < data.users.size(); i++)
						if (data.userStatuses[i].status != UserStatus.playing)
							RoomConnectionSend(conns[i], id, UpdateScoreboardSectionScoresEvent(1, data.scoreboardSections[1].scores));
				}
				RoomConnectionSend(item.connArg, id, data);
			}
			else if (item.name == "AddChatMessage") {
				for (int i = 0; i < conns.size(); i++) 
					RoomConnectionSend(conns[i], id, AddChatMessageEvent(item.iValue, item.msg));
			}
			else if (item.name == "ResetScoreboard") {
				for (int i = 0; i < data.scoreboardSections[0].scores.size(); i++) 
					data.scoreboardSections[0].scores[i].value = "0";
				for (int i = 0; i < conns.size(); i++) {
					RoomConnectionSend(conns[i], id, UpdateScoreboardSectionScoresEvent(0, data.scoreboardSections[0].scores));
					for (int j = 1; j < data.scoreboardSections.size(); j++)
						RoomConnectionSend(conns[i], id, RemoveScoreboardSectionEvent(1));
				}
				while (data.scoreboardSections.size() > 1) data.scoreboardSections.pop_back();
			}
			else if (item.name == "AddSuggestion") {
				auto res = db.query("SELECT * FROM UserSession where session = \"" + item.connArg.request.argv["sonolus-room-session"] + "\"", "UserSession")[0];
				data.suggestions.push_back(Suggestion(res["uid"], item.level));
				for (int i = 0; i < conns.size(); i++) 
					RoomConnectionSend(conns[i], id, AddSuggestionEvent(data.suggestions.back()));
			}
			else if (item.name == "ClearSuggestions") {
				while(data.suggestions.size()) data.suggestions.pop_back();
				for (int i = 0; i < conns.size(); i++) 
					RoomConnectionSend(conns[i], id, ClearSuggestionsEvent());
			}
			else if (item.name == "SwapSuggestions") {
				int id1 = -1, id2 = -1;
				for (int i = 0; i < data.suggestions.size(); i++)
					if (data.suggestions[i].userId == item.suggestionA.userId && 
						data.suggestions[i].level.name == item.suggestionA.level.name &&
						data.suggestions[i].level.address == item.suggestionA.level.address) id1 = i;
				for (int i = 0; i < data.suggestions.size(); i++)
					if (data.suggestions[i].userId == item.suggestionB.userId && 
						data.suggestions[i].level.name == item.suggestionB.level.name &&
						data.suggestions[i].level.address == item.suggestionB.level.address) id2 = i;
				swap(data.suggestions[id1], data.suggestions[id2]);
				for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, SwapSuggestionsEvent(item.suggestionA, item.suggestionB));
			}
			else if (item.name == "RemoveSuggestion") {
				int id1 = -1;
				for (int i = 0; i < data.suggestions.size(); i++)
					if (data.suggestions[i].userId == item.suggestionA.userId && 
						data.suggestions[i].level.name == item.suggestionA.level.name &&
						data.suggestions[i].level.address == item.suggestionA.level.address) id1 = i;
				data.suggestions.erase(data.suggestions.begin() + id1);
				for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, RemoveSuggestionEvent(item.suggestionA));
			}
			else if (item.name == "RemoveUser2") {
				int id1 = -1;
				for (int i = 0; i < conns.size(); i++) {
					auto res = db.query("SELECT * FROM UserSession where session = \"" + conns[i].request.argv["sonolus-room-session"] + "\"", "UserSession")[0];
					if (res["uid"] == item.sValue) { id1 = i; break; }
				}
				roomEvents[id].push_back({ name: "RemoveUser", connArg: conns[id1] });
				ws_exitRequest(conns[id1].conn);
			}
			else if (item.name == "UpdateAutoExit") {
				data.autoExit = item.sValue;
				for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, UpdateAutoExitEvent(item.sValue));
			}
			else if (item.name == "UpdateIsSuggestionsLocked") {
				data.isSuggestionsLocked = item.bValue;
				for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, UpdateIsSuggestionsLockedEvent(item.bValue));
			}
			else if (item.name == "UpdateLead") {
				data.lead = item.sValue;
				for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, UpdateLeadEvent(item.sValue));
			}
			else if (item.name == "UpdateMaster") {
				data.master = item.sValue;
				for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, UpdateMasterEvent(item.sValue));
			}
			else if (item.name == "UpdateOptionValues") {
				data.optionValues = item.sValue;
				for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, UpdateOptionValuesEvent(item.sValue));
			}
			else if (item.name == "UpdateLevelOption") {
				int id1 = -1;
				for (int i = 0; i < data.levelOptions.size(); i++)
					if (data.levelOptions[i].index == item.levelOption.index)
						data.levelOptions[i] = item.levelOption, id1 = i;
				if (id1 == -1) data.levelOptions.push_back(item.levelOption);
				for (int i = 0; i < conns.size(); i++) RoomConnectionSend(conns[i], id, UpdateLevelOptionEvent(item.levelOption));
			}

			next:
			roomEvents[id].erase(roomEvents[id].begin());
		}
		#ifdef __linux__
		usleep(10 * 1000);
		#else
		Sleep(10);
		#endif
	}
	return NULL;
}

// 子线程负责获取用户指令并转换成对应指令给房间主线程
auto SonolusRoomConnection = [](client_conn conn, http_request request, param argv){
	auto items = roomsList("name = \"" + argv[0] + "\"", "");
	if (items.size() == 0) ws_exitRequest(conn), pthread_exit(NULL);
	auto sessions = db.query("SELECT * FROM UserSession where session = \"" + request.argv["sonolus-room-session"] + "\"", "UserSession");
	if (items.size() == 0) ws_exitRequest(conn), pthread_exit(NULL);
	auto session = sessions[0];
	
	srand(time(0));
	string roomId = argv[0]; int userSessionId = getThreadId();
	if (roomEvents.find(argv[0]) == roomEvents.end()) {
		pthread_t pt;
		roomEvents[argv[0]] = {};
		pthread_create(&pt, NULL, RoomWorkThread, (void*)&roomId);
	}
	roomEvents[argv[0]].push_back({ name: "AddUser", connArg: { routeId: userSessionId, conn: conn, request: request, argv: argv } });
	writeLog(LOG_LEVEL_INFO, "Connected to room server! (thread = " + to_string(userSessionId) + ", roomId = " + argv[0] + ", userId = " + session["uid"] + ")");
	
	while(true) {
		string command = ws_recv(conn);
		if (command == ws_recv_error) {
			roomEvents[argv[0]].push_back({ name: "RemoveUser", connArg: { routeId: userSessionId, conn: conn, request: request, argv: argv } });
			writeLog(LOG_LEVEL_INFO, "Sonolus client was disconnected. (roomId = " + argv[0] + ", thread = " + to_string(userSessionId) + ")");
			ws_exitRequest(conn);
			pthread_exit(NULL);
		}
		Json::Value Command; json_decode(command, Command);
		string type = Command["type"].asString(); type[0] -= 'a' - 'A';
		writeLog(LOG_LEVEL_INFO, "Received " + type + "Command from client. (roomId = " + argv[0] + ", thread = " + to_string(userSessionId) + ")");

		if (type == "AddChatMessage") roomEvents[argv[0]].push_back({
			name: "AddChatMessage",
			connArg: { routeId: userSessionId, conn: conn, request: request, argv: argv },
			iValue: AddChatMessageCommand(Command).nonce,
			msg: AddChatMessageCommand(Command).message
		});
		else if (type == "AddSuggestion") roomEvents[argv[0]].push_back({
			name: "AddSuggestion",
			connArg: { routeId: userSessionId, conn: conn, request: request, argv: argv },
			level: AddSuggestionCommand(Command).level
		});
		else if (type == "ClearSuggestions") roomEvents[argv[0]].push_back({
			name: "ClearSuggestions"	
		});
		else if (type == "FinishGameplay") roomEvents[argv[0]].push_back({
			name: "FinishGameplay",
			connArg: { routeId: userSessionId, conn: conn, request: request, argv: argv },
			result: FinishGameplayCommand(Command).result
		});
		else if (type == "RemoveSuggestion") roomEvents[argv[0]].push_back({
			name: "RemoveSuggestion",
			suggestionA: RemoveSuggestionCommand(Command).suggestion
		});
		else if (type == "RemoveUser") roomEvents[argv[0]].push_back({
			name: "RemoveUser2",
			sValue: RemoveUserCommand(Command).userId	
		});
		else if (type == "ReportUser") {}
		else if (type == "ResetScoreboard") roomEvents[argv[0]].push_back({
			name: "ResetScoreboard"
		});
		else if (type == "StartGameplay") roomEvents[argv[0]].push_back({
			name: "StartGameplay",
			levelItem: StartGameplayCommand(Command).level
		});
		else if (type == "SwapSuggestions") roomEvents[argv[0]].push_back({
			name: "SwapSuggestions",
			suggestionA: SwapSuggestionsCommand(Command).suggestionA,
			suggestionB: SwapSuggestionsCommand(Command).suggestionB
		});
		else if (type == "UpdateAutoExit") roomEvents[argv[0]].push_back({
			name: "UpdateAutoExit",
			sValue: UpdateAutoExitCommand(Command).autoExit	
		});
		else if (type == "UpdateIsSuggestionsLocked") roomEvents[argv[0]].push_back({
			name: "UpdateIsSuggestionsLocked",
			bValue: UpdateIsSuggestionsLockedCommand(Command).isSuggestionsLocked
		});
		else if (type == "UpdateLead") roomEvents[argv[0]].push_back({
			name: "UpdateLead",
			sValue: UpdateLeadCommand(Command).lead
		});
		else if (type == "UpdateLevel") roomEvents[argv[0]].push_back({
			name: "UpdateLevel",
			level: UpdateLevelCommand(Command).level
		});
		else if (type == "UpdateLevelOption") roomEvents[argv[0]].push_back({
			name: "UpdateLevelOption",
			levelOption: UpdateLevelOptionCommand(Command).levelOption
		});
		else if (type == "UpdateMaster") roomEvents[argv[0]].push_back({
			name: "UpdateMaster",
			sValue: UpdateMasterCommand(Command).master
		});
		else if (type == "UpdateOptionValues") roomEvents[argv[0]].push_back({
			name: "UpdateOptionValues",
			sValue: UpdateOptionValuesCommand(Command).optionValues
		});
		else if (type == "UpdateStatus") roomEvents[argv[0]].push_back({ 
			name: "UpdateStatus", 
			sValue: UpdateStatusCommand(Command).status 
		});
		else if (type == "UpdateUserStatus") roomEvents[argv[0]].push_back({ 
			name: "UpdateUserStatus",
			connArg: { routeId: userSessionId, conn: conn, request: request, argv: argv },
			sValue: UpdateUserStatusCommand(Command).status
		});
		else writeLog(LOG_LEVEL_INFO, "Unknown " + type + "Command. (roomId = " + argv[0] + ", thread = " + to_string(userSessionId) + ")");

		#ifdef __linux__
		usleep(10 * 1000);
		#else
		Sleep(10);
		#endif
	}
};