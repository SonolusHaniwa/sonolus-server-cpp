#ifndef HTML_H
#define HTML_H
using namespace std;

class H {
    public:

    bool isString;
    string context;

    bool doubleTags = false;
    string tagName;
    map<string, string> properties;
    vector<H> sons;

    H(){}
    H(string context): isString(true), context(context){}
    H(bool doubleTags, string tagName): isString(false), doubleTags(doubleTags), tagName(tagName){}
    H(bool doubleTags, string tagName, string context): 
        isString(false), doubleTags(doubleTags), tagName(tagName) {
        H node = H();
        node.isString = true;
        node.context = context;
        sons.push_back(node);
    }
    string& operator [] (string key) {
        return properties[key];
    }
    H& operator [] (int key) {
        return sons[key];
    }
    H& append(H node) {
        sons.push_back(node);
        return sons.back();
    }
    string output() {
        if (isString) return context;
        stringstream buffer;
        buffer << "<" << tagName;
        for (auto property : properties)
            buffer << " " << property.first << "=\"" << property.second << "\"";
        if (!doubleTags) {
            buffer << "/>";
            return buffer.str();
        } else { 
            buffer << ">";
            for (int i = 0; i < sons.size(); i++) buffer << sons[i].output();
            buffer << "</" << tagName << ">";
        } return buffer.str();
    }
};

H fetchOpenInSonolus(string url) {
    string source = readFile("./web/html/components/open_in_sonolus.html");
    argvar args;
    args["url"] = url;
    return str_replace(source, args);
}

H indexButton(string icon, string text, string link) {
    string source = readFile("./web/html/components/indexButton.html");
    argvar args;
    args["icon"] = icon;
    args["text"] = text;
    args["link"] = link;
    return str_replace(source, args);
}

string disableClass = "router-link-active router-link-exact-active flex select-none space-x-2 p-2 transition-colors sm:space-x-3 sm:p-3 pointer-events-none bg-sonolus-ui-button-disabled text-sonolus-ui-text-disabled";
string enableClass = "router-link-active router-link-exact-active flex select-none space-x-2 p-2 transition-colors sm:space-x-3 sm:p-3 cursor-pointer bg-sonolus-ui-button-normal hover:bg-sonolus-ui-button-highlighted active:bg-sonolus-ui-button-pressed";
H fetchBottomBar(string sonolusUrl, string topUrl, string previousUrl, string nextUrl, string bottomUrl, string searchUrl, string jumpUrl, int currentPage, int totalPage) {
    string source = readFile("./web/html/components/bottomBar.html");
    argvar args;
    args["class.previous"] = currentPage > 0 ? enableClass : disableClass;
    args["class.next"] = currentPage < totalPage - 1 ? enableClass : disableClass;
    args["url.sonolus"] = sonolusUrl;
    args["url.top"] = topUrl;
    args["url.previous"] = previousUrl;
    args["url.next"] = nextUrl;
    args["url.bottom"] = bottomUrl;
    args["url.search"] = searchUrl;
    args["url.jump"] = jumpUrl;
    args["pages.current"] = to_string(currentPage + 1);
    args["pages.all"] = to_string(totalPage);
    return str_replace(source, args);
}

H fetchSearchText(string query, string name, string placeholder, string def, string realDef, bool isMargin, bool isRequired) {
    string source = readFile("./web/html/components/searchText.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.placeholder"] = placeholder;
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    args["search.default"] = def;
    args["search.isRequired"] = to_string(isRequired);
    args["search.realDefault"] = realDef;
    return str_replace(source, args);
}

H fetchSearchToggle(string query, string name, string def, string realDef, bool isMargin, bool isRequired) {
    string source = readFile("./web/html/components/searchToggle.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.default"] = def;
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    args["search.isRequired"] = to_string(isRequired);
    args["search.realDefault"] = realDef;
    return str_replace(source, args);
}

H fetchSearchSelect(string query, string name, vector<pair<string, string> > options, string def, string realDef, bool isMargin, bool isRequired) {
    string source = readFile("./web/html/components/searchSelect.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.default"] = def;
    args["search.options"] = "";
    for (int i = 0; i < options.size(); i++) {
        H optionsObject = H(true, "option", options[i].second);
        optionsObject["class"] = "bg-sonolus-ui-surface";
        optionsObject["id"] = "search-" + query + "-" + to_string(i);
        optionsObject["value"] = options[i].first;
        args["search.options"] += optionsObject.output();
    }
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    args["search.isRequired"] = to_string(isRequired);
    args["search.realDefault"] = realDef;
    return str_replace(source, args);
}

H fetchSearchSlider(string query, string name, string def, string realDef, int min, int max, int step, bool isMargin, bool isRequired) {
    string source = readFile("./web/html/components/searchSlider.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.default"] = def;
    args["search.min"] = to_string(min);
    args["search.max"] = to_string(max);
    args["search.step"] = to_string(step);
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    args["search.isRequired"] = to_string(isRequired);
    args["search.realDefault"] = realDef;
    return str_replace(source, args);
}

