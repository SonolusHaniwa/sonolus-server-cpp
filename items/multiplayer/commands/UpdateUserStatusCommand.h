#pragma once

class UpdateUserStatusCommand {
    public:

    string type = "updateUserStatus";
    string status;

    UpdateUserStatusCommand(){}
    UpdateUserStatusCommand(string status): status(status){}
    UpdateUserStatusCommand(Json::Value obj) {
        status = obj["status"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["status"] = status;
        return res;
    }
};