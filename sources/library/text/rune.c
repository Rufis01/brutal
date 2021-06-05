#include <library/text/rune.h>

str_fix8_t rune_to_utf8(rune_t rune)
{
    if (rune <= 0x7F)
    {
        return (str_fix8_t){
            .len = 1,
            .buffer = {
                (uint8_t)rune,
            },
        };
    }
    else if (rune <= 0x07FF)
    {
        return (str_fix8_t){
            .len = 2,
            .buffer = {
                (uint8_t)(((rune >> 6) & 0x1F) | 0xC0),
                (uint8_t)(((rune >> 0) & 0x3F) | 0x80),
            },
        };
    }
    else if (rune <= 0xFFFF)
    {
        return (str_fix8_t){
            .len = 3,
            .buffer = {
                (uint8_t)(((rune >> 12) & 0x0F) | 0xE0),
                (uint8_t)(((rune >> 6) & 0x3F) | 0x80),
                (uint8_t)(((rune >> 0) & 0x3F) | 0x80),
            },
        };
    }
    else if (rune <= 0x10FFFF)
    {
        return (str_fix8_t){
            .len = 4,
            .buffer = {
                (uint8_t)(((rune >> 18) & 0x07) | 0xF0),
                (uint8_t)(((rune >> 12) & 0x3F) | 0x80),
                (uint8_t)(((rune >> 6) & 0x3F) | 0x80),
                (uint8_t)(((rune >> 0) & 0x3F) | 0x80),
            },
        };
    }
    else
    {
        return (str_fix8_t){
            .len = 3,
            .buffer = {0xEF, 0xBF, 0xBD},
        };
    }
}

size_t rune_length_utf8(rune_t rune)
{
    if (rune <= 0x7F)
    {
        return 1;
    }
    else if (rune <= 0x07FF)
    {
        return 2;
    }
    else if (rune <= 0xFFFF)
    {
        return 3;
    }
    else if (rune <= 0x10FFFF)
    {
        return 4;
    }
    else
    {
        return 0;
    }
}

rune_t utf8_to_rune(str_t str)
{
    if (str.len == 0 ||
        utf8_length(str.buffer[0]) > str.len)
    {
        return U'�';
    }

    if ((str.buffer[0] & 0xf8) == 0xf0)
    {
        return ((0x07 & str.buffer[0]) << 18) |
               ((0x3f & str.buffer[1]) << 12) |
               ((0x3f & str.buffer[2]) << 6) |
               ((0x3f & str.buffer[3]));
    }
    else if ((str.buffer[0] & 0xf0) == 0xe0)
    {
        return ((0x0f & str.buffer[0]) << 12) |
               ((0x3f & str.buffer[1]) << 6) |
               ((0x3f & str.buffer[2]));
    }
    else if ((str.buffer[0] & 0xe0) == 0xc0)
    {
        return ((0x1f & str.buffer[0]) << 6) |
               ((0x3f & str.buffer[1]));
    }
    else
    {
        return str.buffer[0];
    }
}

size_t utf8_length(uint8_t first)
{
    if ((first & 0xf8) == 0xf0)
    {
        return 4;
    }
    else if ((first & 0xf0) == 0xe0)
    {
        return 3;
    }
    else if ((first & 0xe0) == 0xc0)
    {
        return 2;
    }
    else
    {
        return 1;
    }
}
