auto SonolusLeaderboardDetails = [](client_conn conn, http_request request, param argv) {
    Json::Value ItemLeaderboardDetails;
    ItemLeaderboardDetails["topRecords"].resize(0);
    quickSendObj(ItemLeaderboardDetails);
};