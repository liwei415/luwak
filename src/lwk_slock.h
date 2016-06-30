#ifndef _LWK_SLOCK_
#define _LWK_SLOCK_

#include <sched.h>

typedef struct lwk_slock_s lwk_slock_t;

struct lwk_slock_s{
    volatile long spin_;
    volatile long flag_;
};

void lwk_slock_init(lwk_slock_t *lock, long *flag);
void lwk_slock_lock(lwk_slock_t *lock);
void lwk_slock_unlock(lwk_slock_t *lock);
int  lwk_slock_trylock(lwk_slock_t *lock);
int  lwk_slock_islock(lwk_slock_t *lock);

#endif
