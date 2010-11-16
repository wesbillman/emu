# Welcome to EMU
EMU is a simple EMbedded Unit testing framework

## Quickstart
1. Create a new .c file for your tests (see emu_test.c for an example)
2. Include emu.h in your file

        #include "emu.h"

3. Override the follow defines to suit your environment

        //emu overrides
        #define emu_malloc(size)				malloc(size)
        #define	emu_free(ptr)					free(ptr)
        #define emu_memset(ptr, value, size)	memset(ptr, value, size)
        #define	emu_printf(...)					printf(__VA_ARGS__)

4. Modify the size defines to suit your needs

        #define MAX_TESTS_IN_SUITE		32
        #define MAX_SUITES_IN_REPO		32
        #define MAX_SUITE_NAME_LENGTH	32  

5. Add a suite

        emu_suite* suite = emu_suite_add("suite_a", setup, teardown);

6. Add some tests

        emu_test* test = emu_test_add(suite, "passing_test", passing_test);
        emu_test_add(suite, "failing_test", failing_test);
        
7. Run the suite

        emu_suite_run(suite);    
        
8. Display the results

        emu_display_summary();
        emu_display_details();

## Test Macros
Available Macros

    #define emu_fail(message)
    #define emu_pass(message)
    #define emu_fail_if(test)
    #define emu_fail_unless(test)

These will pass

    emu_pass("this should pass");
    emu_fail_if(0 != 0);
    emu_fail_unless(0 == 0);
    
These will fail

    emu_fail("this should fail");
    emu_fail_if(1 == 1);
    emu_fail_unless(0 == 1);
    
## Results
Use `emu_display_summary()` to display a brief summary

    emu run summary
    ---------------
    .....FFFFF
   
Use `emu_display_details()` to display more detailed information

    emu run details
    ---------------
    suite: suite_a
      num_tests: 2
     num_failed: 0
    suite_a tests
    --
    test passing_test
      assertions: 5 (5 passed, 0 failed)
    test failing_test
      assertions: 5 (0 passed, 5 failed)

    test passing_test
      assertions: 5 (5 passed, 0 failed)