#pragma once
#include <stdint.h>

#define bitSet(x, bitPosition) \
    ((x) |= (1 << (bitPosition)))

#define bitClear(x, bitPosition) \
    ((x) &= ~(1 << (bitPosition)))

#define LEN_NIBBLE_STRING 128

namespace genericTools
{
    /*
    * @brief Data decoding methods
    */
    uint16_t decodeUint16(uint8_t data[]);
    uint32_t decodeUint32(uint8_t data[]);
    uint64_t decodeUint40(uint8_t data[]);
    uint64_t decodeUint64(uint8_t data[]);

    /*
    * @brief Data encoding methods
    */
    void encodeUint16(uint16_t value, uint8_t data[]);
    void encodeUint32(uint32_t value, uint8_t data[]);
    void encodeUint40(uint64_t value, uint8_t data[]);
    void encodeUint64(uint64_t value, uint8_t data[]);

    /*
    * @brief Print uint64 value to serial output for debug
    */
    void printUint64Hex(uint64_t qwValue);
    void printUint64Dec(uint64_t qwValue);

    /*
    * @brief Helpers
    */
    bool bitRead(uint8_t b, int bitPos);
    bool getBit(uint8_t *data, uint16_t n, uint16_t bit);
    void setBit(uint8_t *data, uint16_t n, uint16_t bit, bool val);
    void writeValueToBytes(uint8_t *data, uint64_t val, uint8_t n);
    uint64_t bytesAsValue(uint8_t *data, uint8_t n);
    uint8_t nibbleFromChar(char c);
    void convertToByte(char *string, uint8_t *eui_byte);
}