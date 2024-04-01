#ifndef PARTICLEITEM_H
#define PARTICLEITEM_H

using namespace std;

class ParticleItem {
    public:

    int id;
    string name;
    int version = particleVersion;
    string title;
    string subtitle;
    string author;
    SRL<ParticleThumbnail> thumbnail;
    SRL<ParticleData> data;
    SRL<ParticleTexture> texture;
    vector<Tag> tags;
    string description;
    string source;

    ParticleItem(){}
    ParticleItem(int id, string name, string title, string subtitle, string author,
        SRL<ParticleThumbnail> thumbnail, SRL<ParticleData> data, SRL<ParticleTexture> texture, 
        vector<Tag> tags, string description = "", string source = ""):
        id(id), name(name), title(title), subtitle(subtitle), author(author),
        thumbnail(thumbnail), data(data), texture(texture), 
        tags(tags), description(description), source(source){}
    ParticleItem(int particle_id, Json::Value arr) {
        id = particle_id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        title = arr["title"].asString();
        subtitle = arr["subtitle"].asString();
        author = arr["author"].asString();
        thumbnail = SRL<ParticleThumbnail>(arr["thumbnail"]);
        data = SRL<ParticleData>(arr["data"]);
        texture = SRL<ParticleTexture>(arr["texture"]);
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
        res["thumbnail"] = thumbnail.toJsonObject();
        res["data"] = data.toJsonObject();
        res["texture"] = texture.toJsonObject();
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
        args["texture"] = texture.url;
        args["tags"] = "";
        for (int i = 0; i < tags.size(); i++) args["tags"] += "<div class='flex' style='background-color:rgba(255,255,255,0.125);padding:5px;'>"
            "<div class='tagIcon'>{{icon." + tags[i].icon + "}}</div>&nbsp;" + tags[i].title + "</div>";
        args["url"] = "/particles/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/particles/" + name;
        args["description"] = description;
        args["source"] = source;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/particles.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int particlesNumber(string filter) {
    itemNumberTemplate(Particle, filter);
    dbres res = db.query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

vector<ParticleItem> particlesList(string filter, string order, int st = 1, int en = 20) {
    itemListTemplate(Particle, filter, order, st, en);

    dbres res = db.query(sql.c_str());
    vector<ParticleItem> list = {};

    for (int i = 0; i < res.size(); i++) {
        ParticleItem data = ParticleItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            SRL<ParticleThumbnail>(res[i]["thumbnail"], dataPrefix + res[i]["thumbnail"]),
            SRL<ParticleData>(res[i]["data"], dataPrefix + res[i]["data"]),
            SRL<ParticleTexture>(res[i]["texture"], dataPrefix + res[i]["texture"]),
            deserializeTagString(res[i]["tags"]),
            str_replace("\\n", "\n", res[i]["description"]),
            (appConfig["server.enableSSL"].asBool() ? "https://" : "http://") + appConfig["server.rootUrl"].asString()
        ); list.push_back(data);
    } return list;
}

int particlesCreate(ParticleItem item, string localization = "default") {
    stringstream sqlbuffer;
    auto res = db.query("SELECT id FROM Particle WHERE id = " + to_string(item.id));
    if (res.size() == 0) res = db.query("SELECT id FROM Particle WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"");
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Particle SET name = \"" << item.name << "\", version = " << item.version << ", ";
        sqlbuffer << "title = \"" << item.title << "\", subtitle = \"" << item.subtitle << "\", ";
        sqlbuffer << "author = \"" << item.author << "\", thumbnail = \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "data = \"" << item.data.hash << "\", texture = \"" << item.texture.hash << "\", ";
        sqlbuffer << "tags=\"" << serializeTagString(item.tags) << "\", ";
        sqlbuffer << "description = \"" << str_replace("\n", "\\n", item.description) << "\", localization = \"" << localization << "\" WHERE id = " << id << ";";
    } else {
        int id = atoi(db.query("SELECT COUNT(*) AS sum FROM Particle;")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Particle (id, name, version, title, subtitle, author, thumbnail, tags, data, texture, description, localization) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "\"" << serializeTagString(item.tags) << "\", ";
        sqlbuffer << "\"" << item.data.hash << "\", \"" << item.texture.hash << "\", \"" << str_replace("\n", "\\n", item.description) << "\", \"" << localization << "\");";
    } return db.execute(sqlbuffer.str());
}

#endif
