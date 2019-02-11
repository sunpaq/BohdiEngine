#ifndef WIN32

#include "MCUnitTest.h"
#include "MCLog.h"
#include "MCHashTable.h"

/* Assert Functions */

void assertYES(int expression) throws(MCAssertYESException)
{
	if ((expression)!=1)
		throw(MCAssertYESException);
}

void assertNO(int expression) throws(MCAssertNOException)
{
	if ((expression)!=0)
		throw(MCAssertNOException);
}

void assertMull(void* ptr) throws(MCAssertMullException)
{
	if (ptr!=null)
		throw(MCAssertnullException);
}

void assertNotMull(void* ptr) throws(MCAssertNotMullException)
{
	if (ptr==null)
		throw(MCAssertNotnullException);
}

void assertSame(struct MCObject* obj1, struct MCObject* obj2) throws(MCAssertSameException)
{
	if (obj1!=obj2)
		throw(MCAssertSameException);
}

void assertNotSame(struct MCObject* obj1, struct MCObject* obj2) throws(MCAssertNotSameException)
{
	if (obj1==obj2)
		throw(MCAssertNotSameException);
}

void assertEquals(int exp1, int exp2) throws(MCAssertEqualsException)
{
	if ((exp1)!=(exp2))
		throw(MCAssertEqualsException);
}

void fail(char* message)
{
	error_log("%s\n", message);
	abort();
}

/* Test Case */

fun(initWithTestResult, struct MCUnitTestCase*), struct MCUnitTestResult* resultRef) as(MCUnitTestCase)
	if(resultRef!=null){
        ((obj)resultRef)->retain(resultRef);
		it->unitTestResultRef = resultRef;
	}else{
		it->unitTestResultRef = null;
	}
	return it;
}

fun(MCUnitTestCase_release, void)) as(MCUnitTestCase)
    as(MCObject)
       it->release(it);
    }
	if(it->unitTestResultRef!=null) {
        ((obj)it->unitTestResultRef)->release(it->unitTestResultRef);
    }
}

fun(setUp, void))
{
	//set up fixture
	runtime_log("----MCUnitTestCase setUp\n");
}

fun(tearDown, void))
{
	//tear down fixture
	runtime_log("----MCUnitTestCase tearDown\n");
}

static void runMethodByPointer(struct MCUnitTestCase* it, struct MCHashItem* amethod)
{
	it->setUp(it);
	runtime_log("%s\n", "runMethodByPointer start");
	try{
		obj c = (obj)it;
		MCFunction f = c->responseTo(c, amethod->key);
		if (f) {
			f(c);
		}
		//if exception generated, this line will never be reached
	}
	catch(MCAssertYESException){
		error_log("MCAssertNOException\n");
	}
	catch(MCAssertNOException){
		error_log("MCAssertNOException\n");
	}
	catch(MCAssertnullException){
		error_log("MCAssertnullException\n");
	}
	catch(MCAssertNotnullException){
		error_log("MCAssertNotnullException\n");
	}
	catch(MCAssertSameException){
		error_log("MCAssertSameException\n");
	}
	catch(MCAssertNotSameException){
		error_log("MCAssertNotSameException\n");
	}
	catch(MCAssertEqualsException){
		error_log("MCAssertEqualsException\n");
	}
	finally{
		error_log("testcase: %s at method: [%s]\n", ((obj)it)->claz->name, amethod->key);
	}
	it->tearDown(it);
}

fun(runTests, void)) as(MCUnitTestCase)
	runtime_log("%s\n", "MCUnitTestCase runTests");
	unsigned i;
	unsigned bye_key = MCHashTable_hash("bye");
	unsigned setUp_key = MCHashTable_hash("setUp");
	unsigned tearDown_key = MCHashTable_hash("tearDown");

	if(it==null || ((obj)it)->claz==null)
		return;

	runtime_log("%s\n", "MCUnitTestCase runTests before for loop");
	struct MCHashItem* amethod;
	for (i = 0; i < get_tablesize(MCHashTableLevel1); i++)
	{
		//runtime_log("MCUnitTestCase runTests in for loop index:[%d]\n", i);
		amethod = ((obj)it)->claz->methodtable->items[i];
		if(amethod->value.p!=null
		&& i!=bye_key
		&& i!=setUp_key
		&& i!=tearDown_key){
			runtime_log("%s\n", "MCUnitTestCase runTests hit a matched method");
			runMethodByPointer(it, amethod);
		}
	}
}

