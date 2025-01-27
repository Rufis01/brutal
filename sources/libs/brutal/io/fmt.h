#pragma once

#include <brutal/base/any.h>
#include <brutal/io/traits.h>
#include <brutal/parse/scan.h>
#include <brutal/text/case.h>
#include <brutal/text/str.h>

typedef enum
{
    FMT_NONE,

    FMT_CHAR,
    FMT_STRING,
    FMT_BINARY,
    FMT_OCTAL,
    FMT_DECIMAL,
    FMT_HEXADECIMAL,
    FMT_POINTER,
} FmtType;

typedef enum
{
    FMT_PRINTF_INT,
    FMT_PRINTF_LONG,
    FMT_PRINTF_LONGLONG,

    FMT_PRINTF_UINT,
    FMT_PRINTF_ULONG,
    FMT_PRINTF_ULONGLONG,

    FMT_PRINTF_FLOAT,

    FMT_PRINTF_PTR,
    FMT_PRINTF_STRING,
} FmtPrintfType;

typedef enum
{
    FMT_COL_NONE,
    FMT_BLACK,
    FMT_WHITE,
    FMT_RED,
    FMT_GREEN,
    FMT_BLUE,
    FMT_YELLOW,
    FMT_MAGENTA,
    FMT_CYAN,
    FMT_GRAY,
    FMT_BLACK_GRAY,
} FmtColorTypes;

typedef struct
{
    FmtColorTypes type;
    bool bright;
} FmtColor;

typedef struct
{
    bool has_style;
    bool bold;
    bool underline;
    FmtColor fg_color;
    FmtColor bg_color;
} FmtStyle;

typedef struct
{
    FmtType type;
    FmtStyle style;
    Case casing;

    long precison;
    long min_width;
    char fill;
    bool prefix;
} Fmt;

Fmt fmt_parse(Scan *scan);

FmtPrintfType fmt_parse_printf(Scan *scan, Fmt *fmt);

IoResult fmt_signed(Fmt self, IoWriter writer, int64_t value);

IoResult fmt_unsigned(Fmt self, IoWriter writer, uint64_t value);

#ifndef __freestanding__
IoResult fmt_float(Fmt self, IoWriter writer, double value);
#endif

IoResult fmt_string(Fmt self, IoWriter writer, Str string);

IoResult fmt_char(Fmt self, IoWriter writer, unsigned int character);

IoResult fmt_any(Fmt self, IoWriter writer, Any value);
