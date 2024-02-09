using namespace std;

auto js_import = [](client_conn conn, http_request request, param argv) {
    string filename = argv[0];
    ifstream fin(("./web/js/" + filename).c_str());
    if (!fin) quickSendMsg(404);
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    fin.seekg(0, ios::beg);
    char* ch = new char[len];
    __default_response["Content-Length"] = to_string(len);
    putRequest(conn, 200, merge(__default_response, mime(".js")));
    fin.read(ch, len);
    send(conn, ch, len);
    delete[] ch;
    exitRequest(conn);
};