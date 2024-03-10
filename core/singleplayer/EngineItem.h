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
	SRL<EngineTutorialData> tutorialData;
	SRL<EnginePreviewData> previewData;
	SRL<EngineWatchData> watchData;
    vector<Tag> tags;
    string description;
    string source;

    EngineItem(){}
    EngineItem(int id, string name, string title, string subtitle, string author,
        SkinItem skin, BackgroundItem background, EffectItem effect, ParticleItem particle,
        SRL<EngineThumbnail> thumbnail, SRL<EngineData> data, SRL<EngineTutorialData> tutorialData, SRL<EnginePreviewData> previewData, 
        SRL<EngineWatchData> watchData, SRL<EngineConfiguration> configuration, vector<Tag> tags, SRL<EngineRom> rom = SRL<EngineRom>("", ""), 
        string description = "", string source = ""):
        id(id), name(name), title(title), subtitle(subtitle), author(author),
        skin(skin), background(background), effect(effect), particle(particle),
        thumbnail(thumbnail), data(data), tutorialData(tutorialData), previewData(previewData), 
        watchData(watchData), configuration(configuration), rom(rom), 
        tags(tags), description(description), source(source){}
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
		tutorialData = SRL<EngineTutorialData>(arr["tutorialData"]);
		previewData = SRL<EnginePreviewData>(arr["previewData"]);
		watchData = SRL<EngineWatchData>(arr["watchData"]);
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
        res["skin"] = skin.toJsonObject();
        res["background"] = background.toJsonObject();
        res["effect"] = effect.toJsonObject();
        res["particle"] = particle.toJsonObject();
        res["thumbnail"] = thumbnail.toJsonObject();
        res["data"] = data.toJsonObject();
		SRL<EnginePlayData> playData;
		playData.url = data.url; playData.hash = data.hash;
		res["playData"] = playData.toJsonObject();
        res["configuration"] = configuration.toJsonObject();
		res["tutorialData"] = tutorialData.toJsonObject();
		res["previewData"] = previewData.toJsonObject();
		res["watchData"] = watchData.toJsonObject();
        if (rom.hash != "") res["rom"] = rom.toJsonObject();
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
        args["skin"] = skin.name;
        args["background"] = background.name;
        args["effect"] = effect.name;
        args["particle"] = particle.name;
        args["thumbnail"] = thumbnail.url;
        args["data"] = data.url;
		args["playData"] = data.url;
		args["tutorialData"] = tutorialData.url;
		args["previewData"] = previewData.url;
		args["watchData"] = watchData.url;
        args["configuration"] = configuration.url;
        args["rom"] = rom.url;
        args["tags"] = "";
        for (int i = 0; i < tags.size(); i++) args["tags"] += "<div class='flex' style='background-color:rgba(255,255,255,0.125);padding:5px;'>"
            "<div class='tagIcon'>{{icon." + tags[i].icon + "}}</div>&nbsp;" + tags[i].title + "</div>";
        args["url"] = "/engines/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/engines/" + name;
        args["description"] = description;
        args["source"] = source;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/engines.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int enginesNumber(string filter) {
    itemNumberTemplate(Engine, filter);
    dbres res = db.query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

vector<EngineItem> enginesList(string filter, string order, int st = 1, int en = 20) {
    itemListTemplate(Engine, filter, order, st, en);

    dbres res = db.query(sql.c_str());
    vector<EngineItem> list = {};

    for (int i = 0; i < res.size(); i++) {
        SkinItem skin = skinsList("id = " + res[i]["skin"], "", 1, 1)[0];
        BackgroundItem background = backgroundsList("id = " + res[i]["background"], "", 1, 1)[0];
        EffectItem effect = effectsList("id = " + res[i]["effect"], "", 1, 1)[0];
        ParticleItem particle = particlesList("id = " + res[i]["particle"], "", 1, 1)[0];
        EngineItem data = EngineItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            skin, background, effect, particle,
            SRL<EngineThumbnail>(res[i]["thumbnail"], dataPrefix + res[i]["thumbnail"]),
            SRL<EngineData>(res[i]["data"], dataPrefix + res[i]["data"]),
			SRL<EngineTutorialData>(res[i]["tutorialData"], dataPrefix + res[i]["tutorialData"]),
			SRL<EnginePreviewData>(res[i]["previewData"], dataPrefix + res[i]["previewData"]),
			SRL<EngineWatchData>(res[i]["watchData"], dataPrefix + res[i]["watchData"]),
            SRL<EngineConfiguration>(res[i]["configuration"], dataPrefix + res[i]["configuration"]),
            deserializeTagString(res[i]["tags"]),
            SRL<EngineRom>(res[i]["rom"], dataPrefix + res[i]["rom"]),
            str_replace("\\n", "\n", res[i]["description"]),
            (appConfig["server.enableSSL"].asBool() ? "https://" : "http://") + appConfig["server.rootUrl"].asString()
        ); list.push_back(data);
    } return list;
}

int enginesCreate(EngineItem item, string localization = "default") {
    stringstream sqlbuffer;
    auto res = db.query("SELECT id FROM Engine WHERE id = " + to_string(item.id));
    if (res.size() == 0) res = db.query("SELECT id FROM Engine WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"");
    int skinId = atoi(db.query("SELECT id FROM Skin WHERE name = \"" + item.skin.name + "\";")[0]["id"].c_str());
    int backgroundId = atoi(db.query("SELECT id FROM Background WHERE name = \"" + item.background.name + "\";")[0]["id"].c_str());
    int effectId = atoi(db.query("SELECT id FROM Effect WHERE name = \"" + item.effect.name + "\";")[0]["id"].c_str());
    int particleId = atoi(db.query("SELECT id FROM Particle WHERE name = \"" + item.particle.name + "\";")[0]["id"].c_str());
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Engine SET name = \"" << item.name << "\", version = " << item.version << ", title = \"" << item.title << "\", ";
        sqlbuffer << "subtitle = \"" << item.subtitle << "\", author = \"" << item.author << "\", skin = " << skinId << ", background = " << backgroundId << ", ";
        sqlbuffer << "effect = " << effectId << ", particle = " << particleId << ", thumbnail = \"" << item.thumbnail.hash << "\", data = \"" << item.data.hash << "\", tutorialData = \"" << item.tutorialData.hash << "\", previewData = \"" << item.previewData.hash << "\", watchData = \"" << item.watchData.hash << "\", ";
        sqlbuffer << "configuration = \"" << item.configuration.hash << "\", rom = \"" << item.rom.hash << "\", description = \"" << str_replace("\n", "\\n", item.description) << "\", localization = \"" << localization << "\" WHERE id = " << id << ";";
    } else {
        int id = atoi(db.query("SELECT COUNT(*) AS sum FROM Engine;")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Engine (id, name, version, title, subtitle, author, skin, background, effect, particle, thumbnail, data, configuration, rom, description, localization, tutorialData, previewData, watchData) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", " << skinId << ", " << backgroundId << ", " << effectId << ", " << particleId << ", ";
        sqlbuffer << "\"" << item.thumbnail.hash << "\", \"" << item.data.hash << "\", \"" << item.configuration.hash << "\", ";
        sqlbuffer << "\"" << item.rom.hash << "\", \"" << str_replace("\n", "\\n", item.description) << "\", \"" << localization << "\", \"" << item.tutorialData.hash << "\", \"" << item.previewData.hash << "\", \"" << item.watchData.hash << "\");";
    } return db.execute(sqlbuffer.str());
}

#endif
