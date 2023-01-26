#ifndef JSON_H
#define JSON_H

using namespace std;

string json_encode(Json::Value val) {
    Json::FastWriter writer;
    return writer.write(val);
}

string json_pretty_encode(Json::Value val) {
    Json::StyledWriter writer;
    return writer.write(val);
}

bool json_decode(string json, Json::Value& res) {
    Json::Reader reader;
    return reader.parse(json, res);
}

argvar transfer(Json::Value source, string prefix = "") {
    argvar result;
    Json::Value::Members mem = source.getMemberNames();
    for (auto it = mem.begin(); it != mem.end(); it++) {
        if (source[*it].type() == Json::objectValue) continue;
        result[prefix + (*it)] = source[*it].asString();
    } return result;
}

#endif