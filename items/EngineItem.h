#ifndef ENGINEITEM_H
#define ENGINEITEM_H

using namespace std;

class EngineItem {
    public:

    int id;
    string name;
    int version = 7;
    string title;
    string subtitle;
    string author;
    SkinItem skin;
    BackgroundItem background;
    EffectItem effect;
    ParticleItem particle;
    SRL<EngineThumbnail> thumbnail;
    SRL<EngineData> data;
    SRL<EngineConfiguration> configuration;
    SRL<EngineRom> rom;

    EngineItem(){}
    EngineItem(int id, string name, string title, string subtitle, string author,
        SkinItem skin, BackgroundItem background, EffectItem effect, ParticleItem particle,
        SRL<EngineThumbnail> thumbnail, SRL<EngineData> data, SRL<EngineConfiguration> configuration, SRL<EngineRom> rom = SRL<EngineRom>("", "")):
        id(id), name(name), title(title), subtitle(subtitle), author(author),
        skin(skin), background(background), effect(effect), particle(particle),
        thumbnail(thumbnail), data(data), configuration(configuration), rom(rom){}
    
    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["version"] = version;
        res["title"] = title;
        res["subtitle"] = subtitle;
        res["author"] = author;
        res["skin"] = skin.toJsonObject();
        res["background"] = background.toJsonObject();
        res["effect"] = effect.toJsonObject();
        res["particle"] = particle.toJsonObject();
        res["thumbnail"] = thumbnail.toJsonObject();
        res["data"] = data.toJsonObject();
        res["configuration"] = configuration.toJsonObject();
        if (rom.hash != "") res["rom"] = rom.toJsonObject();
        return res;
    }
};

Section<EngineItem> engineList(string filter, int st = 1, int en = 20) {
    // 获取数据条数
    string sql = "SELECT COUNT(*) AS sum FROM Engine";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id ASC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    mysqld res = mysqli_query(mysql, sql.c_str());
    int pageCount = atoi(res[0]["sum"].c_str()) / 20;

    // 获取数据
    sql = "SELECT * FROM Engine";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id ASC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    res = mysqli_query(mysql, sql.c_str());
    Section<EngineItem> list = Section<EngineItem>(pageCount, EngineSearch);
    for (int i = 0; i < res.size(); i++) {
        SkinItem skin = skinList("id = " + res[i]["skin"], 1, 1).items[0];
        BackgroundItem background = backgroundList("id = " + res[i]["background"], 1, 1).items[0];
        EffectItem effect = effectList("id = " + res[i]["effect"], 1, 1).items[0];
        ParticleItem particle = particleList("id = " + res[i]["particle"], 1, 1).items[0];
        EngineItem data = EngineItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            skin, background, effect, particle,
            SRL<EngineThumbnail>(res[i]["thumbnail"], "/data/" + res[i]["thumbnail"]),
            SRL<EngineData>(res[i]["data"], "/data/" + res[i]["data"]),
            SRL<EngineConfiguration>(res[i]["configuration"], "/data/" + res[i]["configuration"]),
            SRL<EngineRom>(res[i]["rom"], "/data/" + res[i]["rom"])
        ); list.append(data);
    } return list;
}



#endif