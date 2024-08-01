#ifndef SEARCH_H
#define SEARCH_H

using namespace std;

class SearchTextOption {
    public:

    string query = "";
    string name = "";
    string type = "text";
    string def = "";
    string placeholder = "";
    int limit = 0;
    string description = "";
    bool required = false;
    vector<string> shortcuts;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["def"] = def;
        res["placeholder"] = placeholder;
        res["limit"] = limit;
        if (description != "") res["description"] = description;
        res["required"] = required;
        res["shortcuts"].resize(0);
        for (int i = 0; i < shortcuts.size(); i++) res["shortcuts"].append(shortcuts[i]);
        return res;
    }
};

class SearchSliderOption {
    public:

    string query = "";
    string name = "";
    string type = "slider";
    int def = 0;
    int min = 0;
    int max = 0;
    int step = 0;
    string unit = "";
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
        res["required"] = required;
        return res;
    }
};

class SearchToggleOption {
    public:

    string query = "";
    string name = "";
    string type = "toggle";
    bool def = false;
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["def"] = def;
        if (description != "") res["description"] = description;
        res["required"] = required;
        return res;
    }
};

class SearchSelectOption {
    public:

    string query = "";
    string name = "";
    string type = "select";
    int def = 0;
    vector<string> values = {};
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
        res["required"] = required;
        return res;
    }
};

class SearchFileOption {
    public:

    string query = "";
    string name = "";
    string type = "file";
    string def = "";
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["def"] = def;
        if (description != "") res["description"] = description;
        res["required"] = required;
        return res;
    }
};

class SearchTitleOption {
    public:

    string name = "";
    int level = 1;
    string type = "title";
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["name"] = name;
        res["level"] = level;
        res["type"] = type;
        if (description != "") res["description"] = description;
        res["required"] = required;
        return res; 
    }
};

class SearchColorOption {
    public:

    string query = "";
    string name = "";
    string type = "color";
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        if (description != "") res["description"] = description;
        res["required"] = required;
        return res;
    }
};

class SearchMultiOption {
    public:

    string query = "";
    string name = "";
    string type = "multi";
    vector<bool> def = {};
    vector<string> values = {};
    string description = "";
    bool required = false;
    
    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["def"].resize(0);
        for (int i = 0; i < def.size(); i++) res["def"].append((bool)def[i]);
        res["defs"] = res["def"];
        for (int i = 0; i < values.size(); i++) res["values"].append(values[i]);
        if (description != "") res["description"] = description;
        res["required"] = required;
        return res;
    }
};

class SearchTextAreaOption {
    public:

    string query = "";
    string name = "";
    string type = "textArea";
    string def = "";
    string placeholder = "";
    int limit = 0;
    string description = "";
    bool required = false;
    vector<string> shortcuts;
    
    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["def"] = def;
        res["placeholder"] = placeholder;
        res["limit"] = limit;
        if (description != "") res["description"] = description;
        res["required"] = required;
        res["shortcuts"].resize(0);
        for (int i = 0; i < shortcuts.size(); i++) res["shortcuts"].append(shortcuts[i]);
        return res;
    }
};

class SearchServerItemOption {
    public:

    string query = "";
    string name = "";
    string type = "serverItem";
    string def = "";
    string itemType = "";
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        if (def != "") {
            res["def"]["address"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/sonolus/" + itemType + "s/" + def;
            res["def"]["name"] = def;
        } else res["def"] = Json::nullValue;
        res["itemType"] = itemType;
        if (description != "") res["description"] = description;
        res["required"] = required;
        res["allowOtherServers"] = false;
        return res;
    }
};

class SearchCollectionItemOption {
    public:

    string query = "";
    string name = "";
    string type = "collectionItem";
    string def = "";
    string collectionType;
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["def"] = def;
        res["collectionType"] = collectionType;
        if (description != "") res["description"] = description;
        res["required"] = required;
        return res;
    }
};

class SearchLocalizationItemOption {
    public:

    string query = "";
    string name = "";
    string type = "localizationItem";
    int def = 0;
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        vector<string> values = {"default"};
        for (int i = 0; i < i18n_raw.size(); i++) values.push_back(i18n_raw[i]["name"].asString());
        return SearchSelectOption({
            query: query,
            name: name,
            def: def,
            values: values,
            description: description,
            required: required
        }).toJsonObject();
    }
};

class SearchServerItemsOption {
    public:

