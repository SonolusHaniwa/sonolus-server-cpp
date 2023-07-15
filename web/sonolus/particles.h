using namespace std;

auto sonolus_particles = [](client_conn conn, http_request request, param argv){
    auto $_GET = getParam(request);
    Section<ParticleItem> section = particleList("name = \"" + str_replace("\"", "\\\"", argv[0]) + "\" AND localization = \"" + $_GET["localization"] + "\"", 1, 1);
    if (section.items.size() == 0) section = particleList("name = \"" + str_replace("\"", "\\\"", argv[0]) + "\"", 1, 1);
    if (section.items.size() == 0) {
        putRequest(conn, 404, __api_default_response);
        send(conn, json_encode(msg[404]));
        exitRequest(conn);
    }
    ParticleItem item = section.items[0];
    Section<ParticleItem> recommended = particleList("author = \"" + str_replace("\"", "\\\"", item.author) + "\" AND (localization = \"" + $_GET["localization"] + "\" OR localization = \"default\")", 1, 10);
    ItemDetails<ParticleItem> detail = ItemDetails<ParticleItem>(item, item.description);
    detail.recommended = recommended.items;
    Json::Value val = detail.toJsonObject();
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(val));
    exitRequest(conn);
};