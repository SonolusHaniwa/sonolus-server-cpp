#include<zip.h>
using namespace std;

string getFileFromZip(string zipPath, string filePath) {
    #ifdef ENABLE_ZIP
    zip* z = zip_open(zipPath.c_str(), 0, NULL);
    if (z == NULL) {
        writeLog(LOG_LEVEL_ERROR, "Cannot open zip file.");
        return "";
    }
    zip_stat_t st;
    zip_stat_init(&st);
    zip_stat(z, filePath.c_str(), 0, &st);
    char* contents = new char[st.size];
    zip_file_t* f = zip_fopen(z, filePath.c_str(), 0);
    if (f == NULL) {
        writeLog(LOG_LEVEL_ERROR, "Cannot open file in zip.");
        return "";
    }
    zip_fread(f, contents, st.size);
    zip_fclose(f);
    zip_close(z);
    string ret = string(contents, st.size);
    delete[] contents;
    return ret;
    #else
    writeLog(LOG_LEVEL_ERROR, "This program doesn't support libzip. You need to compile this program with the option `-DENABLE_ZIP -lzip`.");
	exit(3);
    #endif
}

vector<string> getFileListFromZip(string zipPath) {
    #ifdef ENABLE_ZIP
    zip* z = zip_open(zipPath.c_str(), 0, NULL);
    if (z == NULL) {
        writeLog(LOG_LEVEL_ERROR, "Cannot open zip file.");
        return {};
    } vector<string> ret;
    zip_int64_t i, c = zip_get_num_entries(z, ZIP_FL_UNCHANGED);
    for (i = 0; i < c; i++) ret.push_back(zip_get_name(z, i, ZIP_FL_ENC_GUESS));
    return ret;
    #else
    writeLog(LOG_LEVEL_ERROR, "This program doesn't support libzip. You need to compile this program with the option `-DENABLE_ZIP -lzip`.");
	exit(3);
    #endif
}