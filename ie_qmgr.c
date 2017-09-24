#include "ie_qmgr.h"

int
ie_qmgr_init(struct ie_qmgr *qmgr, uint32_t size, uint32_t max_alloc_size)
{
	if (size < 1)
		return (EINVAL);

	qmgr->unallocated = size;
	qmgr->size = size;
	qmgr->max_alloc_size = max_alloc_size;

	return (0);
}

uint32_t
ie_qmgr_get_size(struct ie_qmgr *qmgr)
{
	return (qmgr->size);
}

uint32_t
ie_qmgr_get_unallocated(struct ie_qmgr *qmgr)
{
	return (qmgr->unallocated);
}

int
ie_qmgr_allocate(struct ie_qmgr *qmgr, uint32_t num)
{
	if (qmgr->unallocated - (long)num < 0)
		return (EINVAL);
	if (num > qmgr->max_alloc_size)
		return (EINVAL);

	qmgr->unallocated -= num;

	return (0);
}

int
ie_qmgr_deallocate(struct ie_qmgr *qmgr, uint32_t num)
{
	qmgr->unallocated += num;

	return (0);
}

