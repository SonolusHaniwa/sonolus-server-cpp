#pragma once

class GameplayResultGrade {
	public:

	string allPerfect = "allPerfect";
	string fullCombo = "fullCombo";
	string pass = "pass";
	string fail	= "fail";
}GameplayResultGrade;

class GameplayResult {
	public:

	string grade;
	int arcadeScore;
	int accuracyScore;
	int combo;
	int perfect;
	int great;
	int good;
	int miss;
	int totalCount;

	GameplayResult(){}
	GameplayResult(string grade, int arcadeScore, int accuracyScore, int combo,
		int perfect, int great, int good, int miss, int totalCount):
		grade(grade), arcadeScore(arcadeScore), accuracyScore(accuracyScore), combo(combo),
		perfect(perfect), great(great), good(good), miss(miss), totalCount(totalCount){}
	GameplayResult(Json::Value obj) {
		grade = obj["grade"].asString();
		arcadeScore = obj["arcadeScore"].asInt();
		accuracyScore = obj["accuracyScore"].asInt();
		combo = obj["combo"].asInt();
		perfect = obj["perfect"].asInt();
		great = obj["great"].asInt();
		good = obj["good"].asInt();
		miss = obj["miss"].asInt();
		totalCount = obj["totalCount"].asInt();
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["grade"] = grade;
		res["arcadeScore"] = arcadeScore;
		res["accuracyScore"] = accuracyScore;
		res["combo"] = combo;
		res["perfect"] = perfect;
		res["great"] = great;
		res["good"] = good;
		res["miss"] = miss;
		res["totalCount"] = totalCount;
		return res;
	}
};