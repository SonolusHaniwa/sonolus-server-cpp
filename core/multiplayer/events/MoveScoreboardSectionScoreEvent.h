#pragma once

class MoveScoreboardSectionScoreEvent {
    public:

    string type = "moveScoreboardSectionScore";
    int sectionIndex;
    int fromIndex;
    int toIndex;

    MoveScoreboardSectionScoreEvent(){}
    MoveScoreboardSectionScoreEvent(int sectionIndex, int fromIndex, int toIndex): sectionIndex(sectionIndex), fromIndex(fromIndex), toIndex(toIndex){}
    MoveScoreboardSectionScoreEvent(Json::Value obj) {
        sectionIndex = obj["sectionIndex"].asInt();
        fromIndex = obj["fromIndex"].asInt();
        toIndex = obj["toIndex"].asInt();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["sectionIndex"] = sectionIndex;
        res["fromIndex"] = fromIndex;
        res["toIndex"] = toIndex;
        return res;
    }
};