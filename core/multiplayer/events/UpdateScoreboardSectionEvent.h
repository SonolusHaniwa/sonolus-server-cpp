#pragma once

class UpdateScoreboardSectionEvent {
    public:

    string type = "updateScoreboardSection";
    int index;
    ScoreboardSection scoreboardSection;

    UpdateScoreboardSectionEvent(){}
    UpdateScoreboardSectionEvent(int index, ScoreboardSection scoreboardSection): index(index), scoreboardSection(scoreboardSection){}
    UpdateScoreboardSectionEvent(Json::Value obj) {
        index = obj["index"].asInt();
        scoreboardSection = ScoreboardSection(obj["scoreboardSection"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["index"] = index;
        res["scoreboardSection"] = scoreboardSection.toJsonObject();
        return res;
    }
};