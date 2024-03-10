#pragma once

class UpdateAutoExitEvent {
    public:

    string type = "updateAutoExit";
    string autoExit;

    UpdateAutoExitEvent(){}
    UpdateAutoExitEvent(string autoExit): autoExit(autoExit){}
    UpdateAutoExitEvent(Json::Value obj) {
        autoExit = obj["autoExit"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["autoExit"] = autoExit;
        return res;
    }
};