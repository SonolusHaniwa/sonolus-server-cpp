#pragma once

class UpdateAutoExitCommand {
    public:

    string type = "updateAutoExit";
    string autoExit;

    UpdateAutoExitCommand(){}
    UpdateAutoExitCommand(string autoExit): autoExit(autoExit){}
    UpdateAutoExitCommand(Json::Value obj) {
        autoExit = obj["autoExit"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["autoExit"] = autoExit;
        return res;
    }
};