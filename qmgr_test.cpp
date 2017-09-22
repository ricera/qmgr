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
		ie_qmgr_init(&qmgr1, 1536);
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

TEST(qmgr_init, SuccessfulInitValidSize)
{
	int error = -1;

	error = ie_qmgr_init(&qmgr1, 1);

	LONGS_EQUAL(error, 0);
}

TEST(qmgr_init, FailInitInvalidSize)
{
	/* Negative errors should be invalid */
	int error = -1;

	error = ie_qmgr_init(&qmgr1, 0);

	LONGS_EQUAL(error, EINVAL);
}

TEST(qmgr_init, CorrectSizeAfterInit)
{
	uint32_t size = 0;

	ie_qmgr_init(&qmgr1, 1536);
	size = ie_qmgr_get_size(&qmgr1);

	UNSIGNED_LONGS_EQUAL(size, 1536);
}
