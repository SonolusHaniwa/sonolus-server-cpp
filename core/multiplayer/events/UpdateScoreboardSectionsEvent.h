#pragma once

class UpdateScoreboardSectionsEvent {
    public:

    string type = "updateScoreboardSections";
    vector<ScoreboardSection> scoreboardSections;

    UpdateScoreboardSectionsEvent(){}
    UpdateScoreboardSectionsEvent(vector<ScoreboardSection> scoreboardSections): scoreboardSections(scoreboardSections){}
    UpdateScoreboardSectionsEvent(Json::Value obj) {
        for (int i = 0; i < obj["scoreboardSections"].size(); i++) {
            scoreboardSections.push_back(ScoreboardSection(obj["scoreboardSections"][i]));
        }
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["scoreboardSections"].resize(0);
        for (int i = 0; i < scoreboardSections.size(); i++) res["scoreboardSections"].append(scoreboardSections[i].toJsonObject());
        return res;
    }
};