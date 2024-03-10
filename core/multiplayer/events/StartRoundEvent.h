#pragma once

class StartRoundEvent {
    public:

    string type = "startRound";
    string state;
    double seed;

    StartRoundEvent(){}
    StartRoundEvent(string state, double seed): state(state), seed(seed){}
    StartRoundEvent(Json::Value obj) {
        state = obj["state"].asString();
        seed = obj["seed"].asDouble();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["state"] = state;
        res["seed"] = seed;
        return res;
    }
};