#pragma once

template<typename T>
class Record {
    public:

    int id = -1;
    string name = "";
    string type = "";
    T item;
    ReplayItem replay;
    UserProfile player;
    time_t startTime = 0;
    time_t saveTime = 0;
    double duration = 0;
    double inputOffset = 0;
    int playAreaWidth = 0;
    int playAreaHeight = 0;
    string grade = "";
    int arcadeScore = 0;
    int accuracyScore = 0;
    int combo = 0;
    int perfect = 0;
    int great = 0;
    int good = 0;
    int miss = 0;
    int totalCount = 0;
    bool isPrivate = false;
    bool allowRank = true;
    bool isRank = true;

    Record(){};
    Record(
        int id, string name, string type, T item, ReplayItem replay, UserProfile player,
        time_t startTime, time_t saveTime, double duration, double inputOffset,
        int playAreaWidth, int playAreaHeight, string grade,
        int arcadeScore, int accuracyScore, int combo,
        int perfect, int great, int good, int miss, int totalCount,
        bool isPrivate, bool allowRank, bool isRank
    ):  id(id), name(name), type(type), item(item), replay(replay), player(player),
        startTime(startTime), saveTime(saveTime), duration(duration), inputOffset(inputOffset),
        playAreaWidth(playAreaWidth), playAreaHeight(playAreaHeight), grade(grade),
        arcadeScore(arcadeScore), accuracyScore(accuracyScore), combo(combo),
        perfect(perfect), great(great), good(good), miss(miss),
        isPrivate(isPrivate), allowRank(allowRank), isRank(isRank){};
    Record(int record_id, Json::Value obj) {
        id = record_id;
        name = obj["name"].asString();
        type = obj["type"].asString();
        item = T(obj["item"]["id"].asInt(), obj["item"]);
        replay = ReplayItem(obj["replay"]["id"].asInt(), obj["replay"]);
        player = UserProfile(obj["player"]);
        startTime = obj["startTime"].asInt64();
        saveTime = obj["saveTime"].asInt64();
        duration = obj["duration"].asDouble();
        inputOffset = obj["inputOffset"].asDouble();
        playAreaWidth = obj["playAreaWidth"].asInt();
        playAreaHeight = obj["playAreaHeight"].asInt();
        grade = obj["grade"].asString();
        arcadeScore = obj["arcadeScore"].asInt();
        accuracyScore = obj["accuracyScore"].asInt();
        combo = obj["combo"].asInt();
        perfect = obj["perfect"].asInt();
        great = obj["great"].asInt();
        good = obj["good"].asInt();
        miss = obj["miss"].asInt();
        totalCount = obj["totalCount"].asInt();
        isPrivate = obj["isPrivate"].asInt();
        allowRank = obj["allowRank"].asInt();
        isRank = obj["isRank"].asInt();
    }
    
    Json::Value toJsonObject() {
        Json::Value obj;
        obj["id"] = id;
        obj["name"] = name;
        obj["type"] = type;
        obj["item"] = item.toJsonObject();
        obj["replay"] = replay.toJsonObject();
        obj["player"] = player.toJsonObject();
        obj["startTime"] = startTime;
        obj["saveTime"] = saveTime;
        obj["duration"] = duration;
        obj["inputOffset"] = inputOffset;
        obj["playAreaWidth"] = playAreaWidth;
        obj["playAreaHeight"] = playAreaHeight;
        obj["grade"] = grade;
        obj["arcadeScore"] = arcadeScore;
        obj["accuracyScore"] = accuracyScore;
        obj["combo"] = combo;
        obj["perfect"] = perfect;
        obj["great"] = great;
        obj["good"] = good;
        obj["miss"] = miss;
        obj["totalCount"] = totalCount;
        obj["isPrivate"] = isPrivate;
        obj["allowRank"] = allowRank;
        obj["isRank"] = isRank;
        return obj;
    }

