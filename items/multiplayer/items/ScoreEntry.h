#pragma once

class ScoreEntry {
	public:

	string userId;
	string value;

	ScoreEntry(){}
	ScoreEntry(string userId, string value): userId(userId), value(value){}
	ScoreEntry(Json::Value obj) {
		userId = obj["userId"].asString();
		value = obj["value"].asString();
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["userId"] = userId;
		res["value"] = value;
		return res;
	}
};