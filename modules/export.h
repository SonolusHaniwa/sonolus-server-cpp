using namespace std;

set<pair<string, string> > fileSha;
map<string, string> hashMap;
vector<pair<int, string> > json;

void exportSkin(SkinItem skin) {
    json.push_back({exportSkinId, json_encode(skin.toJsonObject())});
    fileSha.insert({skin.thumbnail.hash, skin.thumbnail.url});
    fileSha.insert({skin.data.hash, skin.data.url});
    fileSha.insert({skin.texture.hash, skin.texture.url});
}

void exportBackground(BackgroundItem background) {
    json.push_back({exportBackgroundId, json_encode(background.toJsonObject())});
    fileSha.insert({background.thumbnail.hash, background.thumbnail.url});
    fileSha.insert({background.data.hash, background.data.url});
    fileSha.insert({background.image.hash, background.image.url});
    fileSha.insert({background.configuration.hash, background.configuration.url});
}

void exportEffect(EffectItem effect) {
    json.push_back({exportEffectId, json_encode(effect.toJsonObject())});
    fileSha.insert({effect.thumbnail.hash, effect.thumbnail.url});
    fileSha.insert({effect.data.hash, effect.data.url});
    fileSha.insert({effect.audio.hash, effect.audio.url});
}

void exportParticle(ParticleItem particle) {
    json.push_back({exportParticleId, json_encode(particle.toJsonObject())});
    fileSha.insert({particle.thumbnail.hash, particle.thumbnail.url});
    fileSha.insert({particle.data.hash, particle.data.url});
    fileSha.insert({particle.texture.hash, particle.texture.url});
}

void exportEngine(EngineItem engine) {
    exportSkin(engine.skin);
    exportBackground(engine.background);
    exportEffect(engine.effect);
    exportParticle(engine.particle);
    json.push_back({exportEngineId, json_encode(engine.toJsonObject())});
    fileSha.insert({engine.thumbnail.hash, engine.thumbnail.url});
    fileSha.insert({engine.data.hash, engine.data.url});
    fileSha.insert({engine.configuration.hash, engine.configuration.url});
    fileSha.insert({engine.rom.hash, engine.rom.url});
}

void exportLevel(LevelItem level) {
    exportEngine(level.engine);
    if (level.useSkin.useDefault == false) exportSkin(level.useSkin.item);
    if (level.useBackground.useDefault == false) exportBackground(level.useBackground.item);
    if (level.useEffect.useDefault == false) exportEffect(level.useEffect.item);
    if (level.useParticle.useDefault == false) exportParticle(level.useParticle.item);
    json.push_back({exportLevelId, json_encode(level.toJsonObject())});
    fileSha.insert({level.cover.hash, level.cover.url});
    fileSha.insert({level.bgm.hash, level.bgm.url});
    fileSha.insert({level.data.hash, level.data.url});
    fileSha.insert({level.preview.hash, level.preview.url});
}

string solveUrl(string url) {
    auto path = explode("/", url.c_str());
    if (path.size() < 3) return "";
    string rootUrl = "";
    for (int i = 0; i < path.size() - 2; i++) rootUrl += path[i] + "/";
    if (path[path.size() - 3] != "sonolus") rootUrl += "sonolus/";
    rootUrl += path[path.size() - 2] + "/" + path[path.size() - 1];
    if (rootUrl.substr(0, 7) != "http://" && rootUrl.substr(0, 8) != "https://" && rootUrl.substr(0, 2) != "//") rootUrl = "//" + rootUrl;
    return rootUrl;
}

