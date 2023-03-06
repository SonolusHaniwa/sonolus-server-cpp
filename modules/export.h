using namespace std;

set<string> fileSha;
vector<pair<int, string> > json;

int levelNum = sizeof(exportLevelId) / sizeof(int);
int skinNum = sizeof(exportSkinId) / sizeof(int);
int backgroundNum = sizeof(exportBackgroundId) / sizeof(int);
int effectNum = sizeof(exportEffectId) / sizeof(int);
int particleNum = sizeof(exportParticleId) / sizeof(int);
int engineNum = sizeof(exportEngineId) / sizeof(int);

void exportSkin(int id) {
    SkinItem skin = skinList("id = " + to_string(id)).items[0];
    json.push_back({2, json_encode(skin.toJsonObject())});
    fileSha.insert(skin.thumbnail.hash);
    fileSha.insert(skin.data.hash);
    fileSha.insert(skin.texture.hash);
}

void exportBackground(int id) {
    BackgroundItem background = backgroundList("id = " + to_string(id)).items[0];
    json.push_back({3, json_encode(background.toJsonObject())});
    fileSha.insert(background.thumbnail.hash);
    fileSha.insert(background.data.hash);
    fileSha.insert(background.image.hash);
    fileSha.insert(background.configuration.hash);
}

void exportEffect(int id) {
    EffectItem effect = effectList("id = " + to_string(id)).items[0];
    json.push_back({4, json_encode(effect.toJsonObject())});
    fileSha.insert(effect.thumbnail.hash);
    fileSha.insert(effect.data.hash);
    fileSha.insert(effect.audio.hash);
}

void exportParticle(int id) {
    ParticleItem particle = particleList("id = " + to_string(id)).items[0];
    json.push_back({5, json_encode(particle.toJsonObject())});
    fileSha.insert(particle.thumbnail.hash);
    fileSha.insert(particle.data.hash);
    fileSha.insert(particle.texture.hash);
}

void exportEngine(string name) {
    auto section = engineList("name = \"" + str_replace("\"", "\\\"", name) + "\"");
    if (section.items.size() == 0) {
        writeLog(LOG_LEVEL_ERROR, "No such engine exist.");
        writeLog(LOG_LEVEL_DEBUG, "0 files were written.");
        exit(3);
    }
    EngineItem engine = section.items[0];
    exportSkin(engine.skin.id); exportBackground(engine.background.id);
    exportEffect(engine.effect.id); exportParticle(engine.particle.id);
    json.push_back({6, json_encode(engine.toJsonObject())});
    fileSha.insert(engine.thumbnail.hash);
    fileSha.insert(engine.data.hash);
    fileSha.insert(engine.configuration.hash);
    fileSha.insert(engine.rom.hash);
}

void exportLevel(string name) {
    auto section = levelList("name = \"" + str_replace("\"", "\\\"", name) + "\"");
    if (section.items.size() == 0) {
        writeLog(LOG_LEVEL_ERROR, "No such level exist.");
        writeLog(LOG_LEVEL_DEBUG, "0 files were written.");
        exit(3);
    }
    LevelItem level = section.items[0];
    exportEngine(level.engine.name);
    if (level.useSkin.useDefault == false) exportSkin(level.useSkin.item.id);
    if (level.useBackground.useDefault == false) exportBackground(level.useBackground.item.id);
    if (level.useEffect.useDefault == false) exportEffect(level.useEffect.item.id);
    if (level.useParticle.useDefault == false) exportParticle(level.useParticle.item.id);
    json.push_back({1, json_encode(level.toJsonObject())});
    fileSha.insert(level.cover.hash);
    fileSha.insert(level.bgm.hash);
    fileSha.insert(level.data.hash);
    fileSha.insert(level.preview.hash);
}

void exportData(const char* path) {
    ofstream fout(path);
    int fileNum = 0;
    for (auto v : fileSha) if (v != "") fileNum++;
    int filenum = fileNum;
    vector<int> x; for (int i = 0; i < 8; i++) x.push_back(fileNum % (1 << 8)), fileNum >>= 8;
    reverse(x.begin(), x.end());
    char* ch = new char[8];
    for (int i = 0; i < 8; i++) ch[i] = x[i];
    fout.write(ch, 8); delete[] ch;
    for (auto v : fileSha) {
        if (v == "") continue;
        char* sha = new char[20];
        for (int i = 0; i < 40; i += 2) {
            char a = v[i], b = v[i + 1];
            int val = isdigit(a) ? a - '0' : a - 'a' + 10;
            val <<= 4;
            val += isdigit(b) ? b - '0' : b - 'a' + 10;
            sha[i / 2] = val;
        }
        fout.write(sha, 20); delete[] sha;

        ifstream fin(("./data/" + v).c_str());
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
        writeLog(LOG_LEVEL_DEBUG, "Add file " + v + " into binary file.");
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