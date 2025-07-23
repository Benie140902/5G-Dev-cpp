#include <iostream>

using namespace std;

namespace segment{

extern int zc; // Zero codeword length, defined in segment.cpp
extern int K; // Length of codeword, defined in segment.cpp
extern int ldpc_baseg; // Base graph for LDPC encoding, defined in segment.cpp

vector<vector<int>> segment_codeblock(const vector<int>& transport_block, int block_size);
int ldpc_basegraph_select(int num_tb_bits);

}
