using namespace std;

auto sonolus_levels = [](client_conn conn, http_request request, param argv){
    Section<LevelItem> section = levelList("name = \"" + str_replace("\"", "\\\"", argv[0]) + "\"", 1, 1);
    if (section.items.size() == 0) {
        putRequest(conn, 404, __api_default_response);
        send(conn, json_encode(msg[404]));
        exitRequest(conn);
    }
    LevelItem item = section.items[0];
    Section<LevelItem> recommended = levelList("author = \"" + str_replace("\"", "\\\"", item.author) + "\"", 1, 10);
    ItemDetails<LevelItem> detail = ItemDetails<LevelItem>(item, item.title);
    detail.recommended = recommended.items;
    Json::Value val = detail.toJsonObject();
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(val));
    exitRequest(conn);
};