#pragma once

class UpdateScoreboardSectionTitleEvent {
    public:

    string type = "updateScoreboardSectionTitle";
    int index;
    string title;

    UpdateScoreboardSectionTitleEvent(){}
    UpdateScoreboardSectionTitleEvent(int index, string title): index(index), title(title){}
    UpdateScoreboardSectionTitleEvent(Json::Value obj) {
        index = obj["index"].asInt();
        title = obj["title"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["index"] = index;
        res["title"] = title;
        return res;
    }
};