#include <vector>
#include <string>
#include "Queue.h"

void Create(const std::string &archive_name, std::vector<std::string> &filenames, unsigned size_of_control);

void List(const std::string &archive_name);

void Extract(const std::string &archive_name);

void Extract(const std::string &archive_name, const std::string &file_to_extract);

void Append(const std::string &archive_name, const std::string &filename_to_append);

void Delete(const std::string &archive_name, const std::string &filename_to_delete);

void Merge(const std::string &archive_result_name, const std::string &archive1_name, const std::string &archive2_name);

void Correction(Queue &total, unsigned size_of_total);

void ReadNBytes(std::ifstream &in, Queue &total_in, const uint32_t& N);

void WriteNBytes(std::ofstream &out, Queue &total_out, const uint32_t& N);

void TransfusionAndGet3Chars(Queue& total_in, Queue& total_out, uint8_t ch[]);

void TransfusionAndGetUnsigned(Queue& total_in, Queue& total_out, uint32_t& ui);

void TransfusionNChars(Queue &total_in, Queue &total_out,const uint32_t& N);

void UnsignedFromQueue(Queue& total_in, uint32_t& ui);

void QueueFromUnsigned(Queue& total_out, uint32_t& ui);

void TransfusionAndGetChar(Queue& total_in, Queue& total_out, uint8_t& ch);

void QueueFromChar(Queue &total_out, const char &ch);

void CharFromQueue(Queue &total_in, uint8_t &ch);