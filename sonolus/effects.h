using namespace std;

auto sonolus_effects = [](client_conn conn, http_request request, param argv){
    Section<EffectItem> section = effectList("name = \"" + str_replace("\"", "\\\"", argv[0]) + "\"", 1, 1);
    if (section.items.size() == 0) {
        putRequest(conn, 404, __api_default_response);
        send(conn, json_encode(msg[404]));
        exitRequest(conn);
    }
    EffectItem item = section.items[0];
    ItemDetails<EffectItem> detail = ItemDetails<EffectItem>(item, item.title);
    Json::Value val = detail.toJsonObject();
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(val));
    exitRequest(conn);
};