#include"SRL.h"
#include"Search.h"

Search LevelSearch, SkinSearch, BackgroundSearch, EffectSearch, ParticleSearch, EngineSearch;
Search LevelCreate, SkinCreate, BackgroundCreate, EffectCreate, ParticleCreate, EngineCreate;

#include"Section.h"
#include"ItemList.h"
#include"ItemDetails.h"
#include"SkinItem.h"
#include"BackgroundItem.h"
#include"EffectItem.h"
#include"ParticleItem.h"
#include"EngineItem.h"
#include"LevelItem.h"
// #include"UserProfile.h"
using namespace std;

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

    Json::Value json = Json::Value();
    string str = ""; for (int i = 0; i < len; i++) str.push_back(ch[i]);
    if (!json_decode(str, json)) {
        writeLog(LOG_LEVEL_ERROR, ("Invalid json file in \"" + path + "\"").c_str()); 
        exit(3);
    }
    delete[] ch;

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
            for (int j = 0; j < json[i]["values"].size(); j++) 
                select.values.push_back(json[i]["values"][j].asString());
            search.append(select);
        } else if (json[i]["type"].asString() == "file") {
            SearchFileOption file = SearchFileOption(
                json[i]["query"].asString(),
                json[i]["name"].asString()
            ); search.append(file);
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
    LevelCreate = readJson("./config/level_create.json");
    SkinSearch = readJson("./config/skin_search.json");
    SkinCreate = readJson("./config/skin_create.json");
    BackgroundSearch = readJson("./config/background_search.json");
    BackgroundCreate = readJson("./config/background_create.json");
    EffectSearch = readJson("./config/effect_search.json");
    EffectCreate = readJson("./config/effect_create.json");
    ParticleSearch = readJson("./config/particle_search.json");
    ParticleCreate = readJson("./config/particle_create.json");
    EngineSearch = readJson("./config/engine_search.json");
    EngineCreate = readJson("./config/engine_create.json");
    string json = readFile("./i18n/index.json");
    Json::Value index; json_decode(json, index);
    for (int i = 0; i < index.size(); i++) {
        json = readFile(index[i]["path"].asString());
        json_decode(json, i18n[index[i]["name"].asString()]);
    } i18n_raw = index;
}