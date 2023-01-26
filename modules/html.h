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

H fetchComponentsTitle(string searchUrl, string listUrl, string langId) {
    string source = readFile("./web/html/components/componentTitle.html");
    argvar args;
    args["searchUrl"] = searchUrl;
    args["listUrl"] = listUrl;
    args["title"] = "{{language." + langId + "}}";
    return str_replace(source, args);
}

H fetchComponentsBottom(string searchUrl, string listUrl) {
    string source = readFile("./web/html/components/componentBottom.html");
    argvar args;
    args["searchUrl"] = searchUrl;
    args["listUrl"] = listUrl;
    return str_replace(source, args);
}

#endif