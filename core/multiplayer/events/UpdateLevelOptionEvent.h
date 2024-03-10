#pragma once

class UpdateLevelOptionEvent {
    public:

    string type = "updateLevelOption";
    LevelOptionEntry levelOption;

    UpdateLevelOptionEvent(){}
    UpdateLevelOptionEvent(LevelOptionEntry levelOption): levelOption(levelOption){}
    UpdateLevelOptionEvent(Json::Value obj) {
        levelOption = LevelOptionEntry(obj["levelOption"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["levelOption"] = levelOption.toJsonObject();
        return res;
    }
};