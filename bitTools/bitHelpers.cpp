#include "bitHelpers.h"
#include <string.h>
#include <cstdio>

/* ###########################################################################
 * #### Helper functions #####################################################
 * ######################################################################### */

namespace genericTools
{
    bool bitRead(uint8_t b, int bitPos)
    {
        uint8_t x = b & (1 << bitPos);
        return x == 0 ? false : true;
    }

    void setBit(uint8_t *data, uint16_t n, uint16_t bit, bool val)
    {
        uint16_t idx;
        uint8_t shift;

        idx = bit / 8;
        if (idx >= n)
        {
            printf("DW1000 setBit error out of bound \n");
            return;
        }
        uint8_t *targetuint8_t = &data[idx];
        shift = bit % 8;
        if (val)
            *targetuint8_t |= (1 << shift);
        else
            *targetuint8_t &= (~(1 << shift));
    }

    bool getBit(uint8_t *data, uint16_t n, uint16_t bit)
    {
        uint16_t idx;
        uint8_t shift;

        idx = bit / 8;
        if (idx >= n)
        {
            printf("DW1000 setBit error out of bound  \n");
            return false;
        }
        uint8_t targetuint8_t = data[idx];
        shift = bit % 8;

        return bitRead(targetuint8_t, shift);
    }

    void writeValueToBytes(uint8_t *data, uint64_t val, uint8_t n)
    {
        for (auto i = 0; i < n; i++)
        {
            data[i] = ((val >> (i * 8)) & 0xFF);
        }
    }

    uint64_t bytesAsValue(uint8_t *data, uint8_t n)
    {
        uint64_t value = 0;
        for (auto i = 0; i < n; i++)
        {
            value |= ((uint64_t)data[i] << (i * 8));
        }
        return value;
    }

    uint8_t nibbleFromChar(char c)
    {
        if (c >= '0' && c <= '9')
        {
            return c - '0';
        }
        if (c >= 'a' && c <= 'f')
        {
            return c - 'a' + 10;
        }
        if (c >= 'A' && c <= 'F')
        {
            return c - 'A' + 10;
        }
        return 255;
    }

    void convertToByte(char *string, uint8_t *eui_byte)
    {
        uint8_t eui[LEN_NIBBLE_STRING];

        for (uint16_t i = 0; i < LEN_NIBBLE_STRING; i++)
        {
            eui[i] = (nibbleFromChar(string[i * 3]) << 4) + nibbleFromChar(string[i * 3 + 1]);
        }
        memcpy(eui, eui, LEN_NIBBLE_STRING);
    }

    ////////////////////////////////////////////////////////////////////////////////
    /// Data decoding methods.
    ////////////////////////////////////////////////////////////////////////////////

    uint16_t decodeUint16(uint8_t data[])
    {
        uint16_t value = 0;

        for (int i = 1; i >= 0; i--)
        {
            value <<= 8;
            value |= (uint16_t)data[i];
        }

        return value;
    }

    uint32_t decodeUint32(uint8_t data[])
    {
        uint32_t value = 0;

        for (int i = 3; i >= 0; i--)
        {
            value <<= 8;
            value |= (uint32_t)data[i];
        }

        return value;
    }

    uint64_t decodeUint40(uint8_t data[])
    {
        uint64_t value = 0;

        for (int i = 4; i >= 0; i--)
        {
            value <<= 8;
            value |= (uint64_t)data[i];
        }

        return value;
    }

    uint64_t decodeUint64(uint8_t data[])
    {
        uint64_t value = 0;

        for (int i = 7; i >= 0; i--)
        {
            value <<= 8;
            value |= (uint64_t)data[i];
        }

        return value;
    }

    ////////////////////////////////////////////////////////////////////////////////
    /// Data encoding methods.
    ////////////////////////////////////////////////////////////////////////////////

    void encodeUint16(uint16_t value, uint8_t data[])
    {
        for (int i = 0; i < 2; i++)
        {
            data[i] = (uint8_t)(value & 0x0FF);
            value >>= 8;
        }
    }

    void encodeUint32(uint32_t value, uint8_t data[])
    {
        for (int i = 0; i < 4; i++)
        {
            data[i] = (uint8_t)(value & 0x0FF);
            value >>= 8;
        }
    }

    void encodeUint40(uint64_t value, uint8_t data[])
    {
        for (int i = 0; i < 5; i++)
        {
            data[i] = (uint8_t)(value & 0x0FF);
            value >>= 8;
        }
    }

    void encodeUint64(uint64_t value, uint8_t data[])
    {
        for (int i = 0; i < 8; i++)
        {
            data[i] = (uint8_t)(value & 0x0FF);
            value >>= 8;
        }
    }
}