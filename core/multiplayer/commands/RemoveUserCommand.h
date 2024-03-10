#pragma once

class RemoveUserCommand {
    public:

    string type = "removeUser";
    string userId;

    RemoveUserCommand(){}
    RemoveUserCommand(string userId): userId(userId){}
    RemoveUserCommand(Json::Value obj) {
        userId = obj["userId"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["userId"] = userId;
        return res;
    }
};