#pragma once

class UpdateIsSuggestionsLockedEvent {
    public:

    string type = "updateIsSuggestionLocked";
    bool isSuggestionLocked;

    UpdateIsSuggestionsLockedEvent(){}
    UpdateIsSuggestionsLockedEvent(bool isSuggestionLocked): isSuggestionLocked(isSuggestionLocked){}
    UpdateIsSuggestionsLockedEvent(Json::Value obj) {
        isSuggestionLocked = obj["isSuggestionLocked"].asBool();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["isSuggestionLocked"] = isSuggestionLocked;
        return res;
    }
};