#pragma once

class LevelLocator {
	public:

	string address;
	string name;

	LevelLocator(){}
	LevelLocator(string address, string name): address(address), name(name){}
	LevelLocator(Json::Value obj) {
		address = obj["address"].asString();
		name = obj["name"].asString();
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["address"] = address;
		res["name"] = name;
		return res;
	}
};