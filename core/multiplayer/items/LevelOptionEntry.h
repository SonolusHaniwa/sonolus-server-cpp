#pragma once

class LevelOptionEntry {
	public:

	int index;
	double value;

	LevelOptionEntry(){}
	LevelOptionEntry(int index, double value): index(index), value(value){}
	LevelOptionEntry(Json::Value obj) {
		index = obj["index"].asInt();
		value = obj["value"].asDouble();
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["index"] = index;
		res["value"] = value;
		return res;
	}
};