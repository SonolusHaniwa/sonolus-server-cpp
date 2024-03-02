#pragma once

class Tag {
    public:

    string title;
    string icon;

    Tag(){}
    Tag(string title, string icon): title(title), icon(icon){}
    Tag(Json::Value arr) {
        title = arr["title"].asString();
        icon = arr["icon"].asString();
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["title"] = title;
        res["icon"] = icon;
        return res;
    }
};

vector<Tag> deserializeTagString(string tagString) {
    vector<Tag> res;
    bool inTag = false; Tag tag("", ""); bool isTitle = true;
    for (int i = 0; i < tagString.size(); i++) {
        char ch = tagString[i]; bool isSpecial = false;
        if (ch == '\\') ch = tagString[++i], isSpecial = true;
        if (!inTag) { 
            if (!isSpecial) {
                if (ch == ',') {
                    if (iconList.find("icon." + tag.icon) == iconList.end()) tag.icon = "";
                    res.push_back(tag), tag = Tag("", ""), isTitle = true;
                } else if (ch == '(') inTag = true;
            }
        } else if (inTag) {
            if (isSpecial) isTitle ? tag.title += ch : tag.icon += ch;
            else if (ch == ',') isTitle = false;
            else if (ch == ')') inTag = false, isTitle = true;
            else if (ch == '(') isTitle ? tag.title += ch : tag.icon += ch;
            else isTitle ? tag.title += ch : tag.icon += ch;
        }
    } if (tag.icon != "" || tag.title != "") {
        if (iconList.find("icon." + tag.icon) == iconList.end()) tag.icon = "";
        res.push_back(tag);
    } return res;
}

string serializeTagString(vector<Tag> tags) {
    string res = "";
    for (int i = 0; i < tags.size(); i++) {
        if (i) res += ",";
        res += "(" + tags[i].title + "," + tags[i].icon + ")";
    } return res;
}