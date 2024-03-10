#pragma once

class LevelOptionEntry {
	public:

	int index;
	int value;

	LevelOptionEntry(){}
	LevelOptionEntry(int index, int value): index(index), value(value){}
	LevelOptionEntry(Json::Value obj) {
		index = obj["index"].asInt();
		value = obj["value"].asInt();
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["index"] = index;
		res["value"] = value;
		return res;
	}
};