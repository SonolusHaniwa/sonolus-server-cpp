using namespace std;

void serverPackageImport(char* ch, int len) {
    int filenum = 0, pt = 0;
    if (pt + 8 > len) {
        writeLog(LOG_LEVEL_ERROR, "Invalid file format.");
        writeLog(LOG_LEVEL_DEBUG, "0 files were written.");
        writeLog(LOG_LEVEL_DEBUG, "0 raws were affected.");
        exit(3);
    } for (int i = 0; i < 8; i++) filenum <<= 8, filenum += (unsigned char)ch[pt++];
    for (int i = 1; i <= filenum; i++) {
        int filesize = 0;
        unsigned char* fileSha1 = new unsigned char[20];
        if (pt + 28 > len) {
            writeLog(LOG_LEVEL_ERROR, "Invalid file format.");
            writeLog(LOG_LEVEL_DEBUG, to_string(i - 1) + " files were written.");
            writeLog(LOG_LEVEL_DEBUG, "0 raws were affected.");
            exit(3);
        } for (int j = 0; j < 20; j++) fileSha1[j] = ch[pt++];
        for (int j = 0; j < 8; j++) filesize <<= 8, filesize += (unsigned char)ch[pt++];
        char* filePointer = new char[filesize];
        if (pt + filesize > len) {
            writeLog(LOG_LEVEL_ERROR, "Invalid file format.");
            writeLog(LOG_LEVEL_DEBUG, to_string(i - 1) + " files were written.");
            writeLog(LOG_LEVEL_DEBUG, "0 raws were affected.");
            exit(3);
        } for (int j = 0; j < filesize; j++) filePointer[j] = ch[pt++];
        unsigned char* fileSha1Real = sha1(filePointer, filesize);
        for (int j = 0; j < 20; j++) {
            if (fileSha1Real[j] != fileSha1[j]) {
                writeLog(LOG_LEVEL_ERROR, "Invalid file sha1 hash.");
                writeLog(LOG_LEVEL_DEBUG, to_string(i - 1) + " files were written.");
                writeLog(LOG_LEVEL_DEBUG, "0 raws were affected.");
                exit(3);
            }
        }
        stringstream buffer;
        for (int i = 0; i < 20; i++)
            buffer << hex << setw(2) << setfill('0') << int(fileSha1[i]);
        ofstream fout(("./data/" + buffer.str()).c_str());
        fout.write(filePointer, filesize);
        fout.close(); delete[] filePointer; delete[] fileSha1; delete[] fileSha1Real;
        writeLog(LOG_LEVEL_DEBUG, "Saved file " + buffer.str() + " into data.");
    }
    int jsonnum = 0, effected_raws = 0;
    if (pt + 8 > len) {
        writeLog(LOG_LEVEL_ERROR, "Invalid file format.");
        writeLog(LOG_LEVEL_DEBUG, to_string(filenum) + " files were written.");
        writeLog(LOG_LEVEL_DEBUG, "0 raws were affected.");
        exit(3);
    } for (int i = 0; i < 8; i++) jsonnum <<= 8, jsonnum += (unsigned char)ch[pt++];
    for (int i = 1; i <= jsonnum; i++) {
        int jsonsize = 0; string json; int type = ch[pt++];
        if (pt + 8 > len) {
            writeLog(LOG_LEVEL_ERROR, "Invalid file format.");
            writeLog(LOG_LEVEL_DEBUG, to_string(filenum) + " files were written.");
            writeLog(LOG_LEVEL_DEBUG, to_string(effected_raws) + " raws were affected.");
            exit(3);
        } for (int j = 0; j < 8; j++) jsonsize <<= 8, jsonsize += (unsigned char)ch[pt++];
        if (pt + jsonsize > len) {
            writeLog(LOG_LEVEL_ERROR, "Invalid file format.");
            writeLog(LOG_LEVEL_DEBUG, to_string(filenum) + " files were written.");
            writeLog(LOG_LEVEL_DEBUG, to_string(effected_raws) + " raws were affected.");
            exit(3);
        } for (int j = 0; j < jsonsize; j++) json.push_back(ch[pt++]);
        Json::Value arr; json_decode(json, arr);
        switch(type) {
            case 1: effected_raws += levelCreate(LevelItem(-1, arr)); break;
            case 2: effected_raws += skinCreate(SkinItem(-1, arr)); break;
            case 3: effected_raws += backgroundCreate(BackgroundItem(-1, arr)); break;
            case 4: effected_raws += effectCreate(EffectItem(-1, arr)); break;
            case 5: effected_raws += particleCreate(ParticleItem(-1, arr)); break;
            case 6: effected_raws += engineCreate(EngineItem(-1, arr)); break;
        }
    }
    writeLog(LOG_LEVEL_INFO, "Import data successfully.");
    writeLog(LOG_LEVEL_DEBUG, to_string(filenum) + " files were written.");
    writeLog(LOG_LEVEL_DEBUG, to_string(effected_raws) + " raws were affected.");
}

