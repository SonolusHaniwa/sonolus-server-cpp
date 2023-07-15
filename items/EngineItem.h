#ifndef ENGINEITEM_H
#define ENGINEITEM_H

using namespace std;

class EngineItem {
    public:

    int id;
    string name;
    int version = engineVersion;
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
    string description;

    EngineItem(){}
    EngineItem(int id, string name, string title, string subtitle, string author,
        SkinItem skin, BackgroundItem background, EffectItem effect, ParticleItem particle,
        SRL<EngineThumbnail> thumbnail, SRL<EngineData> data, SRL<EngineConfiguration> configuration, SRL<EngineRom> rom = SRL<EngineRom>("", ""), string description = ""):
        id(id), name(name), title(title), subtitle(subtitle), author(author),
        skin(skin), background(background), effect(effect), particle(particle),
        thumbnail(thumbnail), data(data), configuration(configuration), rom(rom), description(description){}
    EngineItem(int engine_id, Json::Value arr) {
        id = engine_id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        title = arr["title"].asString();
        subtitle = arr["subtitle"].asString();
        author = arr["author"].asString();
        skin = SkinItem(-1, arr["skin"]);
        background = BackgroundItem(-1, arr["background"]);
        effect = EffectItem(-1, arr["effect"]);
        particle = ParticleItem(-1, arr["particle"]);
        thumbnail = SRL<EngineThumbnail>(arr["thumbnail"]);
        data = SRL<EngineData>(arr["data"]);
        configuration = SRL<EngineConfiguration>(arr["configuration"]);
        rom = SRL<EngineRom>(arr["rom"]);
        description = arr["description"].asString();
    }
    
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
        args["skin"] = "/skins/" + skin.name;
        args["background"] = "/backgrounds/" + background.name;
        args["effect"] = "/effects/" + effect.name;
        args["particle"] = "/particles/" + particle.name;
        args["thumbnail"] = thumbnail.url;
        args["data"] = data.url;
        args["configuration"] = configuration.url;
        args["rom"] = rom.url;
        args["url"] = "/engines/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/engines/" + name;
        args["description"] = description;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/engines.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int engineNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Engine";
    if (filter != "") sql += " WHERE (" + filter + ")";
    dbres res = (new DB_Controller)->query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

Section<EngineItem> engineList(string filter, int st = 1, int en = 20) {
    // 获取数据条数
    int pageCount = ceil(1.0 * engineNumber(filter) / 20);

    // 获取数据
    string sql = "SELECT * FROM Engine";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id DESC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    dbres res = (new DB_Controller)->query(sql.c_str());
    Section<EngineItem> list = Section<EngineItem>(pageCount, EngineSearch);
    sort(res.begin(), res.end(), [](argvar a, argvar b){
        if (a["name"] == b["name"]) return (a["localization"] == "default") < (b["localization"] == "default");
        else return atoi(a["id"].c_str()) > atoi(b["id"].c_str());
    }); map<string, bool> nameUsed;
    for (int i = 0; i < res.size(); i++) {
        if (nameUsed[res[i]["name"]]) continue;
        nameUsed[res[i]["name"]] = true;
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
            SRL<EngineRom>(res[i]["rom"], "/data/" + res[i]["rom"]),
            res[i]["description"]
        ); list.append(data);
    } return list;
}

string engineFilter(argvar arg) {
    string filter = "(localization = \"" + arg["localization"] + "\" OR localization = \"default\")";
    if (arg["keywords"] != "") filter += " AND title like \"%" + str_replace("\"", "\\\"", urldecode(arg["keywords"])) + "%\"";
    return filter;
}

int engineCreate(EngineItem item, string localization = "default") {
    stringstream sqlbuffer;
    auto res = (new DB_Controller)->query("SELECT id FROM Engine WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"");
    int skinId = atoi((new DB_Controller)->query("SELECT id FROM Skin WHERE name = \"" + item.skin.name + "\";")[0]["id"].c_str());
    int backgroundId = atoi((new DB_Controller)->query("SELECT id FROM Background WHERE name = \"" + item.background.name + "\";")[0]["id"].c_str());
    int effectId = atoi((new DB_Controller)->query("SELECT id FROM Effect WHERE name = \"" + item.effect.name + "\";")[0]["id"].c_str());
    int particleId = atoi((new DB_Controller)->query("SELECT id FROM Particle WHERE name = \"" + item.particle.name + "\";")[0]["id"].c_str());
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Engine SET name = \"" << item.name << "\", version = " << item.version << ", title = \"" << item.title << "\", ";
        sqlbuffer << "subtitle = \"" << item.subtitle << "\", author = \"" << item.author << "\", skin = " << skinId << ", background = " << backgroundId << ", ";
        sqlbuffer << "effect = " << effectId << ", particle = " << particleId << ", thumbnail = \"" << item.thumbnail.hash << "\", data = \"" << item.data.hash << "\", ";
        sqlbuffer << "configuration = \"" << item.configuration.hash << "\", rom = \"" << item.rom.hash << "\", description = \"" << str_replace("\n", "\\n", item.description) << "\", localization = \"" << localization << "\" WHERE id = " << id << ";";
    } else {
        int id = atoi((new DB_Controller)->query("SELECT COUNT(*) AS sum FROM Engine;")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Engine (id, name, version, title, subtitle, author, skin, background, effect, particle, thumbnail, data, configuration, rom, description, localization) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", " << skinId << ", " << backgroundId << ", " << effectId << ", " << particleId << ", ";
        sqlbuffer << "\"" << item.thumbnail.hash << "\", \"" << item.data.hash << "\", \"" << item.configuration.hash << "\", ";
        sqlbuffer << "\"" << item.rom.hash << "\", \"" << str_replace("\n", "\\n", item.description) << "\", \"" << localization << "\");";
    } return (new DB_Controller)->execute(sqlbuffer.str());
}

#endif