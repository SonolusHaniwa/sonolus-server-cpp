auto SonolusDetailsSubmit = [](client_conn conn, http_request request, param argv){
    request.postdata = json_decode(request.postdata)["values"].asString();
    auto $_POST = postParam(request);
    if (!checkLogin(request)) quickSendMsg(401, "Unauthorized.");
    UserProfile profile = getUserProfile(request);

    // 目前只支持 Replays 的提交
    if (argv[0] != "replays") quickSendMsg(404, "Action not found.");
    auto replays = replaysList("name = \"" + argv[1] +"\"", "");
    if (replays.size() == 0) quickSendMsg(400, "Item not found.");
    auto replay = replays[0];
    if (profile.handle != replay.author.substr(replay.author.rfind("#") + 1)) quickSendMsg(403, "Permission denied.");
    if ($_POST["type"] == "private") {
        db.execute("UPDATE Replay SET isPrivate = true WHERE name = \"" + argv[1] + "\"", "Replay");
        db.execute("UPDATE Record SET isPrivate = true WHERE replay = " + to_string(replay.id), "Record");
    } else if ($_POST["type"] == "public") {
        db.execute("UPDATE Replay SET isPrivate = false WHERE name = \"" + argv[1] + "\"", "Replay");
        db.execute("UPDATE Record SET isPrivate = false WHERE replay = " + to_string(replay.id), "Record");
    } else if ($_POST["type"] == "rank") {
        db.execute("UPDATE Replay SET isRank = true WHERE name = \"" + argv[1] + "\"", "Replay");
        db.execute("UPDATE Record SET isRank = true WHERE replay = " + to_string(replay.id), "Record");
    } else if ($_POST["type"] == "unrank") {
        db.execute("UPDATE Replay SET isRank = false WHERE name = \"" + argv[1] + "\"", "Replay");
        db.execute("UPDATE Record SET isRank = false WHERE replay = " + to_string(replay.id), "Record");
    } 

    Json::Value ServerSubmitItemActionResponse;
    ServerSubmitItemActionResponse["shouldUpdate"] = true;
    ServerSubmitItemActionResponse["shouldRemove"] = false;
    ServerSubmitItemActionResponse["key"] = "";
    ServerSubmitItemActionResponse["hashes"].resize(0);
    ServerSubmitItemActionResponse["shouldUpdateItem"] = true;
    ServerSubmitItemActionResponse["shouldRemoveItem"] = false;
    quickSendObj(ServerSubmitItemActionResponse);
};