void importDataFromOfficialPackage(string path, Json::Value obj, int& fileCnt) {
    if (obj.isObject() && obj.isMember("url") && obj.isMember("hash")) {
        string fileContent = getFileFromZip(path, obj["url"].asString().substr(1));
        ofstream fout("./data/" + obj["hash"].asString());
        fout.write(const_cast<char*>(fileContent.c_str()), fileContent.size());
        fout.close();
        writeLog(LOG_LEVEL_DEBUG, "Saved file " + obj["hash"].asString() + " into data.");
        fileCnt++; return;
    } if (obj.isArray() || obj.isObject()) for (auto v : obj) importDataFromOfficialPackage(path, v, fileCnt);
}

void officialPackageImport(string path) {
    string supportComponents[] = {"skins", "backgrounds", "effects", "particles"};
    int files = 0, raws = 0;
    for (int i = 0; i < 4; i++) {
        string filePath = "sonolus/" + supportComponents[i] + "/list";
        string json = getFileFromZip(path, filePath);
        Json::Value obj; json_decode(json, obj); int fileCnt = 0;
        importDataFromOfficialPackage(path, obj, fileCnt);
        for (int j = 0; j < obj["items"].size(); j++) {
            Json::Value arr = obj["items"][j];
            switch(i) {
                case 0: raws += skinCreate(SkinItem(-1, arr)); break;
                case 1: raws += backgroundCreate(BackgroundItem(-1, arr)); break;
                case 2: raws += effectCreate(EffectItem(-1, arr)); break;
                case 3: raws += particleCreate(ParticleItem(-1, arr)); break;
            }
        } files += fileCnt;
    } writeLog(LOG_LEVEL_INFO, "Import data successfully.");
    writeLog(LOG_LEVEL_DEBUG, to_string(files) + " files were written.");
    writeLog(LOG_LEVEL_DEBUG, to_string(raws) + " raws were affected.");
}

void import(const char* path) {
    ifstream fin(path);
    if (!fin) {
        writeLog(LOG_LEVEL_ERROR, "Couldn't open binary file!");
        writeLog(LOG_LEVEL_DEBUG, "0 files were written");
        writeLog(LOG_LEVEL_DEBUG, "0 raws were affected");
        exit(3);
    }
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    fin.seekg(0, ios::beg);
    char* ch = new char[len];
    fin.read(ch, len);
    int filenum = 0, pt = 0;
    if (len < fileHeader.size()) {
        writeLog(LOG_LEVEL_ERROR, "Invalid file format.");
        writeLog(LOG_LEVEL_DEBUG, "0 files were written.");
        writeLog(LOG_LEVEL_DEBUG, "0 raws were affected.");
        exit(3);
    } string head = "";
    for (int i = 0; i < fileHeader.size(); i++) head.push_back(ch[pt++]);
    if (head == fileHeader) serverPackageImport(ch + fileHeader.size(), len - fileHeader.size());
    else officialPackageImport(path);
    delete[] ch;
}