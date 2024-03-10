#pragma once

class InsertScoreboardSectionScoreEvent {
    public:

    string type = "insertScoreboardSectionScore";
    int sectionIndex;
    int index;
    ScoreEntry score;

    InsertScoreboardSectionScoreEvent(){}
    InsertScoreboardSectionScoreEvent(int sectionIndex, int index, ScoreEntry score): sectionIndex(sectionIndex), index(index), score(score){}
    InsertScoreboardSectionScoreEvent(Json::Value obj) {
        sectionIndex = obj["sectionIndex"].asInt();
        index = obj["index"].asInt();
        score = ScoreEntry(obj["score"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["sectionIndex"] = sectionIndex;
        res["index"] = index;
        res["score"] = score.toJsonObject();
        return res;
    }
};