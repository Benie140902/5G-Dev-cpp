#include <vector>

extern std::vector<int>msg;
extern int num_bits;
std::vector <int> crc_append24a(const std:: vector<int>& input_codeblock);
std::vector <int> crc_append24b(const std:: vector<int>& input_codeblock);
std::vector <int> crc_append16(const std:: vector<int>& input_codeblock);

namespace segment{
std::vector<std::vector<int>>segment_codeblock(const std::vector<int>& transport_codeblock,int block_size);
int ldpc_basegraph_select(int num_tb_bits)
}


