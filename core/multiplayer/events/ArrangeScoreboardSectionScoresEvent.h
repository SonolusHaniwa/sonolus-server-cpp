#pragma once

class ArrangeScoreboardSectionScoresEvent {
    public:

    string type = "arrangeScoreboardSectionScores";
    int sectionIndex;
    vector<int> indexes;

    ArrangeScoreboardSectionScoresEvent(){}
    ArrangeScoreboardSectionScoresEvent(int sectionIndex, vector<int> indexes): sectionIndex(sectionIndex), indexes(indexes){}
    ArrangeScoreboardSectionScoresEvent(Json::Value obj) {
        sectionIndex = obj["sectionIndex"].asInt();
        for (int i = 0; i < obj["indexes"].size(); i++) indexes.push_back(obj["indexes"][i].asInt());
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["sectionIndex"] = sectionIndex;
        res["indexes"].resize(0);
        for (int i = 0; i < indexes.size(); i++) res["indexes"].append(indexes[i]);
        return res;
    }
};