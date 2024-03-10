#pragma once

class InsertScoreboardSectionEvent {
    public:

    string type = "insertScoreboardSection";
    int index;
    ScoreboardSection scoreboardSection;

    InsertScoreboardSectionEvent(){}
    InsertScoreboardSectionEvent(int index, ScoreboardSection scoreboardSection): index(index), scoreboardSection(scoreboardSection){}
    InsertScoreboardSectionEvent(Json::Value obj) {
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