string getRootUrl(string url) {
    auto path = explode("/", url.c_str());
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
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void exportData(string path) {
    if (path.find(".") == string::npos) path += ".srp";
    ofstream fout(path);
    fout << fileHeader;
    int fileNum = 0;
    for (auto v : fileSha) if (!isUrl(v.second) && v.first != "" || isUrl(v.second)) fileNum++;
    int filenum = fileNum;
    vector<int> x; for (int i = 0; i < 8; i++) x.push_back(fileNum % (1 << 8)), fileNum >>= 8;
    reverse(x.begin(), x.end());
    char* ch = new char[8];
    for (int i = 0; i < 8; i++) ch[i] = x[i];
    fout.write(ch, 8); delete[] ch;
    for (auto v : fileSha) {
        if (!isUrl(v.second) || v.second == "") continue;
        int retry_time = appConfig["export.retryTime"].asInt();;
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
        if (hashRemote != "") {
            unsigned char* hashLocal = sha1(const_cast<char*>(data.c_str()), data.size());
            stringstream buffer;
            for (int i = 0; i < 20; i++)
                buffer << hex << setw(2) << setfill('0') << int(hashLocal[i]);
            for (int i = 0; i < hashRemote.size(); i++) hashRemote[i] = tolower(hashRemote[i]);
            if (hashRemote != buffer.str()) {
                writeLog(LOG_LEVEL_ERROR, "Sha1 Hash Check Failed.");
                writeLog(LOG_LEVEL_DEBUG, "Read \"" + buffer.str() + "\", but expected \"" + hashRemote + "\".");
                if (retry_time > 0) {
                    writeLog(LOG_LEVEL_INFO, "Retry again. (" + to_string(retry_time) + ")");
                    retry_time--;
                    goto start;
                } else exit(0);
            } delete[] hashLocal;
        } else writeLog(LOG_LEVEL_WARNING, "Due to the limitation of remote server, we can't do hash check for this resource. This resource may be invalid.");
        
        unsigned char* hash = sha1(const_cast<char*>(data.c_str()), data.size());
        char* hashLocal = new char[20];
        for (int i = 0; i < 20; i++) hashLocal[i] = hash[i];
        fout.write(hashLocal, 20);
        stringstream buffer;
        for (int i = 0; i < 20; i++)
            buffer << hex << setw(2) << setfill('0') << int(hash[i]);
        delete[] hash; delete[] hashLocal;

        int len = data.size();
        vector<int> x; for (int i = 0; i < 8; i++) x.push_back(len % (1 << 8)), len >>= 8;
        reverse(x.begin(), x.end());
        char* ch = new char[8];
        for (int i = 0; i < 8; i++) ch[i] = x[i];
        fout.write(ch, 8); delete[] ch;

        fout.write(const_cast<char*>(data.c_str()), data.size());

        hashMap[v.second] = buffer.str();
        writeLog(LOG_LEVEL_INFO, "Add file \"" + buffer.str() + "\" into binary file. (" + formatSize(speed) + "/s)");
    }
    for (auto v : fileSha) {
        if (isUrl(v.second) || v.first == "") continue;
        char* sha = new char[20];
        for (int i = 0; i < 40; i += 2) {
            char a = v.first[i], b = v.first[i + 1];
            int val = isdigit(a) ? a - '0' : a - 'a' + 10;
            val <<= 4;
            val += isdigit(b) ? b - '0' : b - 'a' + 10;
            sha[i / 2] = val;
        }
        fout.write(sha, 20); delete[] sha;

        ifstream fin(("./data/" + v.first).c_str());
        fin.seekg(0, ios::end);
        int len = fin.tellg(); int tmp = len;
        vector<int> x; for (int i = 0; i < 8; i++) x.push_back(len % (1 << 8)), len >>= 8;
        reverse(x.begin(), x.end());
        char* ch = new char[8];
        for (int i = 0; i < 8; i++) ch[i] = x[i];
        fout.write(ch, 8); delete[] ch;

        len = tmp;
        fin.seekg(0, ios::beg);
        ch = new char[len];
        fin.read(ch, len); fin.close();
        fout.write(ch, len); delete[] ch;
        writeLog(LOG_LEVEL_DEBUG, "Add file " + v.first + " into binary file.");
    }
    int jsonNum = json.size(); x.clear();
    for (int i = 0; i < 8; i++) x.push_back(jsonNum % (1 << 8)), jsonNum >>= 8;
    reverse(x.begin(), x.end());
    ch = new char[8];
    for (int i = 0; i < 8; i++) ch[i] = x[i];
    fout.write(ch, 8); delete[] ch;
    for (int i = 0; i < json.size(); i++) {
        char* ch = new char[1];
        ch[0] = json[i].first;
        fout.write(ch, 1); delete[] ch;

        Json::Value obj; json_decode(json[i].second, obj);
        for (auto& v : obj) {
            if (v.isObject() && v.isMember("hash") && v["hash"].asString() == "") {
                v["hash"] = hashMap[v["url"].asString()];
            }
        } json[i].second = json_encode(obj);
        
        int len = json[i].second.size();
        vector<int> x; for (int i = 0; i < 8; i++) x.push_back(len % (1 << 8)), len >>= 8;
        reverse(x.begin(), x.end());
        ch = new char[8];
        for (int i = 0; i < 8; i++) ch[i] = x[i];
        fout.write(ch, 8); delete[] ch;
        
        fout << json[i].second;
    }
    fout.close();
    writeLog(LOG_LEVEL_INFO, "Export data successfully!");
    writeLog(LOG_LEVEL_DEBUG, to_string(filenum) + " files were written");
}