using namespace std;

auto sonolus_levels = [](client_conn conn, http_request request, param argv){
    LevelItem item = levelList("name = \"" + str_replace("\"", "\\\"", argv[0]) + "\"", 1, 1).items[0];
    ItemDetails<LevelItem> detail = ItemDetails<LevelItem>(item, item.title);
    Json::Value val = detail.toJsonObject();
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(val));
    exitRequest(conn);
};