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
    vector<Tag> tags;
    string description;
    string source;

    EffectItem(){}
    EffectItem(int id, string name, string title, string subtitle, string author,
        SRL<EffectThumbnail> thumbnail, SRL<EffectData> data, SRL<EffectAudio> audio, 
        vector<Tag> tags, string description = "", string source = ""):
        id(id), name(name), title(title), subtitle(subtitle), author(author),
        thumbnail(thumbnail), data(data), audio(audio), 
        tags(tags), description(description), source(source){}
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
        res["thumbnail"] = thumbnail.toJsonObject();
        res["data"] = data.toJsonObject();
        res["audio"] = audio.toJsonObject();
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
        args["audio"] = audio.url;
        args["tags"] = serializeTagString(tags);
        args["tags.html"] = "";
        for (int i = 0; i < tags.size(); i++) args["tags.html"] += "<div class='flex' style='background-color:rgba(255,255,255,0.125);padding:5px;'>"
            "<div class='tagIcon'>{{icon." + tags[i].icon + "}}</div>&nbsp;" + tags[i].title + "</div>";
        args["url"] = "/effects/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/effects/" + name;
        args["description"] = description;
        args["source"] = source;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/effects.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int effectsNumber(string filter) {
    itemNumberTemplate(Effect, filter);
    dbres res = db.query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

vector<EffectItem> effectsList(string filter, string order, int st = 1, int en = 20) {
    itemListTemplate(Effect, filter, order, st, en);

    auto res = db.query(sql.c_str());
    vector<EffectItem> list = {};

    for (int i = 0; i < res.size(); i++) {
        EffectItem data = EffectItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            SRL<EffectThumbnail>(res[i]["thumbnail"], dataPrefix + res[i]["thumbnail"]),
            SRL<EffectData>(res[i]["data"], dataPrefix + res[i]["data"]),
            SRL<EffectAudio>(res[i]["audio"], dataPrefix + res[i]["audio"]),
            deserializeTagString(res[i]["tags"]),
            str_replace("\\n", "\n", res[i]["description"]),
            (appConfig["server.enableSSL"].asBool() ? "https://" : "http://") + appConfig["server.rootUrl"].asString()
        ); list.push_back(data);
    } return list;
}

int effectsCreate(EffectItem item, string localization = "default") {
    stringstream sqlbuffer;
    auto res = db.query("SELECT id FROM Effect WHERE id = " + to_string(item.id));
    if (res.size() == 0) res = db.query("SELECT id FROM Effect WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"");
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Effect SET name = \"" << item.name << "\", version = " << item.version << ", ";
        sqlbuffer << "title = \"" << item.title << "\", subtitle = \"" << item.subtitle << "\", ";
        sqlbuffer << "author = \"" << item.author << "\", thumbnail = \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "data = \"" << item.data.hash << "\", audio = \"" << item.audio.hash << "\", ";
        sqlbuffer << "tags=\"" << serializeTagString(item.tags) << "\", ";
        sqlbuffer << "description = \"" << str_replace("\n", "\\n", item.description) << "\", localization = \"" << localization << "\" WHERE id = " << id;
    } else {
        int id = atoi(db.query("SELECT COUNT(*) AS sum FROM Effect;")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Effect (id, name, version, title, subtitle, author, thumbnail, tags, data, audio, description, localization) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "\"" << serializeTagString(item.tags) << "\", ";
        sqlbuffer << "\"" << item.data.hash << "\", \"" << item.audio.hash << "\", \"" << str_replace("\n", "\\n", item.description) << "\", \"" << localization << "\")";
    } return db.execute(sqlbuffer.str());
}

#endif
