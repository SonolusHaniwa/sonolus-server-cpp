#pragma once

class ClearSuggestionsCommand {
    public:

    string type = "clearSuggestions";

    ClearSuggestionsCommand(){}

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        return res;
    }
};