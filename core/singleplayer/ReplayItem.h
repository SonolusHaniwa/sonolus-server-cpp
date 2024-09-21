#ifndef REPLAYITEM_H
#define REPLAYITEM_H

using namespace std;

class ReplayItem {
    public:

    int id;
    string name;
    int version = replayVersion;
    string title;
    string subtitle;
    string author;
    LevelItem level;
    SRL<ReplayData> data;
    SRL<ReplayConfiguration> configuration;
    vector<Tag> tags;
    string description;
    string source;
    bool isPrivate = false;
    bool isRank = true;
    bool allowRank = true;

    ReplayItem(){}
    ReplayItem(int id, string name, string title, string subtitle, string author, LevelItem level, 
        SRL<ReplayData> data, SRL<ReplayConfiguration> configuration, 
        vector<Tag> tags, string description = "", string source = "", bool isPrivate = false, bool isRank = true, bool allowRank = true):
        id(id), name(name), title(title), subtitle(subtitle), author(author), level(level), 
        data(data), configuration(configuration), 
        tags(tags), description(description), source(source), isPrivate(isPrivate), isRank(isRank), allowRank(allowRank){}
    ReplayItem(int replay_id, Json::Value arr) {
        id = replay_id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        title = arr["title"].asString();
        subtitle = arr["subtitle"].asString();
        author = arr["author"].asString();
        level = LevelItem(arr["level"]["id"].asInt(), arr["level"]);
        data = SRL<ReplayData>(arr["data"]);
        configuration = SRL<ReplayConfiguration>(arr["configuration"]);
        for (int i = 0; i < arr["tags"].size(); i++) tags.push_back(Tag(arr["tags"][i]));
        description = arr["description"].asString();
        source = arr["source"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["id"] = id;
        res["name"] = name;
        res["version"] = version;
        res["title"] = title;
        res["subtitle"] = subtitle;
        res["author"] = author;
        res["level"] = level.toJsonObject();
        res["data"] = data.toJsonObject();
        res["configuration"] = configuration.toJsonObject();
        res["tags"].resize(0);
        for (int i = 0; i < tags.size(); i++) res["tags"].append(tags[i].toJsonObject());
        res["description"] = description;
        res["source"] = source;
        return res;
    }

    argvar fetchParamList() {
        argvar args;
        args["id"] = to_string(id);
        args["name"] = name;
        args["version"] = to_string(version);
        args["title"] = title;
        args["rating"] = to_string(level.rating);
        args["subtitle"] = subtitle;
        args["author"] = author;
        args["level"] = level.title;
        args["engine"] = level.engine.title;
        args["data"] = data.url;
        args["configuration"] = configuration.url;
        args["cover"] = level.cover.url;
        args["tags"] = serializeTagString(tags);
        args["tags.html"] = "";
        for (int i = 0; i < tags.size(); i++) args["tags.html"] += "<div class='flex' style='background-color:rgba(255,255,255,0.125);padding:5px;'>"
            "<div class='tagIcon'>{{icon." + tags[i].icon + "}}</div>&nbsp;" + tags[i].title + "</div>";
        args["url"] = "/replays/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/replays/" + name;
        args["description"] = description;
        args["source"] = source;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/replays.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int replaysNumber(string filter) {
    itemNumberTemplate(Replay, filter);
    dbres res = db.query(sql.c_str(), "Replay");
    return atoi(res[0]["sum"].c_str());
}

vector<ReplayItem> replaysList(string filter, string order, int st = 1, int en = 20) {
    itemListTemplate(Replay, filter, order, st, en);

    auto res = db.query(sql.c_str(), "Replay");
    vector<ReplayItem> list = {};

    for (int i = 0; i < res.size(); i++) {
        LevelItem level = levelsList("id = " + res[i]["level"], "", 1, 1)[0];
        ReplayItem data = ReplayItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            level,
            SRL<ReplayData>(res[i]["data"], dataPrefix + res[i]["data"]),
            SRL<ReplayConfiguration>(res[i]["configuration"], dataPrefix + res[i]["configuration"]),
            deserializeTagString(res[i]["tags"]),
            str_replace("\\n", "\n", res[i]["description"]),
            (appConfig["server.enableSSL"].asBool() ? "https://" : "http://") + appConfig["server.rootUrl"].asString(),
            atoi(res[i]["isPrivate"].c_str()),
            atoi(res[i]["isRank"].c_str()),
            atoi(res[i]["allowRank"].c_str())
        ); list.push_back(data);
    } return list;
}

int replaysCreate(ReplayItem item, string localization = "default") {
    stringstream sqlbuffer;
    auto res = db.query("SELECT id FROM Replay WHERE id = " + to_string(item.id), "Replay");
    if (res.size() == 0) res = db.query("SELECT id FROM Replay WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"", "Replay");
    int levelId = atoi(db.query("SELECT id FROM Level WHERE name = \"" + item.level.name + "\";", "Level")[0]["id"].c_str());
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Replay SET name=\"" << item.name << "\", version=" << item.version;
        sqlbuffer << ", title=\"" << item.title << "\", subtitle=\"" << item.subtitle << "\", author=\"" << item.author;
        sqlbuffer << "\", level=" << levelId << ", data=\"" << item.data.hash << "\", configuration=\"" << item.configuration.hash << "\", ";
        sqlbuffer << "tags=\"" << serializeTagString(item.tags) << "\", ";
        sqlbuffer << "description=\"" << str_replace("\n", "\\n", item.description) << "\", localization=\"" << localization << "\", ";
        sqlbuffer << "isPrivate=" << item.isPrivate << ", isRank=" << item.isRank << ", allowRank=" << item.allowRank << " WHERE id=" << id;
    } else {
        int id = atoi(db.query("SELECT COUNT(*) AS sum FROM Replay;", "Replay")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Replay (id, name, version, title, subtitle, author, level, tags, data, configuration, description, localization, isPrivate, isRank, allowRank) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", " << levelId << ", ";
        sqlbuffer << "\"" << serializeTagString(item.tags) << "\", \"" << item.data.hash << "\", \"" << item.configuration.hash << "\", ";
        sqlbuffer << "\"" << str_replace("\n", "\\n", item.description) << "\", \"" << localization << "\", ";
        sqlbuffer << item.isPrivate << ", " << item.isRank << ", " << item.allowRank << ")";
    } return db.execute(sqlbuffer.str(), "Replay");
}

#endif
