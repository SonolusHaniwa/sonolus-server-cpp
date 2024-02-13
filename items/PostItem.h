#ifndef POSTITEM_H
#define POSTITEM_H

using namespace std;

class PostItem {
    public:

    int id;
    string name;
    int version = postVersion;
    string title;
    time_t time;
    string author;
    SRL<PostThumbnail> thumbnail;
    vector<Tag> tags;
    string description;
    string source;

    PostItem(){}
    PostItem(int id, string name, string title, time_t time, string author, SRL<PostThumbnail> thumbnail, 
        vector<Tag> tags, string description = "", string source = ""):
        id(id), name(name), title(title), time(time), author(author), thumbnail(thumbnail), 
        tags(tags), description(description), source(source){}
    PostItem(int post_id, Json::Value arr) {
        id = post_id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        title = arr["title"].asString();
        time = arr["time"].asInt64();
        author = arr["author"].asString();
        thumbnail = SRL<PostThumbnail>(arr["thumbnail"]);
        for (int i = 0; i < arr["tags"].size(); i++) tags.push_back(Tag(arr["tags"][i]));
        description = arr["description"].asString();
        source = arr["source"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["version"] = version;
        res["title"] = title;
        res["time"] = (long long)time;
        res["author"] = author;
        res["thumbnail"] = thumbnail.toJsonObject();
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
        args["time"] = to_string(time);
        args["author"] = author;
        args["thumbnail"] = thumbnail.url;
        args["url"] = "/posts/" + name;
        args["sonolus.url"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/posts/" + name;
        args["description"] = description;
        args["source"] = source;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/posts.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int postsNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Post";
    if (filter != "") sql += " WHERE (" + filter + ")";
    dbres res = (new DB_Controller)->query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

vector<PostItem> postsList(string filter, string order, int st = 1, int en = 20) {
    string sql = "SELECT * FROM Post";
    if (filter != "") sql += " WHERE (" + filter + ")";
    if (order != "") sql += " ORDER BY " + order;
    sql += " LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    auto res = (new DB_Controller)->query(sql.c_str());
    vector<PostItem> list = {};
    sort(res.begin(), res.end(), [](argvar a, argvar b){
        if (a["name"] == b["name"]) return (a["localization"] == "default") < (b["localization"] == "default");
        else return atoi(a["id"].c_str()) > atoi(b["id"].c_str());
    }); map<string, bool> nameUsed;
    for (int i = 0; i < res.size(); i++) {
        if (nameUsed[res[i]["name"]]) continue;
        nameUsed[res[i]["name"]] = true;
        PostItem data = PostItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            atol(res[i]["time"].c_str()),
            res[i]["author"],
            SRL<PostThumbnail>(res[i]["thumbnail"], dataPrefix + res[i]["thumbnail"]),
            deserializeTagString(res[i]["tags"]),
            str_replace("\\n", "\n", res[i]["description"]),
            (appConfig["server.enableSSL"].asBool() ? "https://" : "http://") + appConfig["server.rootUrl"].asString() + "/sonolus/backgrounds/" + res[i]["name"]
        ); list.push_back(data);
    } return list;
}

// int replayCreate(ReplayItem item, string localization = "default") {
//     stringstream sqlbuffer;
//     auto res = (new DB_Controller)->query("SELECT id FROM Replay WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"");
//     int levelId = atoi((new DB_Controller)->query("SELECT id FROM Level WHERE name = \"" + item.level.name + "\";")[0]["id"].c_str());
//     if (res.size() != 0) {
//         int id = atoi(res[0]["id"].c_str());
//         sqlbuffer << "UPDATE Replay SET name=\"" << item.name << "\", version=" << item.version;
//         sqlbuffer << ", title=\"" << item.title << "\", subtitle=\"" << item.subtitle << "\", author=\"" << item.author;
//         sqlbuffer << "\", level=" << levelId << ", data=\"" << item.data.hash << "\", configuration=\"" << item.configuration.hash << "\", ";
//         sqlbuffer << "description=\"" << str_replace("\n", "\\n", item.description) << "\", localization=\"" << localization << "\" WHERE id=" << id;
//     } else {
//         int id = atoi((new DB_Controller)->query("SELECT COUNT(*) AS sum FROM Replay;")[0]["sum"].c_str()) + 1;
//         sqlbuffer << "INSERT INTO Replay (id, name, version, title, subtitle, author, level, data, configuration, description, localization) VALUES (";
//         sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
//         sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", " << levelId << ", ";
//         sqlbuffer << "\"" << item.data.hash << "\", \"" << item.configuration.hash << "\", \"" << str_replace("\n", "\\n", item.description) << "\", \"" << localization << "\")";
//     } return (new DB_Controller)->execute(sqlbuffer.str());
// }

#endif
