#pragma once

class RemoveUserEvent {
    public:

    string type = "removeUser";
    RoomUser user;

    RemoveUserEvent(){}
    RemoveUserEvent(RoomUser user): user(user){}
    RemoveUserEvent(Json::Value obj) {
        user = RoomUser(obj["user"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["user"] = user.toJsonObject();
        return res;
    }
};