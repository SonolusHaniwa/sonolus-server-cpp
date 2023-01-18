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

class SearchOption {
    public:

    string type;
    SearchTextOption text;
    SearchSliderOption slider;
    SearchToggleOption toggle;
    SearchSelectOption select;

    SearchOption(){}
    SearchOption(SearchTextOption text):
        text(text), type("text"){}
    SearchOption(SearchSliderOption slider):
        slider(slider), type("slider"){}
    SearchOption(SearchToggleOption toggle):
        toggle(toggle), type("toggle"){}
    SearchOption(SearchSelectOption select):
        select(select), type("select"){}
};

class Search {
    public:

    vector<SearchOption> options;

    void append(SearchOption option) {
        options.push_back(option);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["options"].resize(0);
        for (int i = 0; i < options.size(); i++) {
            if (options[i].type == "text") res["options"].append(options[i].text.toJsonObject());
            if (options[i].type == "slider") res["options"].append(options[i].slider.toJsonObject());
            if (options[i].type == "toggle") res["options"].append(options[i].toggle.toJsonObject());
            if (options[i].type == "select") res["options"].append(options[i].select.toJsonObject());
        }
        return res;
    }
};

#endif