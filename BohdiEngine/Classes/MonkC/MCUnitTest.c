#include "MCUnitTest.h"
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

void assertSame(MCObject* obj1, MCObject* obj2) throws(MCAssertSameException)
{
	if (obj1!=obj2)
		throw(MCAssertSameException);
}

void assertNotSame(MCObject* obj1, MCObject* obj2) throws(MCAssertNotSameException)
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

onload(MCUnitTestCase)
{
    if (load(MCObject)) {
        bid(MCUnitTestCase, MCUnitTestCase*, initWithTestResult, MCUnitTestResult* resultRef);
        bid(MCUnitTestCase, void, bye);
        bid(MCUnitTestCase, void, setUp);
        bid(MCUnitTestCase, void, tearDown);
        bid(MCUnitTestCase, void, runTests);
        bid(MCUnitTestCase, void, runATestMethod, char* errmsg);
        return cla;
    }else{
        return null;
    }
}

oninit(MCUnitTestCase)
{
    if (init(MCObject)) {
        obj->next_case = null;
        return obj;
    }else{
        return null;
    }
}

fun(MCUnitTestCase, MCUnitTestCase*, initWithTestResult, MCUnitTestResult* resultRef)
{
	if(resultRef!=null){
		retain(resultRef);
		obj->unitTestResultRef = resultRef;
	}else{
		obj->unitTestResultRef = null;
	}
	return obj;
}

fun(MCUnitTestCase, void, bye, voida)
{
	if(obj->unitTestResultRef!=null)
		release(&(obj->unitTestResultRef));
}

fun(MCUnitTestCase, void, setUp, voida)
{
	//set up fixture
	runtime_log("----MCUnitTestCase setUp\n");
}

fun(MCUnitTestCase, void, tearDown, voida)
{
	//tear down fixture
	runtime_log("----MCUnitTestCase tearDown\n");
}

static void runMethodByPointer(MCUnitTestCase* obj, mc_hashitem* amethod)
{
	ff(obj, setUp, null);
	runtime_log("%s\n", "runMethodByPointer start");

	try{
		_push_jump(response_to(cast(MCObject*, obj), amethod->key), null);
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
		error_log("testcase: %s at method: [%s]\n", cast(MCObject*, obj)->isa->item->key, amethod->key);
	}

	ff(obj, tearDown, null);
}

fun(MCUnitTestCase, void, runTests, voida)
{
	runtime_log("%s\n", "MCUnitTestCase runTests");
	unsigned i;
	unsigned bye_key = hash("bye");
	unsigned setUp_key = hash("setUp");
	unsigned tearDown_key = hash("tearDown");

	if(obj==null || cast(MCObject*, obj)->isa==null)
		return;

	runtime_log("%s\n", "MCUnitTestCase runTests before for loop");
    mc_hashitem* amethod;
	for (i = 0; i < get_tablesize(cast(MCObject*, obj)->isa->table->level); i++)
	{
		//runtime_log("MCUnitTestCase runTests in for loop index:[%d]\n", i);
		amethod = cast(MCObject*, obj)->isa->table->items[i];
		if(amethod->value.mcvoidptr!=null
		&& i!=bye_key
		&& i!=setUp_key
		&& i!=tearDown_key){
			runtime_log("%s\n", "MCUnitTestCase runTests hit a matched method");
			if(obj==null){
				error_log("MCUnitTestCase runTests this pointer is null\n");
			}
			runMethodByPointer(obj, amethod);
		}
	}
}

fun(MCUnitTestCase, void, runATestMethod, char* methodName)
{
    mc_hashtable* table = cast(MCObject*, obj)->isa->table;
    MCHashTableIndex index = hash(methodName) % get_tablesize(table->level);
    runMethodByPointer(obj, table->items[index]);
}

/* Test Suite */

