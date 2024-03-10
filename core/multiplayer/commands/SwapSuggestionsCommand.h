#pragma once

class SwapSuggestionsCommand {
    public:

    string type = "swapSuggestions";
    Suggestion suggestionA;
    Suggestion suggestionB;

    SwapSuggestionsCommand(){}
    SwapSuggestionsCommand(Suggestion suggestionA, Suggestion suggestionB): suggestionA(suggestionA), suggestionB(suggestionB){}
    SwapSuggestionsCommand(Json::Value obj) {
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