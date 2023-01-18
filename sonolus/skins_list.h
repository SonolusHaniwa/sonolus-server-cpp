using namespace std;

auto sonolus_skins_list = [](client_conn conn, http_request request, param argv){
    auto $_GET = getParam(request);
    string filter = "";
    if ($_GET["keywords"] != "") filter = "title like \"%" + str_replace("\"", "\\\"", $_GET["keywords"]) + "%\"";
    Section<SkinItem> res = skinList(filter);
    Json::Value val = res.toJsonObject();
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(val));
    exitRequest(conn);
};