fun(runATestMethod, void), char* methodName) as(MCUnitTestCase)
    struct MCHashTable* table = ((obj)it)->claz->methodtable;
	unsigned index = MCHashTable_hash(methodName) % get_tablesize(MCHashTableLevel1);
	runMethodByPointer(it, table->items[index]);
}

constructor(MCUnitTestCase)) {
	MCObject(any);
	as(MCUnitTestCase)
		it->next_case = null;
		it->release = MCUnitTestCase_release;
	}
	dynamic(MCUnitTestCase)
		funbind(initWithTestResult);
		funbind(setUp);
		funbind(tearDown);
		funbind(runTests);
		funbind(runATestMethod);
	}
	return any;
}

/* Test Suite */

fun(MCUnitTestSuite_release, void)) as(MCUnitTestSuite)
	as(MCObject)
		it->release(it);
	}
	struct MCUnitTestCase *iter, *save;
	for(iter=it->first_case; (save=iter)!=null; )
		iter = iter->next_case;
}

fun(addTestCase, void), struct MCUnitTestCase* volatile tcase) as(MCUnitTestSuite)
	as(MCObject)
		it->retain(tcase);
	}
    *(it->last_case_p) = tcase;
    it->last_case_p = (struct MCUnitTestCase**)&tcase;
	it->test_case_count++;
}

fun(runTestCases, void)) as(MCUnitTestSuite)
	runtime_log("%s\n", "MCUnitTestSuite runTestCases");
	struct MCUnitTestCase *iter = null;
	for(iter=it->first_case; iter!=null; iter = iter->next_case)
		it->runTestCases(it);
}

constructor(MCUnitTestSuite)) {
	MCObject(any);
	as(MCUnitTestSuite)
	    it->first_case = null;
        it->last_case_p = &(it->first_case);
        it->test_case_count = 0;
        it->next_suite = null;
        it->release = MCUnitTestSuite_release;
	}
	dynamic(MCUnitTestSuite)
		funbind(addTestCase);
		funbind(runTestCases);
	}
	return any;
}

/* Test Result */

fun(addSuccessInfo, void), char* succinfo)
{
	//
}

fun(addFailInfo, void), char* failinfo)
{
	//
}

constructor(MCUnitTestResult)) {
	MCObject(any);
	dynamic(MCUnitTestResult)
		funbind(addSuccessInfo);
		funbind(addFailInfo);
	}
	return any;
}

/* Test Runner */

fun(MCUnitTestRunner_release, void)) as(MCUnitTestRunner)
	struct MCUnitTestSuite *iter, *save;
	for(iter=it->first_suite; (save=iter)!=null; save->release(save))
		iter = iter->next_suite;
}

fun(addTestSuite, void), struct MCUnitTestSuite* testSuite) as(MCUnitTestRunner)
	((obj)testSuite)->retain(testSuite);
	struct MCUnitTestSuite **iter;
	for(iter=&(it->first_suite); (*iter)!=null; iter=&((*iter)->next_suite)){}
	(*iter)=testSuite;
	it->test_suite_count++;
}

fun(runTestSuites, void)) as(MCUnitTestRunner)
	runtime_log("%s\n", "MCUnitTestRunner runTestSuites");
	struct MCUnitTestSuite *iter;
	for(iter=it->first_suite; iter!=null; iter = iter->next_suite)
		iter->runTestCases(iter);
}

constructor(MCUnitTestRunner)) {
	MCObject(any);
	as(MCUnitTestRunner)
		it->first_suite = null;
		it->test_suite_count = 0;
		it->release = MCUnitTestRunner_release;
	}
	dynamic(MCUnitTestRunner)
		funbind(addTestSuite);
		funbind(runTestSuites);
	}
	return any;
}

#endif

