#pragma once

class AddResultEvent {
    public:

    string type = "addResult";
    ResultEntry result;

    AddResultEvent(){}
    AddResultEvent(ResultEntry result): result(result){}
    AddResultEvent(Json::Value obj) {
        result = ResultEntry(obj["result"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["result"] = result.toJsonObject();
        return res;
    }
};