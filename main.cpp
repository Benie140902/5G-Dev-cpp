#include<bits/stdc++.h>
using namespace std;

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
    vector <int> msg=generator_bits(num_bits);
    for (int i = 0; i < num_bits; ++i) {
        cout << msg[i]<<endl;
    }
    return 0;


}