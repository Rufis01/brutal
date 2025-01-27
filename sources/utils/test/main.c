#include <brutal/cli.h>
#include <brutal/io.h>
#include <brutal/task.h>
#include <brutal/text.h>
#include <brutal/time.h>
#include <unistd.h>
#include "test/test.h"

static int _len = 0;
static Test _tests[1024] = {};

void test_register(Test test)
{
    _tests[_len] = test;
    _len++;
}

bool test_case_begin(MAYBE_UNUSED Str name)
{
    io_print$("    {fg-light-black}: ", name);
    cli_clear_line_after_cursor(io_chan_out());
    return true;
}

void test_case_end(void)
{
    io_print$("{fg-green}\n", "ok");
}

bool test_run(Test test)
{
    struct _Task runner;
    task_fork(&runner);
    task_run(&runner);

    if (task_is_child(&runner))
    {
        test_alloc_begin_test();
        test.func();
        test_alloc_end_test();
        task_exit(&runner, TASK_EXIT_SUCCESS);
        panic$("tak_exit returned")
    }
    else
    {
        int result = UNWRAP(task_wait(&runner));
        bool pass = result == TASK_EXIT_SUCCESS;

        if (test.flags & TEST_EXPECTED_TO_FAIL)
        {
            pass = result != TASK_EXIT_SUCCESS;
        }

        if (pass)
        {
            io_print$("{fg-white bg-green} {fg-light-green}{fg-white}:{}: {fg-white}\n", " PASS ", io_dirname(test.loc.file), io_filename(test.loc.file), test.loc.line, test.name);
        }
        else
        {
            io_print$("{fg-white bg-red bold} {fg-light-red}{fg-white}:{}: {fg-white}\n", " FAIL ", io_dirname(test.loc.file), io_filename(test.loc.file), test.loc.line, test.name);
        }

        return pass;
    }
}

int test_run_by_pattern(Str pattern)
{
    int pass_count = 0;
    int fail_count = 0;

    for (int i = 0; i < _len; i++)
    {
        if (glob_match_str(pattern, str$(_tests[i].name)))
        {
            if (test_run(_tests[i]))
            {
                pass_count++;
            }
            else
            {
                fail_count++;
            }
        }
    }

    return fail_count != 0 ? TASK_EXIT_FAILURE : TASK_EXIT_SUCCESS;
}

int test_run_all(void)
{
    int pass_count = 0;
    int fail_count = 0;

    cli_cursor_hide(io_chan_out());
    io_print$("\nRunning {fg-blue} tests...\n\n", _len);

    for (int i = 0; i < _len; i++)
    {
        Test test = _tests[i];

        cli_clear_display_after_cursor(io_chan_out());
        io_print$("\n{fg-cyan} Testing '{}'...\n", cli_spinner(CLI_SPINNER_DOTS), test.name);
        cli_cursor_horizontal(io_chan_out(), 0);
        cli_cursor_prevline(io_chan_out(), 2);

        if (test_run(test))
        {
            pass_count++;
        }
        else
        {
            fail_count++;
        }
    }

    cli_clear_line_after_cursor(io_chan_out());

    if (pass_count == _len)
    {
        io_print$("\n🤘 all {} tests passed - {fg-light-black}\n\n", _len, txt_nice(tick_now()));
    }
    else
    {
        io_print$("\n❌ {fg-red} fail {fg-green} pass {} total - {fg-light-black}\n\n", fail_count, pass_count, _len, txt_witty(tick_now()));
    }

    cli_cursor_show(io_chan_out());

    return fail_count != 0 ? TASK_EXIT_FAILURE : TASK_EXIT_SUCCESS;
}

int main(int argc, char const *argv[])
{
    if (argc == 3 && str_eq(str$(argv[1]), str$("-t")))
    {
        return test_run_by_pattern(str$(argv[2]));
    }
    else
    {
        return test_run_all();
    }
}
