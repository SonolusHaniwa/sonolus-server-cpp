#pragma once

class UpdateLeadEvent {
    public:

    string type = "updateLead";
    string lead;

    UpdateLeadEvent(){}
    UpdateLeadEvent(string lead): lead(lead){}
    UpdateLeadEvent(Json::Value obj) {
        lead = obj["lead"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["lead"] = lead;
        return res;
    } 
};