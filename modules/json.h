#ifndef JSON_H
#define JSON_H

using namespace std;

string json_encode(Json::Value val) {
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    builder["emitUTF8"] = true;
    unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    ostringstream os;
    writer->write(val, &os);
    return os.str();
}

string json_pretty_encode(Json::Value val) {
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "    ";
    builder["emitUTF8"] = true;
    builder["enableYAMLCompatibility"] = true;
    unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    ostringstream os;
    writer->write(val, &os);
    return os.str();
}

bool json_decode(string json, Json::Value& res) {
    Json::Reader reader;
    return reader.parse(json, res);
}

Json::Value json_decode(string json) {
    Json::Value res;
    Json::Reader reader;
    reader.parse(json, res);
    return res;
}

void json_merge(Json::Value& origin, Json::Value source) {
    if (origin == Json::nullValue) { origin = source; return; }
    if (source.type() == Json::objectValue) {
        auto mem = source.getMemberNames();
        for (auto it = mem.begin(); it != mem.end(); it++) json_merge(origin[*it], source[*it]);
    } else if (source.type() == Json::arrayValue) 
        for (int i = 0; i < source.size(); i++) json_merge(origin[i], source[i]);
    else origin = source;
}

#endif