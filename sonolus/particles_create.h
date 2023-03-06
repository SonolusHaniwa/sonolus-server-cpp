using namespace std;

auto sonolus_particles_create = [](client_conn conn, http_request request, param argv){
    if (request.method != "POST") {
        putRequest(conn, 405, __api_default_response);
        send(conn, json_encode(msg[405]));
        exitRequest(conn);
    }

    auto $_POST = postParam(request);
    int id = particleNumber("") + 1;
    string name = $_POST["name"];
    string title = $_POST["title"];
    string subtitle = $_POST["subtitle"];
    string author = $_POST["author"];
    string thumbnail = $_POST["thumbnail"];
    string data = $_POST["data"];
    string texture = $_POST["texture"];

    int raws = particleCreate(ParticleItem(id, name, title, subtitle, author,
        SRL<ParticleThumbnail>(thumbnail, thumbnail), SRL<ParticleData>(data, data), SRL<ParticleTexture>(texture, texture)));
    if (raws == 0) putRequest(conn, 400, __api_default_response), send(conn, json_encode(msg[400])), exitRequest(conn);

    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(msg[200]));
    exitRequest(conn);
};