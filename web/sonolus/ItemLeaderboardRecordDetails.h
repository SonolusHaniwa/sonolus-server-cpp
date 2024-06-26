auto SonolusLeaderboardRecordDetails = [](client_conn conn, http_request request, param argv) {
    Json::Value ItemLeaderboardRecordDetails;
    ItemLeaderboardRecordDetails["replays"].resize(0);
    quickSendObj(ItemLeaderboardRecordDetails);
};