#include "lwk_slock.h"

void lwk_slock_init(lwk_slock_t *lock, long *flag)
{
  __sync_and_and_fetch((long*)&lock->spin_, 0);
  //__sync_lock_test_and_set((long*)&lock->spin_,flag?(long)flag:(long)&lock->flag_);
}

void lwk_slock_lock(lwk_slock_t *lock)
{
  for (;0!=__sync_fetch_and_or((long*)&lock->spin_, 1);) {
    ;
  }
}

void lwk_slock_unlock(lwk_slock_t *lock)
{
  __sync_and_and_fetch((long*)&lock->spin_, 0);
}

int lwk_slock_trylock(lwk_slock_t *lock)
{
  return !__sync_fetch_and_or((long*)&lock->spin_, 1);
}

int lwk_slock_islock(lwk_slock_t *lock)
{
  return __sync_add_and_fetch((long*)&lock->spin_, 0);
}
