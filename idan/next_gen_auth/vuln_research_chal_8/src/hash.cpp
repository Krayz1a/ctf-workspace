#include "hash.h"

#include <array>
#include <iostream>
using namespace std;

#include <openssl/sha.h>

static vector<byte> sha256(vector<byte> const& data) {
    vector<byte> digest(SHA256_DIGEST_LENGTH);
    SHA256((uint8_t const*) data.data(), data.size(), (uint8_t*) digest.data());

    return digest;
}


vector<byte> hash_calc(vector<byte> const& in) {
    return sha256(in);
}
