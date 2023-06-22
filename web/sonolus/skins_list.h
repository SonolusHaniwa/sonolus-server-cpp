using namespace std;

auto sonolus_skins_list = [](client_conn conn, http_request request, param argv){
    auto $_GET = getParam(request);
    int page = atoi($_GET["page"].c_str());
    Section<SkinItem> res = skinList(skinFilter($_GET), page * 20 + 1, (page + 1) * 20);
    Json::Value val = res.toJsonObject();
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(val));
    exitRequest(conn);
};