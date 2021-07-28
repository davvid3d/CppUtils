#pragma once
#include <vector>

typedef std::vector<unsigned char> ByteVector;

class BitStream
{
public:
    BitStream();
    BitStream(ByteVector input_vector);
    BitStream(unsigned char *input_array, int size);

    ~BitStream();

    ByteVector extract_bits(int start, int stop);
    ByteVector next_bits(int n);
    ByteVector next_bits_to_end() { return next_bits(((unsigned int)bytes_buffer_.size() * 8) - position_); };

    unsigned long long extract_value(int start, int stop);
    unsigned long long next_value(int n);

    void insert_bits(ByteVector input, int start, int stop);
    void next_put_bits(ByteVector input, int n);
    void next_put_bits(ByteVector input);

    void insert_value(unsigned long long value, int start, int stop);
    void next_put_value(unsigned long long value, int n);

    void skip(int n);
    void reset() { position_ = 0; };

    ByteVector all_bits();
    unsigned long get_position() { return position_; }

private:
    ByteVector bytes_buffer_;
    unsigned long position_ = 0;
};