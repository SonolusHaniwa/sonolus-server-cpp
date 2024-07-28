#ifndef SECTION_H
#define SECTION_H

template<typename T>
class ItemSection {
    public:

    string title = "";
    string icon = "'";
    string itemType = "";
    vector<T> items = {};
    Search search = Search();
    string searchValues = "";
    
    void append(T item) {
        items.push_back(item);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["title"] = title;
        res["icon"] = icon;
        res["itemType"] = itemType;
        res["items"].resize(0);
        for (int i = 0; i < items.size(); i++) 
            res["items"].append(items[i].toJsonObject());
        if (search.type != "quick") res["search"] = search.toJsonObject();
        if (searchValues != "") res["searchValues"] = searchValues;
        return res;
    }
};

#endif