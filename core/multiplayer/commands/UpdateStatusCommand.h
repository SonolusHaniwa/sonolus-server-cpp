#pragma once

class UpdateStatusCommand {
    public:

    string type = "updateStatus";
    string status;

    UpdateStatusCommand(){}
    UpdateStatusCommand(string status): status(status){}
    UpdateStatusCommand(Json::Value obj) {
        status = obj["status"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["status"] = status;
        return res;
    }
};