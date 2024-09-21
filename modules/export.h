using namespace std;

// 文件操作
class FileStream {
    ifstream fin;
    ofstream fout;
    int in;

    public:
    FileStream(string path, int in = 0) {
        this->in = in;
        if (in == 0) fin.open(path.c_str(), ios::binary);
        else if (in == 1) fout.open(path.c_str(), ios::binary);
        else fout.open(path.c_str(), ios::ate | ios::in | ios::binary);
    }

    void move(__int64_t pt) {
        if (in == 0) fin.seekg(pt, ios::beg);
        else fout.seekp(pt, ios::beg);
    }

    __int64_t size() {
        if (in == 0) {
            __int64_t now = fin.tellg();
            fin.seekg(0, ios::end);
            __int64_t res = fin.tellg();
            move(now); return res;
        } else {
            __int64_t now = fout.tellp();
            fout.seekp(0, ios::end);
            __int64_t res = fout.tellp();
            move(now); return res;
        }
    }

    __int64_t get() {
        if (in == 0) return fin.tellg();
        else return fout.tellp();
    }

    bool check(__int64_t pt) {
        return pt <= size();
    }

    __int64_t getInteger() {
        if (in != 0 || !check(get() + 8)) return 0;
        char* ch = new char[8];
        fin.read(ch, 8);
        __int64_t res = 0;
        for (int i = 0; i < 8; i++) res <<= 8ll, res += (unsigned char)ch[i];
        delete[] ch; return res;
    }

    string getBuffer(__int64_t size) {
        if (in != 0 || !check(get() + size)) return "";
        char* ch = new char[size];
        fin.read(ch, size);
        string res = "";
        for (int i = 0; i < size; i++) res.push_back(ch[i]);
        delete[] ch; return res;
    }

    void writeInteger(__int64_t integer) {
        if (in == 0) return;
        vector<int> x; for (int i = 0; i < 8; i++) x.push_back(integer % (1 << 8ll)), integer >>= 8ll;
        char* ch = new char[8];
        for (int i = 0; i < 8; i++) ch[i] = (char)x[7 - i];
        fout.write(ch, 8); delete[] ch;
    }

    void writeBuffer(string buffer) {
        if (in == 0) return;
        char* ch = new char[buffer.size()];
        for (int i = 0; i < buffer.size(); i++) ch[i] = buffer[i];
        fout.write(ch, buffer.size()); delete[] ch;
    }
};

bool fileExist(string path) {
    ifstream fin(path.c_str());
    bool res = fin.good();
    fin.close();
    return res;
}

int exportLevelNumber = 0;
int exportSkinNumber = 0;
int exportBackgroundNumber = 0;
int exportEffectNumber = 0;
int exportParticleNumber = 0;
int exportEngineNumber = 0;
int exportReplayNumber = 0;

// 临时文件类
class tmpFile {
    string path;
    time_t createTime = -1;
    time_t lastTime = -1;
    int processNumber = -1;
    int processNumberPt = -1;
    int jsonNumber = -1;
    int jsonNumberPt = -1;
    vector<string> argvSource;
    vector<int> jsonPt;

    public:
    tmpFile(){}
    tmpFile(string name) {
        path = "./.tmp/" + name;
        if (!fileExist(path)) {
            writeLog(LOG_LEVEL_ERROR, "Task file \"" + path + "\" not found.");
            exit(3);
        } FileStream fin(path, 0);
        createTime = fin.getInteger();
        lastTime = fin.getInteger();
        int argc = fin.getInteger();
        for (int i = 0; i < argc; i++) argvSource.push_back(fin.getBuffer(fin.getInteger()));
        processNumberPt = fin.get(); processNumber = fin.getInteger();
        jsonNumberPt = fin.get(); jsonNumber = fin.getInteger();
        for (int i = 0; i < jsonNumber; i++) {
            jsonPt.push_back(fin.get());
            int type = fin.getBuffer(1)[0] - '0';
            switch(type) {
                case exportLevelId: exportLevelNumber++; break;
                case exportSkinId: exportSkinNumber++; break;
                case exportBackgroundId: exportBackgroundNumber++; break;
                case exportEffectId: exportEffectNumber++; break;
                case exportParticleId: exportParticleNumber++; break;
                case exportEngineId: exportEngineNumber++; break;
                case exportReplayId: exportReplayNumber++; break;
            } fin.move(fin.getInteger() + fin.get());
        } jsonPt.push_back(fin.get());
    }

    int argc() {
        return argvSource.size();
    }

    string argv(int i) {
        if (i >= argvSource.size() || i < 0) return "";
        return argvSource[i];
    }

    int jsonSize() {
        return jsonNumber;
    }

    pair<int, string> getJson(int i) {
        if (i >= jsonNumber || i < 0) return {0, ""};
        FileStream fin(path, 0);
        fin.move(jsonPt[i]);
        int type = fin.getBuffer(1)[0] - '0';
        int len = fin.getInteger();
        string res = fin.getBuffer(len);
        return {type, res};
    }

