#pragma once

class StartGameplayCommand {
    public:

    string type = "startGameplay";
    string state;
    LevelItem level;

    StartGameplayCommand(){}
    StartGameplayCommand(string state, LevelItem level): state(state), level(level){}
    StartGameplayCommand(Json::Value obj) {
        state = obj["state"].asString();
        level = LevelItem(-1, obj["level"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["state"] = state;
        res["level"] = level.toJsonObject();
        return res;
    }
};