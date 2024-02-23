#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

using namespace std;

class PlaylistItem {
    public:

    int id;
    string name;
    int version = playlistVersion;
    string title;
    string subtitle;
    string author;
    vector<LevelItem> levels;
    SRL<PlaylistThumbnail> thumbnail;
    vector<Tag> tags;
    string description;
    string source;

    PlaylistItem(){}
    PlaylistItem(int id, string name, string title, string subtitle, string author, vector<LevelItem> levels,
        SRL<PlaylistThumbnail> thumbnail, vector<Tag> tags, string description = "", string source = ""):
        id(id), name(name), title(title), subtitle(subtitle), author(author), levels(levels),
        thumbnail(thumbnail), tags(tags), description(description), source(source){}
    PlaylistItem(int playlist_id, Json::Value arr) {
        id = playlist_id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        title = arr["title"].asString();
        subtitle = arr["subtitle"].asString();
        author = arr["author"].asString();
        for (int i = 0; i < arr["levels"].size(); i++) levels.push_back(LevelItem(-1, arr["levels"][i]));
        thumbnail = SRL<PlaylistThumbnail>(arr["thumbnail"]);
        for (int i = 0; i < arr["tags"].size(); i++) tags.push_back(Tag(arr["tags"][i]));
        description = arr["description"].asString();
        source = arr["source"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["version"] = version;
        res["title"] = title;
        res["subtitle"] = subtitle;
        res["author"] = author;
        res["levels"].resize(0);
        for (int i = 0; i < levels.size(); i++) res["levels"].append(levels[i].toJsonObject());
        res["thumbnail"] = thumbnail.toJsonObject();
        res["tags"].resize(0);
        for (int i = 0; i < tags.size(); i++) res["tags"].append(tags[i].toJsonObject());
        res["description"] = description;
        res["source"] = source;
        return res;
    }

    argvar fetchParamList() {
        argvar args;
        args["name"] = name;
        args["version"] = to_string(version);
        args["title"] = title;
        args["subtitle"] = subtitle;
        args["author"] = author;
        for (int i = 0; i < levels.size(); i++) args["levels"] += levels[i].toHTMLObject().output();
        args["tags"] = "";
        for (int i = 0; i < tags.size(); i++) args["tags"] += "<div class='flex' style='background-color:rgba(255,255,255,0.125);padding:5px;'>"
            "<div class='tagIcon'>{{icon." + tags[i].icon + "}}</div>&nbsp;" + tags[i].title + "</div>";
        args["url"] = "/playlists/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/playlists/" + name;
        args["description"] = description;
        args["source"] = source;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/playlists.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int playlistsNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Playlist";
    if (filter != "") sql += " WHERE (" + filter + ")";
    dbres res = db.query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

vector<PlaylistItem> playlistsList(string filter, string order, int st = 1, int en = 20) {
    string sql = "SELECT * FROM Playlist";
    if (filter != "") sql += " WHERE (" + filter + ")";
    if (order != "") sql += " ORDER BY " + order;
    sql += " LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    auto res = db.query(sql.c_str());
    vector<PlaylistItem> list = {};
    sort(res.begin(), res.end(), [](argvar a, argvar b){
        if (a["name"] == b["name"]) return (a["localization"] == "default") < (b["localization"] == "default");
        else return atoi(a["id"].c_str()) > atoi(b["id"].c_str());
    }); map<string, bool> nameUsed;
    for (int i = 0; i < res.size(); i++) {
        if (nameUsed[res[i]["name"]]) continue;
        nameUsed[res[i]["name"]] = true;
        vector<LevelItem> levels = {};
        string levelsJson = res[i]["levels"]; Json::Value levelsId;
        json_decode(levelsJson, levelsId);
        for (int j = 0; j < levelsId.size(); j++) levels.push_back(levelsList("id = " + levelsId[j].asString(), "", 1, 1)[0]);
        PlaylistItem data = PlaylistItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            levels,
            SRL<PlaylistThumbnail>(res[i]["thumbnail"], dataPrefix + res[i]["thumbnail"]),
            deserializeTagString(res[i]["tags"]),
            str_replace("\\n", "\n", res[i]["description"]),
            (appConfig["server.enableSSL"].asBool() ? "https://" : "http://") + appConfig["server.rootUrl"].asString() + "/sonolus/playlists/" + res[i]["name"]
        ); list.push_back(data);
    } return list;
}

int playlistsCreate(PlaylistItem item, string localization = "default") {
    stringstream sqlbuffer;
    auto res = db.query("SELECT id FROM Playlist WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"");
    Json::Value levels; levels.resize(0);
    for (int i = 0; i < item.levels.size(); i++) levels.append(item.levels[i].id);
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Playlist SET name=\"" << item.name << "\", version=" << item.version;
        sqlbuffer << ", title=\"" << item.title << "\", subtitle=\"" << item.subtitle << "\", author=\"" << item.author << "\"";
        sqlbuffer << ", levels=\"" << json_encode(levels) << "\"";
        sqlbuffer << ", thumbnail=\"" << item.thumbnail.hash << "\"";
        sqlbuffer << ", description=\"" << str_replace("\n", "\\n", item.description) << "\", localization=\"" << localization << "\" WHERE id=" << id;
    } else {
        int id = atoi(db.query("SELECT COUNT(*) AS sum FROM Replay;")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Playlist (id, name, version, title, subtitle, author, levels, thumbnail, description, localization) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", \"" << json_encode(levels) << "\", ";
        sqlbuffer << "\"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "\"" << str_replace("\n", "\\n", item.description) << "\", \"" << localization << "\")";
    } return db.execute(sqlbuffer.str());
}

#endif
