#include "segment.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
// need zc , code rate and base graph type
vector<vector<int>> ldpc_encoding (const vector<vector<int>>& code_block){
    vector<vector<int>> encoded_blocks;
    int N; // length of encoded blck of each row

    if(segment::K==22)
        N=66*segment::zc;
    else
        N=50*segment::zc;

}
