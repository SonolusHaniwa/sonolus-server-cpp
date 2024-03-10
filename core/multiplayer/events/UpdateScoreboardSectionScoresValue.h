#pragma once

class UpdateScoreboardSectionScoresValue {
    public:

    string type = "updateScoreboardSectionScoresValue";
    int index;
    string values;

    UpdateScoreboardSectionScoresValue(){}
    UpdateScoreboardSectionScoresValue(int index, string values): index(index), values(values){}
    UpdateScoreboardSectionScoresValue(Json::Value obj) {
        index = obj["index"].asInt();
        values = obj["values"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["index"] = index;
        res["values"] = values;
        return res;
    }
};