#ifndef LEVELITEM_H
#define LEVELITEM_H

using namespace std;

template<typename T>
class UseItem {
    public:

    bool useDefault = true;
    T item = T();

    UseItem(){}
    UseItem(bool useDefault, T item = T()): useDefault(useDefault), item(item){}
    UseItem(Json::Value arr) {
        useDefault = arr["useDefault"].asInt();
        item = T(-1, arr["item"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["useDefault"] = useDefault;
        res["item"] = item.toJsonObject();
        return res;
    }
};

class LevelItem {
    public:

    int id;
    string name;
    int version = levelVersion;
    int rating;
    string title;
    string artists;
    string author;
    EngineItem engine;
    UseItem<SkinItem> useSkin;
    UseItem<BackgroundItem> useBackground;
    UseItem<EffectItem> useEffect;
    UseItem<ParticleItem> useParticle;
    SRL<LevelCover> cover;
    SRL<LevelBgm> bgm;
    SRL<LevelData> data;
    SRL<LevelPreview> preview;
    string description;

    LevelItem(){}
    LevelItem(int id, string name, int rating, string title, string artists, string author, EngineItem engine, 
        UseItem<SkinItem> useSkin, UseItem<BackgroundItem> useBackground, UseItem<EffectItem> useEffect, UseItem<ParticleItem> useParticle,
        SRL<LevelCover> cover, SRL<LevelBgm> bgm, SRL<LevelData> data, SRL<LevelPreview> preview, string description = ""):
        id(id), name(name), rating(rating), title(title), artists(artists), author(author), engine(engine), 
        useSkin(useSkin), useBackground(useBackground), useEffect(useEffect), useParticle(useParticle),
        cover(cover), bgm(bgm), data(data), preview(preview), description(description){}
    LevelItem(int level_id, Json::Value arr) {
        id = level_id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        rating = arr["rating"].asInt();
        title = arr["title"].asString();
        artists = arr["artists"].asString();
        author = arr["author"].asString();
        engine = EngineItem(-1, arr["engine"]);
        useSkin = UseItem<SkinItem>(arr["useSkin"]);
        useBackground = UseItem<BackgroundItem>(arr["useBackground"]);
        useEffect = UseItem<EffectItem>(arr["useEffect"]);
        useParticle = UseItem<ParticleItem>(arr["useParticle"]);
        cover = SRL<LevelCover>(arr["cover"]);
        bgm = SRL<LevelBgm>(arr["bgm"]);
        data = SRL<LevelData>(arr["data"]);
        preview = SRL<LevelPreview>(arr["preview"]);
        description = arr["description"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["version"] = version;
        res["rating"] = rating;
        res["title"] = title;
        res["artists"] = artists;
        res["author"] = author;
        res["engine"] = engine.toJsonObject();
        res["useSkin"] = useSkin.toJsonObject();
        res["useBackground"] = useBackground.toJsonObject();
        res["useEffect"] = useEffect.toJsonObject();
        res["useParticle"] = useParticle.toJsonObject();
        res["cover"] = cover.toJsonObject();
        res["bgm"] = bgm.toJsonObject();
        res["data"] = data.toJsonObject();
        res["preview"] = preview.toJsonObject();
        res["description"] = description;
        return res;
    }

    argvar fetchParamList() {
        argvar args;
        args["name"] = name;
        args["version"] = to_string(version);
        args["rating"] = to_string(rating);
        args["title"] = title;
        args["artists"] = artists;
        args["author"] = author;
        args["engine"] = engine.title;
        args["skin"] = useSkin.useDefault ? engine.skin.title : useSkin.item.title;
        args["background"] = useBackground.useDefault ? engine.background.title : useBackground.item.title;
        args["effect"] = useEffect.useDefault ? engine.effect.title : useEffect.item.title;
        args["particle"] = useParticle.useDefault ? engine.particle.title : useParticle.item.title;
        args["cover"] = cover.url;
        args["bgm"] = bgm.url;
        args["data"] = data.url;
        args["preview"] = preview.url;
        args["url"] = "/levels/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/levels/" + name;
        args["description"] = description;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/levels.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int levelNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Level";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id";
    dbres res = (new DB_Controller)->query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

Section<LevelItem> levelList(string filter, int st = 1, int en = 20) {
    // 获取数据条数
    int pageCount = ceil(1.0 * levelNumber(filter) / 20);

    // 获取数据
    string sql = "SELECT * FROM Level";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id DESC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    auto res = (new DB_Controller)->query(sql.c_str());
    Section<LevelItem> list = Section<LevelItem>(pageCount, LevelSearch);
    for (int i = 0; i < res.size(); i++) {
        EngineItem engine = engineList("id = " + res[i]["engine"], 1, 1).items[0];
        UseItem<SkinItem> useSkin = UseItem<SkinItem>(
            res[i]["skin"] == "0", res[i]["skin"] == "0" ? SkinItem() : skinList("id = " + res[i]["skin"], 1, 1).items[0]);
        UseItem<BackgroundItem> useBackground = UseItem<BackgroundItem>(
            res[i]["background"] == "0", res[i]["background"] == "0" ? BackgroundItem() : backgroundList("id = " + res[i]["background"], 1, 1).items[0]);
        UseItem<EffectItem> useEffect = UseItem<EffectItem>(
            res[i]["effect"] == "0", res[i]["effect"] == "0" ? EffectItem() : effectList("id = " + res[i]["effect"], 1, 1).items[0]);
        UseItem<ParticleItem> useParticle = UseItem<ParticleItem>(
            res[i]["particle"] == "0", res[i]["particle"] == "0" ? ParticleItem() : particleList("id = " + res[i]["particle"], 1, 1).items[0]);
        LevelItem data = LevelItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            atoi(res[i]["rating"].c_str()),
            res[i]["title"],
            res[i]["artists"],
            res[i]["author"],
            engine,
            useSkin,
            useBackground,
            useEffect,
            useParticle,
            SRL<LevelCover>(res[i]["cover"], "/data/" + res[i]["cover"]),
            SRL<LevelBgm>(res[i]["bgm"], "/data/" + res[i]["bgm"]),
            SRL<LevelData>(res[i]["data"], "/data/" + res[i]["data"]),
            SRL<LevelPreview>(res[i]["preview"], "/data/" + res[i]["preview"]),
            res[i]["description"]
        ); list.append(data);
    } return list;
}

string levelFilter(argvar arg) {
    string filter = ""; bool pre = false;
    if (arg["keywords"] != "") filter += (pre ? "AND " : "") + string("title like \"%") + str_replace("\"", "\\\"", urldecode(arg["keywords"])) + "%\" ", pre = true;
    if (arg["artist"] != "") filter += (pre ? "AND " : "") + string("artists like \"%") + str_replace("\"", "\\\"", urldecode(arg["artist"])) + "%\" ", pre = true;
    if (arg["author"] != "") filter += (pre ? "AND " : "") + string("author like \"%") + str_replace("\"", "\\\"", urldecode(arg["author"])) + "%\" ", pre = true;
    if (arg["minRating"] != "") filter += (pre ? "AND " : "") + string("rating >= ") + arg["minRating"] + " ", pre = true;
    if (arg["maxRating"] != "") filter += (pre ? "AND " : "") + string("rating <= ") + arg["maxRating"] + " ", pre = true;
    return filter;
}

int levelCreate(LevelItem item) {
    stringstream sqlbuffer;
    auto res = (new DB_Controller)->query("SELECT id FROM Level WHERE name = \"" + item.name + "\"");
    int skinId = 0, backgroundId = 0, effectId = 0, particleId = 0, engineId = 0;
    if (item.useSkin.useDefault == false)
        skinId = atoi((new DB_Controller)->query("SELECT id FROM Skin WHERE name = \"" + item.useSkin.item.name + "\";")[0]["id"].c_str());
    if (item.useBackground.useDefault == false)
        backgroundId = atoi((new DB_Controller)->query("SELECT id FROM Background WHERE name = \"" + item.useBackground.item.name + "\";")[0]["id"].c_str());
    if (item.useEffect.useDefault == false)
        effectId = atoi((new DB_Controller)->query("SELECT id FROM Effect WHERE name = \"" + item.useEffect.item.name + "\";")[0]["id"].c_str());
    if (item.useParticle.useDefault == false)
        particleId = atoi((new DB_Controller)->query("SELECT id FROM Particle WHERE name = \"" + item.useParticle.item.name + "\";")[0]["id"].c_str());
    engineId = atoi((new DB_Controller)->query("SELECT id FROM Engine WHERE name = \"" + item.engine.name + "\";")[0]["id"].c_str());
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Level SET name=\"" << item.name << "\", version=" << item.version << ", rating=\"" << item.rating;
        sqlbuffer << "\", title=\"" << item.title << "\", artists=\"" << item.artists << "\", author=\"" << item.author;
        sqlbuffer << "\", engine=" << engineId << ", skin=" << skinId << ", background=" << backgroundId << ", effect=" << effectId << ", particle=" << particleId;
        sqlbuffer << ", cover=\"" << item.cover.hash << "\", bgm=\"" << item.bgm.hash << "\", data=\"" << item.data.hash << "\", preview=\"" << item.preview.hash << "\", ";
        sqlbuffer << "\"" << str_replace("\n", "\\n", item.description) << "\" WHERE id=" << id;
    } else {
        int id = atoi((new DB_Controller)->query("SELECT COUNT(*) AS sum FROM Level;")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Level (id, name, version, rating, title, artists, author, engine, skin, background, effect, particle, cover, bgm, data, preview, description) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", " << item.rating << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.artists << "\", \"" << item.author << "\", " << engineId << ", " << skinId << ", " << backgroundId << ", " << effectId << ", " << particleId << ", ";
        sqlbuffer << "\"" << item.cover.hash << "\", \"" << item.bgm.hash << "\", \"" << item.data.hash << "\", \"" << item.preview.hash << "\", \"" << str_replace("\n", "\\n", item.description) << "\")";
    } return (new DB_Controller)->execute(sqlbuffer.str());
}

#endif