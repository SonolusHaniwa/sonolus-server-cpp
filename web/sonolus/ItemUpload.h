auto SonolusUpload = [](client_conn conn, http_request request, param argv){
    if (!checkLogin(request)) quickSendMsg(401);
    
    string boundary = request.argv["content-type"].substr(request.argv["content-type"].find("boundary=") + 9);
    string data = request.postdata;
    auto fileParts = explode(("\r\n--" + boundary).c_str(), data.c_str());
    for (int i = 1; i < fileParts.size() - 1; i++) {
        string data = fileParts[i];
        string hash = data.substr(data.find("filename=\"") + 10, 40);
        string file = data.substr(data.find("\r\n\r\n") + 4);
        string realHash = sha1(file);
        if (hash != realHash) quickSendMsg(403);
        ofstream fout("./data/" + hash, ios::binary);
        fout.write(file.c_str(), file.size());
        fout.close();
    }

    quickSendMsg(200);
};