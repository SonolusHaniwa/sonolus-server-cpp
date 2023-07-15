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
    string description;

    ParticleItem(){}
    ParticleItem(int id, string name, string title, string subtitle, string author,
        SRL<ParticleThumbnail> thumbnail, SRL<ParticleData> data, SRL<ParticleTexture> texture, string description = ""):
        id(id), name(name), title(title), subtitle(subtitle), author(author),
        thumbnail(thumbnail), data(data), texture(texture), description(description){}
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
        res["texture"] = texture.toJsonObject();
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
        args["texture"] = texture.url;
        args["url"] = "/particles/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/particles/" + name;
        args["description"] = description;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/particles.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int particleNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Particle";
    if (filter != "") sql += " WHERE (" + filter + ")";
    dbres res = (new DB_Controller)->query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

Section<ParticleItem> particleList(string filter, int st = 1, int en = 20) {
    // 获取数据条数
    int pageCount = ceil(1.0 * particleNumber(filter) / 20);

    // 获取数据
    string sql = "SELECT * FROM Particle";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id DESC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    dbres res = (new DB_Controller)->query(sql.c_str());
    Section<ParticleItem> list = Section<ParticleItem>(pageCount, ParticleSearch);
    sort(res.begin(), res.end(), [](argvar a, argvar b){
        if (a["name"] == b["name"]) return (a["localization"] == "default") < (b["localization"] == "default");
        else return atoi(a["id"].c_str()) > atoi(b["id"].c_str());
    }); map<string, bool> nameUsed;
    for (int i = 0; i < res.size(); i++) {
        if (nameUsed[res[i]["name"]]) continue;
        nameUsed[res[i]["name"]] = true;
        ParticleItem data = ParticleItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            SRL<ParticleThumbnail>(res[i]["thumbnail"], "/data/" + res[i]["thumbnail"]),
            SRL<ParticleData>(res[i]["data"], "/data/" + res[i]["data"]),
            SRL<ParticleTexture>(res[i]["texture"], "/data/" + res[i]["texture"]),
            res[i]["description"]
        ); list.append(data);
    } return list;
}

string particleFilter(argvar arg) {
    string filter = "(localization = \"" + arg["localization"] + "\" OR localization = \"default\")";
    if (arg["keywords"] != "") filter += " AND title like \"%" + str_replace("\"", "\\\"", urldecode(arg["keywords"])) + "%\"";
    return filter;
}

int particleCreate(ParticleItem item, string localization = "default") {
    stringstream sqlbuffer;
    auto res = (new DB_Controller)->query("SELECT id FROM Particle WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"");
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Particle SET name = \"" << item.name << "\", version = " << item.version << ", ";
        sqlbuffer << "title = \"" << item.title << "\", subtitle = \"" << item.subtitle << "\", ";
        sqlbuffer << "author = \"" << item.author << "\", thumbnail = \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "data = \"" << item.data.hash << "\", texture = \"" << item.texture.hash << "\", ";
        sqlbuffer << "description = \"" << str_replace("\n", "\\n", item.description) << "\", localization = \"" << localization << "\" WHERE id = " << id << ";";
    } else {
        int id = atoi((new DB_Controller)->query("SELECT COUNT(*) AS sum FROM Particle;")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Particle (id, name, version, title, subtitle, author, thumbnail, data, texture, description, localization) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "\"" << item.data.hash << "\", \"" << item.texture.hash << "\", \"" << str_replace("\n", "\\n", item.description) << "\", \"" << localization << "\");";
    } return (new DB_Controller)->execute(sqlbuffer.str());
}

#endif