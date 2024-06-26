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
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["placeholder"] = placeholder;
        if (limit != 0) res["limit"] = limit;
        if (description != "") res["description"] = description;
        if (required) res["required"] = required;
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
    string description = "";
    bool required = false;

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
        if (description != "") res["description"] = description;
        if (required) res["required"] = required;
        return res;
    }
};

class SearchToggleOption {
    public:

    string query;
    string name;
    string type = "toggle";
    bool def = 0;
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["def"] = def;
        if (description != "") res["description"] = description;
        if (required) res["required"] = required;
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
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["def"] = def;
        for (int i = 0; i < values.size(); i++)
            res["values"].append(values[i]);
        if (description != "") res["description"] = description;
        if (required) res["required"] = required;
        return res;
    }
};

class SearchFileOption {
    public:

    string query;
    string name;
    string type = "file";
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        if (description != "") res["description"] = description;
        if (required) res["required"] = required;
        return res;
    }
};

class SearchTitleOption {
    public:

    string name;
    int level;
    string type = "title";
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["level"] = level;
        res["type"] = type;
        if (description != "") res["description"] = description;
        if (required) res["required"] = required;
        return res; 
    }
};

class SearchColorOption {
    public:

    string query;
    string name;
    string type = "color";
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        if (description != "") res["description"] = description;
        if (required) res["required"] = required;
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
    string description = "";
    bool required = false;
    
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
        if (description != "") res["description"] = description;
        if (required) res["required"] = required;
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
    string description = "";
    bool required = false;
    
    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["placeholder"] = placeholder;
        if (limit != 0) res["limit"] = limit;
        if (description != "") res["description"] = description;
        if (required) res["required"] = required;
        return res;
    }
};

class SearchServerItemOption {
    public:

    string query;
    string name;
    string type = "serverItem";
    string itemType;
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["itemType"] = itemType;
        if (description != "") res["description"] = description;
        if (required) res["required"] = required;
        return res;
    }
};

class SearchCollectionItemOption {
    public:

    string query;
    string name;
    string type = "collectionItem";
    string collectionType;
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["collectionType"] = collectionType;
        if (description != "") res["description"] = description;
        if (required) res["required"] = required;
        return res;
    }
};

class SearchLocalizationItemOption {
    public:

