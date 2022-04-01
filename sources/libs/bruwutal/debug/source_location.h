#pragma once

#include <bruwutal/text/str.h>

typedef struct
{
    int line;
    Str function;
    Str filename;
} SourceLocation;

#define source_location_current \
    (SourceLocation) { __LINE__, str$(__func__), str$(__FILE__), }
