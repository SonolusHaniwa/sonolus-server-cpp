#pragma once

class SwapScoreboardSectionsEvent {
    public:

    string type = "swapScoreboardSections";
    int indexA;
    int indexB;

    SwapScoreboardSectionsEvent(){}
    SwapScoreboardSectionsEvent(int indexA, int indexB): indexA(indexA), indexB(indexB){}
    SwapScoreboardSectionsEvent(Json::Value obj) {
        indexA = obj["indexA"].asInt();
        indexB = obj["indexB"].asInt();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["indexA"] = indexA;
        res["indexB"] = indexB;
        return res;
    }
};