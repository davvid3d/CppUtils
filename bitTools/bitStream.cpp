
#include "BitStream.h"

BitStream::BitStream()
{
}

BitStream::BitStream(ByteVector input_vector) : bytes_buffer_(input_vector)
{
}

BitStream::BitStream(unsigned char *input_array, int size) : bytes_buffer_(input_array, input_array + size)
{
}

BitStream::~BitStream()
{
}

ByteVector BitStream::extract_bits(int start, int stop)
{
    int last_bit = stop - 1;
    int start_byte = start / 8;
    int stop_byte = last_bit / 8;
    int nb_complete_bytes = (last_bit - start) / 8;

    ByteVector result_buffer(nb_complete_bytes + 1);

    // Handle complete bytes, except the last.
    int byte_offset = start % 8;
    unsigned char byte_out, b1, b2;
    unsigned char mask_b1 = 0xff >> byte_offset;
    unsigned char mask_b2 = (0xff << (8 - byte_offset)) & 0xff;

    for (int i = 0; i < nb_complete_bytes; i++)
    {
        b1 = bytes_buffer_[start_byte + i];
        b2 = bytes_buffer_[start_byte + i + 1];
        byte_out = ((b1 & mask_b1) << byte_offset) + ((b2 & mask_b2) >> (8 - byte_offset));
        result_buffer[i] = byte_out;
    }

    // Handle last output byte.
    int last_byte_offset = 7 - (last_bit % 8);
    unsigned char last_byte;
    unsigned char last_byte_mask = (0xff << last_byte_offset) & 0xff;
    if (start_byte + nb_complete_bytes == stop_byte)
    {
        // The last output byte only span a single input byte.
        last_byte_mask = mask_b1 & last_byte_mask;
        last_byte = (bytes_buffer_[stop_byte] & last_byte_mask) << byte_offset;
    }
    else
    {
        // The last output byte span the last 2 input bytes.
        b1 = bytes_buffer_[stop_byte - 1];
        b2 = bytes_buffer_[stop_byte];
        last_byte = (b1 & mask_b1) << byte_offset;
        last_byte += (b2 & last_byte_mask) >> (8 - byte_offset);
    }

    result_buffer[nb_complete_bytes] = last_byte;

    return result_buffer;
}

ByteVector BitStream::next_bits(int n)
{
    ByteVector result_buff = extract_bits(position_, position_ + n);
    position_ += n;
    return result_buff;
}

unsigned long long BitStream::extract_value(int start, int stop)
{
    if ((stop - start) > 64)
        throw("Stop - start should be <= 64");

    int padding = 7 - (stop - start - 1) % 8;
    ByteVector value_bytes = extract_bits(start, stop);

    unsigned long long result = 0;
    ByteVector::iterator it;
    for (it = value_bytes.begin(); it != value_bytes.end(); ++it)
    {
        result = (result << 8) + *it;
    }

    if (padding > 0)
        result = result >> padding;

    return result;
}

unsigned long long BitStream::next_value(int n)
{
    unsigned long long result = extract_value(position_, position_ + n);
    position_ += n;
    return result;
}

void BitStream::insert_bits(ByteVector input, int start, int stop)
{
    int last_bit = stop - 1;
    int start_byte = start / 8;
    int stop_byte = last_bit / 8;
    int nb_complete_bytes = (last_bit - start) / 8;

    // Ensure buffer has enough elements
    if ((int)bytes_buffer_.size() < stop_byte + 1)
        bytes_buffer_.resize(stop_byte + 1, 0);

    // Handle complete bytes, except the last
    int byte_offset = start % 8;
    unsigned char new_byte, b1_masked, b2_masked, b1, b2;
    unsigned char mask_b1 = 0xff >> byte_offset;
    unsigned char mask_b2 = (0xff << (8 - byte_offset)) & 0xff;

    for (int i = 0; i < nb_complete_bytes; i++)
    {
        new_byte = input[i];
        b1_masked = bytes_buffer_[start_byte + i] & (0xff - mask_b1);
        b2_masked = bytes_buffer_[start_byte + i + 1] & (0xff - mask_b2);

        b1 = b1_masked | (new_byte >> byte_offset);
        b2 = b2_masked | ((new_byte << (8 - byte_offset)) & 0xff);

        bytes_buffer_[start_byte + i] = b1;
        bytes_buffer_[start_byte + i + 1] = b2;
    }

    // Handle last input byte.
    int last_byte_offset = 7 - (last_bit % 8);
    unsigned char last_new_byte = input[nb_complete_bytes];
    unsigned char last_byte_mask = (0xff << last_byte_offset) & 0xff;
    unsigned char missing_bits, last_masked_byte;
    if (start_byte + nb_complete_bytes == stop_byte)
    {
        // The last input byte only span a single buffer byte.
        last_byte_mask = mask_b1 & last_byte_mask;
        missing_bits = (last_new_byte >> byte_offset) & last_byte_mask;
        last_masked_byte = bytes_buffer_[stop_byte] & (0xff - last_byte_mask);
        bytes_buffer_[stop_byte] = last_masked_byte | missing_bits;
    }
    else
    {
        // The last input byte span 2 buffer bytes.
        b1_masked = bytes_buffer_[stop_byte - 1] & (0xff - mask_b1);
        b2_masked = bytes_buffer_[stop_byte] & (0xff - last_byte_mask);

        b1 = b1_masked | (last_new_byte >> byte_offset);
        b2 = b2_masked | ((last_new_byte << (8 - byte_offset)) & last_byte_mask);

        bytes_buffer_[stop_byte - 1] = b1;
        bytes_buffer_[stop_byte] = b2;
    }
}

void BitStream::next_put_bits(ByteVector input, int n)
{
    insert_bits(input, position_, position_ + n);
    position_ += n;
}

void BitStream::next_put_bits(ByteVector input)
{
    next_put_bits(input, 8 * (int)input.size());
}

void BitStream::insert_value(unsigned long long value, int start, int stop)
{
    int nb_bytes = (stop - 1 - start) / 8 + 1;
    int padding = 7 - (stop - start - 1) % 8;
    unsigned long long left_aligned_value;
    unsigned char byte;
    ByteVector value_bytes;
    // Bytes are pushed back so the msb must be added first.
    for (int i = nb_bytes - 1; i >= 0; i--)
    {
        left_aligned_value = value << padding;
        byte = (left_aligned_value >> (8 * i)) & 0xff;
        value_bytes.push_back(byte);
    }
    insert_bits(value_bytes, start, stop);
}

void BitStream::next_put_value(unsigned long long value, int n)
{
    insert_value(value, position_, position_ + n);
    position_ += n;
}

void BitStream::skip(int n)
{
    position_ = position_ + n;
}

ByteVector BitStream::all_bits()
{
    return extract_bits(0, position_);
}