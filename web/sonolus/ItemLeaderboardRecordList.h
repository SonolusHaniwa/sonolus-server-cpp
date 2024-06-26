auto SonolusLeaderboardRecordList = [](client_conn conn, http_request request, param argv) {
    Json::Value ItemLeaderboardRecordList;
    ItemLeaderboardRecordList["pageCount"] = 0;
    ItemLeaderboardRecordList["records"].resize(0);
    quickSendObj(ItemLeaderboardRecordList);
};