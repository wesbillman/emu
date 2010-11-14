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

#ifndef __EMU_H__
#define __EMU_H__

#ifdef __cplusplus
extern "C" {
#endif    
	
#define MAX_TESTS_IN_SUITE		32
#define MAX_SUITES_IN_REPO		32
#define MAX_SUITE_NAME_LENGTH	32

#define EMU_OK		1
#define EMU_FAIL	-1

//memory defines
#define emu_malloc(size)	malloc(size)
#define	emu_free(ptr)		free(ptr)
	
typedef int emu_error;
typedef struct emu_test emu_test;
typedef struct emu_suite emu_suite;

typedef emu_error (*emu_setup)(emu_suite*);
typedef emu_error (*emu_teardown)(emu_suite*);
typedef emu_error (*emu_test_func)(emu_test*);

struct emu_test {
	char*			name;
	emu_test_func	test_func;
	unsigned char	passed;
	unsigned char	ran;
	unsigned int	assertions;
	unsigned int	failed_assertions;
	const char*		message;
};

struct emu_suite{
	char*			name;
	emu_setup		setup;
	emu_teardown	teardown;
	unsigned int	num_tests;
	unsigned int	num_failed;
	emu_test*		tests[MAX_TESTS_IN_SUITE];
};

//suite functions
emu_suite* emu_suite_add(const char* name, emu_setup setup, emu_teardown teardown);
void emu_suite_delete(emu_suite* suite);
void emu_suite_run(emu_suite* suite);
void emu_suite_display(emu_suite* suite);

//test functions
emu_test* emu_test_add(emu_suite* suite, const char* name, emu_test_func test);
void emu_test_delete(emu_test* test);
void emu_test_run(emu_test* test);
void emu_test_display(emu_test* test);

//framework functions
void emu_all_run(void);
void emu_display_summary(void);
void emu_display_details(void);

//internal functions, do not use!
void _emu_internal_fail_unless(int result, const char* file, int line, const char* message);

//test macros
#define emu_fail(message)		_emu_internal_fail_unless((0), __FILE__, __LINE__, (message))
#define emu_pass(message)		_emu_internal_fail_unless((1), __FILE__, __LINE__, (message))
#define emu_fail_if(test)		_emu_internal_fail_unless((!test), __FILE__, __LINE__, ("fail if '"#test"' failed"))
#define emu_fail_unless(test)	_emu_internal_fail_unless((test), __FILE__, __LINE__, ("fail unless '"#test"' failed"))

#ifdef __cplusplus
}
#endif    

#endif
