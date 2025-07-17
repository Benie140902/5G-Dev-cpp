#include <iostream>
#include <vector>
#include<cmath>
#include <map>
#include <algorithm>
#include <iterator>
#include "/home/mantiswave/5G_DEV_CPP/Transmitter/main.h"

using namespace std;
using namespace crcattach; // namespace crcattach;

const uint32_t CRC_24a = 0x1864CFB; // CRC-24a polynomial
const uint32_t CRC_24b = 0x1800063; // CRC-24b polynomial

namespace segment {

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

    int ldpc_baseg = ldpc_basegraph_select(num_bits);
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
    float prod = static_cast<float>(num_cb_bits) / ldpc_baseg;
    for (const auto& [key, vec] : zero_shift) {
        for (int val : vec) {
            if (prod >= val) {
                zc = max(val, zc);
            }
        }
    }

    int K = ldpc_baseg * zc;

    // Initialize outer vector with num_segments and each inner vector with num_cb_bits size
    code_block = vector<vector<int>>(num_segments, vector<int>(num_cb_bits, 0));

    int s = 0;
    for (int r = 0; r < num_segments; ++r) {
        for (int k = 0; k < (num_cb_bits - l) && s < transport_block.size(); ++k) {
            code_block[r][k] = transport_block[s];
            ++s;
        }

        if (num_segments > 1) {
            vector<int> dum_cb = crcattach::crc_append24b(transport_block);
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

int ldpc_basegraph_select(int num_tb_bits) {

    if(num_tb_bits >= 8448){
        return 22; 
    }
    else 
        return 10;
   
}
}
