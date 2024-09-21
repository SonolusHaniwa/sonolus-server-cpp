#pragma once
using namespace std;

class RoomItem {
    public:

    int id;
    string name;
    int version = -1;
    string title;
    string subtitle;
    string master;
    SRL<Unknown> cover;
    SRL<Unknown> bgm;
    SRL<Unknown> preview;
    vector<Tag> tags;

    RoomItem(){}
    RoomItem(int id, string name, string title, string subtitle, string master, 
        SRL<Unknown> cover, SRL<Unknown> bgm, SRL<Unknown> preview, vector<Tag> tags):
        id(id), name(name), title(title), subtitle(subtitle), master(master),
        cover(cover), bgm(bgm), preview(preview), tags(tags){}
    RoomItem(int room_id, Json::Value arr) {
        id = room_id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        title = arr["title"].asString();
        subtitle = arr["subtitle"].asString();
        master = arr["master"].asString();
        cover = SRL<Unknown>(arr["cover"]);
        bgm = SRL<Unknown>(arr["bgm"]);
        preview = SRL<Unknown>(arr["preview"]);
        for (int i = 0; i < arr["tags"].size(); i++) tags.push_back(Tag(arr["tags"][i]));
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["id"] = id;
        res["name"] = name;
        res["version"] = version;
        res["title"] = title;
        res["subtitle"] = subtitle;
        res["master"] = master;
        res["cover"] = cover.toJsonObject();
        res["bgm"] = bgm.toJsonObject();
        res["preview"] = preview.toJsonObject();
        res["tags"].resize(0);
        for (int i = 0; i < tags.size(); i++) res["tags"].append(tags[i].toJsonObject());
        return res;
    }

    argvar fetchParamList() {
        argvar args;
        args["id"] = to_string(id);
        args["name"] = name;
        args["version"] = to_string(version);
        args["title"] = title;
        args["subtitle"] = subtitle;
        args["master"] = master;
        args["cover"] = cover.url;
        args["bgm"] = bgm.url;
        args["preview"] = preview.url;
        args["tags"] = serializeTagString(tags);
        args["tags.html"] = "";
        for (int i = 0; i < tags.size(); i++) args["tags.html"] += "<div class='flex' style='background-color:rgba(255,255,255,0.125);padding:5px;'>"
            "<div class='tagIcon'>{{icon." + tags[i].icon + "}}</div>&nbsp;" + tags[i].title + "</div>";
        args["url"] = "/rooms/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/rooms/" + name;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/rooms.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int roomsNumber(string filter) {
    itemNumberTemplate(Room, filter);
    dbres res = db.query(sql.c_str(), "Room");
    return atoi(res[0]["sum"].c_str());
}

vector<RoomItem> roomsList(string filter, string order, int st = 1, int en = 20) {
    itemListTemplate(Room, filter, order, st, en);

    auto res = db.query(sql.c_str(), "Room");
    vector<RoomItem> list = {};
    
    for (int i = 0; i < res.size(); i++) {
        RoomItem data = RoomItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["master"],
            SRL<Unknown>(res[i]["cover"], dataPrefix + res[i]["cover"]),
            SRL<Unknown>(res[i]["bgm"], dataPrefix + res[i]["bgm"]),
            SRL<Unknown>(res[i]["preview"], dataPrefix + res[i]["preview"]),
            deserializeTagString(res[i]["tags"])
        ); list.push_back(data);
    } return list;
}

int roomsCreate(RoomItem item, string localization = "default") {
    stringstream sqlbuffer;
    auto res = db.query("SELECT id FROM Room WHERE id = " + to_string(item.id), "Room");
    if (res.size() == 0) res = db.query("SELECT id FROM Room WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"", "Room");
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Room SET name=\"" << item.name << "\", version=" << item.version;
        sqlbuffer << ", title=\"" << item.title << "\", subtitle=\"" << item.subtitle << "\", master=\"" << item.master;
        sqlbuffer << "\", cover=\"" << item.cover.hash << "\", bgm=\"" << item.bgm.hash << "\", preview=\"" << item.preview.hash << "\", ";
        sqlbuffer << "tags=\"" << serializeTagString(item.tags) << "\", ";
        sqlbuffer << "localization=\"" << localization << "\" WHERE id=" << id;
    } else {
        int id = atoi(db.query("SELECT COUNT(*) AS sum FROM Room;", "Room")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Room (id, name, version, title, subtitle, master, tags, cover, bgm, preview, localization) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.subtitle << "\", \"" << item.master << "\", ";
        sqlbuffer << "\"" << serializeTagString(item.tags) << "\", ";
        sqlbuffer << "\"" << item.cover.hash << "\", \"" << item.bgm.hash << "\", \"" << item.preview.hash << "\", \"" << localization << "\")";
    } return db.execute(sqlbuffer.str(), "Room");
}