#include"httpd.h"
#include"html.h"
#include"mysqli.h"
#include"json.h"
#include"encrypt.h"

using namespace std;

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
    for (int i = 0; i < 8; i++) filenum <<= 8, filenum += (unsigned char)ch[pt++];
    for (int i = 1; i <= filenum; i++) {
        int filesize = 0;
        unsigned char* fileSha1 = new unsigned char[20];
        for (int j = 0; j < 20; j++) fileSha1[j] = ch[pt++];
        for (int j = 0; j < 8; j++) filesize <<= 8, filesize += (unsigned char)ch[pt++];
        char* filePointer = new char[filesize];
        for (int j = 0; j < filesize; j++) filePointer[j] = ch[pt++];
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
    string sql = "";
    while (pt < len) sql.push_back(ch[pt++]);
    int raws = mysqli_execute(mysql, sql.c_str());
    delete[] ch;
    writeLog(LOG_LEVEL_INFO, "Import data successfully.");
    writeLog(LOG_LEVEL_DEBUG, to_string(filenum) + " files were written.");
    writeLog(LOG_LEVEL_DEBUG, to_string(raws) + " raws were affected.");
}