    argvar fetchParamList() {
        argvar args;
        args["id"] = to_string(id);
        args["name"] = name;
        args["type"] = type;
        args["item"] = item.name;
        args["replay"] = replay.name;
        args["player.id"] = player.id;
        args["player.name"] = player.name;
        args["player.handle"] = player.handle;
        args["player.avatarBackgroundColor"] = player.avatarBackgroundColor;
        args["player.avatarForegroundColor"] = player.avatarForegroundColor;
        args["player.aboutMe"] = player.aboutMe;
        args["startTime"] = to_string(startTime);
        args["saveTime"] = to_string(saveTime);
        args["duration"] = to_string(duration);
        args["inputOffset"] = to_string(inputOffset);
        args["playAreaWidth"] = to_string(playAreaWidth);
        args["playAreaHeight"] = to_string(playAreaHeight);
        args["grade"] = grade;
        args["arcadeScore"] = to_string(arcadeScore);
        args["accuracyScore"] = to_string(accuracyScore);
        args["combo"] = to_string(combo);
        args["perfect"] = to_string(perfect);
        args["great"] = to_string(great);
        args["good"] = to_string(good);
        args["miss"] = to_string(miss);
        args["totalCount"] = to_string(totalCount);
        args["isPrivate"] = to_string(isPrivate);
        args["allowRank"] = to_string(allowRank);
        args["isRank"] = to_string(isRank);
        return args;
    }
};

int recordsNumber(string groupFunction, string groupVariable, string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM ("
        "SELECT *, " + groupFunction + "(" + groupVariable + ") AS " + groupVariable + " FROM Record "
        "WHERE (" + (filter == "" ? "1" : filter) + ")"
        "GROUP BY player"
    ") AS A";
    dbres res = db.query(sql.c_str(), "Record");
    return atoi(res[0]["sum"].c_str());
}

int recordsNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Record WHERE (" + (filter == "" ? "1" : filter) + ")";
    dbres res = db.query(sql.c_str(), "Record");
    return atoi(res[0]["sum"].c_str());
}

template<typename T>
vector<Record<T> > recordsList(string groupFunction, string groupVariable, string filter, string order, int st = 1, int en = 20) {
    int limit = en - st + 1, offset = st - 1;
    string sql = "SELECT *, " + groupFunction + "(" + groupVariable + ") AS " + groupVariable + " FROM Record "
        "WHERE (" + (filter == "" ? "1" : filter) + ") "
        "GROUP BY player " + 
        "ORDER BY " + (order == "" ? "id DESC" : order) + " " +
        "LIMIT " + to_string(offset) + ", " + to_string(limit);
    dbres res = db.query(sql.c_str(), "Record");
    vector<Record<T> > vec;
    for (int i = 0; i < res.size(); i++) {
        vec.push_back(Record<T>(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["type"],
            [&](){ T item; item.name = res[i]["item"]; return item; }(),
            replaysList("id = \"" +res[i]["replay"]  + "\"", "")[0],
            usersList("id = \"" + res[i]["player"] + "\"", "")[0],
            atoll(res[i]["startTime"].c_str()),
            atoll(res[i]["saveTime"].c_str()),
            atof(res[i]["duration"].c_str()),
            atof(res[i]["inputOffset"].c_str()),
            atoi(res[i]["playAreaWidth"].c_str()),
            atoi(res[i]["playAreaHeight"].c_str()),
            res[i]["grade"],
            atoi(res[i]["arcadeScore"].c_str()),
            atoi(res[i]["accuracyScore"].c_str()),
            atoi(res[i]["combo"].c_str()),
            atoi(res[i]["perfect"].c_str()),
            atoi(res[i]["great"].c_str()),
            atoi(res[i]["good"].c_str()),
            atoi(res[i]["miss"].c_str()),
            atoi(res[i]["totalCount"].c_str()),
            atoi(res[i]["isPrivate"].c_str()),
            atoi(res[i]["allowRank"].c_str()),
            atoi(res[i]["isRank"].c_str())
        )); 
    }
    return vec;
}

