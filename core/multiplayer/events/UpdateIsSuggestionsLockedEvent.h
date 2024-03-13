#pragma once

class UpdateIsSuggestionsLockedEvent {
    public:

    string type = "updateIsSuggestionsLocked";
    bool isSuggestionsLocked;

    UpdateIsSuggestionsLockedEvent(){}
    UpdateIsSuggestionsLockedEvent(bool isSuggestionsLocked): isSuggestionsLocked(isSuggestionsLocked){}
    UpdateIsSuggestionsLockedEvent(Json::Value obj) {
        isSuggestionsLocked = obj["isSuggestionsLocked"].asBool();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["isSuggestionsLocked"] = isSuggestionsLocked;
        return res;
    }
};