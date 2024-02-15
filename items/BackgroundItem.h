#ifndef BACKGROUNDITEM_H
#define BACKGROUNDITEM_H

using namespace std;

class BackgroundItem {
    public:

    int id;
    string name;
    int version = backgroundVersion;
    string title;
    string subtitle;
    string author;
    SRL<BackgroundThumbnail> thumbnail;
    SRL<BackgroundData> data;
    SRL<BackgroundImage> image;
    SRL<BackgroundConfiguration> configuration;
    vector<Tag> tags;
    string description;
    string source;

    BackgroundItem(){}
    BackgroundItem(int id, string name, string title, string subtitle, string author, 
        SRL<BackgroundThumbnail> thumbnail, SRL<BackgroundData> data, SRL<BackgroundImage> image, SRL<BackgroundConfiguration> configuration, 
        vector<Tag> tags, string description = "", string source = ""):
        id(id), name(name), title(title), subtitle(subtitle), author(author),
        thumbnail(thumbnail), data(data), image(image), configuration(configuration), 
        tags(tags), description(description), source(source){}
    BackgroundItem(int background_id, Json::Value arr) {
        id = background_id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        title = arr["title"].asString();
        subtitle = arr["subtitle"].asString();
        author = arr["author"].asString();
        thumbnail = SRL<BackgroundThumbnail>(arr["thumbnail"]);
        data = SRL<BackgroundData>(arr["data"]);
        image = SRL<BackgroundImage>(arr["image"]);
        configuration = SRL<BackgroundConfiguration>(arr["configuration"]);
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
        res["image"] = image.toJsonObject();
        res["configuration"] = configuration.toJsonObject();
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
        args["image"] = image.url;
        args["configuration"] = configuration.url;
        args["tags"] = "";
        for (int i = 0; i < tags.size(); i++) args["tags"] += "<div class='flex' style='background-color:rgba(255,255,255,0.125);padding:5px;'>"
            "<div class='tagIcon'>{{icon." + tags[i].icon + "}}</div>&nbsp;" + tags[i].title + "</div>";
        args["url"] = "/backgrounds/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/backgrounds/" + name;
        args["description"] = description;
        args["source"] = source;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/backgrounds.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int backgroundsNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Background";
    if (filter != "") sql += " WHERE (" + filter + ")";
    dbres res = db.query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

vector<BackgroundItem> backgroundsList(string filter, string order, int st = 1, int en = 20) {
    string sql = "SELECT * FROM Background";
    if (filter != "") sql += " WHERE (" + filter + ")";
    if (order != "") sql += " ORDER BY " + order;
    sql += " LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    auto res = db.query(sql.c_str());
    vector<BackgroundItem> list = {};
    sort(res.begin(), res.end(), [](argvar a, argvar b){
        if (a["name"] == b["name"]) return (a["localization"] == "default") < (b["localization"] == "default");
        else return atoi(a["id"].c_str()) > atoi(b["id"].c_str());
    }); map<string, bool> nameUsed;
    for (int i = 0; i < res.size(); i++) {
        if (nameUsed[res[i]["name"]]) continue;
        nameUsed[res[i]["name"]] = true;
        BackgroundItem data = BackgroundItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"], 
            res[i]["title"], 
            res[i]["subtitle"],
            res[i]["author"],
            SRL<BackgroundThumbnail>(res[i]["thumbnail"], dataPrefix + res[i]["thumbnail"]),
            SRL<BackgroundData>(res[i]["data"], dataPrefix + res[i]["data"]),
            SRL<BackgroundImage>(res[i]["image"], dataPrefix + res[i]["image"]),
            SRL<BackgroundConfiguration>(res[i]["configuration"], dataPrefix + res[i]["configuration"]),
            deserializeTagString(res[i]["tags"]),
            str_replace("\\n", "\n", res[i]["description"]),
            (appConfig["server.enableSSL"].asBool() ? "https://" : "http://") + appConfig["server.rootUrl"].asString() + "/sonolus/backgrounds/" + res[i]["name"]
        ); list.push_back(data);
    } return list;
}

int backgroundCreate(BackgroundItem item, string localization = "default") {
    stringstream sqlbuffer;
    auto res = db.query("SELECT id FROM Background WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"");
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Background SET name = \"" << item.name << "\", version = " << item.version << ", title = \"" << item.title << "\", ";
        sqlbuffer << "subtitle = \"" << item.subtitle << "\", author = \"" << item.author << "\", thumbnail = \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "data = \"" << item.data.hash << "\", image = \"" << item.image.hash << "\", configuration = \"" << item.configuration.hash << "\", ";
        sqlbuffer << "description = \"" << str_replace("\n", "\\n", item.description) << "\", localization = \"" << localization << "\" WHERE id = " << id << ";";
    } else {
        int id = atoi(db.query("SELECT COUNT(*) AS sum FROM Background;")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Background (id, name, version, title, subtitle, author, thumbnail, data, image, configuration, description, localization) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", \"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "\"" << item.data.hash << "\", \"" << item.image.hash << "\", \"" << item.configuration.hash << "\", ";
        sqlbuffer << "\"" << str_replace("\n", "\\n", item.description) << "\", \"" << localization << "\");";
    } return db.execute(sqlbuffer.str());
}

#endif