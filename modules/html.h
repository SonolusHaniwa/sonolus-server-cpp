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

H fetchSearchText(string query, string name, string placeholder, string def, bool isMargin) {
    string source = readFile("./web/html/components/searchText.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.placeholder"] = placeholder;
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    args["search.default"] = def;
    return str_replace(source, args);
}

H fetchSearchToggle(string query, string name, bool def, bool isMargin) {
    string source = readFile("./web/html/components/searchToggle.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.defaultLang"] = def ? "{{language.yes}}" : "{{language.no}}";
    args["search.default"] = to_string(def);
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    return str_replace(source, args);
}

H fetchSearchSelect(string query, string name, vector<string> options, int def, bool isMargin) {
    string source = readFile("./web/html/components/searchSelect.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.defaultValues"] = def < options.size() && def >= 0 ? options[def] : "";
    args["search.default"] = to_string(def);
    args["search.options"] = "";
    for (int i = 0; i < options.size(); i++) {
        H optionsObject = H(true, "option", options[i]);
        optionsObject["class"] = "bg-sonolus-ui-surface";
        optionsObject["id"] = "search-" + query + "-" + to_string(i);
        optionsObject["value"] = to_string(i);
        if (i == def) optionsObject["selected"] = "selected";
        args["search.options"] += optionsObject.output();
    }
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    return str_replace(source, args);
}

H fetchSearchSlider(string query, string name, int def, int min, int max, int step, bool isMargin) {
    string source = readFile("./web/html/components/searchSlider.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.default"] = to_string(def);
    args["search.min"] = to_string(min);
    args["search.max"] = to_string(max);
    args["search.step"] = to_string(step);
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    return str_replace(source, args);
}

H fetchSearchFile(string query, string name, string def, bool isMargin) {
    string source = readFile("./web/html/components/searchFile.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    args["search.def"] = def;
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

H fetchSearchColor(string query, string name, string def, bool isMargin) {
    string source = readFile("./web/html/components/searchColor.html");
    argvar args;
    args["search.query"] = query;
    args["search.name"] = name;
    args["search.default"] = def;
    args["search.isMargin"] = isMargin ? "style=\"margin-top: 12px;\"" : "";
    return str_replace(source, args);
}

H fetchSectionSearch(string searchOptions, string url, string type) {
    string source = readFile("./web/html/components/sectionSearch.html");
    argvar args;
    args["html.searchOptions"] = searchOptions;
    args["url"] = url;
    args["type"] = type;
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

vector<string> iconName = {"advanced", "award", "bookmark", "crown", "heart", "medal", "ranking", "search", "shuffle", "star", "thumbsUp", "thumbsDown", "trophy",
    "home", "global", "post", "playlist", "level", "skin", "background", "effect", "particle", "engine", "replay", "login", "logout", "description", "tags"};
argvar iconList;
bool iconLoader = [](){
    for (int i = 0; i < iconName.size(); i++) {
        string svg = readFile("./web/html/icons/" + iconName[i] + ".svg");
        iconList["icon." + iconName[i]] = svg;
    } return true;
}();

#endif