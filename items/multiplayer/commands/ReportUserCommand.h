#pragma once

class ReportUserCommand {
    public:

    string type = "reportUser";
    string userId;
    string optionValues;

    ReportUserCommand(){}
    ReportUserCommand(string userId, string optionValues): userId(userId), optionValues(optionValues){}
    ReportUserCommand(Json::Value obj) {
        userId = obj["userId"].asString();
        optionValues = obj["optionValues"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["userId"] = userId;
        res["optionValues"] = optionValues;
        return res;
    }
};