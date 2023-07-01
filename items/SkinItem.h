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

    SkinItem(){}
    SkinItem(int id, string name, string title, string subtitle, string author, 
        SRL<SkinThumbnail> thumbnail, SRL<SkinData> data, SRL<SkinTexture> texture):
        id(id), name(name), title(title), subtitle(subtitle), author(author), thumbnail(thumbnail), data(data), texture(texture){}
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
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/skins.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int skinNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Skin";
    if (filter != "") sql += " WHERE (" + filter + ")";
    dbres res = (new DB_Controller)->query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

Section<SkinItem> skinList(string filter, int st = 1, int en = 20) {
    // 获取数据条数
    int pageCount = ceil(1.0 * skinNumber(filter) / 20);

    // 获取数据
    string sql = "SELECT * FROM Skin";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id DESC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    dbres res = (new DB_Controller)->query(sql.c_str());
    Section<SkinItem> list = Section<SkinItem>(pageCount, SkinSearch);
    for (int i = 0; i < res.size(); i++) {
        SkinItem data = SkinItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            SRL<SkinThumbnail>(res[i]["thumbnail"], "/data/" + res[i]["thumbnail"]),
            SRL<SkinData>(res[i]["data"], "/data/" + res[i]["data"]),
            SRL<SkinTexture>(res[i]["texture"], "/data/" + res[i]["texture"])
        ); list.append(data);
    } return list;
}

string skinFilter(argvar arg) {
    string filter = "";
    if (arg["keywords"] != "") filter = "title like \"%" + str_replace("\"", "\\\"", urldecode(arg["keywords"])) + "%\"";
    return filter;
}

int skinCreate(SkinItem item) {
    stringstream sqlbuffer;
    auto res = (new DB_Controller)->query("SELECT id FROM Skin WHERE name = \"" + item.name + "\"");
    if (res.size() > 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Skin SET name = \"" << item.name << "\", version = " << item.version << ", title = \"" << item.title << "\", ";
        sqlbuffer << "subtitle = \"" << item.subtitle << "\", author = \"" << item.author << "\", thumbnail = \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "data = \"" << item.data.hash << "\", texture = \"" << item.texture.hash << "\" WHERE id = " << id;
    } else {
        int id = atoi((new DB_Controller)->query("SELECT COUNT(*) AS sum FROM Skin;")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Skin (id, name, version, title, subtitle, author, thumbnail, data, texture) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "\"" << item.data.hash << "\", \"" << item.texture.hash << "\")";
    }
    return (new DB_Controller)->execute(sqlbuffer.str());
}

#endif