    string query = "";
    string name = "";
    string type = "serverItems";
    vector<string> def;
    string itemType = "";
    bool allowOtherServers = false;
    int limit = 0;
    string description = "";
    bool required = false;

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["def"].resize(0);
        for (int i = 0; i < def.size(); i++) 
            res["def"][i]["address"] = "sonolus://" + appConfig["server.rootUrl"].asString() + "/sonolus/" + itemType + "s/" + def[i],
            res["def"][i]["name"] = def[i];
        res["itemType"] = itemType;
        if (description != "") res["description"] = description;
        res["allowOtherServers"] = allowOtherServers;
        res["limit"] = limit;
        res["required"] = required;
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
    SearchServerItemOption serverItem;
    SearchCollectionItemOption collectionItem;
    SearchLocalizationItemOption localizationItem;
    SearchServerItemsOption serverItems;

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
    SearchOption(SearchServerItemsOption serverItems):
        serverItems(serverItems), type("serverItems"){}

    Json::Value toJsonObject() {
        if (type == "text") return text.toJsonObject();
        if (type == "slider") return slider.toJsonObject();
        if (type == "toggle") return toggle.toJsonObject();
        if (type == "select") return select.toJsonObject();
        if (type == "file") return file.toJsonObject();
        if (type == "title") return title.toJsonObject();
        if (type == "color") return color.toJsonObject();
        if (type == "multi") return multi.toJsonObject();
        if (type == "textArea") return textArea.toJsonObject();
        if (type == "serverItem") return serverItem.toJsonObject();
        if (type == "collectionItem") return collectionItem.toJsonObject();
        if (type == "localizationItem") return localizationItem.toJsonObject();
        if (type == "serverItems") return serverItems.toJsonObject();
        return Json::Value::null;
    }
};

class Search {
    public:

    string type = "quick";
    string title = "";
    string icon = "";
    string filter = "";
    string order = "";
    string description = "";
    bool requireConfirmation = false;
    vector<SearchOption> options = {};

    void append(SearchOption option) {
        options.push_back(option);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = type;
        res["title"] = title;
        res["icon"] = icon;
        if (description != "") res["description"] = description;
        res["requireConfirmation"] = requireConfirmation;
        res["options"].resize(0);
        for (int i = 0; i < options.size(); i++) res["options"].append(options[i].toJsonObject());
        return res;
    }
};

