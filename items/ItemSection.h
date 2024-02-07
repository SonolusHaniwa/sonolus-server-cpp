#ifndef SECTION_H
#define SECTION_H

template<typename T>
class ItemSection {
    public:

    string title;
    string icon;
    vector<T> items;

    ItemSection(){}
    ItemSection(string title, string icon): title(title), icon(icon){}
    ItemSection(string title, string icon, vector<T> items): title(title), icon(icon), items(items){}
    
    void append(T item) {
        items.push_back(item);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["title"] = title;
        res["icon"] = icon;
        res["items"].resize(0);
        for (int i = 0; i < items.size(); i++) 
            res["items"].append(items[i].toJsonObject());
        return res;
    }
};

#endif