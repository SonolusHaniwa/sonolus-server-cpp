using namespace std;

auto auth_sonolus_levels = [](client_conn conn, http_request request, param argv){
    const string yes_sha1 = "87e0c3a0879345e2721f71b0540c5bd8a0f8d6c3";
    const string no_sha1 = "bd32c9acb1607e6ff65b52c75d8566667c75902f";

    // 身份验证
    if (request.argv.find("Sonolus-Session-Id") == request.argv.end()
        || request.argv.find("Sonolus-Session-Data") == request.argv.end()) {
        putRequest(conn, 401, __api_default_response);
        send(conn, json_encode(msg[401]));
        exitRequest(conn);
    }
    string session_id = request.argv["Sonolus-Session-Id"];
    string session_data = request.argv["Sonolus-Session-Data"];
    auto res = (new DB_Controller)->query("SELECT * FROM UserSession WHERE id=\"" + session_id + "\" LIMIT 1");
    LevelItem errorItem = LevelItem(
        0, "sonolus-server-auth", 0, "Login Failed",
        "Please see the error info below!", "System",
        EngineItem(0, "sonolus-server-auth", "(Message)", "", "", {}, {}, {}, {}, {}, {}, {}, {}), 
        UseItem<SkinItem>(), UseItem<BackgroundItem>(), UseItem<EffectItem>(), UseItem<ParticleItem>(),
        SRL<LevelCover>(no_sha1, "/data/" + no_sha1), SRL<LevelBgm>(), SRL<LevelData>(), SRL<LevelPreview>()
    );
    if (res.size() == 0) {
        putRequest(conn, 401, __api_default_response);
        send(conn, json_encode(msg[401]));
        exitRequest(conn);
    }
    string key = res[0]["aes_key"];
    string iv = res[0]["aes_iv"];

    // 请求查询
    res = (new DB_Controller)->query("SELECT * FROM LoginRequest WHERE code=\"" + argv[0] + "\" AND userId=0 LIMIT 1");
    if (res.size() == 0) {
        ItemDetails<LevelItem> detail = ItemDetails<LevelItem>(errorItem, "Invalid Login Request Key!");
        Json::Value val = detail.toJsonObject();
        putRequest(conn, 200, __api_default_response);
        send(conn, json_encode(val));
        exitRequest(conn);
    }

    // 用户信息处理
    int data_length = 0;
    char* sessionData = base64_decode(session_data, data_length);
    session_data.clear(); for (int i = 0; i < data_length; i++) session_data.push_back(sessionData[i]);
    string userData = aes_256_cbc_decode(key, iv, session_data);
    Json::Value userInfo; json_decode(userData, userInfo); userInfo = userInfo["userProfile"];
    res = (new DB_Controller)->query("SELECT * FROM UserProfile WHERE id=\"" + userInfo["id"].asString() + "\"");
    if (res.size() == 0) {
        string sql = "INSERT INTO UserProfile (aboutMe, avatarBackgroundColor, avatarForegroundColor, " +
            string("favorites, handle, id, name, socialLinks) VALUES (\"") + quote_encode(userInfo["aboutMe"].asString()) + "\", \"" +
            quote_encode(userInfo["avatarBackgroundColor"].asString()) + "\", \"" + quote_encode(userInfo["avatarForegroundColor"].asString()) + "\", \"" +
            quote_encode(json_encode(userInfo["favorites"])) + "\", \"" + quote_encode(userInfo["handle"].asString()) + "\", \"" + quote_encode(userInfo["id"].asString()) + "\", \"" +
            quote_encode(userInfo["name"].asString()) + "\", \"" + quote_encode(json_encode(userInfo["socialLinks"])) + "\")";
        (new DB_Controller)->execute(sql);
    }
    (new DB_Controller)->execute("UPDATE LoginRequest SET userId=\"" + userInfo["id"].asString() + "\" WHERE code=\"" + argv[0] + "\" AND userId=0");

    // 结果展示
    LevelItem item = LevelItem(
        0, "sonolus-server-auth", 0, "Login Success",
        "You can refresh the browser to see the result!", "System",
        EngineItem(0, "sonolus-server-auth", "(Message)", "", "", {}, {}, {}, {}, {}, {}, {}, {}), 
        UseItem<SkinItem>(), UseItem<BackgroundItem>(), UseItem<EffectItem>(), UseItem<ParticleItem>(),
        SRL<LevelCover>(yes_sha1, "/data/" + yes_sha1), SRL<LevelBgm>(), SRL<LevelData>(), SRL<LevelPreview>()
    );

    ItemDetails<LevelItem> detail = ItemDetails<LevelItem>(item, "");
    Json::Value val = detail.toJsonObject();
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(val));
    exitRequest(conn);
};

/*
{
    "address" : "http://192.168.0.102:8080/auth",
    "userProfile" : 
    {
        "aboutMe" : "A sonolus api website developer\nproject address: https://github.com/LittleYang0531/sonolus-server-cpp",
        "avatarBackgroundColor" : "#000020ff",
        "avatarForegroundColor" : "#ffffffff",
        "favorites" : [],
        "handle" : "84718",
        "id" : "398d54edc7f22aab8934cf020a7cb0cdaa6532f3c6b4e0e3a043eb2d89633ba0",
        "name" : "LittleYang0531",
        "socialLinks" : 
        [
            {
                "address" : "https://github.com/LittleYang0531/sonolus-server-cpp",
                "title" : "Sonolus Server"
            }
        ]
    }
}
*/