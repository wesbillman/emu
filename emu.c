/*
 *  emu.c
 *  emu
 *
 *  Created by Wes Billman on 11/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
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
	char* newStr = (char*)malloc(sizeof(char) * (len + 1));
	strcpy(newStr, old);
	return newStr;
}

void emu_all_display(void)
{
	unsigned int i;
	for (i = 0; i < _repo.num_suites; i++) {
		if (_repo.suites[i]) {
			emu_suite_display(_repo.suites[i]);
		}
	}
}

emu_suite* emu_suite_add(const char* name, emu_setup setup, emu_teardown teardown)
{
	assert(name != NULL);
	emu_suite* suite = ((emu_suite*) malloc(sizeof(emu_suite)));
	suite->name = _emu_strcpy(name);
	suite->num_tests = 0;
	suite->num_failed = 0;
	suite->setup = setup;
	suite->teardown = teardown;
	memset(suite->tests, 0, sizeof(suite->tests));
	
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
			//emu_test_delete(suite->list[i]);
		}
	}
	free(suite);
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
			printf("%s setup failed\n",suite->name);
		}
	}
	
	//run the tests
	for (i = 0 ; i < suite->num_tests; ++i)
	{
		emu_test* test = suite->tests[i];
		if (test == NULL) continue;

		emu_test_run(test);
		if (!test->passed) suite->num_failed++;
	}
	
	//run suite teardown
	if (suite->teardown) {
		if(suite->teardown(suite) != EMU_OK) {
			printf("%s teardown failed\n", suite->name);
		}
	}
}

void emu_suite_display(emu_suite* suite)
{
	printf("suite %s\n", suite->name);
	printf("  num_tests: %d\n", suite->num_tests);
	printf(" num_failed: %d\n", suite->num_failed);
}

emu_test* emu_test_add(emu_suite* suite, const char* name, emu_test_func test_func)
{
	assert(suite != NULL);
	assert(suite->num_tests < MAX_TESTS_IN_SUITE);
	assert(name != NULL);
	
	emu_test* test = ((emu_test*) malloc(sizeof(emu_test)));
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
	free(test->name);
	free(test);
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
		printf(".");
	}
	for (i = 0; i < test->failed_assertions; i++) {
		printf("F");
	}
}

void emu_test_display(emu_test* test)
{
	printf("test %s\n", test->name);
}

void emu_display_summary(void)
{
	unsigned int suite_i, test_i;
	printf("\n");
	printf("emu summary\n");
	printf("-----------\n");
	for (suite_i = 0; suite_i < _repo.num_suites; suite_i++) {
		emu_suite* suite = _repo.suites[suite_i];
		for (test_i = 0; test_i < suite->num_tests; test_i++) {
			emu_test_display_summary(suite->tests[test_i]);
		}
	}
	printf("\n");
}

void emu_display_details(void)
{

}

void _emu_internal_fail_unless(int result, const char* file, int line, const char* message)
{		
	_current_test->assertions++;
	if (!result) {
		_current_test->passed = 0;
		_current_test->failed_assertions++;
		printf("%s step %s\n", _current_test->name, message);
		printf("  file: %s\n", file);
		printf("  line: %d\n", line);
	}
}
