#pragma once

class UpdateScoreboardDescriptionEvent {
    public:

    string type = "updateScoreboardDescription";
    string scoreboardDescription;

    UpdateScoreboardDescriptionEvent(){}
    UpdateScoreboardDescriptionEvent(string scoreboardDescription): scoreboardDescription(scoreboardDescription){}
    UpdateScoreboardDescriptionEvent(Json::Value obj) {
        scoreboardDescription = obj["scoreboardDescription"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["scoreboardDescription"] = scoreboardDescription;
        return res;
    }
};