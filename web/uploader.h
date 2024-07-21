using namespace std;
auto uploader = [](client_conn conn, http_request request, param argv){
    if (!checkLogin(request)) quickSendMsg(401);
    auto $_POST = postParam(request);
    string filePointerBeg = base64_decode($_POST["file"]);
    int len = filePointerBeg.size();
    unsigned char* fileSha1 = sha1(const_cast<char*>(filePointerBeg.c_str()), len);
    stringstream buffer;
    for (int i = 0; i < 20; i++) buffer << hex << setw(2) << setfill('0') << int(fileSha1[i]);
    ofstream fout(("./data/" + buffer.str()).c_str(), ios::binary);
    fout.write(filePointerBeg.c_str(), len); fout.close();
    free(fileSha1);
    Json::Value res;
    res["status"] = "success";
    res["hash"] = buffer.str();
    res["url"] = dataPrefix + buffer.str();
    quickSendObj(res);
};