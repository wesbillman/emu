# Welcome to EMU
EMU is a simple EMbedded Unit testing framework

## Quickstart
1. Create a new .c file for you tests (see emu_test.c for an example)
2. Include emu.h in your file
    #include "emu.h"
3. Modify the memory defines to suit your needs
    #define emu_malloc(size)	malloc(size)
    #define	emu_free(ptr)		free(ptr)
4. Modify the size defines to suit your needs
    #define MAX_TESTS_IN_SUITE		32
    #define MAX_SUITES_IN_REPO		32
    #define MAX_SUITE_NAME_LENGTH	32  
5. Add some tests using test macros

    #define emu_fail(message)
    #define emu_pass(message)
    #define emu_fail_if(test)
    #define emu_fail_unless(test)
    
## Test Macros
These will pass
    emu_pass("this should pass");
    emu_fail_if(0 != 0);
    emu_fail_unless(0 == 0);
    
These will fail
    emu_fail("this should fail");
    emu_fail_if(1 == 1);
    emu_fail_unless(0 == 1);