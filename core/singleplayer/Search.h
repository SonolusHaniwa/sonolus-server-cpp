#ifndef SEARCH_H
#define SEARCH_H

using namespace std;

class SearchTextOption {
    public:

    string query;
    string name;
    string type = "text";
    string placeholder;
    int limit = 0;

    SearchTextOption(){}
    SearchTextOption(string query, string name, string placeholder, int limit = 0):
        query(query), name(name), placeholder(placeholder), limit(limit){}

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["placeholder"] = placeholder;
        if (limit != 0) res["limit"] = limit;
        return res;
    }
};

class SearchSliderOption {
    public:

    string query;
    string name;
    string type = "slider";
    int def;
    int min;
    int max;
    int step;
    string unit;

    SearchSliderOption(){}
    SearchSliderOption(string query, string name, int def, int min, int max, int step, string unit = ""):
        query(query), name(name), def(def), min(min), max(max), step(step), unit(unit){}

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["def"] = def;
        res["min"] = min;
        res["max"] = max;
        res["step"] = step;
        res["unit"] = unit;
        return res;
    }
};

class SearchToggleOption {
    public:

    string query;
    string name;
    string type = "toggle";
    bool def = 0;

    SearchToggleOption(){}
    SearchToggleOption(string query, string name, bool def):
        query(query), name(name), def(def){}

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["def"] = def;
        return res;
    }
};

class SearchSelectOption {
    public:

    string query;
    string name;
    string type = "select";
    int def;
    vector<string> values;

    SearchSelectOption(){}
    SearchSelectOption(string query, string name, int def, vector<string> values):
        query(query), name(name), def(def), values(values){}

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["def"] = def;
        for (int i = 0; i < values.size(); i++)
            res["values"].append(values[i]);
        return res;
    }
};

class SearchFileOption {
    public:

    string query;
    string name;
    string type = "file";

    SearchFileOption(){};
    SearchFileOption(string query, string name): query(query), name(name){};

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        return res;
    }
};

class SearchTitleOption {
    public:

    string name;
    int level;
    string type = "title";

    SearchTitleOption(){};
    SearchTitleOption(string name, int level): name(name), level(level){};

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["level"] = level;
        res["type"] = type;
        return res; 
    }
};

class SearchColorOption {
    public:

    string query;
    string name;
    string type = "color";

    SearchColorOption(){};
    SearchColorOption(string query, string name): query(query), name(name){};

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        return res;
    }
};

class SearchMultiOption {
    public:

    string query;
    string name;
    string type = "multi";
    vector<bool> defs;
    vector<string> values;

    SearchMultiOption(){};
    SearchMultiOption(string query, string name, vector<bool> defs, vector<string> values):
        query(query), name(name), defs(defs), values(values){};
    
    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["defs"].resize(0);
        for (int i = 0; i < defs.size(); i++)
            res["defs"].append((int)defs[i]);
        for (int i = 0; i < values.size(); i++)
            res["values"].append(values[i]);
        return res;
    }
};

class SearchTextAreaOption {
    public:

    string query;
    string name;
    string type = "textArea";
    string placeholder;
    int limit = 0;

    SearchTextAreaOption(){}
    SearchTextAreaOption(string query, string name, string placeholder, int limit = 0):
        query(query), name(name), placeholder(placeholder), limit(limit){}
    
    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["placeholder"] = placeholder;
        if (limit != 0) res["limit"] = limit;
        return res;
    }
};

class SearchOption {
    public:

    string type;
    SearchTextOption text;
    SearchSliderOption slider;
    SearchToggleOption toggle;
    SearchSelectOption select;
    SearchFileOption file;
    SearchTitleOption title;
    SearchColorOption color;
    SearchMultiOption multi;
    SearchTextAreaOption textArea;

    SearchOption(){}
    SearchOption(SearchTextOption text):
        text(text), type("text"){}
    SearchOption(SearchSliderOption slider):
        slider(slider), type("slider"){}
    SearchOption(SearchToggleOption toggle):
        toggle(toggle), type("toggle"){}
    SearchOption(SearchSelectOption select):
        select(select), type("select"){}
    SearchOption(SearchFileOption file): 
        file(file), type("file"){}
    SearchOption(SearchTitleOption title):
        title(title), type("title"){}
    SearchOption(SearchColorOption color):
        color(color), type("color"){}
    SearchOption(SearchMultiOption multi):
        multi(multi), type("multi"){}
    SearchOption(SearchTextAreaOption textArea):
        textArea(textArea), type("textArea"){}
};

class Search {
    public:

    string type = "quick";
    string title;
    string icon;
    string filter;
    string order;
    vector<SearchOption> options;

    void append(SearchOption option) {
        options.push_back(option);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["title"] = title;
        res["icon"] = icon;
        res["options"].resize(0);
        for (int i = 0; i < options.size(); i++) {
            if (options[i].type == "text") res["options"].append(options[i].text.toJsonObject());
            if (options[i].type == "slider") res["options"].append(options[i].slider.toJsonObject());
            if (options[i].type == "toggle") res["options"].append(options[i].toggle.toJsonObject());
            if (options[i].type == "select") res["options"].append(options[i].select.toJsonObject());
            if (options[i].type == "file") res["options"].append(options[i].file.toJsonObject());
            if (options[i].type == "title") res["options"].append(options[i].title.toJsonObject());
            if (options[i].type == "color") res["options"].append(options[i].color.toJsonObject());
            if (options[i].type == "multi") res["options"].append(options[i].multi.toJsonObject());
            if (options[i].type == "textArea") res["options"].append(options[i].textArea.toJsonObject());
        }
        return res;
    }
};

