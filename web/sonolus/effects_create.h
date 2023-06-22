using namespace std;

auto sonolus_effects_create = [](client_conn conn, http_request request, param argv){
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
    int id = effectNumber("") + 1;
    string name = $_POST["name"];
    string title = $_POST["title"];
    string subtitle = $_POST["subtitle"];
    string author = $_POST["author"];
    string thumbnail = $_POST["thumbnail"];
    string data = $_POST["data"];
    string audio = $_POST["audio"];

    int raws = effectCreate(EffectItem(id, name, title, subtitle, author, 
        SRL<EffectThumbnail>(thumbnail, thumbnail), SRL<EffectData>(data, data), SRL<EffectAudio>(audio, audio)));
    if (raws == 0) putRequest(conn, 400, __api_default_response), send(conn, json_encode(msg[400])), exitRequest(conn);

    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(msg[200]));
    exitRequest(conn);
};