#pragma once

class ScoreboardSection {
	public:

	string title;
	string icon;
	vector<ScoreEntry> scores;

	ScoreboardSection(){}
	ScoreboardSection(string title, string icon, vector<ScoreEntry> scores): title(title), icon(icon), scores(scores){}
	ScoreboardSection(Json::Value obj) {
		title = obj["title"].asString();
		icon = obj["icon"].asString();
		for (int i = 0; i < obj["scores"].size(); i++) scores.push_back(ScoreEntry(obj["scores"][i]));
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["title"] = title;
		res["icon"] = icon;
		res["scores"].resize(0);
		for (int i = 0; i < scores.size(); i++) res["scores"].append(scores[i].toJsonObject());
		return res;
	}
};