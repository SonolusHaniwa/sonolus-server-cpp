#pragma once

class RemoveSuggestionCommand {
    public:

    string type = "removeSuggestion";
    Suggestion suggestion;

    RemoveSuggestionCommand(){}
    RemoveSuggestionCommand(Suggestion suggestion): suggestion(suggestion){}\
    RemoveSuggestionCommand(Json::Value obj) {
        suggestion = Suggestion(obj["suggestion"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["suggestion"] = suggestion.toJsonObject();
        return res;
    }
};