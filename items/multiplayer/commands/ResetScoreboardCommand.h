#pragma once

class ResetScoreboardCommand { 
    public:

    string type = "resetScoreboard";

    ResetScoreboardCommand(){}

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        return res;
    }
};