#pragma once

class RemoveScoreboardSectionEvent {
    public:

    string type = "removeScoreboardSection";
    int index;

    RemoveScoreboardSectionEvent(){}
    RemoveScoreboardSectionEvent(int index): index(index){}
    RemoveScoreboardSectionEvent(Json::Value obj) {
        index = obj["index"].asInt();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["index"] = index;
        return res;
    }
};