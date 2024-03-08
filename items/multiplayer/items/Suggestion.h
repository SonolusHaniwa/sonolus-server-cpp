#pragma once

class Suggestion {
	public:

	string userId;
	LevelLocator level;

	Suggestion(){}
	Suggestion(string userId, LevelLocator level): userId(userId), level(level){}
	Suggestion(Json::Value obj) {
		userId = obj["userId"].asString();
		level = LevelLocator(obj["level"]);
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["userId"] = userId;
		res["level"] = level.toJsonObject();
		return res;
	}
};