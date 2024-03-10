#pragma once

class UpdateUserStatusesEvent {
    public:

    string type = "updateUserStatuses";
    vector<UserStatusEntry> userStatuses;

    UpdateUserStatusesEvent(){}
    UpdateUserStatusesEvent(vector<UserStatusEntry> userStatuses): userStatuses(userStatuses){}
    UpdateUserStatusesEvent(Json::Value obj) {
        for (int i = 0; i < obj["userStatuses"].size(); i++) {
            userStatuses.push_back(UserStatusEntry(obj["userStatuses"][i]));
        }
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["userStatuses"].resize(0);
        for (int i = 0; i < userStatuses.size(); i++) res["userStatuses"].append(userStatuses[i].toJsonObject());
        return res;
    }
};