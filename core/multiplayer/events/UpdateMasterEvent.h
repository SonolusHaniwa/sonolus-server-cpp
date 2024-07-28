#pragma once

class UpdateMasterEvent {
    public:

    string type = "updateMaster";
    string master;

    UpdateMasterEvent(){}
    UpdateMasterEvent(string master): master(master){}
    UpdateMasterEvent(Json::Value obj) {
        master = obj["master"] == Json::nullValue ? roomCode : obj["master"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        if (master == roomCode) res["master"] = Json::nullValue;
        else res["master"] = master;
        return res;
    }
};