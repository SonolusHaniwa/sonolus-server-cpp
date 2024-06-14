#pragma once

class ItemCommunityComment {
    public:

    string targetType = "";
    string targetName = "";
    string name;
    string author;
    time_t time;
    string content;
    vector<Search> actions;

    ItemCommunityComment(){}
    ItemCommunityComment(string name, string author, time_t time, string content, vector<Search> actions):
        name(name), author(author), time(time), content(content), actions(actions){}
    ItemCommunityComment(Json::Value obj) {
        name = obj["name"].asString();
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

    string formatTime(time_t t) {
		t /= 1000; tm* ti = localtime(&t);
		stringstream os;
	 	os << put_time(ti, "%m/%d/%Y %H:%M:%S");
	 	return os.str();
	}
    argvar fetchParamList() {
        argvar args;
        args["targetType"] = targetType;
        args["targetName"] = targetName;
        args["name"] = name;
        args["author"] = author;
        args["time"] = formatTime(time);
        args["content"] = content;
        args["button"] = "";
        for (int i = 0; i < actions.size(); i++)
            args["button"] += fetchIconButton(
                "javascript:" + actions[i].type.substr(0, actions[i].type.find("-")) + "('" + actions[i].type.substr(actions[i].type.find("-") + 1) + "')", 
                "{{icon." + actions[i].icon + "}}"
            ).output();
        return args;
    }

    H toHTMLObject() {
        string buffer = readFile("./web/html/components/comments.html");
        buffer = str_replace(buffer, fetchParamList());
        return H(buffer);
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
        Search replyObject = replyCommunityObject;
        replyObject.type = "reply-" + res[i]["name"];
        ItemCommunityComment comment = ItemCommunityComment(
            res[i]["name"],
            author.name + "#" + author.handle,
            atol(res[i]["time"].c_str()),
            res[i]["content"],
            { replyObject }
        ); 
        comment.targetName = res[i]["targetName"];
        comment.targetType = res[i]["targetType"];
        list.push_back(comment);
    }

    return list;
}