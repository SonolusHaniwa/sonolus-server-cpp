#ifndef GZIP_H
#define GZIP_H

#include<zlib.h>

using namespace std;

class buffer {
    private:
    vector<char> v;

    public:
    buffer(){v.clear();}
    buffer(string res) {
        v.clear();
        for (int i = 0; i < res.size(); i++) v.push_back(res[i]);
    }
    buffer(char* ch, int len) {
        v.clear();
        for (int i = 0; i < len; i++) v.push_back(ch[i]);
    }
    
    void push(char x) {
        v.push_back(x);
    }
    string asString() {
        string res;
        for (int i = 0; i < v.size(); i++) res.push_back(v[i]);
        return res;
    }
    const char* asCString() {
        string res;
        for (int i = 0; i < v.size(); i++) res.push_back(v[i]);
        return res.c_str();
    }
    int size() {
        return v.size();
    }
    void clear() {
        v.clear();
    }
    buffer substr(int st, int len = (1ll << 31) - 1) {
        if (st < 0) st = size() + st;
        buffer res; for (int i = 0, j = st; i < len && j < v.size(); i++, j++) res.push(v[j]);
        return res;
    }
};

#define MOD_GZIP_ZLIB_WINDOWSIZE 15
#define MOD_GZIP_ZLIB_CFACTOR    9
#define MOD_GZIP_ZLIB_BSIZE      8096

buffer compress_gzip(buffer str, int compressionlevel = Z_BEST_COMPRESSION) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (deflateInit2(&zs, 
                     compressionlevel,
                     Z_DEFLATED,
                     MOD_GZIP_ZLIB_WINDOWSIZE + 16, 
                     MOD_GZIP_ZLIB_CFACTOR,
                     Z_DEFAULT_STRATEGY) != Z_OK
    ) throw(std::runtime_error("deflateInit2 failed while compressing."));
    zs.next_in = (Bytef*)str.asCString();
    zs.avail_in = str.size();
    int ret;
    char outbuffer[32768];
    std::string outstring;
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = deflate(&zs, Z_FINISH);
        if (outstring.size() < zs.total_out) outstring.append(outbuffer, zs.total_out - outstring.size());
    } while (ret == Z_OK);
    deflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }
    return buffer(outstring);
}

buffer compress_deflate(buffer str, int compressionlevel = Z_BEST_COMPRESSION) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (deflateInit(&zs, compressionlevel) != Z_OK)
        throw(std::runtime_error("deflateInit failed while compressing."));
    zs.next_in = (Bytef*)str.asCString();
    zs.avail_in = str.size();
    int ret;
    char outbuffer[32768];
    std::string outstring;
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = deflate(&zs, Z_FINISH);
        if (outstring.size() < zs.total_out)
            outstring.append(outbuffer, zs.total_out - outstring.size());
    } while (ret == Z_OK);
    deflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }
	return buffer(outstring);
}

buffer decompress_deflate(buffer str) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));
    zs.next_in = (Bytef*)str.asCString();
    zs.avail_in = str.size();
    int ret;
    char outbuffer[32768];
    std::string outstring;
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = inflate(&zs, 0);
        if (outstring.size() < zs.total_out)
            outstring.append(outbuffer, zs.total_out - outstring.size());
    } while (ret == Z_OK);
    inflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }
    return buffer(outstring);
}

buffer decompress_gzip(buffer str) {
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    if (inflateInit2(&zs, MOD_GZIP_ZLIB_WINDOWSIZE + 16) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));
    zs.next_in = (Bytef*)str.asCString();
    zs.avail_in = str.size();
    int ret;
    char outbuffer[100010];
    std::string outstring;
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);
        ret = inflate(&zs, 0);
        if (outstring.size() < zs.total_out)
            outstring.append(outbuffer, zs.total_out - outstring.size());
    } while (ret == Z_OK);
    inflateEnd(&zs);
    if (ret != Z_STREAM_END) {
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }
    return buffer(outstring);
}

#endif