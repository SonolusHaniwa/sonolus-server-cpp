#pragma once

class UpdateEvent {
	public:

	string type = "update";
	bool allowOtherServers;
	vector<Search> reportUserOptions;
	string title;
	string status = RoomStatus.selecting;
	string master = "room";
	string lead = "room";
	vector<Search> options;
	string optionValues;
	LevelLocator level;
	vector<LevelOptionEntry> levelOptions;
	string autoExit = AutoExit.off;
	bool isSuggestionLocked;
	vector<Suggestion> suggestions;
	string scoreboardDescription;
	vector<ScoreboardSection> scoreboardSections;
	vector<ResultEntry> results;
	vector<RoomUser> users;
	vector<UserStatusEntry> userStatuses;

	UpdateEvent(){}
	UpdateEvent(
		bool allowOtherServers,
		vector<Search> reportUserOptions,
		string title,
		string status,
		string master,
		string lead,
		vector<Search> options,
		string optionValues,
		LevelLocator level,
		vector<LevelOptionEntry> levelOptions,
		string autoExit,
		bool isSuggestionLocked,
		vector<Suggestion> suggestions,
		string scoreboardDescription,
		vector<ScoreboardSection> scoreboardSections,
		vector<ResultEntry> results,
		vector<RoomUser> users,
		vector<UserStatusEntry> userStatuses
	): allowOtherServers(allowOtherServers), reportUserOptions(reportUserOptions), title(title),
		status(status), master(master), lead(lead), options(options), optionValues(optionValues),
		level(level), levelOptions(levelOptions), autoExit(autoExit), isSuggestionLocked(isSuggestionLocked),
		suggestions(suggestions), scoreboardDescription(scoreboardDescription), scoreboardSections(scoreboardSections),
		results(results), users(users), userStatuses(userStatuses){};
	UpdateEvent(Json::Value obj) {
		allowOtherServers = obj["allowOtherServers"].asBool();
		reportUserOptions = constructSearch(obj["reportUserOptions"]);
		title = obj["title"].asString();
		status = obj["status"].asString();
		master = obj["master"].asString();
		lead = obj["lead"].asString();
		options = constructSearch(obj["options"]);
		optionValues = obj["optionValues"].asString();
		level = LevelLocator(obj["level"]);
		for (int i = 0; i < obj["levelOptions"].size(); i++) levelOptions.push_back(LevelOptionEntry(obj["levelOptions"][i]));
		autoExit = obj["autoExit"].asString();
		isSuggestionLocked = obj["isSuggestionLocked"].asBool();
		for (int i = 0; i < obj["suggestions"].size(); i++) suggestions.push_back(Suggestion(obj["suggestions"][i]));
		scoreboardDescription = obj["scoreboardDescription"].asString();
		for (int i = 0; i < obj["scoreboardSections"].size(); i++) scoreboardSections.push_back(ScoreboardSection(obj["scoreboardSections"][i]));
		for (int i = 0; i < obj["results"].size(); i++) results.push_back(ResultEntry(obj["results"][i]));
		for (int i = 0; i < obj["users"].size(); i++) users.push_back(RoomUser(obj["users"][i]));
		for (int i = 0; i < obj["userStatuses"].size(); i++) userStatuses.push_back(UserStatusEntry(obj["userStatuses"][i]));
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["type"] = type;
		res["allowOtherServers"] = allowOtherServers;
		res["reportUserOptions"].resize(0);
		for (int i = 0; i < reportUserOptions.size(); i++) res["reportUserOptions"].append(reportUserOptions[i].toJsonObject());
		res["title"] = title;
		res["status"] = status;
		res["master"] = master;
		res["lead"] = lead;
		res["options"].resize(0);
		for (int i = 0; i < options.size(); i++) res["options"].append(options[i].toJsonObject());
		res["optionValues"] = optionValues;
		res["level"] = level.toJsonObject();
		res["levelOptions"].resize(0);
		for (int i = 0; i < levelOptions.size(); i++) res["levelOptions"].append(levelOptions[i].toJsonObject());
		res["autoExit"] = autoExit;
		res["isSuggestionLocked"] = isSuggestionLocked;
		res["suggestions"].resize(0);
		for (int i = 0; i < suggestions.size(); i++) res["suggestions"].append(suggestions[i].toJsonObject());
		res["scoreboardDescription"] = scoreboardDescription;
		res["scoreboardSections"].resize(0);
		for (int i = 0; i < scoreboardSections.size(); i++) res["scoreboardSections"].append(scoreboardSections[i].toJsonObject());
		res["results"].resize(0);
		for (int i = 0; i < results.size(); i++) res["results"].append(results[i].toJsonObject());
		res["users"].resize(0);
		for (int i = 0; i < users.size(); i++) res["users"].append(users[i].toJsonObject());
		res["userStatuses"].resize(0);
		for (int i = 0; i < userStatuses.size(); i++) res["userStatuses"].append(userStatuses[i].toJsonObject());
		return res;
	}
};