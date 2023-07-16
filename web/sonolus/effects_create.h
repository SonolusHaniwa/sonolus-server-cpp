using namespace std;

auto sonolus_effects_create = [](client_conn conn, http_request request, param argv){
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
    int id = effectNumber("") + 1;
    string name = $_POST["name"];
    string title = $_POST["title"];
    string subtitle = $_POST["subtitle"];
    string author = $_POST["author"];
    string thumbnail = $_POST["thumbnail"];
    string data = $_POST["data"];
    string audio = $_POST["audio"];
    string localization = $_POST["localization"];
    if (localization == "0") localization = "default";
    else localization = i18n_index[atoi(localization.c_str()) - 1];

    int raws = effectCreate(EffectItem(id, name, title, subtitle, author, 
        SRL<EffectThumbnail>(thumbnail, thumbnail), SRL<EffectData>(data, data), SRL<EffectAudio>(audio, audio), $_POST["description"]), localization);
    if (raws == 0) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[400]).size());
        putRequest(conn, 400, __api_default_response), send(conn, json_encode(msg[400])), exitRequest(conn);
    }

    __api_default_response["Content-Length"] = to_string(json_encode(msg[200]).size());
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(msg[200]));
    exitRequest(conn);
};