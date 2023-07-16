#ifndef EFFECTITEM_H
#define EFFECTITEM_H

using namespace std;

class EffectItem {
    public:

    int id;
    string name;
    int version = effectVersion;
    string title;
    string subtitle;
    string author;
    SRL<EffectThumbnail> thumbnail;
    SRL<EffectData> data;
    SRL<EffectAudio> audio;
    string description;

    EffectItem(){}
    EffectItem(int id, string name, string title, string subtitle, string author,
        SRL<EffectThumbnail> thumbnail, SRL<EffectData> data, SRL<EffectAudio> audio, string description = ""):
        id(id), name(name), title(title), subtitle(subtitle), author(author),
        thumbnail(thumbnail), data(data), audio(audio), description(description){}
    EffectItem(int effect_id, Json::Value arr) {
        id = effect_id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        title = arr["title"].asString();
        subtitle = arr["subtitle"].asString();
        author = arr["author"].asString();
        thumbnail = SRL<EffectThumbnail>(arr["thumbnail"]);
        data = SRL<EffectData>(arr["data"]);
        audio = SRL<EffectAudio>(arr["audio"]);
        description = arr["description"].asString();
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
        res["audio"] = audio.toJsonObject();
        res["description"] = description;
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
        args["audio"] = audio.url;
        args["url"] = "/effects/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/effects/" + name;
        args["description"] = description;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/effects.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int effectNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Effect";
    if (filter != "") sql += " WHERE (" + filter + ")";
    dbres res = (new DB_Controller)->query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

Section<EffectItem> effectList(string filter, int st = 1, int en = 20) {
    // 获取数据条数
    int pageCount = ceil(1.0 * effectNumber(filter) / 20);

    // 获取数据
    string sql = "SELECT * FROM Effect";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id DESC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    dbres res = (new DB_Controller)->query(sql.c_str());
    Section<EffectItem> list = Section<EffectItem>(pageCount, EffectSearch);
    sort(res.begin(), res.end(), [](argvar a, argvar b){
        if (a["name"] == b["name"]) return (a["localization"] == "default") < (b["localization"] == "default");
        else return atoi(a["id"].c_str()) > atoi(b["id"].c_str());
    }); map<string, bool> nameUsed;
    for (int i = 0; i < res.size(); i++) {
        if (nameUsed[res[i]["name"]]) continue;
        nameUsed[res[i]["name"]] = true;
        EffectItem data = EffectItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            SRL<EffectThumbnail>(res[i]["thumbnail"], "/data/" + res[i]["thumbnail"]),
            SRL<EffectData>(res[i]["data"], "/data/" + res[i]["data"]),
            SRL<EffectAudio>(res[i]["audio"], "/data/" + res[i]["audio"]),
            str_replace("\\n", "\n", res[i]["description"])
        ); list.append(data);
    } return list;
}

string effectFilter(argvar arg) {
    string filter = "(localization = \"" + arg["localization"] + "\" OR localization = \"default\")";
    if (arg["keywords"] != "") filter += " AND title like \"%" + str_replace("\"", "\\\"", urldecode(arg["keywords"])) + "%\"";
    return filter;
}

int effectCreate(EffectItem item, string localization = "default") {
    stringstream sqlbuffer;
    auto res = (new DB_Controller)->query("SELECT id FROM Effect WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"");
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Effect SET name = \"" << item.name << "\", version = " << item.version << ", ";
        sqlbuffer << "title = \"" << item.title << "\", subtitle = \"" << item.subtitle << "\", ";
        sqlbuffer << "author = \"" << item.author << "\", thumbnail = \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "data = \"" << item.data.hash << "\", audio = \"" << item.audio.hash << "\", ";
        sqlbuffer << "description = \"" << str_replace("\n", "\\n", item.description) << "\", localization = \"" << localization << "\" WHERE id = " << id;
    } else {
        int id = atoi((new DB_Controller)->query("SELECT COUNT(*) AS sum FROM Effect;")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Effect (id, name, version, title, subtitle, author, thumbnail, data, audio, description, localization) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "\"" << item.data.hash << "\", \"" << item.audio.hash << "\", \"" << str_replace("\n", "\\n", item.description) << "\", \"" << localization << "\")";
    } return (new DB_Controller)->execute(sqlbuffer.str());
}

#endif