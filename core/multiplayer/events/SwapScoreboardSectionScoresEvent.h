#pragma once

class SwapScoreboardSectionScoresEvent {
    public:

    string type = "swapScoreboardSectionScores";
    int sectionIndex;
    int indexA;
    int indexB;

    SwapScoreboardSectionScoresEvent(){}
    SwapScoreboardSectionScoresEvent(int sectionIndex, int indexA, int indexB): sectionIndex(sectionIndex), indexA(indexA), indexB(indexB){}
    SwapScoreboardSectionScoresEvent(Json::Value obj) {
        sectionIndex = obj["sectionIndex"].asInt();
        indexA = obj["indexA"].asInt();
        indexB = obj["indexB"].asInt();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["sectionIndex"] = sectionIndex;
        res["indexA"] = indexA;
        res["indexB"] = indexB;
        return res;
    }
};