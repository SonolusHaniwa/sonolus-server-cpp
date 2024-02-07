#ifndef SKINITEM_H
#define SKINITEM_H

using namespace std;

class SkinItem {
    public:

    int id;
    string name;
    int version = skinVersion;
    string title;
    string subtitle;
    string author;
    SRL<SkinThumbnail> thumbnail;
    SRL<SkinData> data;
    SRL<SkinTexture> texture;
    vector<Tag> tags;
    string description;
    string source;

    SkinItem(){}
    SkinItem(int id, string name, string title, string subtitle, string author, 
        SRL<SkinThumbnail> thumbnail, SRL<SkinData> data, SRL<SkinTexture> texture, 
        vector<Tag> tags, string description = "", string source = ""):
        id(id), name(name), title(title), subtitle(subtitle), author(author), 
        thumbnail(thumbnail), data(data), texture(texture), 
        tags(tags), description(description), source(source){}
    SkinItem(int skin_id, Json::Value arr) {
        id = skin_id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        title = arr["title"].asString();
        subtitle = arr["subtitle"].asString();
        author = arr["author"].asString();
        thumbnail = SRL<SkinThumbnail>(arr["thumbnail"]);
        data = SRL<SkinData>(arr["data"]);
        texture = SRL<SkinTexture>(arr["texture"]);
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
        res["thumbnail"] = thumbnail.toJsonObject();
        res["data"] = data.toJsonObject();
        res["texture"] = texture.toJsonObject();
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
        args["thumbnail"] = thumbnail.url;
        args["data"] = data.url;
        args["texture"] = texture.url;
        args["url"] = "/skins/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/skins/" + name;
        args["description"] = description;
        args["source"] = source;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/skins.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int skinsNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Skin";
    if (filter != "") sql += " WHERE (" + filter + ")";
    dbres res = (new DB_Controller)->query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

vector<SkinItem> skinsList(string filter, string order, int st = 1, int en = 20) {
    string sql = "SELECT * FROM Skin";
    if (filter != "") sql += " WHERE (" + filter + ")";
    if (order != "") sql += " ORDER BY " + order;
    sql += " LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    dbres res = (new DB_Controller)->query(sql.c_str());
    vector<SkinItem> list = {};
    sort(res.begin(), res.end(), [](argvar a, argvar b){
        if (a["name"] == b["name"]) return (a["localization"] == "default") < (b["localization"] == "default");
        else return atoi(a["id"].c_str()) > atoi(b["id"].c_str());
    }); map<string, bool> nameUsed;
    for (int i = 0; i < res.size(); i++) {
        if (nameUsed[res[i]["name"]]) continue;
        nameUsed[res[i]["name"]] = true;
        SkinItem data = SkinItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            SRL<SkinThumbnail>(res[i]["thumbnail"], "/data/" + res[i]["thumbnail"]),
            SRL<SkinData>(res[i]["data"], "/data/" + res[i]["data"]),
            SRL<SkinTexture>(res[i]["texture"], "/data/" + res[i]["texture"]),
            deserializeTagString(res[i]["tags"]), 
            str_replace("\\n", "\n", res[i]["description"]),
            (appConfig["server.enableSSL"].asBool() ? "https://" : "http://") + appConfig["server.rootUrl"].asString() + "/sonolus/backgrounds/" + res[i]["name"]
        ); list.push_back(data);
    } return list;
}

int skinCreate(SkinItem item, string localization = "default") {
    stringstream sqlbuffer;
    auto res = (new DB_Controller)->query("SELECT id FROM Skin WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"");
    if (res.size() > 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Skin SET name = \"" << item.name << "\", version = " << item.version << ", title = \"" << item.title << "\", ";
        sqlbuffer << "subtitle = \"" << item.subtitle << "\", author = \"" << item.author << "\", thumbnail = \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "data = \"" << item.data.hash << "\", texture = \"" << item.texture.hash << "\", ";
        sqlbuffer << "description = \"" << str_replace("\n", "\\n", item.description) << "\", localization = \"" << localization << "\" WHERE id = " << id;
    } else {
        int id = atoi((new DB_Controller)->query("SELECT COUNT(*) AS sum FROM Skin;")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Skin (id, name, version, title, subtitle, author, thumbnail, data, texture, description, localization) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "\"" << item.data.hash << "\", \"" << item.texture.hash << "\", \"" << str_replace("\n", "\\n", item.description) << "\", \"" << localization << "\")";
    }
    return (new DB_Controller)->execute(sqlbuffer.str());
}

#endif