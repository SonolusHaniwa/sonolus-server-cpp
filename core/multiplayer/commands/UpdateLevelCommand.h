#pragma once

class UpdateLevelCommand {
    public:

    string type = "updateLevel";
    LevelLocator level;

    UpdateLevelCommand(){}
    UpdateLevelCommand(LevelLocator level): level(level){}
    UpdateLevelCommand(Json::Value obj) {
        level = LevelLocator(obj["level"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["level"] = level.toJsonObject();
        return res;
    }
};