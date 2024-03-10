#pragma once

class AddSuggestionEvent {
    public:

    string type = "addSuggestion";
    Suggestion suggestion;

    AddSuggestionEvent(){}
    AddSuggestionEvent(Suggestion suggestion): suggestion(suggestion){}
    AddSuggestionEvent(Json::Value obj) {
        suggestion = Suggestion(obj["suggestion"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["suggestion"] = suggestion.toJsonObject();
        return res;
    }
};