template<typename T>
vector<Record<T> > recordsList(string filter, string order, int st = 1, int en = 20) {
    int limit = en - st + 1, offset = st - 1;
    string sql = "SELECT * FROM Record WHERE (" + (filter == "" ? "1" : filter) + ") ORDER BY " + (order == "" ? "id DESC" : order) + " LIMIT " + to_string(offset) + ", " + to_string(limit);
    dbres res = db.query(sql.c_str(), "Record");
    vector<Record<T> > vec;
    for (int i = 0; i < res.size(); i++) {
        vec.push_back(Record<T>(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["type"],
            [&](){ T item; item.id = atoi(res[i]["item"].c_str()); return item; }(),
            replaysList("id = \"" +res[i]["replay"]  + "\"", "")[0],
            usersList("id = \"" + res[i]["player"] + "\"", "")[0],
            atoll(res[i]["startTime"].c_str()),
            atoll(res[i]["saveTime"].c_str()),
            atof(res[i]["duration"].c_str()),
            atof(res[i]["inputOffset"].c_str()),
            atoi(res[i]["playAreaWidth"].c_str()),
            atoi(res[i]["playAreaHeight"].c_str()),
            res[i]["grade"],
            atoi(res[i]["arcadeScore"].c_str()),
            atoi(res[i]["accuracyScore"].c_str()),
            atoi(res[i]["combo"].c_str()),
            atoi(res[i]["perfect"].c_str()),
            atoi(res[i]["great"].c_str()),
            atoi(res[i]["good"].c_str()),
            atoi(res[i]["miss"].c_str()),
            atoi(res[i]["totalCount"].c_str()),
            atoi(res[i]["isPrivate"].c_str()),
            atoi(res[i]["allowRank"].c_str()),
            atoi(res[i]["isRank"].c_str())
        )); 
    }
    return vec;
}

template<typename T>
int recordsCreate(Record<T> item) {
    auto res = db.query("SELECT id FROM Record WHERE id = " + to_string(item.id), "Record");
    int id = atoi(db.query("SELECT COUNT(*) AS sum FROM Record", "Record")[0]["sum"].c_str()) + 1;
    string sql = "";

    if (res.size() == 0) sql = 
        "INSERT INTO Record ("
            "id, name, type, item, replay, player, "
            "startTime, saveTime, duration, inputOffset, "
            "playAreaWidth, playAreaHeight, grade, arcadeScore, accuracyScore, "
            "combo, perfect, great, good, miss, totalCount, isPrivate, allowRank, isRank"
        ") VALUES ("
            + to_string(id) + ", "
            "'" + item.name + "', "
            "'" + item.type + "', "
            "'" + to_string(item.item.id) + "', "
            "'" + to_string(item.replay.id) + "', "
            "'" + item.player.id + "', "
            + to_string(item.startTime) + ", "
            + to_string(item.saveTime) + ", "
            + to_string(item.duration) + ", "
            + to_string(item.inputOffset) + ", "
            + to_string(item.playAreaWidth) + ", "
            + to_string(item.playAreaHeight) + ", "
            "'" + item.grade + "', "
            + to_string(item.arcadeScore) + ", "
            + to_string(item.accuracyScore) + ", "
            + to_string(item.combo) + ", "
            + to_string(item.perfect) + ", "
            + to_string(item.great) + ", "
            + to_string(item.good) + ", "
            + to_string(item.miss) + ", "
            + to_string(item.totalCount) + ", "
            + to_string(item.isPrivate) + ", "
            + to_string(item.allowRank) + ", "
            + to_string(item.isRank) +
        ")";
    else sql = 
        "UPDATE Record SET "
            "name = '" + item.name + "', "
            "type = '" + item.type + "', "
            "item = '" + to_string(item.item.id) + "', "
            "replay = '" + to_string(item.replay.id) + "', "
            "player = '" + item.player.id + "', "
            "startTime = " + to_string(item.startTime) + ", "
            "saveTime = " + to_string(item.saveTime) + ", "
            "duration = " + to_string(item.duration) + ", "
            "inputOffset = " + to_string(item.inputOffset) + ", "
            "playAreaWidth = " + to_string(item.playAreaWidth) + ", "
            "playAreaHeight = " + to_string(item.playAreaHeight) + ", "
            "grade = '" + item.grade + "', "
            "arcadeScore = " + to_string(item.arcadeScore) + ", "
            "accuracyScore = " + to_string(item.accuracyScore) + ", "
            "combo = " + to_string(item.combo) + ", "
            "perfect = " + to_string(item.perfect) + ", "
            "great = " + to_string(item.great) + ", "
            "good = " + to_string(item.good) + ", "
            "miss = " + to_string(item.miss) + ", "
            "totalCount = " + to_string(item.totalCount) + ", "
            "isPrivate = " + to_string(item.isPrivate) + ", "
            "allowRank = " + to_string(item.allowRank) + ", "
            "isRank = " + to_string(item.isRank) + " "
        "WHERE id = " + to_string(item.id);
    return db.execute(sql.c_str(), "Record");
}