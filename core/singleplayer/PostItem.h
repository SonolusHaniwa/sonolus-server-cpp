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
        res["id"] = id;
        res["name"] = name;
        res["version"] = version;
        res["title"] = title;
        res["time"] = (Json::Int64)time;
        res["author"] = author;
        res["thumbnail"] = thumbnail.toJsonObject();
        res["tags"].resize(0);
        for (int i = 0; i < tags.size(); i++) res["tags"].append(tags[i].toJsonObject());
        res["description"] = description;
        res["source"] = source;
        return res;
    }

	string formatTime(time_t t) {
		t /= 1000; tm* ti = localtime(&t);
		stringstream os;
	 	os << put_time(ti, "%m/%d/%Y %H:%M:%S");
	 	return os.str();
	}
    argvar fetchParamList() {
        argvar args;
        args["name"] = name;
        args["version"] = to_string(version);
        args["title"] = title;
        args["time"] = formatTime(time);
        args["author"] = author;
        args["thumbnail"] = thumbnail.url;
        args["tags"] = serializeTagString(tags);
        args["tags.html"] = "";
        for (int i = 0; i < tags.size(); i++) args["tags.html"] += "<div class='flex' style='background-color:rgba(255,255,255,0.125);padding:5px;'>"
            "<div class='tagIcon'>{{icon." + tags[i].icon + "}}</div>&nbsp;" + tags[i].title + "</div>";
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
    itemNumberTemplate(Post, filter);
    dbres res = db.query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

vector<PostItem> postsList(string filter, string order, int st = 1, int en = 20) {
    itemListTemplate(Post, filter, order, st, en);

    auto res = db.query(sql.c_str());
    vector<PostItem> list = {};

    for (int i = 0; i < res.size(); i++) {
        PostItem data = PostItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            [&](){
            	time_t num = 0;
            	for (int j = 0; j < res[i]["time"].size(); j++)
            		num *= 10, num += res[i]["time"][j] - '0';
            	return num;
            }(), // 震惊，llvm 的 atol 居然会出负数😨
            res[i]["author"],
            SRL<PostThumbnail>(res[i]["thumbnail"], dataPrefix + res[i]["thumbnail"]),
            deserializeTagString(res[i]["tags"]),
            str_replace("\\n", "\n", res[i]["description"]),
            (appConfig["server.enableSSL"].asBool() ? "https://" : "http://") + appConfig["server.rootUrl"].asString()
        ); list.push_back(data);
    } return list;
}

int postsCreate(PostItem item, string localization = "default") {
    stringstream sqlbuffer;
    auto res = db.query("SELECT id FROM Post WHERE id = " + to_string(item.id));
    if (res.size() == 0) res = db.query("SELECT id FROM Post WHERE name = \"" + item.name + "\" AND localization = \"" + localization + "\"");
    if (res.size() != 0) {
        int id = atoi(res[0]["id"].c_str());
        sqlbuffer << "UPDATE Post SET name=\"" << item.name << "\", version=" << item.version;
        sqlbuffer << ", title=\"" << item.title << "\", time=" << item.time << ", author=\"" << item.author << "\"";
        sqlbuffer << ", thumbnail=\"" << item.thumbnail.hash << "\"";
        sqlbuffer << ", tags=\"" << serializeTagString(item.tags) << "\"";
        sqlbuffer << ", description=\"" << str_replace("\n", "\\n", item.description) << "\", localization=\"" << localization << "\" WHERE id=" << id;
    } else {
        int id = atoi(db.query("SELECT COUNT(*) AS sum FROM Post;")[0]["sum"].c_str()) + 1;
        sqlbuffer << "INSERT INTO Post (id, name, version, title, time, author, thumbnail, tags, description, localization) VALUES (";
        sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
        sqlbuffer << item.time << ", \"" << item.author << "\", ";
        sqlbuffer << "\"" << item.thumbnail.hash << "\", ";
        sqlbuffer << "\"" << serializeTagString(item.tags) << "\", ";
        sqlbuffer << "\"" << str_replace("\n", "\\n", item.description) << "\", \"" << localization << "\")";
    } return db.execute(sqlbuffer.str());
}

#endif
