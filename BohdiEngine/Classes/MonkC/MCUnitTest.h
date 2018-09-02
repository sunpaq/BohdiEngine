#include <assert.h>
#include "MCContext.h"
#include "MCException.h"
/* Assert Functions */
//void assert( int expression);
//all the exception will be catched inside the MCUnitTestCase, user should not catch them
void assertYES(int expression) throws(MCAssertYESException);
void assertNO(int expression) throws(MCAssertNOException);
void assertNil(void* ptr) throws(MCAssertNilException);
void assertNotNil(void* ptr) throws(MCAssertNotNilException);
void assertSame(MCObject* obj1, MCObject* obj2) throws(MCAssertSameException);
void assertNotSame(MCObject* obj1, MCObject* obj2) throws(MCAssertNotSameException);
void assertEquals(int exp1, int exp2) throws(MCAssertEqualsException);
void fail(char* message);

/* Test Result */

#ifndef MCUnitTestResult_
#define MCUnitTestResult_
	
class(MCUnitTestResult, MCObject);

fun(MCUnitTestResult, void, bye, voida);
fun(MCUnitTestResult, void, addSuccessInfo, char* succinfo);
fun(MCUnitTestResult, void, addFailInfo, char* failinfo);
#endif

/* Test Case */

#ifndef MCUnitTestCase_
#define MCUnitTestCase_ 

class(MCUnitTestCase, MCObject,
	MCUnitTestResult* unitTestResultRef;
	struct MCUnitTestCaseStruct* next_case;
);

fun(MCUnitTestCase, MCUnitTestCase*, initWithTestResult, MCUnitTestResult* resultRef);
fun(MCUnitTestCase, void, bye, voida);
fun(MCUnitTestCase, void, setUp, voida);
fun(MCUnitTestCase, void, tearDown, voida);
fun(MCUnitTestCase, void, runTests, voida);
fun(MCUnitTestCase, void, runATestMethod, char* methodName);
#endif

/* Test Suite */

#ifndef MCUnitTestSuite_
#define MCUnitTestSuite_

class(MCUnitTestSuite, MCObject,
	MCUnitTestCase *first_case;
    MCUnitTestCase **last_case_p;
	int test_case_count;
	struct MCUnitTestSuiteStruct* next_suite;
);

fun(MCUnitTestSuite, void, bye, voida);
fun(MCUnitTestSuite, void, addTestCase, MCUnitTestCase* volatile tcase);
fun(MCUnitTestSuite, void, runTestCases, voida);
#endif

/* Test Runner */

#ifndef MCUnitTestRunner_
#define MCUnitTestRunner_

class(MCUnitTestRunner, MCObject,
	MCUnitTestResult* unitTestResult;
	MCUnitTestSuite* first_suite;
	int test_suite_count;
);

fun(MCUnitTestRunner, void, bye, voida);
fun(MCUnitTestRunner, void, addTestSuite, MCUnitTestSuite* testSuite);
fun(MCUnitTestRunner, void, runTestSuites, voida);

#endif
