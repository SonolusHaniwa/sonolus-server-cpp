#pragma once

class RemoveScoreboardSectionScoreEvent {
    public:

    string type = "removeScoreboardSectionScore";
    int sectionIndex;
    int index;

    RemoveScoreboardSectionScoreEvent(){}
    RemoveScoreboardSectionScoreEvent(int sectionIndex, int index): sectionIndex(sectionIndex), index(index){}
    RemoveScoreboardSectionScoreEvent(Json::Value obj) {
        sectionIndex = obj["sectionIndex"].asInt();
        index = obj["index"].asInt();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["sectionIndex"] = sectionIndex;
        res["index"] = index;
        return res;
    }
};