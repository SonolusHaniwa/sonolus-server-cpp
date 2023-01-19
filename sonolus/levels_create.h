using namespace std;

auto sonolus_levels_create = [](client_conn conn, http_request request, param argv){
    if (request.method != "POST") {
        putRequest(conn, 405, __api_default_response);
        send(conn, json_encode(msg[405]));
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

    string sql = "INSERT INTO Level (id, name, version, rating, title, artists, author, engine, skin, background, effect, particle, cover, bgm, data, preview) VALUES (" 
        + to_string(id) + ", \"" + name + "\", " + to_string(version) + ", " + to_string(rating) + ", \"" + title + "\", \"" + artists + "\"" +
        ", \"" + author + "\", " + to_string(engine) + ", " + to_string(skin) + ", " + to_string(background) + ", " + to_string(effect) + ", " + to_string(particle) + ", \""
        + cover + "\", \"" + bgm + "\", \"" + data + "\", \"" + preview + "\")";
    mysqli_execute(mysql, sql.c_str());

    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(msg[200]));
    exitRequest(conn);
};