    void writeJson(int type, string json) {
        FileStream fout(path, 2);
        fout.move(jsonPt.back());
        fout.writeBuffer(to_string(type));
        fout.writeInteger(json.size());
        fout.writeBuffer(json);
        jsonPt.push_back(fout.get());
        fout.move(jsonNumberPt);
        fout.writeInteger(++jsonNumber);
    }

    int getProcess() {
        return processNumber;
    }

    void updateProcess() {
        FileStream fout(path, 2);
        fout.move(processNumberPt);
        fout.writeInteger(++processNumber);
    }
};

string getSha(string file) {
    unsigned char* hash = sha1(const_cast<char*>(file.c_str()), file.size());
    stringstream buffer;
    for (int i = 0; i < 20; i++)
        buffer << hex << setw(2) << setfill('0') << int(hash[i]);
    return buffer.str();
}

class srpFile {
    string path;
    int fileNum = 0;
    int fileNumPt = 4;
    vector<int> filePt;
    map<string, bool> fileList;
    
    public:
    srpFile(string path) {
        this->path = path;
        if (!::fileExist(path)) {
            FileStream fout(path, 1);
            fout.writeBuffer(fileHeader);
            fout.writeInteger(0);
        } FileStream fin(path, 0);
        fin.move(4); fileNum = fin.getInteger();
        for (int i = 0; i < fileNum; i++) {
            filePt.push_back(fin.get());
            string fileShaHex = fin.getBuffer(20);
            stringstream fileSha;
            for (int i = 0; i < 20; i++)
                fileSha << hex << setw(2) << setfill('0') << (int)((unsigned char)fileShaHex[i]);
            string fileShaStr = fileSha.str();
            __int64_t size = fin.getInteger();
            string file = fin.getBuffer(size);
            string realSha = getSha(file);
            if (fileSha.str() != realSha) {
                writeLog(LOG_LEVEL_ERROR, "File \"" + fileSha.str() + "\" is damaged in \"" + path + "\".");
                exit(3);
            } fileList[fileSha.str()] = 1;
        } filePt.push_back(fin.get());
    }

    void writeData(string sha, string file) {
        FileStream fout(path, 2);
        fout.move(filePt.back());
        string shaBuffer = "";
        for (int i = 0; i < sha.size(); i += 2) {
            int x = 0;
            if (sha[i] >= '0' && sha[i] <= '9') x += sha[i] - '0';
            else x += sha[i] - 'a' + 10;
            x <<= 4;
            if (sha[i + 1] >= '0' && sha[i + 1] <= '9') x += sha[i + 1] - '0';
            else x += sha[i + 1] - 'a' + 10;
            shaBuffer.push_back(x);
        } fout.writeBuffer(shaBuffer);
        fout.writeInteger(file.size());
        fout.writeBuffer(file);
        filePt.push_back(fout.get());
        fout.move(fileNumPt);
        fout.writeInteger(++fileNum);
        fileList[sha] = 1;
    }

    bool fileExist(string sha) {
        return fileList.find(sha) != fileList.end();
    }

    int size() {
        return fileNum;
    }
};

map<string, string> hashMap;

void exportSkin(SkinItem skin, tmpFile& tmp) {
    tmp.writeJson(exportSkinId, json_encode(skin.toJsonObject()));
    exportSkinNumber++;
}

void exportBackground(BackgroundItem background, tmpFile& tmp) {
    tmp.writeJson(exportBackgroundId, json_encode(background.toJsonObject()));
    exportBackgroundNumber++;
}

void exportEffect(EffectItem effect, tmpFile& tmp) {
    tmp.writeJson(exportEffectId, json_encode(effect.toJsonObject()));
    exportEffectNumber++;
}

void exportParticle(ParticleItem particle, tmpFile& tmp) {
    tmp.writeJson(exportParticleId, json_encode(particle.toJsonObject()));
    exportParticleNumber++;
}

void exportEngine(EngineItem engine, tmpFile& tmp) {
    exportSkin(engine.skin, tmp);
    exportBackground(engine.background, tmp);
    exportEffect(engine.effect, tmp);
    exportParticle(engine.particle, tmp);
    tmp.writeJson(exportEngineId, json_encode(engine.toJsonObject()));
    exportEngineNumber++;
}

void exportLevel(LevelItem level, tmpFile& tmp) {
    exportEngine(level.engine, tmp);
    if (level.useSkin.useDefault == false) exportSkin(level.useSkin.item, tmp);
    if (level.useBackground.useDefault == false) exportBackground(level.useBackground.item, tmp);
    if (level.useEffect.useDefault == false) exportEffect(level.useEffect.item, tmp);
    if (level.useParticle.useDefault == false) exportParticle(level.useParticle.item, tmp);
    tmp.writeJson(exportLevelId, json_encode(level.toJsonObject()));
    exportLevelNumber++;
}

void exportReplay(ReplayItem replay, tmpFile& tmp) {
	exportLevel(replay.level, tmp);
    tmp.writeJson(exportReplayId, json_encode(replay.toJsonObject()));
    exportReplayNumber++;
}

