#pragma once

class UpdateUserStatusEvent {
    public:

    string type = "updateUserStatus";
    UserStatusEntry userStatus;

    UpdateUserStatusEvent(){}
    UpdateUserStatusEvent(UserStatusEntry userStatus): userStatus(userStatus){}
    UpdateUserStatusEvent(Json::Value obj) {
        userStatus = UserStatusEntry(obj["userStatus"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["userStatus"] = userStatus.toJsonObject();
        return res;
    }
};