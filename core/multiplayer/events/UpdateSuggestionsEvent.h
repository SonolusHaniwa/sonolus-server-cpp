#pragma once

class UpdateSuggestionsEvent {
    public:

    string type = "updateSuggestions";
    vector<Suggestion> suggestions;

    UpdateSuggestionsEvent(){}
    UpdateSuggestionsEvent(vector<Suggestion> suggestions): suggestions(suggestions){}
    UpdateSuggestionsEvent(Json::Value obj) {
        for (Json::Value suggestion : obj["suggestions"]) {
            suggestions.push_back(Suggestion(suggestion));
        }
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["suggestions"].resize(0);
        for (int i = 0; i < suggestions.size(); i++) res["suggestions"].append(suggestions[i].toJsonObject());
        return res;
    }
};