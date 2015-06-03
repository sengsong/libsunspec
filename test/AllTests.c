#include <stdio.h>

#include "CuTest.h"

CuSuite* sunspec_suite();

int RunAllTests(void)
{
    int ret;

    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    CuSuiteAddSuite(suite, sunspec_suite());

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    ret = CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    return ret;
}

int main(void)
{
    return RunAllTests();

}
