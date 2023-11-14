#ifndef ENCRYPT_H
#define ENCRYPT_H

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

char* base64_decode(std::string encoded_string, int& out_len) {
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

    out_len = s.size();
    char* res = new char[out_len];
    for (int i = 0; i < out_len; i++) res[i] = s[i];
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

// 引号编码
string quote_encode(string source) {
    return str_replace("\"", "\"\"", str_replace("\n", "\\n", source));
}

#endif
