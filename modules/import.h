using namespace std;

int importedLevelNumber = 0;
int importedSkinNumber = 0;
int importedBackgroundNumber = 0;
int importedEffectNumber = 0;
int importedParticleNumber = 0;
int importedEngineNumber = 0;
int importedReplayNumber = 0;

void serverPackageImport(FileStream& fin) {
    int filenum = fin.getInteger();
    for (int i = 1; i <= filenum; i++) {
        __int64_t filesize = 0;
        string fileSha1Hex = fin.getBuffer(20);
        stringstream fileSha1;
        for (int i = 0; i < 20; i++) fileSha1 << hex << setw(2) << setfill('0') << int((unsigned char)fileSha1Hex[i]);
        filesize = fin.getInteger();
        string file = fin.getBuffer(filesize);
        string fileSha1Real = getSha(file);
        if (fileSha1Real != fileSha1.str()) {
            writeLog(LOG_LEVEL_ERROR, "Invalid file sha1 hash. Read \"" + fileSha1.str() + "\" but expected \"" + fileSha1Real + "\".");
            writeLog(LOG_LEVEL_DEBUG, to_string(i - 1) + " files were written.");
            writeLog(LOG_LEVEL_DEBUG, "0 raws were affected.");
            exit(3);
        } FileStream fout("./data/" + fileSha1.str(), 1);
        fout.writeBuffer(file);
        writeLog(LOG_LEVEL_DEBUG, "Saved file " + fileSha1.str() + " into data.");
    } int jsonnum = fin.getInteger(), effected_raws = 0;
    for (int i = 1; i <= jsonnum; i++) {
        int jsonsize = 0; string json; int type = fin.getBuffer(1)[0] - '0';
        jsonsize = fin.getInteger();
        json = fin.getBuffer(jsonsize);
        Json::Value arr; json_decode(json, arr);
        switch(type) {
            case 1: effected_raws += levelsCreate(LevelItem(-1, arr)); importedLevelNumber++; break;
            case 2: effected_raws += skinsCreate(SkinItem(-1, arr)); importedSkinNumber++; break;
            case 3: effected_raws += backgroundsCreate(BackgroundItem(-1, arr)); importedBackgroundNumber++; break;
            case 4: effected_raws += effectsCreate(EffectItem(-1, arr)); importedEffectNumber++; break;
            case 5: effected_raws += particlesCreate(ParticleItem(-1, arr)); importedParticleNumber++; break;
            case 6: effected_raws += enginesCreate(EngineItem(-1, arr)); importedEngineNumber++; break;
            case 7: effected_raws += replaysCreate(ReplayItem(-1, arr)); importedReplayNumber++; break;
        }
    }
    writeLog(LOG_LEVEL_INFO, "Import data successfully.");
    writeLog(LOG_LEVEL_DEBUG, to_string(filenum) + " files were written.");
    writeLog(LOG_LEVEL_DEBUG, to_string(effected_raws) + " raws were affected.");
    writeLog(LOG_LEVEL_DEBUG, to_string(importedLevelNumber) + " levels were imported.");
    writeLog(LOG_LEVEL_DEBUG, to_string(importedSkinNumber) + " skins were imported.");
    writeLog(LOG_LEVEL_DEBUG, to_string(importedBackgroundNumber) + " backgrounds were imported.");
    writeLog(LOG_LEVEL_DEBUG, to_string(importedEffectNumber) + " effects were imported.");
    writeLog(LOG_LEVEL_DEBUG, to_string(importedParticleNumber) + " particles were imported.");
    writeLog(LOG_LEVEL_DEBUG, to_string(importedEngineNumber) + " engines were imported.");
    writeLog(LOG_LEVEL_DEBUG, to_string(importedReplayNumber) + " replays were imported.");
}

void importDataFromOfficialPackage(string path, Json::Value obj, int& fileCnt) {
    if (obj.isObject() && obj.isMember("url") && obj.isMember("hash")) {
    	// if (obj["url"].asString().size() == 0) {
    	// 	writeLog(LOG_LEVEL_WARNING, "Empty file info.");
    	// 	return;
    	// } 
    	if (obj["url"].asString().size() == 0) return;
    	string fileContent = getFileFromZip(path, obj["url"].asString().substr(1));
        ofstream fout("./data/" + obj["hash"].asString());
        fout.write(const_cast<char*>(fileContent.c_str()), fileContent.size());
        fout.close();
        writeLog(LOG_LEVEL_DEBUG, "Saved file " + obj["hash"].asString() + " into data.");
        fileCnt++; return;
    } if (obj.isArray() || obj.isObject()) for (auto v : obj) importDataFromOfficialPackage(path, v, fileCnt);
}

void detectDataFromOfficialPackage(string path, Json::Value obj, set<string>& data) {
    if (obj.isObject() && obj.isMember("url") && obj.isMember("hash")) {
        data.insert(obj["hash"].asString()); return;
    } if (obj.isArray() || obj.isObject()) for (auto v : obj) detectDataFromOfficialPackage(path, v, data);
}

