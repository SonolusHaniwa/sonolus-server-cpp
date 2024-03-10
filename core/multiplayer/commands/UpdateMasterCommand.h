#pragma once

class UpdateMasterCommand {
    public:

    string type = "updateMaster";
    string master;

    UpdateMasterCommand(){}
    UpdateMasterCommand(string master): master(master){}
    UpdateMasterCommand(Json::Value obj) {
        master = obj["master"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["master"] = master;
        return res;
    }
};