string solveUrl(string url) {
    auto path = explode("/", url);
    if (path.size() < 3) return "";
    string rootUrl = "";
    for (int i = 0; i < path.size() - 2; i++) rootUrl += path[i] + "/";
    if (path[path.size() - 3] != "sonolus") rootUrl += "sonolus/";
    rootUrl += path[path.size() - 2] + "/" + path[path.size() - 1];
    if (rootUrl.substr(0, 7) != "http://" && rootUrl.substr(0, 8) != "https://" && rootUrl.substr(0, 2) != "//") rootUrl = "//" + rootUrl;
    return rootUrl;
}

string getRootUrl(string url) {
    auto path = explode("/", url);
    if (path.size() < 3) return "";
    string rootUrl = "";
    for (int i = 0; i < path.size() - 3; i++) rootUrl += path[i] + "/";
    if (path[path.size() - 3] != "sonolus") rootUrl += path[path.size() - 3] + "/";
    if (rootUrl.substr(0, 7) != "http://" && rootUrl.substr(0, 8) != "https://" && rootUrl.substr(0, 2) != "//") rootUrl = "//" + rootUrl;
    return rootUrl;
}

bool isUrl(string url) {
    return url.substr(0, 7) == "http://" || url.substr(0, 8) == "https://" || url.substr(0, 2) == "//";
}

void changeUrl(string rootUrl, Json::Value& obj) {
    if (obj.isObject()) {
        if (obj.isMember("url")) {
            if (!isUrl(obj["url"].asString())) {
                string baseUrl = obj["url"].asString();
                if (baseUrl[0] == '/') baseUrl = baseUrl.substr(1);
                obj["url"] = rootUrl + baseUrl;
            }
        } else for (auto& v : obj) changeUrl(rootUrl, v);
    } else if (obj.isArray()) {
        for (auto& v : obj) changeUrl(rootUrl, v);
    }
}

