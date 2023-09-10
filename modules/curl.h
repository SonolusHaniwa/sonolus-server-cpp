#include<curl/curl.h>
using namespace std;

int curl_code = 0;
string shortenUrl(string url) {
    if (url.size() > 80) return url.substr(0, 40) + "..." + url.substr(url.size() - 40, 40);
    return url;
}

#ifdef ENABLE_CURL
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
CURLcode __get_url(string url, string& response) {
    CURL *curl;
    CURLcode res;
    string readBuffer;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, ("Sonolus Server/" + sonolus_server_version).c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &curl_code);
    response = readBuffer;
    return res;
}
#endif
int getRemoteFileLength(string url) {
    #ifdef ENABLE_CURL
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, ("Sonolus Server/" + sonolus_server_version).c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return 0;
    } double length;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &length);
    curl_easy_cleanup(curl);
    return (int)length;
    #else
    writeLog(LOG_LEVEL_ERROR, "This program doesn't support libcurl. You need to compile this program with the option `-DENABLE_CURL -lcurl`.");
    exit(3);
    #endif
}
string get_url(string url, int retry_time = appConfig["export.retryTime"].asInt()) {
    #ifdef ENABLE_CURL
    string response;
    CURLcode res = __get_url(url, response);
    while (res != CURLE_OK) {
        if (retry_time > 0) {
            writeLog(LOG_LEVEL_WARNING, "Failed to download \"" + shortenUrl(url) + "\". retrying...(" + to_string(retry_time) + ")");
            retry_time--;
            res = __get_url(url, response);
        } else {
            writeLog(LOG_LEVEL_ERROR, "Failed to download \"" + shortenUrl(url) + "\".");
            return "";
        }
    } return response;
    #else
    writeLog(LOG_LEVEL_ERROR, "This program doesn't support libcurl. You need to compile this program with the option `-DENABLE_CURL -lcurl`.");
    exit(3);
    #endif
}