void officialPackageImport(string path) {
    string supportComponents[] = {"levels", "skins", "backgrounds", "effects", "particles", "engines", "replays"};
    int files = 0, raws = 0;
    for (int i = 0; i < 7; i++) {
        string filePath = "sonolus/" + supportComponents[i] + "/list";
        string json = getFileFromZip(path, filePath);
        Json::Value obj; json_decode(json, obj); int fileCnt = 0;
        importDataFromOfficialPackage(path, obj, fileCnt);
        for (int j = 0; j < obj["items"].size(); j++) {
            Json::Value arr = obj["items"][j];
            switch(i) {
            	case 0: {
            		LevelItem level = LevelItem(-1, arr);
            		raws += skinsCreate(level.engine.skin);
            		raws += backgroundsCreate(level.engine.background);
            		raws += effectsCreate(level.engine.effect);
            		raws += particlesCreate(level.engine.particle);
            		raws += enginesCreate(level.engine);
            		if (level.useSkin.useDefault == false) raws += skinsCreate(level.useSkin.item);
            		if (level.useBackground.useDefault == false) raws += backgroundsCreate(level.useBackground.item);
            		if (level.useEffect.useDefault == false) raws += effectsCreate(level.useEffect.item);
            		if (level.useParticle.useDefault == false) raws += particlesCreate(level.useParticle.item);
            		raws += levelsCreate(level);
            	} break;
                case 1: raws += skinsCreate(SkinItem(-1, arr)); break;
                case 2: raws += backgroundsCreate(BackgroundItem(-1, arr)); break;
                case 3: raws += effectsCreate(EffectItem(-1, arr)); break;
                case 4: raws += particlesCreate(ParticleItem(-1, arr)); break;
            	case 5: {
            		EngineItem engine = EngineItem(-1, arr);
            		raws += skinsCreate(engine.skin);
            		raws += backgroundsCreate(engine.background);
            		raws += effectsCreate(engine.effect);
            		raws += particlesCreate(engine.particle);
            		raws += enginesCreate(engine);
            	} break;
            	case 6: {
            		ReplayItem replay = ReplayItem(-1, arr);
            		LevelItem level = replay.level;
            		raws += skinsCreate(level.engine.skin);
            		raws += backgroundsCreate(level.engine.background);
            		raws += effectsCreate(level.engine.effect);
            		raws += particlesCreate(level.engine.particle);
            		raws += enginesCreate(level.engine);
            		if (level.useSkin.useDefault == false) raws += skinsCreate(level.useSkin.item);
            		if (level.useBackground.useDefault == false) raws += backgroundsCreate(level.useBackground.item);
            		if (level.useEffect.useDefault == false) raws += effectsCreate(level.useEffect.item);
            		if (level.useParticle.useDefault == false) raws += particlesCreate(level.useParticle.item);
            		raws += levelsCreate(level);
            		raws += replaysCreate(replay);
            	} break;
            }
        } files += fileCnt;
    } writeLog(LOG_LEVEL_INFO, "Import data successfully.");
    writeLog(LOG_LEVEL_DEBUG, to_string(files) + " files were written.");
    writeLog(LOG_LEVEL_DEBUG, to_string(raws) + " raws were affected.");
}

void import(const char* path) {
    if (!fileExist(path)) {
        writeLog(LOG_LEVEL_ERROR, "Failed to import: Couldn't open binary file!");
        exit(3);
    } FileStream fin(path, 0);
    string head = fin.getBuffer(4);
    if (head == fileHeader) serverPackageImport(fin);
    else officialPackageImport(path);
}

int displayMax = 30;

void serverPackageTest(FileStream& fin) {
    int dataSize = fin.getInteger();
    set<string> data;
    for (int i = 0; i < dataSize; i++) {
        string hashHex = fin.getBuffer(20);
        int size = fin.getInteger();
        string content = fin.getBuffer(size);
        stringstream hash;
        for (int j = 0; j < 20; j++) hash << hex << setw(2) << setfill('0') << int((unsigned char)hashHex[j]);
        data.insert(hash.str());
    } int jsonSize = fin.getInteger();
    cout << "Package Type: Sonolus Resources Package (.srp) made by LittleYang0531" << endl;
    cout << "Components Number: " << jsonSize << endl;
    cout << "Resources Number: " << data.size() << endl;
    cout << "Resource List: [" << endl;
    int pt = 0; for (auto v : data) {
        pt++; if (pt > displayMax) {
            cout << "    ... and other " + to_string(data.size() - displayMax) + " files" << endl;
            break;
        } cout << "    \"" + v + "\"" + (pt == data.size() ? "" : ",") << endl;
    } cout << "]" << endl;
}

void officialPackageTest(string path) {
    set<string> data; int jsonSize = 0;
    string supportComponents[] = {"levels", "skins", "backgrounds", "effects", "particles", "engines", "replays"};
    for (int i = 0; i < 7; i++) {
        string filePath = "sonolus/" + supportComponents[i] + "/list";
        string json = getFileFromZip(path, filePath);
        Json::Value obj; json_decode(json, obj); 
        detectDataFromOfficialPackage(path, obj, data);
        jsonSize += obj["items"].size();
    } cout << "Package Type: Sonolus Collection Package (.scp) made by NonSpicyBurrito" << endl;
    cout << "Components Number: " << jsonSize << endl;
    cout << "Resources Number: " << data.size() << endl;
    cout << "Resource List: [" << endl;
    int pt = 0; for (auto v : data) {
        pt++; if (pt > displayMax) {
            cout << "    ... and other " + to_string(data.size() - displayMax) + " files" << endl;
            break;
        } cout << "    \"" + v + "\"" + (pt == data.size() ? "" : ",") << endl;
    } cout << "]" << endl;
}

void testPackage(string path) {
    if (!fileExist(path)) {
        writeLog(LOG_LEVEL_ERROR, "Failed to test: Couldn't open binary file!");
        exit(3);
    } FileStream fin(path, 0);
    string head = fin.getBuffer(4);
    if (head == fileHeader) serverPackageTest(fin);
    else officialPackageTest(path);
}