vector<SearchOption> constructSearchOptions(Json::Value orig) {
    vector<SearchOption> search;
    for (int i = 0; i < orig.size(); i++) {
        if (orig[i]["type"].asString() == "text") {
            search.push_back(SearchTextOption({
                query: orig[i]["query"].asString(), 
                name: orig[i]["name"].asString(), 
                def: orig[i]["def"].asString(),
                placeholder: orig[i]["placeholder"].asString(),
                limit: orig[i]["limit"].asInt(),
                description: orig[i]["description"].asString(),
                required: orig[i]["required"].asBool(),
                shortcuts: [&](){
                    vector<string> res;
                    for (int j = 0; j < orig[i]["shortcuts"].size(); j++) 
                        res.push_back(orig[i]["shortcuts"][j].asString());
                    return res;
                }()
            }));
        } else if (orig[i]["type"].asString() == "slider") {
            search.push_back(SearchSliderOption({
                query: orig[i]["query"].asString(), 
                name: orig[i]["name"].asString(),
                def: orig[i]["def"].asInt(), 
                min: orig[i]["min"].asInt(), 
                max: orig[i]["max"].asInt(),
                step: orig[i]["step"].asInt(),
                unit: orig[i].isMember("unit") ? "" : orig[i]["unit"].asString(),
                description: orig[i]["description"].asString(),
                required: orig[i]["required"].asBool()
            }));
        } else if (orig[i]["type"].asString() == "toggle") {
            search.push_back(SearchToggleOption({
                query: orig[i]["query"].asString(), 
                name: orig[i]["name"].asString(),
                def: orig[i]["def"].asBool(),
                description: orig[i]["description"].asString(),
                required: orig[i]["required"].asBool()
            }));
        } else if (orig[i]["type"].asString() == "select") {
            search.push_back(SearchSelectOption({
                query: orig[i]["query"].asString(), 
                name: orig[i]["name"].asString(),
                def: orig[i]["def"].asInt(),
                values: [&](){
                    vector<string> res;
                    for (int j = 0; j < orig[i]["values"].size(); j++) 
                        res.push_back(orig[i]["values"][j].asString());
                    return res;
                }()
            }));
        } else if (orig[i]["type"].asString() == "file") {
            search.push_back(SearchFileOption({
                query: orig[i]["query"].asString(), 
                name: orig[i]["name"].asString(),
                description: orig[i]["description"].asString(),
                required: orig[i]["required"].asBool()
            }));
        } else if (orig[i]["type"].asString() == "title") {
            search.push_back(SearchTitleOption({
                name: orig[i]["name"].asString(),
                level: orig[i]["level"].asInt(),
                description: orig[i]["description"].asString(),
                required: orig[i]["required"].asBool()
            }));
        } else if (orig[i]["type"].asString() == "color") {
            search.push_back(SearchColorOption({
                query: orig[i]["query"].asString(), 
                name: orig[i]["name"].asString(),
                description: orig[i]["description"].asString(),
                required: orig[i]["required"].asBool()
            }));
        } else if (orig[i]["type"].asString() == "multi") {
            search.push_back(SearchMultiOption({
                query: orig[i]["query"].asString(), 
                name: orig[i]["name"].asString(),
                def: [&](){
                    vector<bool> res;
                    for (int j = 0; j < orig[i]["def"].size(); j++) 
                        res.push_back(orig[i]["def"][j].asBool());
                    return res;
                }(),
                values: [&](){
                    vector<string> res;
                    for (int j = 0; j < orig[i]["values"].size(); j++) 
                        res.push_back(orig[i]["values"][j].asString());
                    return res;
                }(),
                description: orig[i]["description"].asString(),
                required: orig[i]["required"].asBool()
            }));
        } else if (orig[i]["type"].asString() == "textArea") {
            search.push_back(SearchTextAreaOption({
                query: orig[i]["query"].asString(), 
                name: orig[i]["name"].asString(),
                def: orig[i]["def"].asString(),
                placeholder: orig[i]["placeholder"].asString(),
                limit: orig[i]["limit"].asInt(),
                description: orig[i]["description"].asString(),
                required: orig[i]["required"].asBool(),
                shortcuts: [&](){
                    vector<string> res;
                    for (int j = 0; j < orig[i]["shortcuts"].size(); j++) 
                        res.push_back(orig[i]["shortcuts"][j].asString());
                    return res;
                }()
            }));
        } else if (orig[i]["type"].asString() == "serverItem") {
            search.push_back(SearchServerItemOption({
                query: orig[i]["query"].asString(), 
                name: orig[i]["name"].asString(),
                itemType: orig[i]["itemType"].asString(),
                description: orig[i]["description"].asString(),
                required: orig[i]["required"].asBool()
            }));
        } else if (orig[i]["type"].asString() == "collectionItem") {
            search.push_back(SearchCollectionItemOption({
                query: orig[i]["query"].asString(), 
                name: orig[i]["name"].asString(),
                collectionType: orig[i]["collectionType"].asString(),
                description: orig[i]["description"].asString(),
                required: orig[i]["required"].asBool()
            }));
        } else if (orig[i]["type"].asString() == "localizationItem") {
            search.push_back(SearchLocalizationItemOption({
                query: orig[i]["query"].asString(), 
                name: orig[i]["name"].asString(),
                description: orig[i]["description"].asString(),
                required: orig[i]["required"].asBool()
            }));
        } else if (orig[i]["type"].asString() == "serverItems") {
            search.push_back(SearchServerItemsOption({
                query: orig[i]["query"].asString(), 
                name: orig[i]["name"].asString(),
                def: [&](){
                    vector<string> res;
                    for (int j = 0; j < orig[i]["def"].size(); j++) 
                        res.push_back(orig[i]["def"][j].asString());
                    return res;
                }(),
                itemType: orig[i]["itemType"].asString(),
                allowOtherServers: orig[i]["allowOtherServers"].asBool(),
                limit: orig[i]["limit"].asInt(),
                description: orig[i]["description"].asString(),
                required: orig[i]["required"].asBool()
            }));
        } else {
            writeLog(LOG_LEVEL_ERROR, ("Invalid search option type \"" + orig[i]["type"].asString()).c_str());  
            continue;
        }
    }
    return search;
}

