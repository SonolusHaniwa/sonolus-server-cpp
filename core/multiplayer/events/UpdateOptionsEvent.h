#pragma once

class UpdateOptionsEvent {
    public:

    string type = "updateOptions";
    vector<Search> options;
    string optionValues;

    UpdateOptionsEvent(){}
    UpdateOptionsEvent(vector<Search> options, string optionValues): options(options), optionValues(optionValues){}
    UpdateOptionsEvent(Json::Value obj) {
        options = constructSearch(obj["options"]["options"]);
        optionValues = obj["optionValues"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["options"].resize(0);
        for (int i = 0; i < options.size(); i++) res["options"].append(options[i].toJsonObject());
        res["optionValues"] = optionValues;
        return res;
    }
};