#include<bits/stdc++.h>
#include "main.h"

using namespace std;

int num_bits;
vector<int> msg;

vector<int> generator_bits(int num_bits) {
    vector<int> bits(num_bits);
    for (int i = 0; i < num_bits; ++i) {
        bits[i] = rand() % 2;
    }
    return bits;
}
int main(){
    cout << "Enter the number of bits: ";
    cin >> num_bits;
    msg=generator_bits(num_bits);
    auto msg_crc24a=crc_append24a(msg);
    return 0;


}
