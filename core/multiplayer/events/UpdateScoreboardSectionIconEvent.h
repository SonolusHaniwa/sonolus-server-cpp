#pragma once

class UpdateScoreboardSectionIconEvent {
    public:

    string type = "updateScoreboardSectionIcon";
    int index;
    string icon;

    UpdateScoreboardSectionIconEvent(){}
    UpdateScoreboardSectionIconEvent(int index, string icon): index(index), icon(icon){}
    UpdateScoreboardSectionIconEvent(Json::Value obj) {
        index = obj["index"].asInt();
        icon = obj["icon"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["index"] = index;
        res["icon"] = icon;
        return res;
    }
};