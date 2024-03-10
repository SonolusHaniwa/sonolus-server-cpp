#pragma once

class UpdateIsSuggestionsLockedCommand {
    public:

    string type = "updateIsSuggestionsLocked";
    bool isSuggestionsLocked;

    UpdateIsSuggestionsLockedCommand(){}
    UpdateIsSuggestionsLockedCommand(bool isSuggestionsLocked): isSuggestionsLocked(isSuggestionsLocked){}
    UpdateIsSuggestionsLockedCommand(Json::Value obj) {
        isSuggestionsLocked = obj["isSuggestionsLocked"].asBool();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["isSuggestionsLocked"] = isSuggestionsLocked;
        return res;
    }
};