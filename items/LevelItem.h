#ifndef LEVELITEM_h
#define LEVELITEM_H

using namespace std;

template<typename T>
class UseItem {
    public:

    bool useDefault = true;
    T item = T();

    UseItem(){}
    UseItem(bool useDefault, T item = T()): useDefault(useDefault), item(item){}

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
    int version = 1;
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

    LevelItem(){}
    LevelItem(int id, string name, int rating, string title, string artists, string author, EngineItem engine, 
        UseItem<SkinItem> useSkin, UseItem<BackgroundItem> useBackground, UseItem<EffectItem> useEffect, UseItem<ParticleItem> useParticle,
        SRL<LevelCover> cover, SRL<LevelBgm> bgm, SRL<LevelData> data, SRL<LevelPreview> preview):
        id(id), name(name), rating(rating), title(title), artists(artists), author(author), engine(engine), 
        useSkin(useSkin), useBackground(useBackground), useEffect(useEffect), useParticle(useParticle),
        cover(cover), bgm(bgm), data(data), preview(preview){}

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
        return res;
    }
};

int levelNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Level";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id";
    mysqld res = mysqli_query(mysql, sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

Section<LevelItem> levelList(string filter, int st = 1, int en = 20) {
    // 获取数据条数
    int pageCount = levelNumber(filter) / 20;

    // 获取数据
    string sql = "SELECT * FROM Level";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id ASC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    auto res = mysqli_query(mysql, sql.c_str());
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
            SRL<LevelPreview>(res[i]["preview"], "/data/" + res[i]["preview"])
        ); list.append(data);
    } return list;
}

#endif