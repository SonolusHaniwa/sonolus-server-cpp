using namespace std;

auto sonolus_engines = [](client_conn conn, http_request request, param argv){
    Section<EngineItem> section = engineList("name = \"" + str_replace("\"", "\\\"", argv[0]) + "\"", 1, 1);
    if (section.items.size() == 0) {
        putRequest(conn, 404, __api_default_response);
        send(conn, json_encode(msg[404]));
        exitRequest(conn);
    }
    EngineItem item = section.items[0];
    ItemDetails<EngineItem> detail = ItemDetails<EngineItem>(item, item.title);
    Json::Value val = detail.toJsonObject();
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(val));
    exitRequest(conn);
};