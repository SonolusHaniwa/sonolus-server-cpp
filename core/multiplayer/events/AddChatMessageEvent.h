#pragma once

class AddChatMessageEvent {
    public:

    string type = "addChatMessage";
    double nonce;
    ChatMessage message;

    AddChatMessageEvent(){}
    AddChatMessageEvent(double nonce, ChatMessage message): nonce(nonce), message(message){}
    AddChatMessageEvent(Json::Value obj) {
        nonce = obj["nonce"].asDouble();
        message = ChatMessage(obj["message"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["nonce"] = nonce;
        res["message"] = message.toJsonObject();
        return res;
    }
};