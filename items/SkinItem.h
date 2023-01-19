#ifndef SKINITEM_H
#define SKINITEM_H

using namespace std;

class SkinItem {
    public:

    int id;
    string name;
    int version = 2;
    string title;
    string subtitle;
    string author;
    SRL<SkinThumbnail> thumbnail;
    SRL<SkinData> data;
    SRL<SkinTexture> texture;

    SkinItem(){}
    SkinItem(int id, string name, string title, string subtitle, string author, 
        SRL<SkinThumbnail> thumbnail, SRL<SkinData> data, SRL<SkinTexture> texture):
        id(id), name(name), title(title), subtitle(subtitle), author(author), thumbnail(thumbnail), data(data), texture(texture){}

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
        return res;
    }
};

Section<SkinItem> skinList(string filter, int st = 1, int en = 20) {
    // 获取数据条数
    string sql = "SELECT COUNT(*) AS sum FROM Skin";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id ASC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    mysqld res = mysqli_query(mysql, sql.c_str());
    int pageCount = atoi(res[0]["sum"].c_str()) / 20;

    // 获取数据
    sql = "SELECT * FROM Skin";
    if (filter != "") sql += " WHERE (" + filter + ")";
    sql += " ORDER BY id ASC LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);
    res = mysqli_query(mysql, sql.c_str());
    Section<SkinItem> list = Section<SkinItem>(pageCount, SkinSearch);
    for (int i = 0; i < res.size(); i++) {
        SkinItem data = SkinItem(
            atoi(res[i]["id"].c_str()),
            res[i]["name"],
            res[i]["title"],
            res[i]["subtitle"],
            res[i]["author"],
            SRL<SkinThumbnail>(res[i]["thumbnail"], "/data/" + res[i]["thumbnail"]),
            SRL<SkinData>(res[i]["data"], "/data/" + res[i]["data"]),
            SRL<SkinTexture>(res[i]["texture"], "/data/" + res[i]["texture"])
        ); list.append(data);
    } return list;
}

#endif