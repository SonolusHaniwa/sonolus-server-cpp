using namespace std;

auto sonolus_backgrounds_list = [](client_conn conn, http_request request, param argv){
    auto $_GET = getParam(request);
    int page = atoi($_GET["page"].c_str());
    Section<BackgroundItem> res = backgroundsList(backgroundFilter($_GET), page * 20 + 1, (page + 1) * 20);
    Json::Value val = res.toJsonObject();
    __api_default_response["Content-Length"] = to_string(json_encode(val).size());
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(val));
    exitRequest(conn);
};