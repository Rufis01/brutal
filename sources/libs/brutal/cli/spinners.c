#include <brutal/cli/spinners.h>
#include <brutal/debug.h>
#include <brutal/time/query.h>

Str cli_spinner(CliSpinner spinner)
{
    Tick now = tick_now();
    uint64_t frame = now / spinner.interval;

    int n = 0;
    while (!str_eq(spinner.frames[n], nullstr))
    {
        n++;
    }

    return spinner.frames[frame % n];
}

CliSpinner const CLI_SPINNER_DOTS = {
    .interval = 80,
    .frames = (Str[]){
        str_const$("⠋"),
        str_const$("⠙"),
        str_const$("⠹"),
        str_const$("⠸"),
        str_const$("⠼"),
        str_const$("⠴"),
        str_const$("⠦"),
        str_const$("⠧"),
        str_const$("⠇"),
        str_const$("⠏"),
        nullstr,
    },
};
