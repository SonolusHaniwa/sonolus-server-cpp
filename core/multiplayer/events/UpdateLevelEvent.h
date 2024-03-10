#pragma once

class UpdateLevelEvent {
    public:

    string type = "updateLevel";
    LevelLocator level;

    UpdateLevelEvent(){}
    UpdateLevelEvent(LevelLocator level): level(level){}
    UpdateLevelEvent(Json::Value obj) {
        level = LevelLocator(obj["level"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["level"] = level.toJsonObject();
        return res;
    }
};