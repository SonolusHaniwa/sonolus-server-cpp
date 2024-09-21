#define quickSonolusLeaderboardRecordList(name2, name1) { \
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
    int st = page * appConfig[defineToString(name1)".pageSize.leaderboard.record"].asInt() + 1; \
    int en = st + appConfig[defineToString(name1)".pageSize.leaderboard.record"].asInt() - 1; \
    ItemLeaderboardRecordList["pageCount"] = (recordsNumber(leaderboard["groupFunction"].asString(), leaderboard["groupVariable"].asString(), filter) - 1) / \
        appConfig[defineToString(name1)".pageSize.leaderboard.record"].asInt() + 1; \
    auto records = recordsList<name2##Item>( \
        leaderboard["groupFunction"].asString(), leaderboard["groupVariable"].asString(), \
        filter, order, st, en \
    ); \
    for (int i = 0; i < records.size(); i++) { \
        argvar args = records[i].fetchParamList(); \
        args["order"] = to_string(i + st); \
        Json::Value single; \
        single["name"] = records[i].name; \
        single["rank"] = str_replace(leaderboard["constructor"]["rank"].asString(), args); \
        single["player"] = str_replace(leaderboard["constructor"]["player"].asString(), args); \
        single["value"] = str_replace(leaderboard["constructor"]["value"].asString(), args); \
        ItemLeaderboardRecordList["records"].append(single); \
    } \
}

auto SonolusLeaderboardRecordList = [](client_conn conn, http_request request, param argv) {
    Json::Value ItemLeaderboardRecordList;
    auto $_GET = getParam(request);
    int page = $_GET.find("page") == $_GET.end() ? 0 : atoi($_GET["page"].c_str());
    ItemLeaderboardRecordList["pageCount"] = 0;
    ItemLeaderboardRecordList["records"].resize(0);
    if (argv[0] == "levels") { quickSonolusLeaderboardRecordList(Level, levels); }
    quickSendObj(ItemLeaderboardRecordList);
};