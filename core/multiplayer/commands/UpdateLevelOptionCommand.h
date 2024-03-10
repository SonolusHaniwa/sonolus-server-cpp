#pragma once

class UpdateLevelOptionCommand {
    public:

    string type = "updateLevelOption";
    LevelOptionEntry levelOption;

    UpdateLevelOptionCommand(){}
    UpdateLevelOptionCommand(LevelOptionEntry levelOption): levelOption(levelOption){}
    UpdateLevelOptionCommand(Json::Value obj) {
        levelOption = LevelOptionEntry(obj["levelOption"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["levelOption"] = levelOption.toJsonObject();
        return res;
    }
};