#pragma once

class UpdateAutoExitCommand {
    public:

    string type = "updateAutoExit";
    string AutoExit;

    UpdateAutoExitCommand(){}
    UpdateAutoExitCommand(string AutoExit): AutoExit(AutoExit){}
    UpdateAutoExitCommand(Json::Value obj) {
        AutoExit = obj["AutoExit"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["AutoExit"] = AutoExit;
        return res;
    }
};