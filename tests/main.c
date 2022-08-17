#include <stdlib.h>
#include <stdio.h>
#include "../testing.h/testing.h"
#include "./test_utils.h"

int main()
{
    lprintf(LOG_INFO, "Running tests for Discord tts %s for %s @ %s\n", VERSION, OS, REPO_URL);
    unit_test tests[] = {
        {&utils_tests, "Utils tests"}
    };

    int failed_tests = run_tests(tests, sizeof(tests) / sizeof(*tests), "Discord vc tts tests");
    lprintf(LOG_INFO, "%d/%ld tests failed.\n", failed_tests, sizeof(tests) / sizeof(*tests));
    return failed_tests > 0;
}

