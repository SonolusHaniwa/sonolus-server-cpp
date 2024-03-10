#pragma once

class UpdateOptionValuesEvent {
    public:

    string type = "updateOptionValues";
    string optionValues;

    UpdateOptionValuesEvent(){}
    UpdateOptionValuesEvent(string optionValues): optionValues(optionValues){}
    UpdateOptionValuesEvent(Json::Value obj) {
        optionValues = obj["optionValues"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["optionValues"] = optionValues;
        return res;
    }
};