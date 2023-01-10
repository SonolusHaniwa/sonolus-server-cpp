using namespace std;

auto sonolus_engines = [](client_conn conn, http_request request, param argv){
    EngineItem item = engineList("name = \"" + str_replace("\"", "\\\"", argv[0]) + "\"", 1, 1).items[0];
    ItemDetails<EngineItem> detail = ItemDetails<EngineItem>(item, item.title);
    Json::Value val = detail.toJsonObject();
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(val));
    exitRequest(conn);
};