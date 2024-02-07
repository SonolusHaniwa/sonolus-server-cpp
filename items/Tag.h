#pragma once

class Tag {
    public:

    string title;
    string icon;

    Tag(){}
    Tag(string title, string icon = ""): title(title), icon(icon){}
    Tag(Json::Value arr) {
        title = arr["title"].asString();
        icon = arr["icon"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["title"] = title;
        res["icon"] = icon;
        return res;
    }
};