#pragma once

class UserStatusEntry {
	public:

	string userId;
	string status;

	UserStatusEntry(){}
	UserStatusEntry(string userId, string status): userId(userId), status(status){}
	UserStatusEntry(Json::Value obj) {
		userId = obj["userId"].asString();
		status = obj["status"].asString();
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["userId"] = userId;
		res["status"] = status;
		return res;
	}
};