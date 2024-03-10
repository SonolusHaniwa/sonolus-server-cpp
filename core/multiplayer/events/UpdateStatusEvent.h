#pragma once

class UpdateStatusEvent {
    public:

    string type = "updateStatus";
    string status;

    UpdateStatusEvent(){}
    UpdateStatusEvent(string status): status(status){}
    UpdateStatusEvent(Json::Value obj) {
        status = obj["status"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["status"] = status;
        return res;
    }
};