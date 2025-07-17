#include <iostream>
#include <vector>
#include<cmath>
#include <map>
#include <algorithm>
#include <iterator>
#include "/home/mantiswave/5G_DEV_CPP/Transmitter/main.h"

using namespace std;
vector<int> compute_crc(const vector<int>& input_codeblock, uint32_t polynomial, int crc_length);
vector<int> crc_append24a(const vector<int>& input_codeblock);
vector<int> crc_append24b(const vector<int>& input_codeblock);
vector<vector<int>> segment_codeblock(const vector<int>& transport_block, int block_size);
vector<int> generator_bits(int num_bits);
const uint32_t CRC_24a = 0x1864CFB; // CRC-24a polynomial
const uint32_t CRC_24b = 0x1800063; // CRC-24b polynomial

// namespace segment {


// const int MAX_Block_Size = 8448; // Maximum block size for encoding according to 3GPP TS 38.212

// extern vector<int>crc_append24b(const vector<int>&);
// extern vector<int>crc_append16(const vector<int>&);

vector <int> compute_crc(const vector<int>& input_codeblock, uint32_t polynomial, int crc_length){

    uint32_t crc=0;
    for(int bits: input_codeblock){
        bool msb = (crc>>(crc_length-1)&1)^ bits;
        crc <<= 1;
        if(msb) crc ^= polynomial;
    }
    vector<int> crc_bits(crc_length);
    for(int i=0; i<crc_length; ++i){
        crc_bits[i] = (crc >> (crc_length - 1 - i)) & 1;
    }
    return crc_bits;

}

vector <int> crc_append24a(const vector<int>& input_codeblock){
    int crc_length = 24;
    auto crc=compute_crc(input_codeblock,CRC_24a,crc_length);
    vector<int> transport_code_block=input_codeblock;
    transport_code_block.insert(transport_code_block.end(),crc.begin(),crc.end());
    return transport_code_block;
}

vector <int> crc_append24b(const vector<int>& input_codeblock){
    int crc_length = 24;
    auto crc=compute_crc(input_codeblock,CRC_24a,crc_length);
    vector<int> transport_code_block=input_codeblock;
    transport_code_block.insert(transport_code_block.end(),crc.begin(),crc.end());
    return transport_code_block;
}

vector<vector<int>> segment_codeblock(const vector<int>& transport_block, int block_size) {
    vector<vector<int>> code_block;

    int num_bits = transport_block.size() - 24; // Assuming last 24 bits are CRC
    int l; // Length of CRC bits to append
    int num_segments, cb_len = 0;

    if (num_bits < block_size) {
        l = 0;
        num_segments = 1;
        cb_len = num_bits;
    } else {
        l = 24;
        num_segments = ceil(static_cast<double>(num_bits) / (block_size - l));
        cb_len = num_bits + (l * num_segments);
    }

    int num_cb_bits = cb_len / num_segments;

    int ldpc_baseg1 = 22;
    map<int, vector<int>> zero_shift = {
        {0, {2, 4, 8, 16, 32, 64, 128, 256}},
        {1, {3, 6, 12, 24, 48, 96, 192, 384}},
        {2, {5, 10, 20, 40, 80, 160, 320}},
        {3, {7, 14, 28, 56, 112, 224}},
        {4, {9, 18, 36, 72, 144, 288}},
        {5, {11, 22, 44, 88, 176, 352}},
        {6, {13, 26, 52, 104, 208}},
        {7, {15, 30, 60, 120, 240}}
    };

    int zc = 0;
    float prod = static_cast<float>(num_cb_bits) / ldpc_baseg1;

    for (const auto& [key, vec] : zero_shift) {
        for (int val : vec) {
            if (prod >= val) {
                zc = max(val, zc);
            }
        }
    }

    int K = ldpc_baseg1 * zc;

    // Initialize outer vector with num_segments and each inner vector with num_cb_bits size
    code_block = vector<vector<int>>(num_segments, vector<int>(num_cb_bits, 0));

    int s = 0;
    for (int r = 0; r < num_segments; ++r) {
        for (int k = 0; k < (num_cb_bits - l) && s < transport_block.size(); ++k) {
            code_block[r][k] = transport_block[s];
            ++s;
        }

        if (num_segments > 1) {
            vector<int> dum_cb = crc_append24b(transport_block);
            for (int k = (num_cb_bits - l); k < num_cb_bits && (k - (num_cb_bits - l)) < dum_cb.size(); ++k) {
                code_block[r][k] = dum_cb[k - (num_cb_bits - l)];
            }
        }

        for (int k = num_cb_bits; k < K; ++k) {
            code_block[r].push_back(0); // Filler bits
        }
    }

    return code_block;
}


vector<int> generator_bits(int num_bits) {
    vector<int> bits(num_bits);
    for (int i = 0; i < num_bits; ++i) {
        bits[i] = rand() % 2;
    }
    return bits;
}

int main(){
    int num_bits;
    cout << "Enter the number of bits: ";
    cin >> num_bits;
    auto msg=generator_bits(num_bits);
    // using namespace crcattach;
    auto msg_crc24a=crc_append24a(msg);

    auto code_block= segment_codeblock(msg_crc24a,8448); // base graph 1 in LDPC encoding
}

