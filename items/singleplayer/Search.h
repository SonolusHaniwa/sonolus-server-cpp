#ifndef SEARCH_H
#define SEARCH_H

using namespace std;

class SearchTextOption {
    public:

    string query;
    string name;
    string type = "text";
    string placeholder;

    SearchTextOption(){}
    SearchTextOption(string query, string name, string placeholder):
        query(query), name(name), placeholder(placeholder){}

    Json::Value toJsonObject() {
        Json::Value res;
        res["query"] = query;
        res["name"] = name;
        res["type"] = type;
        res["placeholder"] = placeholder;
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
        }
        return res;
    }
};

Search quickSearchObject = [](){
	return Search({
		title: "Quick Search",
		icon: "search",
		filter: "title LIKE \"keywords\"",
		order: "",
		options: {
			SearchTextOption("keywords", "#KEYWORDS", "#KEYWORDS")
		}
	});
}();

#endif