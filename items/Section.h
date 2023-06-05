#ifndef SECTION_H
#define SECTION_H

template<typename T>
class Section {
    public:

    int pageCount = -1;
    vector<T> items;
    Search search;

    Section(){}
    Section(int pageCount, Search search): pageCount(pageCount), search(search){}
    Section(int pageCount, vector<T> items, Search search): pageCount(pageCount), items(items), search(search){}
    
    void append(T item) {
        items.push_back(item);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["pageCount"] = pageCount;
        res["items"].resize(0);
        for (int i = 0; i < items.size(); i++)
            res["items"].append(items[i].toJsonObject());
        res["search"] = search.toJsonObject();
        return res;
    }
};

#endif