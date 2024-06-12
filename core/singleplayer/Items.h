#include"Tag.h"
#include"SRL.h"
#include"Search.h"

vector<Search> LevelSearch, SkinSearch, BackgroundSearch, EffectSearch, ParticleSearch, EngineSearch, ReplaySearch, PostSearch, PlaylistSearch, RoomSearch;
vector<Search> LevelCreate, SkinCreate, BackgroundCreate, EffectCreate, ParticleCreate, EngineCreate, ReplayCreate, PostCreate, PlaylistCreate, RoomCreate;

#include"ItemSection.h"
#include"ItemList.h"
#include"ItemDetails.h"
#include"SkinItem.h"
#include"BackgroundItem.h"
#include"EffectItem.h"
#include"ParticleItem.h"
#include"EngineItem.h"
#include"LevelItem.h"
#include"ReplayItem.h"
#include"PostItem.h"
#include"PlaylistItem.h"
#include"RoomItem.h"
#include"UserProfile.h"
#include"ItemCommunityComment.h"
using namespace std;

vector<Search> readJson(string path) {
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

    return constructSearch(json);
}

vector<string> i18n_index;

void loadConfig() {
    LevelSearch = constructSearch(appConfig["levels.searches"]);
    LevelCreate = constructSearch(appConfig["levels.creates"]);
    SkinSearch = constructSearch(appConfig["skins.searches"]);
    SkinCreate = constructSearch(appConfig["skins.creates"]);
    BackgroundSearch = constructSearch(appConfig["backgrounds.searches"]);
    BackgroundCreate = constructSearch(appConfig["backgrounds.creates"]);
    EffectSearch = constructSearch(appConfig["effects.searches"]);
    EffectCreate = constructSearch(appConfig["effects.creates"]);
    ParticleSearch = constructSearch(appConfig["particles.searches"]);
    ParticleCreate = constructSearch(appConfig["particles.creates"]);
    EngineSearch = constructSearch(appConfig["engines.searches"]);
    EngineCreate = constructSearch(appConfig["engines.creates"]);
    ReplaySearch = constructSearch(appConfig["replays.searches"]);
    ReplayCreate = constructSearch(appConfig["replays.creates"]);
    PostSearch = constructSearch(appConfig["posts.searches"]);
    PostCreate = constructSearch(appConfig["posts.creates"]);
    PlaylistSearch = constructSearch(appConfig["playlists.searches"]);
    PlaylistCreate = constructSearch(appConfig["playlists.creates"]);
    RoomSearch = constructSearch(appConfig["rooms.searches"]);
    RoomCreate = constructSearch(appConfig["rooms.creates"]);
    string json = readFile("./i18n/index.json");
    Json::Value index; json_decode(json, index);
    for (int i = 0; i < index.size(); i++) {
        json = readFile(index[i]["path"].asString());
        json_decode(json, i18n[index[i]["name"].asString()]);
        i18n_index.push_back(index[i]["name"].asString());
    } i18n_raw = index;
}
