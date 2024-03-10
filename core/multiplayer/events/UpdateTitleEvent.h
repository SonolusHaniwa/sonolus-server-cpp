#pragma once

class UpdateTitle {
    public:

    string type = "updateTitle";
    string title;

    UpdateTitle(){}
    UpdateTitle(string title): title(title){}
    UpdateTitle(Json::Value obj) {
        title = obj["title"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["title"] = title;
        return res;
    }
};