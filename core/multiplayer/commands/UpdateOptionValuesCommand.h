#pragma once

class UpdateOptionValuesCommand {
    public:

    string type = "updateOptionValues";
    string optionValues;

    UpdateOptionValuesCommand(){}
    UpdateOptionValuesCommand(string optionValues): optionValues(optionValues){}
    UpdateOptionValuesCommand(Json::Value obj) {
        optionValues = obj["optionValues"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["optionValues"] = optionValues;
        return res;
    }
};