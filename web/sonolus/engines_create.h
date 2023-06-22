using namespace std;

auto sonolus_engines_create = [](client_conn conn, http_request request, param argv){
    if (request.method != "POST") {
        putRequest(conn, 405, __api_default_response);
        send(conn, json_encode(msg[405]));
        exitRequest(conn);
    }
    if (!checkLogin(request)) {
        putRequest(conn, 401, __api_default_response);
        send(conn, json_encode(msg[401]));
        exitRequest(conn);
    }

    auto $_POST = postParam(request);
    int id = engineNumber("") + 1;
    string name = $_POST["name"];
    string title = $_POST["title"];
    string subtitle = $_POST["subtitle"];
    string author = $_POST["author"];
    int skin = atoi($_POST["skin"].c_str());
    int background = atoi($_POST["background"].c_str());
    int effect = atoi($_POST["effect"].c_str());
    int particle = atoi($_POST["particle"].c_str());
    string thumbnail = $_POST["thumbnail"];
    string data = $_POST["data"];
    string configuration = $_POST["configuration"];
    string rom = $_POST["rom"];
    SkinItem skinItem; BackgroundItem backgroundItem; EffectItem effectItem; ParticleItem particleItem;
    auto tmp = skinList("id = " + to_string(skin));
    if (tmp.items.size() == 0) putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
    skinItem = tmp.items[0];
    auto tmp2 = backgroundList("id = " + to_string(background));
    if (tmp2.items.size() == 0) putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
    backgroundItem = tmp2.items[0];
    auto tmp3 = effectList("id = " + to_string(effect));
    if (tmp3.items.size() == 0) putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
    effectItem = tmp3.items[0];
    auto tmp4 = particleList("id = " + to_string(particle));
    if (tmp4.items.size() == 0) putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
    particleItem = tmp4.items[0];

    int raws = engineCreate(EngineItem(id, name, title, subtitle, author, 
        skinItem, backgroundItem, effectItem, particleItem, SRL<EngineThumbnail>(thumbnail, thumbnail), 
        SRL<EngineData>(data, data), SRL<EngineConfiguration>(configuration, configuration), SRL<EngineRom>(rom, rom)));
    if (raws == 0) putRequest(conn, 400, __api_default_response), send(conn, json_encode(msg[400])), exitRequest(conn);

    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(msg[200]));
    exitRequest(conn);
};