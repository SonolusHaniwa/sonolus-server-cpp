#ifndef ENCRYPT_H
#define ENCRYPT_H
#include<openssl/ssl.h>
#include<openssl/aes.h>
#include<openssl/err.h>
#include<openssl/ec.h>
#include<openssl/ecdsa.h>
#include<openssl/obj_mac.h>

string str_replace(string from, string to, string source, bool supportTranfer = false) {
    string result = source;
	int st = 0, wh = result.find(from.c_str(), st);
	while (wh != string::npos) {
        if (supportTranfer && wh >= 1 && result[wh - 1] == '\\') {
            st = wh + 1;
            wh = result.find(from.c_str(), st);
            continue;
        } 
        result.replace(wh, from.size(), to.c_str());
		st = wh + to.size();
		wh = result.find(from.c_str(), st);
	} 
    return result;
}

// base64编码部分
std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";


inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(char* bytes_to_encode, int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];  // store 3 byte of bytes_to_encode
    unsigned char char_array_4[4];  // store encoded character to 4 bytes

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);  // get three bytes (24 bits)
        if (i == 3) {
            // eg. we have 3 bytes as ( 0100 1101, 0110 0001, 0110 1110) --> (010011, 010110, 000101, 101110)
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2; // get first 6 bits of first byte,
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4); // get last 2 bits of first byte and first 4 bit of second byte
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6); // get last 4 bits of second byte and first 2 bits of third byte
            char_array_4[3] = char_array_3[2] & 0x3f; // get last 6 bits of third byte

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}

string base64_decode(std::string encoded_string) {
    size_t in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    vector<int> s;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]) & 0xff;

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                s.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = 0; j < i; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]) & 0xff;

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

        for (j = 0; (j < i - 1); j++) s.push_back(char_array_3[j]);
    }

    string res;
    for (int i = 0; i < s.size(); i++) res += s[i];
    return res;
}


// sha1编码部分
unsigned char* sha1(char* data, int len) {
    SHA_CTX ctx;
    SHA1_Init(&ctx);
    for (int i = 0; i < len; ++i) SHA1_Update(&ctx, (void*)&data[i], 1);
    unsigned char* sSHA = new unsigned char[20];
    int ret = SHA1_Final(sSHA, &ctx);
    return sSHA;
}

// aes256cbc编码部分
std::string aes_256_cbc_decode(std::string key, std::string iv, std::string data) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_CIPHER_CTX_set_padding(ctx, 0);
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.c_str(), (unsigned char*)iv.c_str());
    int len = data.length();
    int outlen = len + 32;
    unsigned char* out = new unsigned char[outlen];
    EVP_DecryptUpdate(ctx, out, &outlen, (unsigned char*)data.c_str(), len);
    int tmplen = 0;
    EVP_DecryptFinal_ex(ctx, out + outlen, &tmplen);
    EVP_CIPHER_CTX_cleanup(ctx);
    std::string res = std::string((char*)out, outlen + tmplen);
    delete[] out;
    return res;
}

// rsa-oaep-sha1编码部分
string rsa_encode(string source, string key) {
    RSA* rsa = RSA_new();
    BIO* keybio;
    keybio = BIO_new_mem_buf((unsigned char*)key.c_str(), -1);
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
    if (!rsa) {
        printf("PEM_read_bio_RSA_PUBKEY failed...\n");
        return "";
    }
    int len = RSA_size(rsa);
    char* pEncode = new char[len + 1];
    int ret = RSA_public_encrypt(source.length(), (const unsigned char*)source.c_str(), (unsigned char*)pEncode, rsa, RSA_PKCS1_OAEP_PADDING);
    if (ret >= 0) {
        string strRet = base64_encode(pEncode, ret);
        delete[] pEncode;
        RSA_free(rsa);
        return strRet;
    }
    else {
        delete[] pEncode;
        RSA_free(rsa);
        return "";
    }
}

// sha256编码部分
string sha256(string src) {
    unsigned char* sSHA = new unsigned char[32];
    SHA256((const unsigned char*)src.c_str(), src.length(), sSHA);
    string res;
    for (int i = 0; i < 32; i++) res += sSHA[i];
    delete[] sSHA;
    return res;
}

// ecdsa-sha256验签部分
bool ecdsa_sha256_verify(string msg, string sig, string pemKey) {
    // 信息转为 SHA256
    msg = sha256(msg);

    // 设置公钥
    EC_KEY* eckey = EC_KEY_new();
    BIO* keybio;
    keybio = BIO_new_mem_buf((unsigned char*)pemKey.c_str(), -1);
    eckey = PEM_read_bio_EC_PUBKEY(keybio, &eckey, NULL, NULL);
    if (!eckey) return false;

    // 设置签名
    ECDSA_SIG* signature = ECDSA_SIG_new();
    int len = sig.length();
    unsigned char* pSig = new unsigned char[len];
    for (int i = 0; i < len; i++) pSig[i] = sig[i];
    int ret = ECDSA_SIG_set0(signature, BN_bin2bn(pSig, len / 2, NULL), BN_bin2bn(pSig + len / 2, len / 2, NULL));

    // 验证
    int res = ECDSA_do_verify((const unsigned char*)msg.c_str(), msg.length(), signature, eckey);
    // if (res == -1) writeLog(LOG_LEVEL_ERROR, "Failed to verify signature! Unknwon Error!");
    EC_KEY_free(eckey);
    ECDSA_SIG_free(signature);
    delete[] pSig;
    return res == 1;
}

// 引号编码
string quote_encode(string source) {
    return str_replace("\"", "\"\"", str_replace("\n", "\\n", source));
}

#endif
