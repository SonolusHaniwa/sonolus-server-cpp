#ifndef ITEMDETAILS_H
#define ITEMDETAILS_H

using namespace std;

template<typename T>
class ItemDetails {
    public:

    T item;
    string description;
    vector<T> recommended;

    ItemDetails(){}
    ItemDetails(T item, string description):
        item(item), description(description){}
    
    void append(T recommend) {
        recommended.push_back(recommend);
    }

    Json::Value toJsonObject() {
        Json::Value res;
        res["item"] = item.toJsonObject();
        res["description"] = description;
        res["recommended"].resize(0);
        for (int i = 0; i < recommended.size(); i++) 
            res["recommended"].append(recommended[i].toJsonObject());
        return res;
    }
};

#endif