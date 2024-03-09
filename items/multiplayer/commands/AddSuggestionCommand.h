#pragma once

class AddSuggestionCommand {
    public:

    string type = "addSuggestion";
    LevelLocator level;

    AddSuggestionCommand(){}
    AddSuggestionCommand(LevelLocator level): level(level){}
    AddSuggestionCommand(Json::Value obj) {
        level = LevelLocator(obj["level"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["level"] = level.toJsonObject();
        return res;
    }
};