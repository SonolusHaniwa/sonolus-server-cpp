#ifndef ITEMLIST_H
#define ITEMLIST_H

using namespace std;

template<typename T> 
class ItemList {
    public:

    int pageCount;
    vector<T> items;
    Search search;

    ItemList(){}
    ItemList(int pageCount, Search search):
        pageCount(pageCount), search(search){}
    
    void append(T item) {
        items.push_back(item);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["pageCount"] = pageCount;
        for (int i = 0; i < items.size(); i++) 
            res["items"].append(items[i].toJsonObject());
        res["search"] = search.toJsonObject();
        return res;
    }
};

#endif