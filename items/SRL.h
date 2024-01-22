#ifndef SRL_H
#define SRL_H

using namespace std;

enum ResourceType {
    LevelCover,
    LevelBgm,
    LevelPreview,
    LevelData,
    SkinThumbnail,
    SkinData,
    SkinTexture,
    BackgroundThumbnail,
    BackgroundData,
    BackgroundImage,
    BackgroundConfiguration,
    EffectThumbnail,
    EffectData,
    EffectAudio,
    ParticleThumbnail,
    ParticleData,
    ParticleTexture,
    EngineThumbnail,
    EngineData,
    EngineRom,
    EngineConfiguration,
    ServerBanner,
	EnginePlayData,
	EngineTutorialData,
	EnginePreviewData,
	EngineWatchData,
	ReplayData,
	ReplayConfiguration
};

string ResourceTypeString[] = {
    "LevelCover",
    "LevelBgm",
    "LevelPreview",
    "LevelData",
    "SkinThumbnail",
    "SkinData",
    "SkinTexture",
    "BackgroundThumbnail",
    "BackgroundData",
    "BackgroundImage",
    "BackgroundConfiguration",
    "EffectThumbnail",
    "EffectData",
    "EffectAudio",
    "ParticleThumbnail",
    "ParticleData",
    "ParticleTexture",
    "EngineThumbnail",
    "EngineData",
    "EngineRom",
    "EngineConfiguration",
    "ServerBanner",
	"EnginePlayData",
	"EngineTutorialData",
	"EnginePreviewData",
	"EngineWatchData",
	"ReplayData",
	"ReplayConfiguration"
};

template<ResourceType T>
class SRL {
    public: 
    
    ResourceType type = T;
    string hash;
    string url;

    SRL(){}
    SRL(string hash, string url):
        hash(hash), url(url){};
    SRL(Json::Value arr) {
        hash = arr["hash"].asString();
        url = arr["url"].asString();
    }
    
    Json::Value toJsonObject() {
        Json::Value res;
        res["type"] = ResourceTypeString[type];
        res["hash"] = hash;
        res["url"] = url;
        return res;
    }
};

#endif
