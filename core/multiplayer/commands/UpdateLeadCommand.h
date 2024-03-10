#pragma once

class UpdateLeadCommand {
    public:

    string type = "updateLead";
    string lead;

    UpdateLeadCommand(){}
    UpdateLeadCommand(string lead): lead(lead){}
    UpdateLeadCommand(Json::Value obj) {
        lead = obj["lead"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["lead"] = lead;
        return res;
    }
};