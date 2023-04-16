#ifndef STUDIOSEFFECTITEM_H
#define STUDIOSEFFECTITEM_H

using namespace std;

class StudiosEffectItem {
    public:

    int id;
    string name;
    int version = 1;
    string title;
    string subtitle;
    string author;
    SRL<EffectThumbnail> thumbnail;
    string clips;

    StudiosEffectItem(){}
    StudiosEffectItem(int id, string name, string title, string subtitle, string author, SRL<EffectThumbnail> thumbnail, string clips): 
        id(id), name(name), title(title), subtitle(subtitle), author(author), thumbnail(thumbnail), clips(clips){}
    StudiosEffectItem(int id, Json::Value arr){
        this->id = id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        title = arr["title"].asString();
        subtitle = arr["subtitle"].asString();
        author = arr["author"].asString();
        thumbnail = SRL<EffectThumbnail>(arr["thumbnail"]);
        clips = arr["clips"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["version"] = version;
        res["title"] = title;
        res["subtitle"] = subtitle;
        res["author"] = author;
        res["thumbnail"] = thumbnail.toJsonObject();
        res["clips"] = clips;
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
        args["clips"] = clips;
        args["url"] = "/studios/effects/" + name;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/studios_effects.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int studios_effectNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Studios_Effect";
    if (filter != "") sql += " WHERE (" + filter + ")";
    dbres res = (new DB_Controller)->query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

Section<StudiosEffectItem> studios_effectList(string filter, int st = 1, int en = 20) {
    // 获取数据条数
    int pageCount = ceil(1.0 * studios_effectNumber(filter) / 20);

    // 获取数据
    string sql = "SELECT * FROM Studios_Effect";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id DESC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    dbres res = (new DB_Controller)->query(sql.c_str());
    Section<StudiosEffectItem> list = Section<StudiosEffectItem>(pageCount, EffectSearch);
    for (int i = 0; i < res.size(); i++) {
        StudiosEffectItem data = StudiosEffectItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            SRL<EffectThumbnail>(res[i]["thumbnail"], "/data/" + res[i]["thumbnail"]),
            res[i]["clips"]
        ); list.append(data);
    } return list;
}

string studios_effectFilter(argvar arg) {
    string filter = "";
    if (arg["keywords"] != "") filter = "title like \"%" + str_replace("\"", "\\\"", urldecode(arg["keywords"])) + "%\"";
    return filter;
}

int studios_effectCreate(StudiosEffectItem item) {
    stringstream sqlbuffer;
    auto res = (new DB_Controller)->query("SELECT id FROM Studios_Effect WHERE name = \"" + item.name + "\"");
    if (res.size() != 0) return 0;
    int id = atoi((new DB_Controller)->query("SELECT COUNT(*) AS sum FROM Studios_Effect;")[0]["sum"].c_str()) + 1;
    sqlbuffer << "INSERT INTO Studios_Effect (id, name, version, title, subtitle, author, thumbnail, clips) VALUES (";
    sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
    sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", \"" << item.thumbnail.hash << "\", ";
    sqlbuffer << "\"" << item.clips << "\")";
    return (new DB_Controller)->execute(sqlbuffer.str());
}

#endif