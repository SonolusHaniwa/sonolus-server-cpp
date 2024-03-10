#pragma once

class UpdateScoreboardSectionScoresEvent {
    public:

    string type = "updateScoreboardSectionScores";
    int index;
    vector<ScoreEntry> scores;

    UpdateScoreboardSectionScoresEvent(){}
    UpdateScoreboardSectionScoresEvent(int index, vector<ScoreEntry> scores): index(index), scores(scores){}
    UpdateScoreboardSectionScoresEvent(Json::Value obj) {
        index = obj["index"].asInt();
        for (int i = 0; i < obj["scores"].size(); i++) scores.push_back(ScoreEntry(obj["scores"][i]));
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["index"] = index;
        res["scores"].resize(0);
        for (int i = 0; i < scores.size(); i++) res["scores"].append(scores[i].toJsonObject());
        return res;
    }
};