#include "ox_slock.h"

void ox_slock_init(ox_slock_t *lock, long *flag)
{
  __sync_and_and_fetch((long*)&lock->spin_, 0);
  //__sync_lock_test_and_set((long*)&lock->spin_,flag?(long)flag:(long)&lock->flag_);
}

void ox_slock_lock(ox_slock_t *lock)
{
  for (;0!=__sync_fetch_and_or((long*)&lock->spin_, 1);) {
    ;
  }
}

void ox_slock_unlock(ox_slock_t *lock)
{
  __sync_and_and_fetch((long*)&lock->spin_, 0);
}

int ox_slock_trylock(ox_slock_t *lock)
{
  return !__sync_fetch_and_or((long*)&lock->spin_, 1);
}

int ox_slock_islock(ox_slock_t *lock)
{
  return __sync_add_and_fetch((long*)&lock->spin_, 0);
}
