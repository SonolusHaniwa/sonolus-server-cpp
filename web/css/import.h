using namespace std;

auto css_import = [](client_conn conn, http_request request, param argv) {
    string filename = argv[0];
    ifstream fin(("./web/css/" + filename).c_str());
    if (!fin) quickSendMsg(404);
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    fin.seekg(0, ios::beg);
    char* ch = new char[len];
    fin.read(ch, len);
    __default_response["Content-Length"] = to_string(len);
    putRequest(conn, 200, merge(__default_response, mime(".css")));
    send(conn, ch, len);
    delete[] ch;
    exitRequest(conn);
};