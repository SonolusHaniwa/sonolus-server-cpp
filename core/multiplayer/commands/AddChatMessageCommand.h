#pragma once

class AddChatMessageCommand {
    public:

    string type = "addChatMessage";
    double nonce;
    ChatMessage message;
    AddChatMessageCommand(){}
    AddChatMessageCommand(int nonce, ChatMessage message): nonce(nonce), message(message){}
    AddChatMessageCommand(Json::Value obj) {
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