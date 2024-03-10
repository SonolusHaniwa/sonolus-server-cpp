#pragma once

class MoveScoreboardSectionEvent {
    public:

    string type = "moveScoreboardSection";
    int fromIndex;
    int toIndex;

    MoveScoreboardSectionEvent(){}
    MoveScoreboardSectionEvent(int fromIndex, int toIndex): fromIndex(fromIndex), toIndex(toIndex){}
    MoveScoreboardSectionEvent(Json::Value obj) {
        fromIndex = obj["fromIndex"].asInt();
        toIndex = obj["toIndex"].asInt();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["fromIndex"] = fromIndex;
        res["toIndex"] = toIndex;
        return res;
    }
};