using namespace std;

auto sonolus_backgrounds = [](client_conn conn, http_request request, param argv){
    auto $_GET = getParam(request);
    Section<BackgroundItem> section = backgroundsList("name = \"" + str_replace("\"", "\\\"", argv[0]) + "\" AND localization = \"" + $_GET["localization"] + "\"", 1, 1);
    if (section.items.size() == 0) section = backgroundsList("name = \"" + str_replace("\"", "\\\"", argv[0]) + "\"", 1, 1);
    if (section.items.size() == 0) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[404]).size());
        putRequest(conn, 404, __api_default_response);
        send(conn, json_encode(msg[404]));
        exitRequest(conn);
    }
    BackgroundItem item = section.items[0];
    Section<BackgroundItem> recommended = backgroundsList("author = \"" + str_replace("\"", "\\\"", item.author) + "\" AND (localization = \"" + $_GET["localization"] + "\" OR localization = \"default\")", 1, 10);
    ItemDetails<BackgroundItem> detail = ItemDetails<BackgroundItem>(item, item.description);
    detail.recommended = recommended.items;
    Json::Value val = detail.toJsonObject();
    __api_default_response["Content-Length"] = to_string(json_encode(val).size());
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(val));
    exitRequest(conn);
};