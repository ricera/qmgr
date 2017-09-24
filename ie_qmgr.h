
#include <sys/types.h>
#include <sys/errno.h>

#ifndef _IE_QMGR_H_
#define _IE_QMGR_H_

#if 0
enum ie_qmgr_alloc_type {
	IE_QMGR_ALLOC_CONTIG
};
#endif

struct ie_qmgr {
	uint32_t size;
	uint32_t unallocated;
	uint32_t max_alloc_size;
};

int 		ie_qmgr_init(struct ie_qmgr *qmgr, uint32_t size, uint32_t max_alloc_size);
uint32_t	ie_qmgr_get_size(struct ie_qmgr *qmgr);
uint32_t	ie_qmgr_get_unallocated(struct ie_qmgr *qmgr);
int		ie_qmgr_allocate(struct ie_qmgr *qmgr, uint32_t num);
int		ie_qmgr_deallocate(struct ie_qmgr *qmgr, uint32_t num);

#endif /* _IE_QMGR_H_ */
