#ifndef BACKGROUNDITEM_H
#define BACKGROUNDITEM_H

using namespace std;

class BackgroundItem {
    public:

    int id;
    string name;
    int version = 2;
    string title;
    string subtitle;
    string author;
    SRL<BackgroundThumbnail> thumbnail;
    SRL<BackgroundData> data;
    SRL<BackgroundImage> image;
    SRL<BackgroundConfiguration> configuration;

    BackgroundItem(){}
    BackgroundItem(int id, string name, string title, string subtitle, string author, 
        SRL<BackgroundThumbnail> thumbnail, SRL<BackgroundData> data, SRL<BackgroundImage> image, SRL<BackgroundConfiguration> configuration):
        id(id), name(name), title(title), subtitle(subtitle), author(author),
        thumbnail(thumbnail), data(data), image(image), configuration(configuration){}
    
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
        args["url"] = "/backgrounds/" + name;
        args["sonolus.url"] = "sonolus:" + appConfig["server.rootUrl"].asString() + "/backgrounds/" + name;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/backgrounds.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int backgroundNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Background";
    if (filter != "") sql += " WHERE (" + filter + ")";
    mysqld res = mysqli_query(mysql, sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

Section<BackgroundItem> backgroundList(string filter, int st = 1, int en = 20) {
    // 获取数据条数
    int pageCount = ceil(1.0 * backgroundNumber(filter) / 20);

    // 获取数据
    string sql = "SELECT * FROM Background";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id DESC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    auto res = mysqli_query(mysql, sql.c_str());
    Section<BackgroundItem> list = Section<BackgroundItem>(pageCount, BackgroundSearch);
    for (int i = 0; i < res.size(); i++) {
        BackgroundItem data = BackgroundItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"], 
            res[i]["title"], 
            res[i]["subtitle"],
            res[i]["author"],
            SRL<BackgroundThumbnail>(res[i]["thumbnail"], "/data/" + res[i]["thumbnail"]),
            SRL<BackgroundData>(res[i]["data"], "/data/" + res[i]["data"]),
            SRL<BackgroundImage>(res[i]["image"], "/data/" + res[i]["image"]),
            SRL<BackgroundConfiguration>(res[i]["configuration"], "/data/" + res[i]["configuration"])
        ); list.append(data);
    } return list;
}

string backgroundFilter(argvar arg) {
    string filter = "";
    if (arg["keywords"] != "") filter = "title like \"%" + str_replace("\"", "\\\"", urldecode(arg["keywords"])) + "%\"";
    return filter;
}

#endif