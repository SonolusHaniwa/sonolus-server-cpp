#include<bits/stdc++.h>
#include<jsoncpp/json/json.h>
#include<curl/curl.h>
using namespace std;

string readFile(string path) {
    ifstream fin(path.c_str());
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    if (len == -1) return "";
    fin.seekg(0, ios::beg);
    char* ch = new char[len];
    fin.read(ch, len);
    string buffer;
    for (int i = 0; i < len; i++) buffer.push_back(ch[i]);
    delete[] ch;
    return buffer;
}

string shortenUrl(string url) {
    if (url.size() > 80) return url.substr(0, 40) + "..." + url.substr(url.size() - 40, 40);
    return url;
}
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
CURLcode __get_url(string url, string& response) {
	int curl_code = 0;
    CURL *curl;
    CURLcode res;
    string readBuffer;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Sonolus Server/1.6.3");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &curl_code);
    response = readBuffer;
    return res;
}
string get_url(string url, int retry_time = 5) {
    string response;
    CURLcode res = __get_url(url, response);
    while (res != CURLE_OK) {
        if (retry_time > 0) {
            cerr << "Failed to download \"" << shortenUrl(url) << "\". retrying...(" << to_string(retry_time) << ")" << endl;
            retry_time--;
            res = __get_url(url, response);
        } else {
            cerr << "Failed to download \"" << shortenUrl(url) << "\"." << endl;
            return "";
        }
    } return response;
}

Json::Value json_decode(string json) {
    Json::Value res;
    Json::Reader reader;
    reader.parse(json, res);
    return res;
}

string json_pretty_encode(Json::Value obj) {
	Json::StreamWriterBuilder builder;
	builder["emitUTF8"] = true;
	builder["enableYAMLCompatibility"] = true;
	return Json::writeString(builder, obj);
}

int main(int argc, char** argv) {
	if (argc < 3) {
		cout << "Usage: " << argc << " <i18n> <branch>";
		return 1;
	}
	string i18n_name = argv[1], branch = argv[2];
	string url = "https://raw.githubusercontent.com/Sonolus/api-venus/" + branch + "/src/localizations/" + i18n_name + "/Localization.json";
	string orig_raw = get_url(url);
	auto orig = json_decode(orig_raw)["Texts"];
	string i18n_raw = readFile(i18n_name + ".json");
	auto i18n = json_decode(i18n_raw);
	cout << i18n_name << ".json: " << i18n.size() << " -> ";
	Json::Value i18n2;
	auto members = i18n.getMemberNames();
	for (auto member: members) {
		string key = member, val = i18n[member].asString();
		if (key[0] >= 'A' && key[0] <= 'Z') continue;
		i18n2[key] = val;
	}	
	members = orig.getMemberNames();
	for (auto member: members) {
		string key = member, val = orig[member].asString();
		key = key.substr(1);
		i18n2[key] = val;
	}
	cout << i18n2.size() << endl;
	string res = json_pretty_encode(i18n2);
	ofstream fout(i18n_name + ".json", ios::binary);
	fout.write(res.c_str(), res.size());
}