#include "VHSHA256.hpp"

#include <cstring>
#include <iomanip>
#include <sstream>

namespace {

constexpr std::array<uint32_t, 64> K = {
    0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u, 0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
    0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u, 0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
    0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu, 0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
    0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u, 0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
    0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u, 0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
    0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u, 0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
    0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u, 0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
    0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u, 0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u};

inline uint32_t rotr(uint32_t x, uint32_t n)
{
    return (x >> n) | (x << (32u - n));
}

inline uint32_t ch(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (~x & z);
}

inline uint32_t maj(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}

inline uint32_t bsig0(uint32_t x)
{
    return rotr(x, 2u) ^ rotr(x, 13u) ^ rotr(x, 22u);
}

inline uint32_t bsig1(uint32_t x)
{
    return rotr(x, 6u) ^ rotr(x, 11u) ^ rotr(x, 25u);
}

inline uint32_t ssig0(uint32_t x)
{
    return rotr(x, 7u) ^ rotr(x, 18u) ^ (x >> 3u);
}

inline uint32_t ssig1(uint32_t x)
{
    return rotr(x, 17u) ^ rotr(x, 19u) ^ (x >> 10u);
}

void transform(std::array<uint32_t, 8> &state, const uint8_t block[64])
{
    uint32_t w[64];
    for (size_t i = 0; i < 16; ++i) {
        const size_t j = i * 4;
        w[i] = (static_cast<uint32_t>(block[j]) << 24u) |
               (static_cast<uint32_t>(block[j + 1]) << 16u) |
               (static_cast<uint32_t>(block[j + 2]) << 8u) |
               (static_cast<uint32_t>(block[j + 3]));
    }
    for (size_t i = 16; i < 64; ++i) {
        w[i] = ssig1(w[i - 2]) + w[i - 7] + ssig0(w[i - 15]) + w[i - 16];
    }

    uint32_t a = state[0];
    uint32_t b = state[1];
    uint32_t c = state[2];
    uint32_t d = state[3];
    uint32_t e = state[4];
    uint32_t f = state[5];
    uint32_t g = state[6];
    uint32_t h = state[7];

    for (size_t i = 0; i < 64; ++i) {
        const uint32_t t1 = h + bsig1(e) + ch(e, f, g) + K[i] + w[i];
        const uint32_t t2 = bsig0(a) + maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}

} // namespace

VHSHA256::Digest VHSHA256::hash(const std::vector<uint8_t> &data)
{
    std::array<uint32_t, 8> state = {
        0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au,
        0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u};

    uint64_t bitlen = static_cast<uint64_t>(data.size()) * 8u;

    size_t i = 0;
    for (; i + 64 <= data.size(); i += 64) {
        transform(state, data.data() + i);
    }

    uint8_t block[64] = {};
    const size_t rem = data.size() - i;
    if (rem > 0) {
        std::memcpy(block, data.data() + i, rem);
    }
    block[rem] = 0x80u;

    if (rem >= 56) {
        transform(state, block);
        std::memset(block, 0, 64);
    }

    for (size_t j = 0; j < 8; ++j) {
        block[63 - j] = static_cast<uint8_t>((bitlen >> (j * 8u)) & 0xffu);
    }
    transform(state, block);

    Digest out{};
    for (size_t s = 0; s < state.size(); ++s) {
        out[s * 4 + 0] = static_cast<uint8_t>((state[s] >> 24u) & 0xffu);
        out[s * 4 + 1] = static_cast<uint8_t>((state[s] >> 16u) & 0xffu);
        out[s * 4 + 2] = static_cast<uint8_t>((state[s] >> 8u) & 0xffu);
        out[s * 4 + 3] = static_cast<uint8_t>(state[s] & 0xffu);
    }
    return out;
}

std::string VHSHA256::toHex(const Digest &digest)
{
    static constexpr char hex[] = "0123456789abcdef";
    std::string out;
    out.resize(digest.size() * 2);
    for (size_t i = 0; i < digest.size(); ++i) {
        out[i * 2] = hex[(digest[i] >> 4u) & 0x0fu];
        out[i * 2 + 1] = hex[digest[i] & 0x0fu];
    }
    return out;
}

std::string VHSHA256::hashHex(const std::vector<uint8_t> &data)
{
    return toHex(hash(data));
}
