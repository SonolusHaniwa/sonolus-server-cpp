#pragma once

class UpdateLevelOptionsEvent {
    public:

    string type = "updateLevelOptions";
    vector<LevelOptionEntry> levelOptions;

    UpdateLevelOptionsEvent(){}
    UpdateLevelOptionsEvent(vector<LevelOptionEntry> levelOptions): levelOptions(levelOptions){}
    UpdateLevelOptionsEvent(Json::Value obj) {
        for (int i = 0; i < obj["levelOptions"].size(); i++) {
            levelOptions.push_back(LevelOptionEntry(obj["levelOptions"][i]));
        }
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["levelOptions"].resize(0);
        for (int i = 0; i < levelOptions.size(); i++) res["levelOptions"].append(levelOptions[i].toJsonObject());
        return res;
    }
};