#ifndef _OX_SLOCK_
#define _OX_SLOCK_

#include <sched.h>

typedef struct ox_slock_s ox_slock_t;

struct ox_slock_s{
    volatile long spin_;
    volatile long flag_;
};

void ox_slock_init(ox_slock_t *lock, long *flag);
void ox_slock_lock(ox_slock_t *lock);
void ox_slock_unlock(ox_slock_t *lock);
int  ox_slock_trylock(ox_slock_t *lock);
int  ox_slock_islock(ox_slock_t *lock);

#endif
