using namespace std;

auto sonolus_levels_create = [](client_conn conn, http_request request, param argv){
    if (request.method != "POST") {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[405]).size());
        putRequest(conn, 405, __api_default_response);
        send(conn, json_encode(msg[405]));
        exitRequest(conn);
    }
    if (!checkLogin(request)) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[401]).size());
        putRequest(conn, 401, __api_default_response);
        send(conn, json_encode(msg[401]));
        exitRequest(conn);
    }
    
    auto $_POST = postParam(request);
    int id = levelNumber("") + 1;
    string name = $_POST["name"];
    int version = LevelItem().version;
    int rating = atoi($_POST["rating"].c_str());
    string title = $_POST["title"];
    string artists = $_POST["artists"];
    string author = $_POST["author"];
    int engine = atoi($_POST["engine"].c_str());
    int skin = atoi($_POST["skin"].c_str());
    int background = atoi($_POST["background"].c_str());
    int effect = atoi($_POST["effect"].c_str());
    int particle = atoi($_POST["particle"].c_str());
    string cover = $_POST["cover"];
    string bgm = $_POST["bgm"];
    string data = $_POST["data"];
    string preview = $_POST["preview"];
    string localization = $_POST["localization"];
    if (localization == "0") localization = "default";
    else localization = i18n_index[atoi(localization.c_str()) - 1];

    auto tmp = engineList("id = " + to_string(engine));
    if (tmp.items.size() == 0) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[404]).size());
        putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
    } EngineItem engineItem = tmp.items[0];
    UseItem<SkinItem> skinItem; UseItem<BackgroundItem> backgroundItem; 
    UseItem<EffectItem> effectItem; UseItem<ParticleItem> particleItem;
    if (skin == 0) skinItem.useDefault = true;
    else {
        auto tmp = skinList("id = " + to_string(skin));
        if (tmp.items.size() == 0) {
            __api_default_response["Content-Length"] = to_string(json_encode(msg[404]).size());
            putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
        } skinItem.item = tmp.items[0];
        skinItem.useDefault = false;
    }
    if (background == 0) backgroundItem.useDefault = true;
    else {
        auto tmp = backgroundList("id = " + to_string(background));
        if (tmp.items.size() == 0) {
            __api_default_response["Content-Length"] = to_string(json_encode(msg[404]).size());
            putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
        } backgroundItem.item = tmp.items[0];
        backgroundItem.useDefault = false;
    }
    if (effect == 0) effectItem.useDefault = true;
    else {
        auto tmp = effectList("id = " + to_string(effect));
        if (tmp.items.size() == 0) {
            __api_default_response["Content-Length"] = to_string(json_encode(msg[404]).size());
            putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
        } effectItem.item = tmp.items[0];
        effectItem.useDefault = false;
    }
    if (particle == 0) particleItem.useDefault = true;
    else {
        auto tmp = particleList("id = " + to_string(particle));
        if (tmp.items.size() == 0) {
            __api_default_response["Content-Length"] = to_string(json_encode(msg[404]).size());
            putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
        } particleItem.item = tmp.items[0];
        particleItem.useDefault = false;
    }
    int raws = levelCreate(LevelItem(
        id, name, rating, title, artists, author, 
        engineItem, skinItem, backgroundItem, effectItem, particleItem, 
        SRL<LevelCover>(cover, cover), SRL<LevelBgm>(bgm, bgm), SRL<LevelData>(data, data), SRL<LevelPreview>(preview, preview), $_POST["description"]), localization);
    if (raws == 0) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[400]).size());
        putRequest(conn, 400, __api_default_response), send(conn, json_encode(msg[400])), exitRequest(conn);
    }

    __api_default_response["Content-Length"] = to_string(json_encode(msg[200]).size());
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(msg[200]));
    exitRequest(conn);
};