string formatSize(double size, int precision = 2) {
    string unit[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    while (size >= 1024) size /= 1024, i++;
    stringstream buffer;
    buffer << fixed << setprecision(precision) << size << unit[i];
    return buffer.str();
}

time_t getMillSeconds() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

void initializeTemporary(string path, vector<string> argv) {
    argv.pop_back();
    FileStream fout(path, 1);
    fout.writeInteger(time(NULL));
    fout.writeInteger(time(NULL));
    fout.writeInteger(argv.size());
    for (int i = 0; i < argv.size(); i++) {
        fout.writeInteger(argv[i].size());
        fout.writeBuffer(argv[i]);
    } fout.writeInteger(0); fout.writeInteger(0);
}

void removeTemporary(string path) {
    #ifdef __linux__
    remove(path.c_str());
    #elif __windows__
    DeleteFile(path.c_str());
    #endif
}

void callExportError(string text) {
    writeLog(LOG_LEVEL_ERROR, text);
    removeTemporary(text);
    exit(3);
}

void remoteExport(string origUrl, tmpFile& tmp) {
    string url = solveUrl(origUrl);
    auto path = explode("/", url);
    if (path.size() < 2) {
        writeLog(LOG_LEVEL_INFO, "Invalid resource link.");
        writeLog(LOG_LEVEL_DEBUG, "0 files were written");
        exit(0);
    } int retry_time = appConfig["export.retryTime"].asInt();
    startget:
    string json = get_url(url); Json::Value obj;
    if (json_decode(json, obj) == false) {
        if (retry_time > 0) {
            writeLog(LOG_LEVEL_ERROR, "Failed to visit \"" + url + "\". retrying...(" + to_string(retry_time) + ")(" + to_string(curl_code) + ")");
            retry_time--;
            goto startget;
        } writeLog(LOG_LEVEL_ERROR, "Failed to visit \"" + url + "\".");
        writeLog(LOG_LEVEL_DEBUG, "0 files were written");
        exit(0);
    } string type = path[path.size() - 2];
    string rootUrl = getRootUrl(origUrl);
    changeUrl(rootUrl, obj); obj["item"]["description"] = obj["description"];
    if (type == "levels") exportLevel(LevelItem(-1, obj["item"]), tmp);
    if (type == "skins") exportSkin(SkinItem(-1, obj["item"]), tmp);
    if (type == "backgrounds") exportBackground(BackgroundItem(-1, obj["item"]), tmp);
    if (type == "effects") exportEffect(EffectItem(-1, obj["item"]), tmp);
    if (type == "particles") exportParticle(ParticleItem(-1, obj["item"]), tmp);
    if (type == "engines") exportEngine(EngineItem(-1, obj["item"]), tmp);
    if (type == "replays") exportReplay(ReplayItem(-1, obj["item"]), tmp);
}

void remoteFull(string url, tmpFile& tmp) {
    string components[] = {"levels", "skins", "backgrounds", "effects", "particles", "engines", "replays"}; int pt = 1;
    if (url.back() == '/') url.pop_back();
    for (int i = 0; i < 7; i++) {
        int retry_time = appConfig["export.retryTime"].asInt();
        getjson1:
        string url1 = url + "/sonolus/" + components[i] + "/list";
        string json = get_url(url1);
        Json::Value obj; 
        if (json_decode(json, obj) == false) {
            if (retry_time > 0) {
                writeLog(LOG_LEVEL_ERROR, "Failed to visit \"" + url1 + "\". retrying...(" + to_string(retry_time) + ")(" + to_string(curl_code) + ")");
                retry_time--;
                goto getjson1;
            } writeLog(LOG_LEVEL_ERROR, "Failed to visit \"" + url1 + "\".");
            exit(0);
        } int page = obj["pageCount"].asInt();
        if (page == -1) page = 1;
        for (int j = 0; j < page; j++) {
            int retry_time = appConfig["export.retryTime"].asInt();
            getjson2:
            string listUrl = url + "/sonolus/" + components[i] + "/list?page=" + to_string(j);
            string json = get_url(listUrl);
            if (json_decode(json, obj) == false) {
                if (retry_time > 0) {
                    writeLog(LOG_LEVEL_ERROR, "Failed to visit \"" + listUrl + "\". retrying...(" + to_string(retry_time) + ")(" + to_string(curl_code) + ")");
                    retry_time--;
                    goto getjson2;
                } writeLog(LOG_LEVEL_ERROR, "Failed to visit \"" + listUrl + "\".");
                exit(0);
            } for (int k = 0; k < obj["items"].size(); k++, pt++) {
                if (tmp.getProcess() >= pt) continue;
                string origUrl = url + "/sonolus/" + components[i] + "/" + obj["items"][k]["name"].asString();
                remoteExport(origUrl, tmp); tmp.updateProcess();
                writeLog(LOG_LEVEL_INFO, "Add \"" + shortenUrl(origUrl) + "\" to package queue.");
            }
        }
    }
}

// 初始化导出数据
vector<string> exportPreload(int argc, char** argv) {
    if (string(argv[2]) == "level") {
        if (argc < 5) invalidUsage();
        return {"0", argv[3], argv[4], (argc >= 6 ? argv[5] : to_string(time(NULL)))};
    } else if (string(argv[2]) == "skin") {
        if (argc < 5) invalidUsage();
        return {"1", argv[3], argv[4], (argc >= 6 ? argv[5] : to_string(time(NULL)))};
    } else if (string(argv[2]) == "background") {
        if (argc < 5) invalidUsage();
        return {"2", argv[3], argv[4], (argc >= 6 ? argv[5] : to_string(time(NULL)))};
    } else if (string(argv[2]) == "effect") {
        if (argc < 5) invalidUsage();
        return {"3", argv[3], argv[4], (argc >= 6 ? argv[5] : to_string(time(NULL)))};
    } else if (string(argv[2]) == "particle") {
        if (argc < 5) invalidUsage();
        return {"4", argv[3], argv[4], (argc >= 6 ? argv[5] : to_string(time(NULL)))};
    } else if (string(argv[2]) == "engine") {
        if (argc < 5) invalidUsage();
        return {"5", argv[3], argv[4], (argc >= 6 ? argv[5] : to_string(time(NULL)))};
    } else if (string(argv[2]) == "replay") {
        if (argc < 5) invalidUsage();
        return {"A", argv[3], argv[4], (argc >= 6 ? argv[5] : to_string(time(NULL)))};
    } else if (string(argv[2]) == "remote") {
        if (argc < 5) invalidUsage();
        return {"6", argv[3], argv[4], (argc >= 6 ? argv[5] : to_string(time(NULL)))};
    } else if (string(argv[2]) == "all") {
        if (argc < 4) invalidUsage();
        return {"7", argv[3], (argc >= 5 ? argv[4] : to_string(time(NULL)))};
    } else if (string(argv[2]) == "remote-all") {
        if (argc < 5) invalidUsage();
        return {"8", argv[3], argv[4], (argc >= 6 ? argv[5] : to_string(time(NULL)))};
    } else if (string(argv[2]) == "filelist") {
        if (argc < 5) invalidUsage();
        string file = readFile(argv[3]);
        return {"9", file, argv[4], (argc >= 6 ? argv[5] : to_string(time(NULL)))};
    } else invalidUsage();
    return {};
}

// 加载 Json 数据
void loadTemporaryData(tmpFile& tmp) {
    if (tmp.argv(0) == "0") {
        if (tmp.getProcess() < 1) {
            if (levelsNumber("name = \"" + string(tmp.argv(1)) + "\"") == 0) 
                callExportError("Level \"" + string(tmp.argv(1)) + "\" not found.");
            exportLevel(levelsList("name = \"" + string(tmp.argv(1)) + "\"", "")[0], tmp),
            tmp.updateProcess();
        } return;
    } else if (tmp.argv(0) == "1") {
        if (tmp.getProcess() < 1) {
            if (skinsNumber("name = \"" + string(tmp.argv(1)) + "\"") == 0) 
                callExportError("Skin \"" + string(tmp.argv(1)) + "\" not found.");
            exportSkin(skinsList("name = \"" + string(tmp.argv(1)) + "\"", "")[0], tmp),
            tmp.updateProcess();
        } return;
    } else if (tmp.argv(0) == "2") {
        if (tmp.getProcess() < 1) {
            if (backgroundsNumber("name = \"" + string(tmp.argv(1)) + "\"") == 0) 
                callExportError("Background \"" + string(tmp.argv(1)) + "\" not found.");
            exportBackground(backgroundsList("name = \"" + string(tmp.argv(1)) + "\"", "")[0], tmp),
            tmp.updateProcess();
        } return;
    } else if (tmp.argv(0) == "3") {
        if (tmp.getProcess() < 1) {
            if (effectsNumber("name = \"" + string(tmp.argv(1)) + "\"") == 0) 
                callExportError("Effect \"" + string(tmp.argv(1)) + "\" not found.");
            exportEffect(effectsList("name = \"" + string(tmp.argv(1)) + "\"", "")[0], tmp),
            tmp.updateProcess();
        } return;
    } else if (tmp.argv(0) == "4") {
        if (tmp.getProcess() < 1) {
            if (particlesNumber("name = \"" + string(tmp.argv(1)) + "\"") == 0) 
                callExportError("Particle \"" + string(tmp.argv(1)) + "\" not found.");
            exportParticle(particlesList("name = \"" + string(tmp.argv(1)) + "\"", "")[0], tmp),
            tmp.updateProcess();
        } return;
    } else if (tmp.argv(0) == "5") {
        if (tmp.getProcess() < 1) {
            if (enginesNumber("name = \"" + string(tmp.argv(1)) + "\"") == 0) 
                callExportError("Engine \"" + string(tmp.argv(1)) + "\" not found.");
            exportEngine(enginesList("name = \"" + string(tmp.argv(1)) + "\"", "")[0], tmp),
            tmp.updateProcess();
        } return;
    } else if (tmp.argv(0) == "A") {
        if (tmp.getProcess() < 1) {
            if (replaysNumber("name = \"" + string(tmp.argv(1)) + "\"") == 0) 
                callExportError("Replay \"" + string(tmp.argv(1)) + "\" not found.");
            exportReplay(replaysList("name = \"" + string(tmp.argv(1)) + "\"", "")[0], tmp),
            tmp.updateProcess();
        } return;
    } else if (tmp.argv(0) == "6") {
        if (tmp.getProcess() < 1) {
            remoteExport(tmp.argv(1), tmp);
            tmp.updateProcess();
        } return;
    } else if (tmp.argv(0) == "7") {
        int pt = 1;
        vector<LevelItem> levels = levelsList("", "", 1, 1e9);
        for (int i = 0; i < levels.size(); i++, pt++) 
            if (tmp.getProcess() < pt) exportLevel(levels[i], tmp), tmp.updateProcess();
        vector<SkinItem> skins = skinsList("", "", 1, 1e9);
        for (int i = 0; i < skins.size(); i++, pt++) 
            if (tmp.getProcess() < pt) exportSkin(skins[i], tmp), tmp.updateProcess();
        vector<BackgroundItem> backgrounds = backgroundsList("", "", 1, 1e9);
        for (int i = 0; i < backgrounds.size(); i++, pt++)
            if (tmp.getProcess() < pt) exportBackground(backgrounds[i], tmp), tmp.updateProcess();
        vector<EffectItem> effects = effectsList("", "", 1, 1e9);
        for (int i = 0; i < effects.size(); i++, pt++)
            if (tmp.getProcess() < pt) exportEffect(effects[i], tmp), tmp.updateProcess();
        vector<ParticleItem> particles = particlesList("", "", 1, 1e9);
        for (int i = 0; i < particles.size(); i++, pt++) 
            if (tmp.getProcess() < pt) exportParticle(particles[i], tmp), tmp.updateProcess();
        vector<EngineItem> engines = enginesList("", "", 1, 1e9);
        for (int i = 0; i < engines.size(); i++, pt++) 
            if (tmp.getProcess() < pt) exportEngine(engines[i], tmp), tmp.updateProcess();
        vector<ReplayItem> replays = replaysList("", "", 1, 1e9);
        for (int i = 0; i < replays.size(); i++, pt++) 
            if (tmp.getProcess() < pt) exportReplay(replays[i], tmp), tmp.updateProcess();
    } else if (tmp.argv(0) == "8") {
        remoteFull(tmp.argv(1), tmp);
        writeLog(LOG_LEVEL_INFO, "Start to process package queue.");
    } else if (tmp.argv(0) == "9") {
        stringstream fin(tmp.argv(1)); int pt = 0;
        while (!fin.eof()) {
            string type; fin >> type;
            if (isUrl(type)) {
                if (++pt <= tmp.getProcess()) continue;
                remoteExport(type, tmp);
                tmp.updateProcess();
                continue;
            } string name; fin >> name;
            if (++pt <= tmp.getProcess()) continue;
            if (type == "level") {
                if (levelsNumber("name = \"" + name + "\"") == 0) {
                    writeLog(LOG_LEVEL_WARNING, "Level \"" + name + "\" not found.");
                    tmp.updateProcess();
                    continue;
                } exportLevel(levelsList("name = \"" + name + "\"", "")[0], tmp);
            } else if (type == "skin") {
                if (skinsNumber("name = \"" + name + "\"") == 0) {
                    writeLog(LOG_LEVEL_WARNING, "Skin \"" + name + "\" not found.");
                    tmp.updateProcess();
                    continue;
                } exportSkin(skinsList("name = \"" + name + "\"", "")[0], tmp);
            } else if (type == "background") {
                if (backgroundsNumber("name = \"" + name + "\"") == 0) {
                    writeLog(LOG_LEVEL_WARNING, "Background \"" + name + "\" not found.");
                    tmp.updateProcess();
                    continue;
                } exportBackground(backgroundsList("name = \"" + name + "\"", "")[0], tmp);
            } else if (type == "effect") {
                if (effectsNumber("name = \"" + name + "\"") == 0) {
                    writeLog(LOG_LEVEL_WARNING, "Effect \"" + name + "\" not found.");
                    tmp.updateProcess();
                    continue;
                } exportEffect(effectsList("name = \"" + name + "\"", "")[0], tmp);
            } else if (type == "particle") {
                if (particlesNumber("name = \"" + name + "\"") == 0) {
                    writeLog(LOG_LEVEL_WARNING, "Particle \"" + name + "\" not found.");
                    tmp.updateProcess();
                    continue;
                } exportParticle(particlesList("name = \"" + name + "\"", "")[0], tmp);
            } else if (type == "engine") {
                if (enginesNumber("name = \"" + name + "\"") == 0) {
                    writeLog(LOG_LEVEL_WARNING, "Engine \"" + name + "\" not found.");
                    tmp.updateProcess();
                    continue;
                } exportEngine(enginesList("name = \"" + name + "\"", "")[0], tmp);
            } else if (type == "replay") {
                if (replaysNumber("name = \"" + name + "\"") == 0) {
                    writeLog(LOG_LEVEL_WARNING, "Replay \"" + name + "\" not found.");
                    tmp.updateProcess();
                    continue;
                } exportReplay(replaysList("name = \"" + name + "\"", "")[0], tmp);
            } else {
                writeLog(LOG_LEVEL_WARNING, "Invalid resource type \"" + type + "\".");
                tmp.updateProcess();
                continue;
            } tmp.updateProcess();
        } writeLog(LOG_LEVEL_INFO, "Start to process package queue.");
    }
}

// 获取数据列表
set<pair<string, string> > getFileList(tmpFile& tmp) {
    set<pair<string, string> > fileSha;
    for (int i = 0; i < tmp.jsonSize(); i++) {
        int jsonType = tmp.getJson(i).first;
        string json = tmp.getJson(i).second;
        Json::Value arr; json_decode(json, arr);
        if (jsonType == exportLevelId) {
            LevelItem level(-1, arr);
            fileSha.insert({level.cover.hash, level.cover.url});
            fileSha.insert({level.bgm.hash, level.bgm.url});
            fileSha.insert({level.data.hash, level.data.url});
            fileSha.insert({level.preview.hash, level.preview.url});
        } else if (jsonType == exportSkinId) {
            SkinItem skin(-1, arr);
            fileSha.insert({skin.thumbnail.hash, skin.thumbnail.url});
            fileSha.insert({skin.data.hash, skin.data.url});
            fileSha.insert({skin.texture.hash, skin.texture.url});
        } else if (jsonType == exportBackgroundId) {
            BackgroundItem background(-1, arr);
            fileSha.insert({background.thumbnail.hash, background.thumbnail.url});
            fileSha.insert({background.data.hash, background.data.url});
            fileSha.insert({background.image.hash, background.image.url});
            fileSha.insert({background.configuration.hash, background.configuration.url});
        } else if (jsonType == exportEffectId) {
            EffectItem effect(-1, arr);
            fileSha.insert({effect.thumbnail.hash, effect.thumbnail.url});
            fileSha.insert({effect.data.hash, effect.data.url});
            fileSha.insert({effect.audio.hash, effect.audio.url});
        } else if (jsonType == exportParticleId) {
            ParticleItem particle(-1, arr);
            fileSha.insert({particle.thumbnail.hash, particle.thumbnail.url});
            fileSha.insert({particle.data.hash, particle.data.url});
            fileSha.insert({particle.texture.hash, particle.texture.url});
        } else if (jsonType == exportEngineId) {
            EngineItem engine(-1, arr);
            fileSha.insert({engine.thumbnail.hash, engine.thumbnail.url});
            fileSha.insert({engine.data.hash, engine.data.url});
            fileSha.insert({engine.configuration.hash, engine.configuration.url});
            fileSha.insert({engine.rom.hash, engine.rom.url});
        } else if (jsonType == exportReplayId) {
            ReplayItem replay(-1, arr);
            fileSha.insert({replay.data.hash, replay.data.url});
            fileSha.insert({replay.configuration.hash, replay.configuration.url});
        }
    } return fileSha;
}

int addRemoteFile(pair<string, string> v, srpFile& srp) {
    if (v.first != "" && srp.fileExist(v.first)) return 0;
    int retry_time = appConfig["export.hashCheckTime"].asInt();
    start:
    int fileSize = getRemoteFileLength(v.second);
    writeLog(LOG_LEVEL_INFO, "Start to download \"" + shortenUrl(v.second) + "\"... (" + formatSize(fileSize) + ")");
    time_t st = getMillSeconds();
    string data = get_url(v.second);
    time_t en = getMillSeconds();
    #ifdef __linux__
    usleep(1000 * 1000);
    #else
    Sleep(100);
    #endif
    double speed = 1.0 * fileSize / (en - st) * 1000.0;

    // 哈希查验
    string hashRemote = v.first;
    string hashLocal = getSha(data);
    if (hashRemote != "") {
        if (hashRemote != hashLocal) {
            writeLog(LOG_LEVEL_ERROR, "Sha1 Hash Check Failed.");
            writeLog(LOG_LEVEL_DEBUG, "Read \"" + hashLocal + "\", but expected \"" + hashRemote + "\".");
            if (retry_time > 0) {
                writeLog(LOG_LEVEL_INFO, "Retry again. (" + to_string(retry_time) + ")");
                retry_time--;
                goto start;
            } else writeLog(LOG_LEVEL_WARNING, "Failed to pass Sha1 Hash Check. This resource may be invalid.");
        }
    } else writeLog(LOG_LEVEL_WARNING, "Due to the limitation of remote server, we can't do hash check for this resource. This resource may be invalid.");
    
    if (srp.fileExist(hashLocal)) {
        writeLog(LOG_LEVEL_INFO, "File \"" + hashLocal + "\" already exist. Skipped!");
        return 0;
    } srp.writeData(hashLocal, data);

    hashMap[v.second] = hashLocal;
    writeLog(LOG_LEVEL_INFO, "Add file \"" + hashLocal + "\" into binary file. (" + formatSize(speed) + "/s)");
    return 1;
}

int addLocalFile(pair<string, string> v, srpFile& srp) {
    string sha = v.first;
    if (srp.fileExist(sha)) return 0;

    ifstream fin(("./data/" + v.first).c_str());
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    fin.seekg(0, ios::beg);
    char* ch = new char[len];
    fin.read(ch, len); fin.close();
    string data = ""; for (int i = 0; i < len; i++) data.push_back(ch[i]);
    srp.writeData(sha, data);
    delete[] ch; hashMap[v.second] = sha;
    writeLog(LOG_LEVEL_INFO, "Add file \"" + sha + "\" into binary file.");
    return 1;
}

int __exportData(tmpFile& tmp) {
    string path = tmp.argv(tmp.argc() - 1);
    if (path.find(".") == string::npos) path += ".srp";
    srpFile srp(path); int cnt = srp.size();
    set<pair<string, string> > fileSha = getFileList(tmp);
    for (auto v : fileSha) {
        if (isUrl(v.second) && v.second != "") cnt += addRemoteFile(v, srp);
        if (!isUrl(v.second) && v.first != "") cnt += addLocalFile(v, srp);
    } return cnt;
}

void __writeJson(tmpFile& tmp) {
    string path = tmp.argv(tmp.argc() - 1);
    if (path.find(".") == string::npos) path += ".srp";
    FileStream fout(path, 2);
    fout.writeInteger(tmp.jsonSize());
    for (int i = 0; i < tmp.jsonSize(); i++) {
        fout.writeBuffer(to_string(tmp.getJson(i).first));

        string json = tmp.getJson(i).second;
        Json::Value obj; json_decode(json, obj);
        for (auto& v : obj) {
            if (v.isObject() && v.isMember("hash") && v["hash"].asString() == "") {
                v["hash"] = hashMap[v["url"].asString()];
            }
        } json = json_encode(obj);

        fout.writeInteger(json.size());
        fout.writeBuffer(json);
    }
}

void exportCore(tmpFile& tmp) {
    loadTemporaryData(tmp);
    int fileSize = __exportData(tmp);
    __writeJson(tmp);
    writeLog(LOG_LEVEL_INFO, "Export data successfully!");
    writeLog(LOG_LEVEL_DEBUG, to_string(fileSize) + " files were written");
    writeLog(LOG_LEVEL_DEBUG, to_string(exportLevelNumber) + " levels were written");
    writeLog(LOG_LEVEL_DEBUG, to_string(exportSkinNumber) + " skins were written");
    writeLog(LOG_LEVEL_DEBUG, to_string(exportBackgroundNumber) + " backgrounds were written");
    writeLog(LOG_LEVEL_DEBUG, to_string(exportEffectNumber) + " effects were written");
    writeLog(LOG_LEVEL_DEBUG, to_string(exportParticleNumber) + " particles were written");
    writeLog(LOG_LEVEL_DEBUG, to_string(exportEngineNumber) + " engines were written");
    writeLog(LOG_LEVEL_DEBUG, to_string(exportReplayNumber) + " replays were written");
}

void exportData(int argc, char** argv) {
    vector<string> args = exportPreload(argc, argv);
    if (fileExist("./.tmp/" + args.back() + ".bin")) {
        writeLog(LOG_LEVEL_ERROR, "Task " + args.back() + " is already exist.");
        exit(3);
    } if (fileExist(args[args.size() - 2])) {
        writeLog(LOG_LEVEL_ERROR, "File " + args[args.size() - 2] + " is already exist.");
        exit(3);
    } initializeTemporary("./.tmp/" + args.back() + ".bin", args);
    tmpFile tmp(args.back() + ".bin");
    exportCore(tmp);
    removeTemporary("./.tmp/" + args.back() + ".bin");
}

void getTmpFileList(string path, vector<string>& fileList) {
    DIR* dir;
    struct dirent* ptr;
    if ((dir = opendir(path.c_str())) == NULL) return;
    while ((ptr = readdir(dir)) != NULL) {
        if (ptr->d_name[0] == '.') continue;
        string fileName = ptr->d_name;
        if (fileName.size() < 4) continue;
        if (fileName.substr(fileName.length() - 4, 4) == ".bin") {
            fileList.push_back(fileName);
        }
    }
    closedir(dir);
}

string formatTime(time_t t) {
    tm* timeinfo = localtime(&t);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;   
}

void taskManage(int argc, char** argv) {
    if (argc < 3) invalidUsage();
    if (string(argv[2]) == "list") {
        vector<string> fileList;
        getTmpFileList("./.tmp/", fileList);
        for (int i = 0; i < fileList.size(); i++) {
            FileStream fin("./.tmp/" + fileList[i], 0);
            time_t createTime = fin.getInteger();
            time_t updatedTime = fin.getInteger();
            int jsonSize = fin.getInteger();
            cout << "(" << (i + 1) << "). Task \"" << fileList[i].substr(0, fileList[i].length() - 4) 
                 << "\" created at " << formatTime(createTime)
                 << ", last updated: " << formatTime(updatedTime) << endl;
        }
    } else if (string(argv[2]) == "continue") {
        if (argc < 4) invalidUsage();
        string fileName = argv[3];
        if (!fileExist("./.tmp/" + fileName + ".bin")) {
            writeLog(LOG_LEVEL_ERROR, "Task \"" + fileName + "\" not found.");
            exit(3);
        } tmpFile tmp(fileName + ".bin");
        writeLog(LOG_LEVEL_INFO, "Continue to execute task \"" + fileName + "\"");
        exportCore(tmp);
        removeTemporary("./.tmp/" + fileName + ".bin");
    } else if (string(argv[2]) == "terminate") {
        if (argc < 4) invalidUsage();
        string fileName = argv[3];
        if (!fileExist("./.tmp/" + fileName + ".bin")) {
            writeLog(LOG_LEVEL_ERROR, "Task \"" + fileName + "\" not found.");
            exit(3);
        } removeTemporary("./.tmp/" + fileName + ".bin");
        writeLog(LOG_LEVEL_INFO, "Task \"" + fileName + "\" terminated.");
    } else if (string(argv[2]) == "info") {
        if (argc < 4) invalidUsage();
        string fileName = argv[3];
        if (!fileExist("./.tmp/" + fileName + ".bin")) {
            writeLog(LOG_LEVEL_ERROR, "Task \"" + fileName + "\" not found.");
            exit(3);
        } FileStream fin("./.tmp/" + fileName + ".bin", 0);
        time_t createTime = fin.getInteger();
        time_t updatedTime = fin.getInteger();
        int argvSize = fin.getInteger();
        vector<string> argv;
        for (int i = 0; i < argvSize; i++) {
            int size = fin.getInteger();
            argv.push_back(fin.getBuffer(size));
        } int processSize = fin.getInteger();
        int jsonSize = fin.getInteger();
        cout << "Task \"" << fileName << "\": " << endl;
        cout << "Created Time: " << formatTime(createTime) << endl;
        cout << "Last Updated: " << formatTime(updatedTime) << endl;
        cout << "Process Id: " << processSize << endl;
        cout << "Saved Json: " << jsonSize << endl;
        cout << "Arguments: [" << endl;
        cout << "    \"export\"";
        for (int i = 0; i < argv.size(); i++) {
            cout << ", " << endl << "    ";
            string x = argv[i];
            x = str_replace("\"", "\\\"", x);
            x = str_replace("\r", "\\r", x);
            x = str_replace("\n", "\\n", x);
            cout << "\"" << argv[i] << "\"";
        } cout << endl << "]" << endl;
    }
}
