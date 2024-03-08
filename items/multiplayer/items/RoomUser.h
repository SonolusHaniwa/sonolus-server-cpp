#pragma once

class RoomUser {
	public:

	string authentication;
	string signature;

	RoomUser(){}
	RoomUser(string authentication, string signature): authentication(authentication), signature(signature){}
	RoomUser(Json::Value obj) {
		authentication = obj["authenticatiin"].asString();
		signature = obj["signature"].asString();
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["authentication"] = authentication;
		res["signature"] = signature;
		return res;
	}
};