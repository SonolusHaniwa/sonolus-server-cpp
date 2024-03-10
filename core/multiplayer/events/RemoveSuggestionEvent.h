#pragma once

class RemoveSuggestionEvent {
    public:

    string type = "removeSuggestion";
    Suggestion suggestion;

    RemoveSuggestionEvent(){}
    RemoveSuggestionEvent(Suggestion suggestion): suggestion(suggestion){}
    RemoveSuggestionEvent(Json::Value obj) {
        suggestion = Suggestion(obj["suggestion"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["suggestion"] = suggestion.toJsonObject();
        return res;
    }
};