H fetchSearchFile(string query, string name, string def, string realDef, bool isMargin, bool isRequired) {
    string source = readFile("./web/html/components/searchFile.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    args["search.def"] = def;
    args["search.isRequired"] = to_string(isRequired);
    args["search.realDefault"] = realDef;
    return str_replace(source, args);
}

H fetchSearchTitle(string name, int level, bool isMargin) {
    string source = readFile("./web/html/components/searchTitle.html");
    argvar args;
    args["search.level"] = to_string(level);
    args["search.name"] = name;
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    args["search.isStyle"] = isMargin ? "style=\"padding-top: " + to_string((7 - level) * 12) + "px\"" : "";
    return str_replace(source, args);
}

H fetchSearchTextArea(string query, string name, string placeholder, string def, string realDef, bool isMargin, bool isRequired) {
    string source = readFile("./web/html/components/searchTextArea.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.placeholder"] = placeholder;
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    args["search.default"] = def;
    args["search.isRequired"] = to_string(isRequired);
    args["search.realDefault"] = realDef;
    return str_replace(source, args);
}

H fetchSearchServerItem(string query, string name, string itemType, string def, string realDef, string localization, bool isMargin, bool isRequired) {
    string source = readFile("./web/html/components/searchSelect.html");
    argvar args, options;
    string tableName = itemType;
    tableName[0] += 'A' - 'a';
    string sql = "SELECT id, name, title FROM " + tableName + " WHERE (localization = \"" + localization +
        "\" OR localization = \"default\") ORDER BY CASE WHEN localization = \"default\" THEN 1 WHEN localization != \"default\" THEN 0 END ASC";
    sql = "SELECT * FROM (" + sql + ") AS A GROUP BY name";
    sql = "SELECT * FROM (" + sql + ") AS B ORDER BY id ASC";
    auto result = db.query(sql, tableName);
    for (int i = 0; i < result.size(); i++) options[result[i]["name"]] = result[i]["title"];
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.default"] = def;
    args["search.options"] = "";
    options[""] = "Default";
    for (auto v : options) {
        H optionsObject = H(true, "option", v.second);
        optionsObject["class"] = "bg-sonolus-ui-surface";
        optionsObject["id"] = "search-" + query + "-" + v.first;
        optionsObject["value"] = v.first;
        args["search.options"] += optionsObject.output();
    }
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    args["search.isRequired"] = to_string(isRequired);
    args["search.realDefault"] = realDef;
    return str_replace(source, args);
}

H fetchSearchLocalizationItem(string query, string name, string def, string realDef, bool isMargin, bool isRequired) {
    string source = readFile("./web/html/components/searchSelect.html");
    argvar args; map<string, string> options = { { "default", "default" } };
    for (int i = 0; i < i18n_raw.size(); i++) options[i18n_raw[i]["name"].asString()] = i18n_raw[i]["name"].asString();
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.default"] = def;
    args["search.options"] = "";
    for (auto v : options) {
        H optionsObject = H(true, "option", v.second);
        optionsObject["class"] = "bg-sonolus-ui-surface";
        optionsObject["id"] = "search-" + query + "-" + v.first;
        optionsObject["value"] = v.first;
        args["search.options"] += optionsObject.output();
    }
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    args["search.isRequired"] = to_string(isRequired);
    args["search.realDefault"] = realDef;
    return str_replace(source, args);
}

H fetchSearchColor(string query, string name, string def, bool isMargin) {
    string source = readFile("./web/html/components/searchColor.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.default"] = def;
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    return str_replace(source, args);
}

H fetchSearchMulti(string query, string name, vector<string> defs, vector<pair<string, string> > values, vector<string> realDefs, bool isMargin, bool isRequired) {
    string source = readFile("./web/html/components/searchMulti.html");
    argvar args;
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.default"] = "[" + [&](){
        string res = "";
        for (int i = 0; i < defs.size(); i++) res += "\"" + defs[i] + "\"" + (i != defs.size() - 1 ? "," : "");
        return res;
    }() + "]";
    args["search.total"] = "[" + [&](){
        string res = "";
        for (int i = 0; i < values.size(); i++) res += "\"" + values[i].first + "\"" + (i != values.size() - 1 ? "," : "");
        return res;
    }() + "]";
    args["search.realDefault"] = "[" + [&](){
        string res = "";
        for (int i = 0; i < realDefs.size(); i++) res += "\"" + realDefs[i] + "\"" + (i != realDefs.size() - 1 ? "," : "");
        return res;
    }() + "]";
    args["search.isRequired"] = to_string(isRequired);
    args["search.options"] = "";
    for (int i = 0; i < values.size(); i++) {
        string source = readFile("./web/html/components/searchMultiOption.html");
        argvar args2;
        args2["id"] = "search-option-" + query + "-" + to_string(i);
        args2["text"] = values[i].second;
        args2["name"] = query;
        args2["index"] = values[i].first;
        args["search.options"] += str_replace(source, args2);
    }
    return str_replace(source, args);
}

H fetchSearchServerItems(string query, string name, string itemType, vector<string> def, vector<string> realDef, string localization, bool isMargin, bool isRequired) {
    argvar args, options;
    string tableName = itemType;
    tableName[0] += 'A' - 'a';
    string sql = "SELECT id, name, title FROM " + tableName + " WHERE (localization = \"" + localization +
        "\" OR localization = \"default\") ORDER BY CASE WHEN localization = \"default\" THEN 1 WHEN localization != \"default\" THEN 0 END ASC";
    sql = "SELECT * FROM (" + sql + ") AS A GROUP BY name";
    sql = "SELECT * FROM (" + sql + ") AS B ORDER BY id ASC";
    auto result = db.query(sql, tableName);
    vector<pair<string, string> > values;
    for (int i = 0; i < result.size(); i++) values.push_back({ result[i]["name"], result[i]["title"] });
    return fetchSearchMulti(query, name, def, values, realDef, isMargin, isRequired);
}

H fetchSectionSearch(string searchOptions, string url, string type, bool hasHelp, string helpText) {
    string source = readFile("./web/html/components/sectionSearch.html");
    argvar args;
    args["html.searchOptions"] = searchOptions;
    args["url"] = url;
    args["type"] = type;
    args["hasHelp"] = hasHelp ? "style=\"margin-right: 10px;\"" : "style=\"display: none;\"";
    args["helpText"] = helpText;
    return str_replace(source, args);
}

H fetchSectionCreate(string searchOptions, string url, string returnUrl, string type, string id = "-1") {
    string source = readFile("./web/html/components/sectionCreate.html");
    argvar args;
    args["html.searchOptions"] = searchOptions;
    args["url"] = url;
    args["returnUrl"] = returnUrl;
    args["type"] = type;
    args["id"] = id;
    return str_replace(source, args);
}

H fetchNavBar(string title) {
    string navSource = readFile("./web/html/components/navbar.html");
    string langSource = readFile("./web/html/components/language.html");
    string htmlLangList = "";
    for (int i = 0; i < i18n_raw.size(); i++) {
        argvar args;
        args["name"] = i18n_raw[i]["name"].asString();
        args["title"] = i18n_raw[i]["title"].asString();
        htmlLangList += str_replace(langSource, args);
    } argvar args;
    args["title"] = title;
    args["html.language"] = htmlLangList;
    return str_replace(navSource, args);
}

H fetchIconButton(string url, string icon) {
    string source = readFile("./web/html/components/iconButton.html");
    argvar args;
    args["url"] = url;
    args["icon"] = icon;
    return str_replace(source, args);
}

H fetchIconTextButton(string onclick, string icon, string text) {
    string source = readFile("./web/html/components/iconTextButton.html");
    argvar args;
    args["onclick"] = onclick;
    args["icon"] = "{{icon." + icon + "}}";
    args["text"] = text;
    return str_replace(source, args);
}

H fetchSectionTitle(string title, string searchUrl, string listUrl, bool hasHelp, string helpText) {
    string source = readFile("./web/html/components/sectionTitle.html");
    argvar args;
    args["title"] = title;
    if (searchUrl == "") args["search.isDisplay"] = "style=\"display: none;\"";
    else args["search.isDisplay"] = "";
    args["searchUrl"] = searchUrl;
    if (listUrl == "") args["list.isDisplay"] = "style=\"display: none;\"";
    else args["list.isDisplay"] = "";
    args["listUrl"] = listUrl;
    args["hasHelp"] = hasHelp ? "" : "style=\"display: none;\"";
    args["helpText"] = helpText;
    return str_replace(source, args);
}

H fetchSectionBottom(string searchUrl, string listUrl, bool hasHelp, string helpText) {
    string source = readFile("./web/html/components/sectionBottom.html");
    argvar args;
    if (searchUrl == "") args["search.isDisplay"] = "style=\"display: none;\"";
    else args["search.isDisplay"] = "";
    args["searchUrl"] = searchUrl;
    if (listUrl == "") args["list.isDisplay"] = "style=\"display: none;\"";
    else args["list.isDisplay"] = "";
    args["listUrl"] = listUrl;
    args["hasHelp"] = hasHelp ? "" : "style=\"display: none;\"";
    args["helpText"] = helpText;
    return str_replace(source, args);
}

H fetchRecord(argvar args) {
    string source = readFile("./web/html/components/records.html");
    return str_replace(source, args);
}

vector<string> iconName = {
    "advanced", "award", "bookmark", "crown", "heart", 
    "medal", "ranking", "search", "shuffle", "star", 
    "thumbsUp", "thumbsDown", "trophy", "home", "global", 
    "post", "playlist", "level", "skin", "background", 
    "effect", "particle", "engine", "replay", "login", 
    "logout", "description", "tags", "more", "heartHollow",
    "comment", "community", "reply", "configuration", "question"
};
argvar iconList;
bool iconLoader = [](){
    iconList["icon."] = ""; // 避免空 Icon 名导致错误
    for (int i = 0; i < iconName.size(); i++) {
        string svg = readFile("./web/html/icons/" + iconName[i] + ".svg");
        iconList["icon." + iconName[i]] = svg;
    } return true;
}();

#endif