    string query;
    string name;
    string type = "localizationItem";
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        vector<string> values = {"default"};
        for (int i = 0; i < i18n_raw.size(); i++) values.push_back(i18n_raw[i]["name"].asString());
        return SearchSelectOption({
            query: query,
            name: name,
            def: 0,
            values: values,
            description: description,
            required: required
        }).toJsonObject();
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
    SearchServerItemOption serverItem;
    SearchCollectionItemOption collectionItem;
    SearchLocalizationItemOption localizationItem;

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
    SearchOption(SearchServerItemOption serverItem):
        serverItem(serverItem), type("serverItem"){}
    SearchOption(SearchCollectionItemOption collectionItem):
        collectionItem(collectionItem), type("collectionItem"){}
    SearchOption(SearchLocalizationItemOption localizationItem):
        localizationItem(localizationItem), type("localizationItem"){}
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
            if (options[i].type == "serverItem") res["options"].append(options[i].serverItem.toJsonObject());
            if (options[i].type == "collectionItem") res["options"].append(options[i].collectionItem.toJsonObject());
            if (options[i].type == "localizationItem") res["options"].append(options[i].localizationItem.toJsonObject());
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
            search.append(SearchTextOption({
                query: orig["options"][i]["query"].asString(), 
                name: orig["options"][i]["name"].asString(), 
                placeholder: orig["options"][i]["placeholder"].asString(),
                limit: orig["options"][i]["limit"].asInt(),
                description: orig["options"][i]["description"].asString(),
                required: orig["options"][i]["required"].asBool()
            }));
        } else if (orig["options"][i]["type"].asString() == "slider") {
            search.append(SearchSliderOption({
                query: orig["options"][i]["query"].asString(), 
                name: orig["options"][i]["name"].asString(),
                def: orig["options"][i]["def"].asInt(), 
                min: orig["options"][i]["min"].asInt(), 
                max: orig["options"][i]["max"].asInt(),
                step: orig["options"][i]["step"].asInt(),
                unit: orig["options"][i].isMember("unit") ? "" : orig["options"][i]["unit"].asString(),
                description: orig["options"][i]["description"].asString(),
                required: orig["options"][i]["required"].asBool()
            }));
        } else if (orig["options"][i]["type"].asString() == "toggle") {
            search.append(SearchToggleOption({
                query: orig["options"][i]["query"].asString(), 
                name: orig["options"][i]["name"].asString(),
                def: orig["options"][i]["def"].asBool(),
                description: orig["options"][i]["description"].asString(),
                required: orig["options"][i]["required"].asBool()
            }));
        } else if (orig["options"][i]["type"].asString() == "select") {
            search.append(SearchSelectOption({
                query: orig["options"][i]["query"].asString(), 
                name: orig["options"][i]["name"].asString(),
                def: orig["options"][i]["def"].asInt(),
                values: vector<string>()
            }));
            for (int j = 0; j < orig["options"][i]["values"].size(); j++) 
                search.options.back().select.values.push_back(orig["options"][i]["values"][j].asString());
        } else if (orig["options"][i]["type"].asString() == "file") {
            search.append(SearchFileOption({
                query: orig["options"][i]["query"].asString(), 
                name: orig["options"][i]["name"].asString(),
                description: orig["options"][i]["description"].asString(),
                required: orig["options"][i]["required"].asBool()
            }));
        } else if (orig["options"][i]["type"].asString() == "title") {
            search.append(SearchTitleOption({
                name: orig["options"][i]["name"].asString(),
                level: orig["options"][i]["level"].asInt(),
                description: orig["options"][i]["description"].asString(),
                required: orig["options"][i]["required"].asBool()
            }));
        } else if (orig["options"][i]["type"].asString() == "color") {
            search.append(SearchColorOption({
                query: orig["options"][i]["query"].asString(), 
                name: orig["options"][i]["name"].asString(),
                description: orig["options"][i]["description"].asString(),
                required: orig["options"][i]["required"].asBool()
            }));
        } else if (orig["options"][i]["type"].asString() == "multi") {
            search.append(SearchMultiOption({
                query: orig["options"][i]["query"].asString(), 
                name: orig["options"][i]["name"].asString(),
                defs: vector<bool>(),
                values: vector<string>(),
                description: orig["options"][i]["description"].asString(),
                required: orig["options"][i]["required"].asBool()
            }));
            for (int j = 0; j < orig["options"][i]["defs"].size(); j++) 
                search.options.back().multi.defs.push_back(orig["options"][i]["defs"][j].asBool());
            for (int j = 0; j < orig["options"][i]["values"].size(); j++) 
                search.options.back().multi.values.push_back(orig["options"][i]["values"][j].asString());
        } else if (orig["options"][i]["type"].asString() == "textArea") {
            search.append(SearchTextAreaOption({
                query: orig["options"][i]["query"].asString(), 
                name: orig["options"][i]["name"].asString(),
                placeholder: orig["options"][i]["placeholder"].asString(),
                limit: orig["options"][i]["limit"].asInt(),
                description: orig["options"][i]["description"].asString(),
                required: orig["options"][i]["required"].asBool()
            }));
        } else if (orig["options"][i]["type"].asString() == "serverItem") {
            search.append(SearchServerItemOption({
                query: orig["options"][i]["query"].asString(), 
                name: orig["options"][i]["name"].asString(),
                itemType: orig["options"][i]["itemType"].asString(),
                description: orig["options"][i]["description"].asString(),
                required: orig["options"][i]["required"].asBool()
            }));
        } else if (orig["options"][i]["type"].asString() == "collectionItem") {
            search.append(SearchCollectionItemOption({
                query: orig["options"][i]["query"].asString(), 
                name: orig["options"][i]["name"].asString(),
                collectionType: orig["options"][i]["collectionType"].asString(),
                description: orig["options"][i]["description"].asString(),
                required: orig["options"][i]["required"].asBool()
            }));
        } else if (orig["options"][i]["type"].asString() == "localizationItem") {
            search.append(SearchLocalizationItemOption({
                query: orig["options"][i]["query"].asString(), 
                name: orig["options"][i]["name"].asString(),
                description: orig["options"][i]["description"].asString(),
                required: orig["options"][i]["required"].asBool()
            }));
        } else {
            writeLog(LOG_LEVEL_ERROR, ("Invalid search option type \"" + orig["options"][i]["type"].asString()).c_str());  
            continue;
        }
    }
    return search;
}

argvar argResolver(argvar source, Json::Value options) {
    argvar result;
    for (int i = 0; i < options.size(); i++) {
        string name = options[i]["query"].asString();
        if (options[i]["type"].asString() == "slider" || options[i]["type"].asString() == "toggle") {
            if (source.find(name) == source.end()) result[name] = options[i]["def"].asString();
            else result[name] = source[name];
        } else if (options[i]["type"].asString() == "select") {
            if (source.find(name) == source.end()) result[name] = options[i]["values"][options[i]["def"].asInt()].asString();
            else result[name] = options[i]["values"][atoi(source[name].c_str())].asString();
        } else if (options[i]["type"].asString() == "localizationItem") {
            int id = atoi(source[name].c_str());
            if (id == 0) result[name] = "default";
            else result[name] = i18n_raw[id - 1]["name"].asString();
        } else result[name] = source[name];
    } return result;
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
            SearchTextOption({
                query: "keywords",
                name: "#KEYWORDS",
                placeholder: "#KEYWORDS_PLACEHOLDER"
            })
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
            SearchTextAreaOption({
                query: "content",
                name: "#CONTENT",
                placeholder: "#CONTENT_PLACEHOLDER"
            })
        }
    });
}();

Search ratingCommunityObject = [](){
    return Search({
        type: "rating",
        title: "#RATING",
        icon: "star",
        options: {
            SearchSliderOption({
                query: "rating",
                name: "#RATING",
                def: 5,
                min: 0,
                max: 5,
                step: 1
            })
        }
    });
}();

Search replyCommunityObject = [](){
    return Search({
        type: "reply",
        title: "#REPLY",
        icon: "reply",
        options: {
            SearchTextAreaOption({
                query: "content",
                name: "#CONTENT",
                placeholder: "#CONTENT_PLACEHOLDER"
            })
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