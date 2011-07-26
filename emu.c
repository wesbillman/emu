/*
 Copyright (c) 2010 Wes Billman
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "emu.h"

typedef struct {
    unsigned int	num_suites;
    emu_suite*		suites[MAX_SUITES_IN_REPO];
} emu_repo;

static emu_repo	_repo;
static emu_suite* _current_suite;
static emu_test* _current_test;

static char* _emu_strcpy(const char* old)
{
    int len = strlen(old);
    char* newStr = (char*)emu_malloc(sizeof(char) * (len + 1));
    strcpy(newStr, old);
    return newStr;
}

emu_test* emu_test_add(emu_suite* suite, const char* name, emu_test_func test_func)
{
    assert(suite != NULL);
    assert(suite->num_tests < MAX_TESTS_IN_SUITE);
    assert(name != NULL);

    emu_test* test = ((emu_test*) emu_malloc(sizeof(emu_test)));
    test->name = _emu_strcpy(name);
    test->message = NULL;
    test->passed = 0;
    test->ran = 0;
    test->test_func = test_func;

    //add this new suite to our repo
    suite->tests[suite->num_tests] = test;
    suite->num_tests++;
    return test;
}

void emu_test_delete(emu_test* test)
{
    if (!test) return;
    emu_free(test->name);
    emu_free(test);
}

void emu_test_run(emu_test* test)
{
    if (test == NULL) return;
    _current_test = test;
    test->ran = 1;

    //clear previous run stats
    test->assertions = 0;
    test->failed_assertions = 0;
    test->passed = 0;

    if (test->test_func == NULL) return;
    test->passed = (test->test_func(test) == EMU_OK);
}

void emu_test_display_summary(emu_test* test)
{
    int i;
    for (i = 0; i < (test->assertions - test->failed_assertions); i++) {
        emu_printf(".");
    }
    for (i = 0; i < test->failed_assertions; i++) {
        emu_printf("F");
    }
}

void emu_test_display_details(emu_test* test)
{
    emu_printf("test %s\n", test->name);
    emu_printf("  assertions: %d (%d passed, %d failed)\n", 
               test->assertions, (test->assertions - test->failed_assertions),
               test->failed_assertions);
}

emu_suite* emu_suite_add(const char* name, emu_setup setup, emu_teardown teardown)
{
    assert(name != NULL);
    emu_suite* suite = ((emu_suite*) emu_malloc(sizeof(emu_suite)));
    suite->name = _emu_strcpy(name);
    suite->num_tests = 0;
    suite->num_failed = 0;
    suite->setup = setup;
    suite->teardown = teardown;
    emu_memset(suite->tests, 0, sizeof(suite->tests));

    //add this new suite to our repo
    _repo.suites[_repo.num_suites] = suite;
    _repo.num_suites++;
    return suite;
}

void emu_suite_delete(emu_suite* suite)
{
    unsigned int i;
    for (i = 0; i < suite->num_tests; i++)
    {
        if (suite->tests[i])
        {
            emu_test_delete(suite->tests[i]);
        }
    }
    emu_free(suite);
    _repo.num_suites--;
}

void emu_suite_run(emu_suite* suite)
{
    unsigned int i;

    _current_suite = suite;

    //clear previous run stats
    suite->num_failed = 0;

    //run suite setup
    if (suite->setup) {
        if(suite->setup(suite) != EMU_OK) {
            emu_printf("%s setup failed\n",suite->name);
        }
    }

    //run the tests
    for (i = 0 ; i < suite->num_tests; ++i) {
        emu_test* test = suite->tests[i];
        if (test == NULL) continue;
        
        emu_test_run(test);
        if (!test->passed) suite->num_failed++;
    }

    //run suite teardown
    if (suite->teardown) {
        if(suite->teardown(suite) != EMU_OK) {
            emu_printf("%s teardown failed\n", suite->name);
        }
    }
}

void emu_suite_display_summary(emu_suite* suite)
{
    unsigned int i;
    for (i = 0; i < suite->num_tests; i++) {
        emu_test_display_summary(suite->tests[i]);
    }
}

void emu_suite_display_details(emu_suite* suite)
{
    unsigned int i;

    emu_printf("suite: %s\n", suite->name);
    emu_printf("  num_tests: %d\n", suite->num_tests);
    emu_printf(" num_failed: %d\n", suite->num_failed);

    if (suite->num_tests > 0) {
        emu_printf("%s test%s\n", suite->name,
                   suite->num_tests == 0 ? "" : "s");
        emu_printf("--\n");
    }

    for (i = 0; i < suite->num_tests; i++) {
        emu_test_display_details(suite->tests[i]);
    }
}


void emu_all_display(void)
{
    unsigned int i;
    for (i = 0; i < _repo.num_suites; i++) {
        if (_repo.suites[i]) {
            emu_suite_display_details(_repo.suites[i]);
        }
    }
}

void emu_display_summary(void)
{
    unsigned int i;

    emu_printf("\n");
    emu_printf("emu run summary\n");
    emu_printf("---------------\n");
    for (i = 0; i < _repo.num_suites; i++) {
        emu_suite_display_summary(_repo.suites[i]);
    }
    emu_printf("\n");
}

void emu_display_details(void)
{
    unsigned int i;

    emu_printf("\n");
    emu_printf("emu run details\n");
    emu_printf("---------------\n");
    for (i = 0; i < _repo.num_suites; i++) {
        emu_suite_display_details(_repo.suites[i]);
    }
    emu_printf("\n");
}

void _emu_internal_fail_unless(int result, const char* file, int line, const char* message)
{		
    _current_test->assertions++;
    if (!result) {
        _current_test->passed = 0;
        _current_test->failed_assertions++;
        emu_printf("%s step %s\n", _current_test->name, message);
        emu_printf("  file: %s\n", file);
        emu_printf("  line: %d\n", line);
    }
}
