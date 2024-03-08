#pragma once

class QuickChatMessageValue {
	public:
	
	string hello = "hello";	
	string glhf = "glhf";
	string gg = "gg";
	string ns = "ns";
	string ty = "ty";
}QuickChatMessageValue;

class QuickChatMessage {
	public:

	string userId;
	string type = "quick";
	string value;

	QuickChatMessage(){}
	QuickChatMessage(string userId, string value): userId(userId), value(value){}
	QuickChatMessage(Json::Value obj) {
		userId = obj["userId"].asString();
		value = obj["value"].asString();
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["userId"] = userId;
		res["type"] = type;
		res["value"] = value;
		return res;
	}
};

class TextChatMessage {
	public:

	string userId;
	string type = "text";
	string value;

	TextChatMessage(){}
	TextChatMessage(string userId, string value): userId(userId), value(value){}
	TextChatMessage(Json::Value obj) {
		userId = obj["userId"].asString();
		value = obj["value"].asString();
	}

	Json::Value toJsonObject() {
		Json::Value res;
		res["userId"] = userId;
		res["type"] = type;
		res["value"] = value;
		return res;
	}
};

class ChatMessage {
	public:

	string type = "text";
	QuickChatMessage quick;
	TextChatMessage text;
	ChatMessage(){}
	ChatMessage(QuickChatMessage quick): quick(quick), type("quick"){}
	ChatMessage(TextChatMessage text): text(text), type("type"){}
	ChatMessage(Json::Value obj) {
		type = obj["type"].asString();
		if (type == "quick") quick = QuickChatMessage(obj);
		if (type == "text") text = TextChatMessage(obj);
	}

	Json::Value toJsonObject() {
		if (type == "quick") return quick.toJsonObject();
		if (type == "text") return text.toJsonObject();
		return Json::Value();
	}
};