using namespace std;

auto sonolus_studios_backgrounds_create = [](client_conn conn, http_request request, param argv){
    if (request.method != "POST") {
        putRequest(conn, 405, __api_default_response);
        send(conn, json_encode(msg[405]));
        exitRequest(conn);
    }

    auto $_POST = postParam(request);
    int id = studios_backgroundNumber("") + 1;
    string name = $_POST["name"];
    
    int raws = studios_backgroundCreate(StudiosBackgroundItem(id, name, "", "", "", SRL<BackgroundThumbnail>("", ""), SRL<BackgroundImage>("", ""), 0, "", "", 0, ""));
    if (raws == 0) putRequest(conn, 400, __api_default_response), send(conn, json_encode(msg[400])), exitRequest(conn);

    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(msg[200]));
    exitRequest(conn);
};