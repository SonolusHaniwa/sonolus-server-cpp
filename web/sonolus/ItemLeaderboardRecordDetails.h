#define quickSonolusLeaderboardRecordDetails(name) { \
    auto records = recordsList<name##Item>("name = \"" + argv[3] + "\"", ""); \
    if (records.size() == 0) quickSendMsg(404, "Record not found."); \
    ItemLeaderboardRecordDetails["replays"].append(records[0].replay.toJsonObject()); \
} 

auto SonolusLeaderboardRecordDetails = [](client_conn conn, http_request request, param argv) { 
    Json::Value ItemLeaderboardRecordDetails;
    ItemLeaderboardRecordDetails["replays"].resize(0);
    if (argv[0] == "levels") { quickSonolusLeaderboardRecordDetails(Level); }
    else if (argv[0] == "skins") { quickSonolusLeaderboardRecordDetails(Skin); }
    else if (argv[0] == "backgrounds") { quickSonolusLeaderboardRecordDetails(Background); }
    else if (argv[0] == "effects") { quickSonolusLeaderboardRecordDetails(Effect); }
    else if (argv[0] == "particles") { quickSonolusLeaderboardRecordDetails(Particle); }
    else if (argv[0] == "engines") { quickSonolusLeaderboardRecordDetails(Engine); }
    else if (argv[0] == "replays") { quickSonolusLeaderboardRecordDetails(Replay); }
    else if (argv[0] == "posts") { quickSonolusLeaderboardRecordDetails(Post); }
    else if (argv[0] == "playlists") { quickSonolusLeaderboardRecordDetails(Playlist); }
    else if (argv[0] == "rooms") { quickSonolusLeaderboardRecordDetails(Room); }
    else quickSendMsg(404, "Item type not found.");
    quickSendObj(ItemLeaderboardRecordDetails);
};