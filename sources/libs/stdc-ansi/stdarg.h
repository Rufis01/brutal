#pragma once

/* --- 7.16 - Variable Arguments --------------------------------------------- */

typedef char *va_list;

#define __size_round_to_word(type)\
    (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

#define va_copy(dest, src)\
    (dest = src)

#define va_arg(ap, type)\
    (ap += __size_round_to_word(type),\
    *(type *)(ap - __size_round_to_word(type)))

#define va_end(ap)\
    (ap = 0)

#define va_start(ap, paramN)\
    (ap = (char *)(&(paramN) + __size_round_to_word(paramN)))
