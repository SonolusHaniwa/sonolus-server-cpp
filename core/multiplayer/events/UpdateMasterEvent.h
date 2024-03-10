#pragma once

class UpdateMasterEvent {
    public:

    string type = "updateMaster";
    string master;

    UpdateMasterEvent(){}
    UpdateMasterEvent(string master): master(master){}
    UpdateMasterEvent(Json::Value obj) {
        master = obj["master"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["master"] = master;
        return res;
    }
};