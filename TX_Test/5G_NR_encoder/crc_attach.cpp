#include "/home/benie/5G-Dev-cpp/Transmitter/main.h"
#include <vector>
#include <cstdint>
#include <iostream>
using namespace std;

const uint32_t CRC_24a = 0x1864CFB; // CRC-24a polynomial
const uint32_t CRC_24b = 0x1800063; // CRC-24b polynomial
const uint32_t CRC_16 = 0x11021; // CRC-16 polynomial
vector<int> generator_bits(int num_bits) {
    vector<int> bits(num_bits);
    for (int i = 0; i < num_bits; ++i) {
        bits[i] = rand() % 2;
    }
    return bits;
}

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
int main(){
    int num_bits=10000;
    int crc_length=24;
    vector<int> code_block_crc(num_bits+crc_length);
    vector <int> input_codeblock= generator_bits(num_bits);
    vector<int> crc_attach_code_bits = compute_crc(input_codeblock, CRC_24a, crc_length);
    for (int i=0;i<(num_bits+crc_length);++i){
        if(i<num_bits)
            code_block_crc[i]=input_codeblock[i];
        else
        code_block_crc[i]=crc_attach_code_bits[i-num_bits];
        cout<<code_block_crc[i];
    }

    return 0;

}

vector <int> crc_append24a(const vector<int>& input_codeblock) {
    int crc_length = 24;
    auto crc=compute_crc(input_codeblock,CRC_24a,crc_length);
    vector<int> transport_code_block=input_codeblock;
    transport_code_block.insert(transport_code_block.end(),crc.begin(),crc.end());
    return transport_code_block;
}

vector <int> crc_append24b(const vector<int>& input_codeblock) {
    int crc_length = 24;
    auto crc=compute_crc(input_codeblock,CRC_24a,crc_length);
    vector<int> transport_code_block=input_codeblock;
    transport_code_block.insert(transport_code_block.end(),crc.begin(),crc.end());
    return transport_code_block;
}

vector <int> crc_append16(const vector<int>& input_codeblock) {
    int crc_length = 16;
    auto crc=compute_crc(input_codeblock,CRC_16,crc_length);
    vector<int> transport_code_block=input_codeblock;
    transport_code_block.insert(transport_code_block.end(),crc.begin(),crc.end());
    return transport_code_block;
}
