#ifndef ITEMLIST_H
#define ITEMLIST_H

using namespace std;

template<typename T> 
class ItemList {
    public:

    int pageCount;
    vector<T> items;
    vector<Search> searches;

    ItemList(){}
    ItemList(int pageCount, vector<Search> searches):
        pageCount(pageCount), searches(searches){}
    
    void append(T item) {
        items.push_back(item);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["pageCount"] = pageCount;
        res["items"].resize(0);
        for (int i = 0; i < items.size(); i++) 
            res["items"].append(items[i].toJsonObject());
        res["searches"].resize(0);
        for (int i = 0; i < searches.size(); i++) 
            res["searches"].append(searches[i].toJsonObject());
        return res;
    }
};

#endif