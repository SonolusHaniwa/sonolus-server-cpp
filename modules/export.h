using namespace std;

set<string> fileSha;
stringstream sqlbuffer;

int levelNum = sizeof(exportLevelId) / sizeof(int);
int skinNum = sizeof(exportSkinId) / sizeof(int);
int backgroundNum = sizeof(exportBackgroundId) / sizeof(int);
int effectNum = sizeof(exportEffectId) / sizeof(int);
int particleNum = sizeof(exportParticleId) / sizeof(int);
int engineNum = sizeof(exportEngineId) / sizeof(int);

void exportSkin(int id) {
    SkinItem skin = skinList("id = " + to_string(id)).items[0];
    sqlbuffer << "SELECT count(*) INTO @id FROM Skin; ";
    sqlbuffer << "INSERT INTO Skin (id, name, version, title, subtitle, author, thumbnail, data, texture) SELECT ";
    sqlbuffer << "(@id + 1), \"" << skin.name << "\", " << skin.version << ", \"" << skin.title << "\", ";
    sqlbuffer << "\"" << skin.subtitle << "\", \"" << skin.author << "\", \"" << skin.thumbnail.hash << "\", ";
    sqlbuffer << "\"" << skin.data.hash << "\", \"" << skin.texture.hash << "\" FROM DUAL WHERE NOT EXISTS ";
    sqlbuffer << "(SELECT id FROM Skin where name = \"" << skin.name << "\"); ";
    fileSha.insert(skin.thumbnail.hash);
    fileSha.insert(skin.data.hash);
    fileSha.insert(skin.texture.hash);
}

void exportBackground(int id) {
    BackgroundItem background = backgroundList("id = " + to_string(id)).items[0];
    sqlbuffer << "SELECT count(*) INTO @id FROM Background; ";
    sqlbuffer << "INSERT INTO Background (id, name, version, title, subtitle, author, thumbnail, data, image, configuration) SELECT ";
    sqlbuffer << "(@id + 1), \"" << background.name << "\", " << background.version << ", \"" << background.title << "\", ";
    sqlbuffer << "\"" << background.subtitle << "\", \"" << background.author << "\", \"" << background.thumbnail.hash << "\", ";
    sqlbuffer << "\"" << background.data.hash << "\", \"" << background.image.hash << "\", \"" << background.configuration.hash << "\" FROM DUAL WHERE NOT EXISTS ";
    sqlbuffer << "(SELECT id FROM Background where name = \"" << background.name << "\"); ";
    fileSha.insert(background.thumbnail.hash);
    fileSha.insert(background.data.hash);
    fileSha.insert(background.image.hash);
    fileSha.insert(background.configuration.hash);
}

void exportEffect(int id) {
    EffectItem effect = effectList("id = " + to_string(id)).items[0];
    sqlbuffer << "SELECT count(*) INTO @id FROM Effect; ";
    sqlbuffer << "INSERT INTO Effect (id, name, version, title, subtitle, author, thumbnail, data, audio) SELECT ";
    sqlbuffer << "(@id + 1), \"" << effect.name << "\", " << effect.version << ", \"" << effect.title << "\", ";
    sqlbuffer << "\"" << effect.subtitle << "\", \"" << effect.author << "\", \"" << effect.thumbnail.hash << "\", ";
    sqlbuffer << "\"" << effect.data.hash << "\", \"" << effect.audio.hash << "\" FROM DUAL WHERE NOT EXISTS ";
    sqlbuffer << "(SELECT id FROM Effect where name = \"" << effect.name << "\"); ";
    fileSha.insert(effect.thumbnail.hash);
    fileSha.insert(effect.data.hash);
    fileSha.insert(effect.audio.hash);
}