Search constructSingleSearch(Json::Value orig) {
    Search search;
    search.type = orig["type"].asString();
    search.title = orig["title"].asString();
    search.icon = orig["icon"].asString();
    search.filter = orig["filter"].asString();
    search.order = orig["order"].asString();
    for (int i = 0; i < orig["options"].size(); i++) {
        if (orig["options"][i]["type"].asString() == "text") {
            SearchTextOption text = SearchTextOption(
                orig["options"][i]["query"].asString(), 
                orig["options"][i]["name"].asString(), 
                orig["options"][i]["placeholder"].asString(),
                orig["options"][i]["limit"].asInt()
            ); search.append(SearchOption(text));
        } else if (orig["options"][i]["type"].asString() == "slider") {
            SearchSliderOption slider = SearchSliderOption(
                orig["options"][i]["query"].asString(), 
                orig["options"][i]["name"].asString(),
                orig["options"][i]["def"].asInt(), 
                orig["options"][i]["min"].asInt(), 
                orig["options"][i]["max"].asInt(),
                orig["options"][i]["step"].asInt(),
                orig["options"][i].isMember("unit") ? "" : orig["options"][i]["unit"].asString()
            ); search.append(slider);
        } else if (orig["options"][i]["type"].asString() == "toggle") {
            SearchToggleOption toggle = SearchToggleOption(
                orig["options"][i]["query"].asString(),
                orig["options"][i]["name"].asString(),
                orig["options"][i]["def"].asInt()
            ); search.append(toggle);
        } else if (orig["options"][i]["type"].asString() == "select") {
            SearchSelectOption select = SearchSelectOption(
                orig["options"][i]["query"].asString(),
                orig["options"][i]["name"].asString(),
                orig["options"][i]["def"].asInt(),
                vector<string>()
            );
            for (int j = 0; j < orig["options"][i]["values"].size(); j++) 
                select.values.push_back(orig["options"][i]["values"][j].asString());
            search.append(select);
        } else if (orig["options"][i]["type"].asString() == "file") {
            SearchFileOption file = SearchFileOption(
                orig["options"][i]["query"].asString(),
                orig["options"][i]["name"].asString()
            ); search.append(file);
        } else if (orig["options"][i]["type"].asString() == "title") {
            SearchTitleOption title = SearchTitleOption(
                orig["options"][i]["name"].asString(),
                orig["options"][i]["level"].asInt()
            ); search.append(title);
        } else if (orig["options"][i]["type"].asString() == "color") {
            SearchColorOption color = SearchColorOption(
                orig["options"][i]["query"].asString(),
                orig["options"][i]["name"].asString()
            ); search.append(color);
        } else if (orig["options"][i]["type"].asString() == "multi") {
            SearchMultiOption multi = SearchMultiOption(
                orig["options"][i]["query"].asString(),
                orig["options"][i]["name"].asString(),
                vector<bool>(),
                vector<string>()
            ); 
            for (int j = 0; j < orig["options"][i]["defs"].size(); j++) 
                multi.values.push_back(orig["options"][i]["defs"][j].asString());
            for (int j = 0; j < orig["options"][i]["values"].size(); j++) 
                multi.values.push_back(orig["options"][i]["values"][j].asString());
            search.append(multi);
        } else if (orig["options"][i]["type"].asString() == "textArea") {
            SearchTextAreaOption textArea = SearchTextAreaOption(
                orig["options"][i]["query"].asString(),
                orig["options"][i]["name"].asString(),
                orig["options"][i]["placeholder"].asString(),
                orig["options"][i]["limit"].asInt()
            ); search.append(textArea);
        } else {
            writeLog(LOG_LEVEL_ERROR, ("Invalid search option type \"" + orig["options"][i]["type"].asString()).c_str());  
            continue;
        }
    }
    return search;
}

vector<Search> constructSearch(Json::Value orig) {
	vector<Search> result; Json::Value json;
	for (int k = 0; k < orig.size(); k++) result.push_back(constructSingleSearch(orig[k]));
    return result;
}

Search quickSearchObject = [](){
	return Search({
        type: "quick",
		title: "Quick Search",
		icon: "search",
		filter: "title LIKE \"keywords\"",
		order: "",
		options: {
			SearchTextOption("keywords", "#KEYWORDS", "#KEYWORDS")
		}
	});
}();

Search likeCommunityObject = [](){
    return Search({
        type: "like",
        title: "#LIKE",
        icon: "heartHollow"
    });
}();

Search unlikeCommunityObject = [](){
    return Search({
        type: "dislike",
        title: "#DISLIKE",
        icon: "heart"
    });
}();

Search commentCommunityObject = [](){
    return Search({
        type: "comment",
        title: "#COMMENT",
        icon: "comment",
        options: {
            SearchTextAreaOption("content", "#CONTENT", "#CONTENT_PLACEHOLDER")
        }
    });
}();

Search ratingCommunityObject = [](){
    return Search({
        type: "rating",
        title: "#RATING",
        icon: "star",
        options: {
            SearchSliderOption("rating", "#RATING", 5, 0, 5, 1)
        }
    });
}();

Search replyCommunityObject = [](){
    return Search({
        type: "reply",
        title: "#REPLY",
        icon: "reply",
        options: {
            SearchTextAreaOption("content", "#CONTENT", "#CONTENT_PLACEHOLDER")
        }
    });
}();

// Search editCommunityObject = [](){
//     return Search({
//         type: "edit",
//         title: "#EDIT",
//         icon: "edit",
//         options: {
//             SearchTextAreaOption("content", "#CONTENT", "#CONTENT_PLACEHOLDER")
//         }
//     });
// }();

Search deleteCommunityObject = [](){
    return Search({
        type: "delete",
        title: "#DELETE",
        icon: "delete"
    });
}();

#endif