#pragma once

class FinishGameplayCommand {
    public:

    string type = "finishGameplay";
    string state;
    GameplayResult result;

    FinishGameplayCommand(){}
    FinishGameplayCommand(string state, GameplayResult result): state(state), result(result){}
    FinishGameplayCommand(Json::Value obj) {
        state = obj["state"].asString();
        result = GameplayResult(obj["result"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["state"] = state;
        res["result"] = result.toJsonObject();
        return res;
    }
};