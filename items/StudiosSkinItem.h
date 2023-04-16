#ifndef STUDIOSSKINITEM_H
#define STUDIOSSKINITEM_H

using namespace std;

class StudiosSkinItem {
    public:

    int id;
    string name;
    int version = 1;
    string title;
    string subtitle;
    string author;
    SRL<SkinThumbnail> thumbnail;
    bool interpolation;
    string spritesJson;

    StudiosSkinItem(){}
    StudiosSkinItem(int id, string name, string title, string subtitle, string author,
        SRL<SkinThumbnail> thumbnail, bool interpolation, string spritesJson): 
        id(id), name(name), title(title), subtitle(subtitle), author(author),
        thumbnail(thumbnail), interpolation(interpolation), spritesJson(spritesJson){}
    StudiosSkinItem(int id, Json::Value arr){
        this->id = id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        title = arr["title"].asString();
        subtitle = arr["subtitle"].asString();
        author = arr["author"].asString();
        thumbnail = SRL<SkinThumbnail>(arr["thumbnail"]);
        interpolation = arr["interpolation"].asBool();
        spritesJson = arr["sprites"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["version"] = version;
        res["title"] = title;
        res["subtitle"] = subtitle;
        res["author"] = author;
        res["thumbnail"] = thumbnail.toJsonObject();
        res["interpolation"] = interpolation;
        res["sprites"] = spritesJson;
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
        args["interpolation"] = to_string(interpolation);
        args["sprites"] = spritesJson;
        args["url"] = "/studios/skins/" + name;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/studios_skins.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int studios_skinNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Studios_Skin";
    if (filter != "") sql += " WHERE (" + filter + ")";
    dbres res = (new DB_Controller)->query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

Section<StudiosSkinItem> studios_skinList(string filter, int st = 1, int en = 20) {
    // 获取数据条数
    int pageCount = ceil(1.0 * studios_skinNumber(filter) / 20);

    // 获取数据
    string sql = "SELECT * FROM Studios_Skin";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id DESC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    dbres res = (new DB_Controller)->query(sql.c_str());
    Section<StudiosSkinItem> list = Section<StudiosSkinItem>(pageCount, SkinSearch);
    for (int i = 0; i < res.size(); i++) {
        StudiosSkinItem data = StudiosSkinItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            SRL<SkinThumbnail>(res[i]["thumbnail"], "/data/" + res[i]["thumbnail"]),
            atoi(res[i]["interpolation"].c_str()),
            res[i]["sprites"]
        ); list.append(data);
    } return list;
}

string studios_skinFilter(argvar arg) {
    string filter = "";
    if (arg["keywords"] != "") filter = "title like \"%" + str_replace("\"", "\\\"", urldecode(arg["keywords"])) + "%\"";
    return filter;
}

int studios_skinCreate(StudiosSkinItem item) {
    stringstream sqlbuffer;
    auto res = (new DB_Controller)->query("SELECT id FROM Studios_Skin WHERE name = \"" + item.name + "\"");
    if (res.size() != 0) return 0;
    item.spritesJson = str_replace("\\", "\\\\", str_replace("\"", "\\\"", item.spritesJson));
    int id = atoi((new DB_Controller)->query("SELECT COUNT(*) AS sum FROM Studios_Skin;")[0]["sum"].c_str()) + 1;
    sqlbuffer << "INSERT INTO Studios_Skin (id, name, version, title, subtitle, author, thumbnail, interpolation, sprites) VALUES (";
    sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
    sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", \"" << item.thumbnail.hash << "\", ";
    sqlbuffer << item.interpolation << ", \"" << item.spritesJson << "\")";
    return (new DB_Controller)->execute(sqlbuffer.str());
}

#endif