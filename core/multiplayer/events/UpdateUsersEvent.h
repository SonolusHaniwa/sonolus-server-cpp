#pragma once

class UpdateUsersEvent {
    public:

    string type = "updateUsers";
    vector<RoomUser> users;

    UpdateUsersEvent(){}
    UpdateUsersEvent(vector<RoomUser> users): users(users){}
    UpdateUsersEvent(Json::Value obj) {
        for (int i = 0; i < obj["users"].size(); i++) users.push_back(RoomUser(obj["users"][i]));
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["users"].resize(0);
        for (int i = 0; i < users.size(); i++) res["users"].append(users[i].toJsonObject());
        return res;
    }
};