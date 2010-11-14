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

#include <stdio.h>
#include <assert.h>
#include "emu.h"

/**
 * Example suite setup
 */
emu_error setup(emu_suite* suite)
{
	printf("Running %s setup\n", suite->name);
	return EMU_OK;
}

emu_error teardown(emu_suite* suite)
{
	printf("Running %s teardown\n", suite->name);
	return EMU_OK;
}

emu_error passing_test(emu_test* test)
{
	emu_pass("this should pass");
	emu_fail_if(0 != 0);
	emu_fail_unless(0 == 0);
	emu_fail_unless(test->assertions == 3);
	emu_fail_unless(test->failed_assertions == 0);
	return EMU_OK;
}

emu_error failing_test(emu_test* test)
{
	emu_fail("this should fail");
	emu_fail_if(1 == 1);
	emu_fail_unless(0 == 1);
	emu_fail_if(test->assertions == 3);
	emu_fail_if(test->failed_assertions == 4);
	return EMU_OK;
}

int main (int argc, const char * argv[]) 
{
	//create a new suite of tests
	emu_suite* suite = emu_suite_add("suite_a", setup, teardown);
	
	//add some tests to the suite
	emu_test* test = emu_test_add(suite, "passing_test", passing_test);
	emu_test_add(suite, "failing_test", failing_test);
	
	//run the whole suite
	emu_suite_run(suite);
	
	//display results
	emu_display_summary();
	emu_display_details();
	
	//run a specific test
	emu_test_run(test);
	emu_test_display_details(test);
	
    return 0;
}
