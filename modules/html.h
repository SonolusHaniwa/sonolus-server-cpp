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

H fetchOpenInSonolus(string url) {
    string source = readFile("./web/html/components/open_in_sonolus.html");
    argvar args;
    args["url"] = url;
    return str_replace(source, args);
}

H fetchIndexTitle(string searchUrl, string listUrl, string langId) {
    string source = readFile("./web/html/components/indexTitle.html");
    argvar args;
    args["searchUrl"] = searchUrl;
    args["listUrl"] = listUrl;
    args["title"] = "{{language." + langId + "}}";
    return str_replace(source, args);
}

H fetchIndexBottom(string searchUrl, string listUrl) {
    string source = readFile("./web/html/components/indexBottom.html");
    argvar args;
    args["searchUrl"] = searchUrl;
    args["listUrl"] = listUrl;
    return str_replace(source, args);
}

string disableClass = "router-link-active router-link-exact-active flex select-none space-x-2 p-2 transition-colors sm:space-x-3 sm:p-3 pointer-events-none bg-sonolus-ui-button-disabled text-sonolus-ui-text-disabled";
string enableClass = "router-link-active router-link-exact-active flex select-none space-x-2 p-2 transition-colors sm:space-x-3 sm:p-3 cursor-pointer bg-sonolus-ui-button-normal hover:bg-sonolus-ui-button-highlighted active:bg-sonolus-ui-button-pressed";
H fetchBottomBar(string sonolusUrl, string topUrl, string previousUrl, string nextUrl, string bottomUrl, int currentPage, int totalPage) {
    string source = readFile("./web/html/components/bottomBar.html");
    argvar args;
    args["class.previous"] = currentPage > 0 ? enableClass : disableClass;
    args["class.next"] = currentPage < totalPage - 1 ? enableClass : disableClass;
    args["url.sonolus"] = sonolusUrl;
    args["url.top"] = topUrl;
    args["url.previous"] = previousUrl;
    args["url.next"] = nextUrl;
    args["url.bottom"] = bottomUrl;
    args["pages.current"] = to_string(currentPage + 1);
    args["pages.all"] = to_string(totalPage);
    return str_replace(source, args);
}

#endif