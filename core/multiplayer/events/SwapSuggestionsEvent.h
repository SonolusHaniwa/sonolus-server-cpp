#pragma once

class SwapSuggestionsEvent {
    public:

    string type = "swapSuggestions";
    Suggestion suggestionA;
    Suggestion suggestionB;

    SwapSuggestionsEvent(){}
    SwapSuggestionsEvent(Suggestion suggestionA, Suggestion suggestionB): suggestionA(suggestionA), suggestionB(suggestionB){}
    SwapSuggestionsEvent(Json::Value obj) {
        suggestionA = Suggestion(obj["suggestionA"]);
        suggestionB = Suggestion(obj["suggestionB"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["suggestionA"] = suggestionA.toJsonObject();
        res["suggestionB"] = suggestionB.toJsonObject();
        return res;
    }
};