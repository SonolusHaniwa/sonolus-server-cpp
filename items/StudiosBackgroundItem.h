#ifndef STUDIOSBACKGROUNDITEM_H
#define STUDIOSBACKGROUNDITEM_H

using namespace std;

class StudiosBackgroundItem {
    public:

    int id;
    string name;
    int version = 1;
    string title;
    string subtitle;
    string author;
    SRL<BackgroundThumbnail> thumbnail;
    SRL<BackgroundImage> image;
    int aspectRadio;
    string fit;
    string color;
    int blur;
    string mask;

    StudiosBackgroundItem(){}
    StudiosBackgroundItem(int id, string name, string title, string subtitle, string author,
        SRL<BackgroundThumbnail> thumbnail, SRL<BackgroundImage> image, int aspectRadio, string fit, string color, int blur, string mask): 
        id(id), name(name), title(title), subtitle(subtitle), author(author),
        thumbnail(thumbnail), image(image), aspectRadio(aspectRadio), fit(fit), color(color), blur(blur), mask(mask){}
    StudiosBackgroundItem(int id, Json::Value arr){
        this->id = id;
        name = arr["name"].asString();
        version = arr["version"].asInt();
        title = arr["title"].asString();
        subtitle = arr["subtitle"].asString();
        author = arr["author"].asString();
        thumbnail = SRL<BackgroundThumbnail>(arr["thumbnail"]);
        image = SRL<BackgroundImage>(arr["image"]);
        aspectRadio = arr["aspectRadio"].asInt();
        fit = arr["fit"].asString();
        color = arr["color"].asString();
        blur = arr["blur"].asInt();
        mask = arr["mask"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["version"] = version;
        res["title"] = title;
        res["subtitle"] = subtitle;
        res["author"] = author;
        res["thumbnail"] = thumbnail.toJsonObject();
        res["image"] = image.toJsonObject();
        res["aspectRadio"] = aspectRadio;
        res["fit"] = fit;
        res["color"] = color;
        res["blur"] = blur;
        res["mask"] = mask;
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
        args["image"] = image.url;
        args["aspectRadio"] = to_string(aspectRadio);
        args["fit"] = fit;
        args["color"] = color;
        args["blur"] = to_string(blur);
        args["mask"] = mask;
        args["url"] = "/studios/backgrounds/" + name;
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/studios_backgrounds.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
    }
};

int studios_backgroundNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Studios_Background";
    if (filter != "") sql += " WHERE (" + filter + ")";
    dbres res = (new DB_Controller)->query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

Section<StudiosBackgroundItem> studios_backgroundList(string filter, int st = 1, int en = 20) {
    // 获取数据条数
    int pageCount = ceil(1.0 * studios_backgroundNumber(filter) / 20);

    // 获取数据
    string sql = "SELECT * FROM Studios_Background";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id DESC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    dbres res = (new DB_Controller)->query(sql.c_str());
    Section<StudiosBackgroundItem> list = Section<StudiosBackgroundItem>(pageCount, BackgroundSearch);
    for (int i = 0; i < res.size(); i++) {
        StudiosBackgroundItem data = StudiosBackgroundItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            SRL<BackgroundThumbnail>(res[i]["thumbnail"], "/data/" + res[i]["thumbnail"]),
            SRL<BackgroundImage>(res[i]["image"], "/data/" + res[i]["image"]),
            atoi(res[i]["aspectRadio"].c_str()),
            res[i]["fit"],
            res[i]["color"],
            atoi(res[i]["blur"].c_str()),
            res[i]["mask"]
        ); list.append(data);
    } return list;
}

string studios_backgroundFilter(argvar arg) {
    string filter = "";
    if (arg["keywords"] != "") filter = "title like \"%" + str_replace("\"", "\\\"", urldecode(arg["keywords"])) + "%\"";
    return filter;
}

int studios_backgroundCreate(StudiosBackgroundItem item) {
    stringstream sqlbuffer;
    auto res = (new DB_Controller)->query("SELECT id FROM Studios_Background WHERE name = \"" + item.name + "\"");
    if (res.size() != 0) return 0;
    int id = atoi((new DB_Controller)->query("SELECT COUNT(*) AS sum FROM Studios_Background;")[0]["sum"].c_str()) + 1;
    sqlbuffer << "INSERT INTO Studios_Background (id, name, version, title, subtitle, author, thumbnail, image, aspectRadio, fit, color, blur, mask) VALUES (";
    sqlbuffer << id << ", \"" << item.name << "\", " << item.version << ", \"" << item.title << "\", ";
    sqlbuffer << "\"" << item.subtitle << "\", \"" << item.author << "\", \"" << item.thumbnail.hash << "\", ";
    sqlbuffer << "\"" << item.image.hash << "\", " << item.aspectRadio << ", \"" << item.fit << "\", \"" << item.color << "\", ";
    sqlbuffer << item.blur << ", \"" << item.mask <<"\")";
    return (new DB_Controller)->execute(sqlbuffer.str());
}

#endif