
#include "CuTest.h"
#include "util.h"
    
void TestUtilFile(CuTest *tc) {
	int h;
	int *hp;
	h = 1;
	hp = &h;
	CuAssertTrue(tc, util_file(hp));
}
   
CuSuite* utilGetSuite() {
	CuSuite* suite = CuSuiteNew();
	SUITE_ADD_TEST(suite, TestUtilFile);
	return suite;
}
 
