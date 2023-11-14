using namespace std;

auto sonolus_engines_create = [](client_conn conn, http_request request, param argv){
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
    string tutorialData = $_POST["tutorialData"];
	string previewData = $_POST["previewData"];
	string watchData = $_POST["watchData"];
    string configuration = $_POST["configuration"];
    string rom = $_POST["rom"];
    string localization = $_POST["localization"];
    if (localization == "0") localization = "default";
    else localization = i18n_index[atoi(localization.c_str()) - 1];
    SkinItem skinItem; BackgroundItem backgroundItem; EffectItem effectItem; ParticleItem particleItem;
    auto tmp = skinList("id = " + to_string(skin));
    if (tmp.items.size() == 0) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[404]).size());
        putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
    } skinItem = tmp.items[0];
    auto tmp2 = backgroundList("id = " + to_string(background));
    if (tmp2.items.size() == 0) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[404]).size());
        putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
    } backgroundItem = tmp2.items[0];
    auto tmp3 = effectList("id = " + to_string(effect));
    if (tmp3.items.size() == 0) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[404]).size());
        putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
    } effectItem = tmp3.items[0];
    auto tmp4 = particleList("id = " + to_string(particle));
    if (tmp4.items.size() == 0) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[404]).size());
        putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
    } particleItem = tmp4.items[0];

    int raws = engineCreate(EngineItem(id, name, title, subtitle, author, 
        skinItem, backgroundItem, effectItem, particleItem, SRL<EngineThumbnail>(thumbnail, thumbnail), 
        SRL<EngineData>(data, data), SRL<EngineTutorialData>(tutorialData, tutorialData), SRL<EnginePreviewData>(previewData, previewData), SRL<EngineWatchData>(watchData, watchData),
		SRL<EngineConfiguration>(configuration, configuration), SRL<EngineRom>(rom, rom), $_POST["description"]), localization);
    if (raws == 0) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[400]).size());
        putRequest(conn, 400, __api_default_response), send(conn, json_encode(msg[400])), exitRequest(conn);
    }

    __api_default_response["Content-Length"] = to_string(json_encode(msg[200]).size());
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(msg[200]));
    exitRequest(conn);
};
