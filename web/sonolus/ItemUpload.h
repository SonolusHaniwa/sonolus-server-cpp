string round(double value, int precision) {
    stringstream ss;
    ss << fixed << setprecision(precision) << value;
    return ss.str();
}

auto SonolusUpload = [](client_conn conn, http_request request, param argv){
    if (!checkLogin(request)) quickSendMsg(401, "Unauthorized.");
    
    string boundary = request.argv["content-type"].substr(request.argv["content-type"].find("boundary=") + 9);
    string data = request.postdata;
    auto fileParts = explode("\r\n--" + boundary, data);
    argvar files;
    for (int i = 1; i < fileParts.size() - 1; i++) {
        string data = fileParts[i];
        string hash = data.substr(data.find("filename=\"") + 10, 40);
        string file = data.substr(data.find("\r\n\r\n") + 4);
        string realHash = sha1(file);
        if (hash != realHash) quickSendMsg(403, "Hash mismatch.");
        ofstream fout("./data/" + hash, ios::binary);
        fout.write(file.c_str(), file.size());
        fout.close();
        files[hash] = file;
    }

    // 如果是 record create
    if (recordsNumber("name = \"" + request.argv["sonolus-upload-key"] + "\"")) {
        auto record = recordsList<LevelItem>("name = \"" + request.argv["sonolus-upload-key"] + "\"", "")[0];
        string configuration = files[record.replay.configuration.hash];
        string data = files[record.replay.data.hash];
        auto replayData = json_decode(decompress_gzip(data));
        record.startTime = replayData["startTime"].asInt64();
        record.saveTime = replayData["saveTime"].asInt64();
        record.duration = replayData["duration"].asDouble();
        record.inputOffset = replayData["inputOffset"].asDouble();
        record.playAreaWidth = replayData["playArea"]["width"].asInt();
        record.playAreaHeight = replayData["playArea"]["height"].asInt();
        record.grade = replayData["result"]["grade"].asString();
        record.arcadeScore = replayData["result"]["arcadeScore"].asInt();
        record.accuracyScore = replayData["result"]["accuracyScore"].asInt();
        record.combo = replayData["result"]["combo"].asInt();
        record.perfect = replayData["result"]["perfect"].asInt();
        record.great = replayData["result"]["great"].asInt();
        record.good = replayData["result"]["good"].asInt();
        record.miss = replayData["result"]["miss"].asInt();
        record.totalCount = replayData["result"]["totalCount"].asInt();
        record.allowRank = 
            record.perfect + record.great + record.good + record.miss == record.totalCount &&
            record.replay.tags.size() == 0;
        auto replay = record.replay;
        replay.allowRank = record.allowRank;
        replay.title = replay.level.title;
        string grade = record.grade;
        if (record.totalCount != record.perfect + record.great + record.good + record.miss) grade = "unfinished";
        if (grade == "allPerfect") grade = "AP";
        else if (grade == "fullCombo") grade = "FC";
        else if (grade == "pass") grade = "P";
        else if (grade == "fail") grade = "F";
        else grade = "X";
        replay.subtitle = grade + " / " + to_string(record.accuracyScore) + " | " + 
            to_string(record.perfect) + " / " + to_string(record.great) + " / " + to_string(record.good) + " / " + to_string(record.miss);
        replay.description = "Grade " + grade + "\n" +
            "Arcade Score " + to_string(record.arcadeScore) + " " + round(1.0 * record.arcadeScore / 1e4, 1) + "%\n" +
            "Accuracy Score " + to_string(record.accuracyScore) + " " + round(1.0 * record.accuracyScore / 1e4, 1) + "%\n" +
            "Combo " + to_string(record.combo) + " " + round(1.0 * record.combo / record.totalCount, 1) + "%\n" +
            "\n" +
            "Perfect " + to_string(record.perfect) + " " + round(1.0 * record.perfect / record.totalCount, 1) + "%\n" +
            "Great " + to_string(record.great) + " " + round(1.0 * record.great / record.totalCount, 1) + "%\n" +
            "Good " + to_string(record.good) + " " + round(1.0 * record.good / record.totalCount, 1) + "%\n" +
            "Miss " + to_string(record.miss) + " " + round(1.0 * record.miss / record.totalCount, 1) + "%\n";
        replaysCreate(replay);
        recordsCreate(record);
    }

    quickSendMsg(200, "OK.");
};