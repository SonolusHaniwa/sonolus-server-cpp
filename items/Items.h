#include"SRL.h"
#include"Search.h"

Search LevelSearch, SkinSearch, BackgroundSearch, EffectSearch, ParticleSearch, EngineSearch;

#include"Section.h"
#include"ItemList.h"
#include"ItemDetails.h"
#include"SkinItem.h"
#include"BackgroundItem.h"
#include"EffectItem.h"
#include"ParticleItem.h"
#include"EngineItem.h"
#include"LevelItem.h"

Search readJson(string path) {
    ifstream fin(path);
    if (!fin) {
        writeLog(LOG_LEVEL_ERROR, ("Failed to open config file \"" + path + "\"").c_str());
        exit(3);
    }
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    fin.seekg(0, ios::beg);
    char* ch = new char[len];
    fin.read(ch, len);
    fin.close();

    Json::Value json;
    if (!json_decode(ch, json)) {
        writeLog(LOG_LEVEL_ERROR, ("Invalid json file in \"" + path + "\"").c_str()); 
        exit(3);
    }
    free(ch);

    Search search;
    json = json["options"];
    for (int i = 0; i < json.size(); i++) {
        if (json[i]["type"].asString() == "text") {
            SearchTextOption text = SearchTextOption(
                json[i]["query"].asString(), 
                json[i]["name"].asString(), 
                json[i]["placeholder"].asString()
            ); search.append(SearchOption(text));
        } else if (json[i]["type"].asString() == "slider") {
            SearchSliderOption slider = SearchSliderOption(
                json[i]["query"].asString(), 
                json[i]["name"].asString(),
                json[i]["def"].asInt(), 
                json[i]["min"].asInt(), 
                json[i]["max"].asInt(),
                json[i]["step"].asInt(),
                json[i].isMember("unit") ? "" : json[i]["unit"].asString()
            ); search.append(slider);
        } else if (json[i]["type"].asString() == "toggle") {
            SearchToggleOption toggle = SearchToggleOption(
                json[i]["query"].asString(),
                json[i]["name"].asString(),
                json[i]["def"].asInt()
            ); search.append(toggle);
        } else if (json[i]["type"].asString() == "select") {
            SearchSelectOption select = SearchSelectOption(
                json[i]["query"].asString(),
                json[i]["name"].asString(),
                json[i]["def"].asInt(),
                vector<string>()
            );
            for (int j = 0; j < json[i]["values"].size(); i++) 
                select.values.push_back(json[i]["values"][j].asString());
            search.append(select);
        } else {
            writeLog(LOG_LEVEL_ERROR, ("Invalid search option type \"" + 
                json[i]["type"].asString() + "\" in file \"" + path + "\"").c_str());  
            exit(3);
        }
    }
    return search;
}

void loadConfig() {
    LevelSearch = readJson("./config/level_search.json");
    SkinSearch = readJson("./config/skin_search.json");
    BackgroundSearch = readJson("./config/background_search.json");
    EffectSearch = readJson("./config/effect_search.json");
    ParticleSearch = readJson("./config/particle_search.json");
    EngineSearch = readJson("./config/engine_search.json");
}