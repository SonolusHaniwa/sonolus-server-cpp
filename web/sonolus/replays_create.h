using namespace std;

auto sonolus_replays_create = [](client_conn conn, http_request request, param argv){
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
    int id = replayNumber("") + 1;
    string name = $_POST["name"];
    int version = ReplayItem().version;
    string title = $_POST["title"];
    string subtitle = $_POST["subtitle"];
    string author = $_POST["author"];
    int level = atoi($_POST["level"].c_str());
    string data = $_POST["data"];
    string configuration = $_POST["configuration"];
    string localization = $_POST["localization"];
    if (localization == "0") localization = "default";
    else localization = i18n_index[atoi(localization.c_str()) - 1];

    auto tmp = levelList("id = " + to_string(level));
    if (tmp.items.size() == 0) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[404]).size());
        putRequest(conn, 404, __api_default_response), send(conn, json_encode(msg[404])), exitRequest(conn);
    } LevelItem levelItem = tmp.items[0];
    int raws = replayCreate(ReplayItem(
        id, name, title, subtitle, author, levelItem,
        SRL<ReplayData>(data, data), SRL<ReplayConfiguration>(configuration, configuration), $_POST["description"]), localization);
    if (raws == 0) {
        __api_default_response["Content-Length"] = to_string(json_encode(msg[400]).size());
        putRequest(conn, 400, __api_default_response), send(conn, json_encode(msg[400])), exitRequest(conn);
    }

    __api_default_response["Content-Length"] = to_string(json_encode(msg[200]).size());
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(msg[200]));
    exitRequest(conn);
};