onload(MCUnitTestSuite)
{
    if (load(MCObject)) {
        bid(MCUnitTestSuite, void, bye);
        bid(MCUnitTestSuite, void, addTestCase, MCUnitTestCase* tcase);
        bid(MCUnitTestSuite, void, runTestCases);
        return cla;
    }else{
        return null;
    }
}

oninit(MCUnitTestSuite)
{
    if (init(MCObject)) {
        obj->first_case = null;
        obj->last_case_p = &(obj->first_case);
        obj->test_case_count = 0;
        obj->next_suite = null;
        return obj;
    }else{
        return null;
    }
}

fun(MCUnitTestSuite, void, bye, voida)
{
	MCUnitTestCase *iter, *save;
	for(iter=obj->first_case; (save=iter)!=null; release(save))
		iter = iter->next_case;
}

fun(MCUnitTestSuite, void, addTestCase, MCUnitTestCase* volatile tcase)
{
	retain(tcase);
    *(obj->last_case_p) = tcase;
    obj->last_case_p = cast(MCUnitTestCase**, &tcase);
	obj->test_case_count++;
}

fun(MCUnitTestSuite, void, runTestCases, voida)
{
	runtime_log("%s\n", "MCUnitTestSuite runTestCases");
	MCUnitTestCase *iter = null;
	for(iter=obj->first_case; iter!=null; iter = iter->next_case)
        MCUnitTestCase_runTests(iter, 0);
}

/* Test Result */

// #ifndef _MCUnitTestResult
// #define _MCUnitTestResult _MCObject;\

oninit(MCUnitTestResult)
{
    if (init(MCObject)) {
        //nothing to init
        return obj;
    }else{
        return null;
    }
}

fun(MCUnitTestResult, void, bye, voida)
{
	//nothing to clean
}

fun(MCUnitTestResult, void, addSuccessInfo, char* succinfo)
{
	//
}

fun(MCUnitTestResult, void, addFailInfo, char* failinfo)
{
	//
}

onload(MCUnitTestResult)
{
    if (load(MCObject)) {
        bid(MCUnitTestResult, void, bye);
        bid(MCUnitTestResult, void, addSuccessInfo, char* succinfo);
        bid(MCUnitTestResult, void, addFailInfo, char* failinfo);
        return cla;
    }else{
        return null;
    }
}

/* Test Runner */

// #ifndef _MCUnitTestRunner
// #define _MCUnitTestRunner _MCObject;\
// 	MCUnitTestResult* unitTestResult;\
// 	MCUnitTestSuite* first_suite;\
// 	int test_suite_count;\

// class(MCUnitTestRunner);

oninit(MCUnitTestRunner)
{
    if (init(MCObject)) {
        obj->first_suite = null;
        obj->test_suite_count = 0;
        return obj;
    }else{
        return null;
    }
}

fun(MCUnitTestRunner, void, bye, voida)
{
	MCUnitTestSuite *iter, *save;
	for(iter=obj->first_suite; (save=iter)!=null; release(save))
		iter = iter->next_suite;
}

fun(MCUnitTestRunner, void, addTestSuite, MCUnitTestSuite* testSuite)
{
	retain(testSuite);
	MCUnitTestSuite **iter;
	for(iter=&(obj->first_suite); (*iter)!=null; iter=&((*iter)->next_suite)){}
	(*iter)=testSuite;
	obj->test_suite_count++;
}

fun(MCUnitTestRunner, void, runTestSuites, voida)
{
	runtime_log("%s\n", "MCUnitTestRunner runTestSuites");
	MCUnitTestSuite *iter;
	for(iter=obj->first_suite; iter!=null; iter = iter->next_suite)
        MCUnitTestSuite_runTestCases(iter, 0);
}

onload(MCUnitTestRunner)
{
    if (load(MCObject)) {
        bid(MCUnitTestRunner, void, bye);
        bid(MCUnitTestRunner, void, addTestSuite, MCUnitTestSuite* testSuite);
        bid(MCUnitTestRunner, void, runTestSuites);
        return cla;
    }else{
        return null;
    }
}
