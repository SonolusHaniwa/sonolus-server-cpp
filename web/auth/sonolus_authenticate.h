using namespace std;

const string sonolus_public_key = 
"-----BEGIN PUBLIC KEY-----\n"
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA8DDWNplPkFiQI2QywLOT\n"
"OLAsIA+H0kc9RjFK4pJV6MKJxvhAGsJ8uA18Wsug4YU7Kp93gV3Zv7/RlV0yMkWv\n"
"CxhsQO/K9NI5MdyJSxTI7UcVukZDAQbiFBT+/1od28XKhn6eO2PqI3E7uXpN44Cd\n"
"O7rgtLSYBBT1/+Aw/gJHn+u5fo60xusfPEYYpXNnIHEL52niNW52wmk/LGItZDlJ\n"
"+oSwZH2qRFol6t63ymzFUNbred0DwJD+RmqWEq/J/57ofCaL65148BmD2KkJoA8k\n"
"MR4hNOP9cYs7iQQguboCa0SsJPl4V2SOG+Mn6IkSkZJRfYkC3SXdjmxf+i4qA801\n"
"RQIDAQAB\n"
"-----END PUBLIC KEY-----";

auto auth_sonolus_authenticate = [](client_conn conn, http_request request, param argv){
    Json::Value val;
    string address = "";
    val["address"] = (https ? "https://" : "http://") + appConfig["server.rootUrl"].asString() + "/auth";
    string id = "", key = "", iv = "";
    for (int i = 0; i < 32; i++) {
        switch(rand() % 2) {
            case 0: id += rand() % 10 + '0'; break;
            case 1: id += rand() % 6 + 'A'; break;
        }
    }
    for (int i = 0; i < 32; i++) {
        switch(rand() % 2) {
            case 0: key += rand() % 10 + '0'; break;
            case 1: key += rand() % 6 + 'A'; break;
        }
    }
    for (int i = 0; i < 16; i++) {
        switch(rand() % 2) {
            case 0: iv += rand() % 10 + '0'; break;
            case 1: iv += rand() % 6 + 'A'; break;
        }
    }
    db.execute("INSERT INTO UserSession (id, aes_key, aes_iv, time) VALUES (\"" 
        + id + "\", \"" + key + "\", \"" + iv + "\", " + to_string(time(NULL)) + ")");
    key = base64_encode(const_cast<char*>(key.c_str()), key.size());
    iv = base64_encode(const_cast<char*>(iv.c_str()), iv.size());
    Json::Value session; session["id"] = id; session["key"] = key; session["iv"] = iv;
    val["session"] = rsa_encode(json_encode(session), sonolus_public_key);
    val["expiration"] = (Json::Int64)((time(NULL) + 3600LL) * 1000LL);
    __api_default_response["Content-Length"] = to_string(json_encode(val).size());
    putRequest(conn, 200, __api_default_response);
    send(conn, json_encode(val));
    exitRequest(conn);
};