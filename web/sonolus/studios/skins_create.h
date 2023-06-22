using namespace std;

auto sonolus_studios_skins_create = [](client_conn conn, http_request request, param argv){
    if (request.method != "POST") {
        putRequest(conn, 405, __api_default_response);
        send(conn, json_encode(msg[405]));
        exitRequest(conn);
    }

    auto $_POST = postParam(request);
    int id = studios_skinNumber("") + 1;
    string name = $_POST["name"];
    
    int raws = studios_skinCreate(StudiosSkinItem(id, name, "", "", "", SRL<SkinThumbnail>("", ""), false, "[]"));
    if (raws == 0) putRequest(conn, 400, __api_default_response), send(conn, json_encode(msg[400])), exitRequest(conn);

    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(msg[200]));
    exitRequest(conn);
};