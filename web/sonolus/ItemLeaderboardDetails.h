#define quickSonolusLeaderboardDetails(name2, name1) { \
    auto items = name1##List("name = \"" + argv[1] + "\"", "");\
    if (items.size() == 0) quickSendMsg(404, "Item not found."); \
    auto item = items[0]; \
    auto leaderboards = appConfig[defineToString(name1)".leaderboards"]; \
    int leaderboardId = -1; \
    for (int i = 0; i < leaderboards.size(); i++) \
        if (leaderboards[i]["name"] == argv[2]) leaderboardId = i; \
    if (leaderboardId == -1) quickSendMsg(404, "Leaderboard not found."); \
    auto leaderboard = leaderboards[leaderboardId]; \
    argvar args = item.fetchParamList(); \
    string filter = str_replace(leaderboard["filter"].asString(), args); \
    string order = str_replace(leaderboard["order"].asString(), args); \
    auto records = recordsList<name2##Item>( \
        leaderboard["groupFunction"].asString(), leaderboard["groupVariable"].asString(), \
        filter, order, 1, appConfig[defineToString(name1)".pageSize.leaderboard.topRecord"].asInt() \
    ); \
    for (int i = 0; i < records.size(); i++) { \
        argvar args = records[i].fetchParamList(); \
        args["order"] = to_string(i + 1); \
        Json::Value single; \
        single["name"] = records[i].name; \
        single["rank"] = str_replace(leaderboard["constructor"]["rank"].asString(), args); \
        single["player"] = str_replace(leaderboard["constructor"]["player"].asString(), args); \
        single["value"] = str_replace(leaderboard["constructor"]["value"].asString(), args); \
        ItemLeaderboardDetails["topRecords"].append(single); \
    } \
}

auto SonolusLeaderboardDetails = [](client_conn conn, http_request request, param argv) {
    Json::Value ItemLeaderboardDetails;
    ItemLeaderboardDetails["topRecords"].resize(0);
    if (argv[0] == "levels") { quickSonolusLeaderboardDetails(Level, levels); }
    else if (argv[0] == "skins") { quickSonolusLeaderboardDetails(Skin, skins); }
    else if (argv[0] == "backgrounds") { quickSonolusLeaderboardDetails(Background, backgrounds); }
    else if (argv[0] == "effects") { quickSonolusLeaderboardDetails(Effect, effects); }
    else if (argv[0] == "particles") { quickSonolusLeaderboardDetails(Particle, particles); }
    else if (argv[0] == "engines") { quickSonolusLeaderboardDetails(Engine, engines); }
    else if (argv[0] == "replays") { quickSonolusLeaderboardDetails(Replay, replays); }
    else if (argv[0] == "posts") { quickSonolusLeaderboardDetails(Post, posts); }
    else if (argv[0] == "playlists") { quickSonolusLeaderboardDetails(Playlist, playlists); }
    else if (argv[0] == "rooms") { quickSonolusLeaderboardDetails(Room, rooms); }
    else quickSendMsg(404, "Item type not found.");
    quickSendObj(ItemLeaderboardDetails);
};