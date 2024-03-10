#pragma once

class ClearSuggestionsEvent {
    public:

    string type = "clearSuggestions";

    ClearSuggestionsEvent(){}

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        return res;
    }
};