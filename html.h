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

#endif