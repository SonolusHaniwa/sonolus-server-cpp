#pragma once

class ItemCommunityComment {
    public:

    string author;
    time_t time;
    string content;
    vector<Search> actions;

    ItemCommunityComment(){}
    ItemCommunityComment(string author, time_t time, string content, vector<Search> actions):
        author(author), time(time), content(content), actions(actions){}
    ItemCommunityComment(Json::Value obj) {
        author = obj["author"].asString();
        time = obj["time"].asInt();
        content = obj["content"].asString();
        actions = constructSearch(obj["actions"]);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["author"] = author;
        res["time"] = time;
        res["content"] = content;
        res["actions"].resize(0);
        for (int i = 0; i < actions.size(); i++)
            res["actions"].append(actions[i].toJsonObject());
        return res;
    }
};

int commentsNumber(string filter) {
    string sql = "SELECT COUNT(*) AS sum FROM Comment WHERE (" + filter + ")";
    dbres res = db.query(sql.c_str());
    return atoi(res[0]["sum"].c_str());
}

vector<ItemCommunityComment> commentsList(string filter, string order, int st = 1, int en = 20) {
    string sql = "SELECT * FROM Comment WHERE (" + filter + ") ORDER BY " + order + " LIMIT " + to_string(st - 1) + ", " + to_string(en - st + 1);

    auto res = db.query(sql.c_str());
    vector<ItemCommunityComment> list;

    for (int i = 0; i < res.size(); i++) {
        UserProfile author = usersList("id = \"" + res[i]["uid"] + "\"", "")[0];
        ItemCommunityComment comment = ItemCommunityComment(
            author.name + "#" + author.handle,
            atol(res[i]["time"].c_str()),
            res[i]["content"],
            {}
        ); 
        list.push_back(comment);
    }

    return list;
}