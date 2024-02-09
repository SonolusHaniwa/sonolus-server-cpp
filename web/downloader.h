using namespace std;
#define __int64_t long long

auto downloader = [](client_conn conn, http_request request, param argv){
    /** 打开文件 */
    ifstream fin(("./data/" + argv[0]).c_str(), ios::binary);
    if (!fin) quickSendMsg(404);

    /** 构造基础响应头 */
    argvar response = __default_response;
    response = merge(response, mime(".bin"));

    /** 获取文件大小信息 */
    fin.seekg(0, ios::end);
    const __int64_t len = 1024 * 1024;
    __int64_t filesize = fin.tellg();
    __int64_t st = 0, en = filesize - 1;

    /** 获取断点续传范围信息 */
    if (request.argv.find("range") != request.argv.end()) {
        string range = explode("=", request.argv["range"].c_str())[1];
        param val = explode("-", range.c_str());
        st = atoi(val[0].c_str()), en = atoi(val[1].c_str());
        if (en == 0) en = filesize - 1;
    }

    /** 构造响应头并发送 */
    __int64_t output_len = en - st + 1;
    response["Content-Length"] = to_string(output_len);
    response["Content-Range"] = "bytes " + to_string(st) + "-" + to_string(en) + "/" + to_string(filesize);
    putRequest(conn, request.argv.find("range") != request.argv.end() ? 206 : 200, response);

    /** 从st起每次读取len长度的数据并发送 */
    fin.seekg(st, ios::beg);
    while (output_len) {
        char* content = new char[min(len, output_len)];
        fin.read(content, min(len, output_len));
        int s = send(conn, content, min(len, output_len));
        delete[] content;
        if (s == -1) fin.close(), exitRequest(conn);
        output_len -= min(len, output_len);
    } fin.close();
    exitRequest(conn);
};