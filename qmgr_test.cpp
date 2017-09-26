#include <CppUTest/TestHarness.h>

extern "C" {
	#include "ie_qmgr.h"
}

TEST_GROUP(qmgr_init)
{
	struct ie_qmgr qmgr1;
};

TEST_GROUP(qmgr_use)
{
	struct ie_qmgr qmgr1;

	void setup()
	{
		ie_qmgr_init(&qmgr1, 1536, 128);
	}

	void teardown()
	{

	}
};

/**
 * Get the number of unallocated queues after not allocating any queues.
 */
TEST(qmgr_use, GetUnallocatedQueuesNoQueuesAllocated)
{
	uint32_t uaq = UINT_MAX;

	uaq = ie_qmgr_get_unallocated(&qmgr1);

	UNSIGNED_LONGS_EQUAL(uaq, 1536);
}

TEST(qmgr_use, AllocateOneQueue)
{
	uint32_t uaq = 0;

	ie_qmgr_allocate(&qmgr1, 1);

	uaq = ie_qmgr_get_unallocated(&qmgr1);

	UNSIGNED_LONGS_EQUAL(uaq, 1535);
}

TEST(qmgr_use, DeallocateOneQueue)
{
	uint32_t uaq = 0;
	ie_qmgr_allocate(&qmgr1, 1);
	uaq = ie_qmgr_get_unallocated(&qmgr1);
	UNSIGNED_LONGS_EQUAL(uaq, 1535);

	ie_qmgr_deallocate(&qmgr1, 1);
	uaq = ie_qmgr_get_unallocated(&qmgr1);
	UNSIGNED_LONGS_EQUAL(uaq, 1536);
}

TEST(qmgr_use, AllocateTooManySingleQueues)
{
	int error_counter = 0;
	int error = -1;

	/* Allocate every queue available */
	for (int i = 0; i < 1536; i++) {
		error = ie_qmgr_allocate(&qmgr1, 1);
		error_counter += (error > 0);
	}

	LONGS_EQUAL(error_counter, 0);
	LONGS_EQUAL(ie_qmgr_get_unallocated(&qmgr1), 0);

	/* Then allocate one more past that */
	error = ie_qmgr_allocate(&qmgr1, 1);

	CHECK(error > 0);
}

static void
qmgr_test_allocate_and_check(struct ie_qmgr *qmgr, int num, int unallocated_after)
{
	int error = -1;

	error = ie_qmgr_allocate(qmgr, num);
	LONGS_EQUAL(error, 0);
	LONGS_EQUAL(ie_qmgr_get_unallocated(qmgr), unallocated_after);
}

TEST(qmgr_use, AllocateVariousSizes)
{
	/* Assumes qmgr1 max alloc size is 128 */
	qmgr_test_allocate_and_check(&qmgr1, 1, 1535);
	qmgr_test_allocate_and_check(&qmgr1, 2, 1533);
	qmgr_test_allocate_and_check(&qmgr1, 4, 1529);
	qmgr_test_allocate_and_check(&qmgr1, 8, 1521);
	qmgr_test_allocate_and_check(&qmgr1, 16, 1505);
	qmgr_test_allocate_and_check(&qmgr1, 32, 1473);
	qmgr_test_allocate_and_check(&qmgr1, 64, 1409);
	qmgr_test_allocate_and_check(&qmgr1, 128, 1281);
}

TEST(qmgr_init, TestAllocationMaxSize)
{
	int error = -1;
	uint32_t uaq = 0;

	ie_qmgr_init(&qmgr1, 1536, 64);
	error = ie_qmgr_allocate(&qmgr1, 64);
	LONGS_EQUAL(error, 0);

	error = ie_qmgr_allocate(&qmgr1, 128);
	CHECK(error > 0);

	// Make sure number of unallocated queues is unaffected
	UNSIGNED_LONGS_EQUAL(ie_qmgr_get_unallocated(&qmgr1), 1472); 
}

TEST(qmgr_init, SuccessfulInitValidSize)
{
	int error = -1;

	error = ie_qmgr_init(&qmgr1, 1, 128);

	LONGS_EQUAL(error, 0);
}

TEST(qmgr_init, FailInitInvalidSize)
{
	/* Negative errors should be invalid */
	int error = -1;

	error = ie_qmgr_init(&qmgr1, 0, 128);

	LONGS_EQUAL(error, EINVAL);
}

TEST(qmgr_init, CorrectSizeAfterInit)
{
	uint32_t size = 0;

	ie_qmgr_init(&qmgr1, 1536, 128);
	size = ie_qmgr_get_size(&qmgr1);

	UNSIGNED_LONGS_EQUAL(size, 1536);
}
