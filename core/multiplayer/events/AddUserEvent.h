#pragma once

class AddUserEvent {
    public:

    string type = "addUser";
    RoomUser user;
    AddUserEvent(){}
    AddUserEvent(RoomUser user): user(user){}
    AddUserEvent(Json::Value obj) {
        user = RoomUser(obj["user"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["user"] = user.toJsonObject();
        return res;
    }
};