void exportParticle(int id) {
    ParticleItem particle = particleList("id = " + to_string(id)).items[0];
    sqlbuffer << "SELECT count(*) INTO @id FROM Particle; ";
    sqlbuffer << "INSERT INTO Particle (id, name, version, title, subtitle, author, thumbnail, data, texture) SELECT ";
    sqlbuffer << "(@id + 1), \"" << particle.name << "\", " << particle.version << ", \"" << particle.title << "\", ";
    sqlbuffer << "\"" << particle.subtitle << "\", \"" << particle.author << "\", \"" << particle.thumbnail.hash << "\", ";
    sqlbuffer << "\"" << particle.data.hash << "\", \"" << particle.texture.hash << "\" FROM DUAL WHERE NOT EXISTS ";
    sqlbuffer << "(SELECT id FROM Particle where name = \"" << particle.name << "\"); ";
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
    sqlbuffer << "SELECT count(*) INTO @id FROM Engine; ";
    sqlbuffer << "SELECT id INTO @skinId FROM Skin WHERE name = \"" << engine.skin.name << "\"; ";
    sqlbuffer << "SELECT id INTO @backgroundId FROM Background WHERE name = \"" << engine.background.name << "\"; ";
    sqlbuffer << "SELECT id INTO @effectId FROM Effect WHERE name = \"" << engine.effect.name << "\"; ";
    sqlbuffer << "SELECT id INTO @particleId FROM Particle WHERE name = \"" << engine.particle.name << "\"; ";
    sqlbuffer << "INSERT INTO Engine (id, name, version, title, subtitle, author, skin, background, effect, particle, thumbnail, data, configuration, rom) SELECT ";
    sqlbuffer << "(@id + 1), \"" << engine.name << "\", " << engine.version << ", \"" << engine.title << "\", ";
    sqlbuffer << "\"" << engine.subtitle << "\", \"" << engine.author << "\", @skinId, @backgroundId, @effectId, @particleId, ";
    sqlbuffer << "\"" << engine.thumbnail.hash << "\", \"" << engine.data.hash << "\", \"" << engine.configuration.hash << "\", ";
    sqlbuffer << "\"" << engine.rom.hash << "\" FROM DUAL WHERE NOT EXISTS ";
    sqlbuffer << "(SELECT id FROM Engine where name = \"" << engine.name << "\"); ";
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
    sqlbuffer << "SELECT count(*) INTO @id FROM Level; ";
    string skinId = "0", backgroundId = "0", effectId = "0", particleId = "0";
    if (level.useSkin.useDefault == false) {
        sqlbuffer << "SELECT id INTO @skinId FROM Skin WHERE name = \"" << level.useSkin.item.name << "\"; ";
        skinId = "@skinId";
    } if (level.useBackground.useDefault == false) {
        sqlbuffer << "SELECT id INTO @backgroundId FROM Background WHERE name = \"" << level.useBackground.item.name << "\"; ";
        backgroundId = "@backgroundId";
    } if (level.useEffect.useDefault == false) {
        sqlbuffer << "SELECT id INTO @effectId FROM Effect WHERE name = \"" << level.useEffect.item.name << "\"; ";
        effectId = "@effectId";
    } if (level.useParticle.useDefault == false) {
        sqlbuffer << "SELECT id INTO @particleId FROM Particle WHERE name = \"" << level.useParticle.item.name << "\"; ";
        particleId = "@particleId";
    } sqlbuffer << "SELECT id INTO @engineId FROM Engine WHERE name = \"" << level.engine.name << "\"; ";
    sqlbuffer << "INSERT INTO Level (id, name, version, rating, title, artists, author, engine, skin, background, effect, particle, cover, bgm, data, preview) SELECT ";
    sqlbuffer << "(@id + 1), \"" << level.name << "\", " << level.version << ", " << level.rating << ", \"" << level.title << "\", ";
    sqlbuffer << "\"" << level.artists << "\", \"" << level.author << "\", @engineId, " << skinId << ", " << backgroundId << ", " << effectId << ", " << particleId << ", ";
    sqlbuffer << "\"" << level.cover.hash << "\", \"" << level.bgm.hash << "\", \"" << level.data.hash << "\", \"" << level.preview.hash << "\" FROM DUAL WHERE NOT EXISTS ";
    sqlbuffer << "(SELECT id FROM Level where name = \"" << level.name << "\"); ";
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
    fout << sqlbuffer.str();
    fout.close();
    writeLog(LOG_LEVEL_INFO, "Export data successfully!");
    writeLog(LOG_LEVEL_DEBUG, to_string(filenum) + " files were written");
}