Search constructDefaultSearchOption(vector<Search> searches, string defaultSearch) {
    int id = -1;
    auto options = getParam(defaultSearch);
    for (int i = 0; i < searches.size(); i++) {
        if (searches[i].type != options["type"]) continue;
        id = i;
    }
    if (id == -1) return Search();
    Search result = searches[id];
    for (int i = 0; i < result.options.size(); i++) {
        if (result.options[i].type == "text" && options.find(result.options[i].text.query) != options.end()) {
            result.options[i].text.def = options[result.options[i].text.query];
        } else if (result.options[i].type == "slider" && options.find(result.options[i].slider.query) != options.end()){
            result.options[i].slider.def = atoi(options[result.options[i].slider.query].c_str());
        } else if (result.options[i].type == "toggle" && options.find(result.options[i].toggle.query) != options.end()){
            result.options[i].toggle.def = (bool)atoi(options[result.options[i].toggle.query].c_str());
        } else if (result.options[i].type == "select" && options.find(result.options[i].select.query) != options.end()){
            result.options[i].select.def = atoi(options[result.options[i].select.query].c_str());
        } else if (result.options[i].type == "file" && options.find(result.options[i].file.query) != options.end()){
            result.options[i].file.def = options[result.options[i].file.query];
        } else if (result.options[i].type == "multi" && options.find(result.options[i].multi.query) != options.end()){
            for (int j = 0; j < result.options[i].multi.def.size(); j++) 
                result.options[i].multi.def[j] = (bool)atoi(options[result.options[i].multi.query + to_string(j)].c_str());
        } else if (result.options[i].type == "textArea" && options.find(result.options[i].textArea.query) != options.end()){
            result.options[i].textArea.def = options[result.options[i].textArea.query];
        } else if (result.options[i].type == "serverItem" && options.find(result.options[i].serverItem.query) != options.end()){
            result.options[i].serverItem.def = options[result.options[i].serverItem.query];
        } else if (result.options[i].type == "collectionItem" && options.find(result.options[i].collectionItem.query) != options.end()){
            result.options[i].collectionItem.def = options[result.options[i].collectionItem.query];
        } else if (result.options[i].type == "localizationItem" && options.find(result.options[i].localizationItem.query) != options.end()){
            result.options[i].localizationItem.def = atoi(options[result.options[i].localizationItem.query].c_str());
        } else if (result.options[i].type == "serverItems" && options.find(result.options[i].serverItems.query) != options.end()){
            for (int j = 0; j < result.options[i].serverItems.def.size(); j++) 
                result.options[i].serverItems.def[j] = options[result.options[i].serverItems.query + to_string(j)];
        }
    }
    return result;
}

vector<Search> constructDefaultSearchOptions(vector<Search> searches, string defaultSearch) {
    int id = -1;
    auto options = getParam(defaultSearch);
    for (int i = 0; i < searches.size(); i++) {
        if (searches[i].type != options["type"]) continue;
        id = i;
    }
    if (id == -1) return searches;
    searches[id] = constructDefaultSearchOption(searches, defaultSearch);
    return searches;
}

Search constructSingleSearch(Json::Value orig) {
    Search search;
    search.type = orig["type"].asString();
    search.title = orig["title"].asString();
    search.icon = orig["icon"].asString();
    search.filter = orig["filter"].asString();
    search.order = orig["order"].asString();
    search.options = constructSearchOptions(orig["options"]);
    return search;
}

argvar argResolver(argvar source, Json::Value options, string localization) {
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
        } else if (options[i]["type"].asString() == "multi") {
            if (source[name].size() != options[i]["values"].size()) {
                source[name] = "";
                for (int j = 0; j < options[i]["values"].size(); j++) source[name] += '0' + options[i]["def"][j].asInt();
            } vector<string> res; Json::Value tmp = Json::arrayValue;
            for (int j = 0; j < options[i]["values"].size(); j++)
                if (source[name][j] == '1')
                    res.push_back(options[i]["values"][j].asString()),
                    tmp.append(options[i]["values"][j].asString());
            result[name] = json_encode(tmp);
            for (int j = 0; j < options[i]["variables"].size(); j++) {
                auto item = options[i]["variables"][j];
                string varName = item["name"].asString();
                string connector = item["connector"].asString();
                result[varName] = "(";
                for (int k = 0; k < res.size(); k++) {
                    string value = str_replace("{{" + name + "}}", res[k], item["expr"].asCString());
                    if (k != 0) result[varName] += " " + connector + " ";
                    result[varName] += value;
                }
                if (res.size() == 0) result[varName] += item["default"].asString(); // 防爆用
                result[varName] += ")";
            }
        } else if (options[i]["type"].asString() == "serverItems") {
            string tableName = options[i]["itemType"].asString();
            tableName[0] += 'A' - 'a';
            string sql = "SELECT id, name, title FROM " + tableName + " WHERE (localization = \"" + localization +
                "\" OR localization = \"default\") ORDER BY CASE WHEN localization = \"default\" THEN 1 WHEN localization != \"default\" THEN 0 END ASC";
            sql = "SELECT * FROM (" + sql + ") AS A GROUP BY name";
            sql = "SELECT * FROM (" + sql + ") AS B ORDER BY id ASC";
            auto res = db.query(sql, tableName);
            Json::Value tmp;
            for (int j = 0; j < source[name].size(); j++) if (source[name][j] == '1') tmp.append(res[j]["name"]);
            result[name] = json_encode(tmp);
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