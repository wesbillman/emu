#include <stdio.h>
#include <assert.h>
#include "emu.h"

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
	emu_suite* suite = emu_suite_add("suite_a", setup, teardown);
	emu_test* test;

	test = emu_test_add(suite, "passing_test", passing_test);
	emu_test_add(suite, "failing_test", failing_test);
	emu_suite_run(suite);
	emu_test_run(test);
	emu_display_summary();
	emu_display_details();
	
	emu_suite_delete(suite);
    return 0;
}
