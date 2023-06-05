using namespace std;
auto uploader = [](client_conn conn, http_request request, param argv){
    if (!checkLogin(request)) {
        putRequest(conn, 401, __default_response);
        send(conn, json_encode(msg[401]));
        exitRequest(conn);
    }
    auto $_POST = postParam(request); int len;
    char* filePointerBeg = base64_decode($_POST["file"], len);
    unsigned char* fileSha1 = sha1(filePointerBeg, len);
    stringstream buffer;
    for (int i = 0; i < 20; i++)
        buffer << hex << setw(2) << setfill('0') << int(fileSha1[i]);
    ofstream fout(("./data/" + buffer.str()).c_str());
    fout.write(filePointerBeg, len); fout.close();
    free(filePointerBeg); free(fileSha1);
    Json::Value res;
    res["status"] = "success";
    res["hash"] = buffer.str();
    res["url"] = "/data/" + buffer.str();
    putRequest(conn, 200, __default_response);
    send(conn, json_encode(res));
    exitRequest(conn);
};