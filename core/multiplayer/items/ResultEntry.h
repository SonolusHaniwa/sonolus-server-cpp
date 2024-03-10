#pragma once

class ResultEntry {
	public:

	string userId;
	GameplayResult result;

	ResultEntry(){}
	ResultEntry(string userId, GameplayResult result): userId(userId), result(result){}
	ResultEntry(Json::Value obj) {
		userId = obj["userId"].asString();
		result = GameplayResult(obj["result"]);
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["userId"] = userId;
		res["result"] = result.toJsonObject();
		return res;
	}
};