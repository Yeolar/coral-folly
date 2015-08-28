/*
 * Copyright 2015 Yeolar
 * Copyright 2015 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

/*
 * This class provides a few spin lock implementations, depending on the
 * platform.  coral/SpinLock.h will select one of these as the coral::SpinLock
 * implementation.
 *
 * The main reason we keep these separated out here is so that we can run unit
 * tests for all supported spin lock implementations, even though only one will
 * be selected as the actual coral::SpinLock implemenatation for any given
 * platform.
 */

#include <boost/noncopyable.hpp>
#include <coral/Portability.h>

#if __x86_64__
#include <coral/SmallLocks.h>

namespace coral {

class SpinLockMslImpl {
 public:
  CORAL_ALWAYS_INLINE SpinLockMslImpl() {
    lock_.init();
  }
  CORAL_ALWAYS_INLINE void lock() const {
    lock_.lock();
  }
  CORAL_ALWAYS_INLINE void unlock() const {
    lock_.unlock();
  }
  CORAL_ALWAYS_INLINE bool trylock() const {
    return lock_.try_lock();
  }
 private:
  mutable coral::MicroSpinLock lock_;
};

}

#endif // __x86_64__

#if __APPLE__
#include <libkern/OSAtomic.h>

namespace coral {

class SpinLockAppleImpl {
 public:
  CORAL_ALWAYS_INLINE SpinLockAppleImpl() : lock_(0) {}
  CORAL_ALWAYS_INLINE void lock() const {
    OSSpinLockLock(&lock_);
  }
  CORAL_ALWAYS_INLINE void unlock() const {
    OSSpinLockUnlock(&lock_);
  }
  CORAL_ALWAYS_INLINE bool trylock() const {
    return OSSpinLockTry(&lock_);
  }
 private:
  mutable OSSpinLock lock_;
};

}

#endif // __APPLE__

#include <pthread.h>
#include <coral/Exception.h>

#if CORAL_HAVE_PTHREAD_SPINLOCK_T

// Apple and Android systems don't have pthread_spinlock_t, so we can't support
// this version on those platforms.
namespace coral {

class SpinLockPthreadImpl {
 public:
  CORAL_ALWAYS_INLINE SpinLockPthreadImpl() {
    int rc = pthread_spin_init(&lock_, PTHREAD_PROCESS_PRIVATE);
    checkPosixError(rc, "failed to initialize spinlock");
  }
  CORAL_ALWAYS_INLINE ~SpinLockPthreadImpl() {
    pthread_spin_destroy(&lock_);
  }
  void lock() const {
    int rc = pthread_spin_lock(&lock_);
    checkPosixError(rc, "error locking spinlock");
  }
  CORAL_ALWAYS_INLINE void unlock() const {
    int rc = pthread_spin_unlock(&lock_);
    checkPosixError(rc, "error unlocking spinlock");
  }
  CORAL_ALWAYS_INLINE bool trylock() const {
    int rc = pthread_spin_trylock(&lock_);
    if (rc == 0) {
      return true;
    } else if (rc == EBUSY) {
      return false;
    }
    throwSystemErrorExplicit(rc, "spinlock trylock error");
  }
 private:
  mutable pthread_spinlock_t lock_;
};

}

#endif // CORAL_HAVE_PTHREAD_SPINLOCK_T

namespace coral {

class SpinLockPthreadMutexImpl {
 public:
  CORAL_ALWAYS_INLINE SpinLockPthreadMutexImpl() {
    int rc = pthread_mutex_init(&lock_, nullptr);
    checkPosixError(rc, "failed to initialize mutex");
  }
  CORAL_ALWAYS_INLINE ~SpinLockPthreadMutexImpl() {
    pthread_mutex_destroy(&lock_);
  }
  void lock() const {
    int rc = pthread_mutex_lock(&lock_);
    checkPosixError(rc, "error locking mutex");
  }
  CORAL_ALWAYS_INLINE void unlock() const {
    int rc = pthread_mutex_unlock(&lock_);
    checkPosixError(rc, "error unlocking mutex");
  }
  CORAL_ALWAYS_INLINE bool trylock() const {
    int rc = pthread_mutex_trylock(&lock_);
    if (rc == 0) {
      return true;
    } else if (rc == EBUSY) {
      return false;
    }
    throwSystemErrorExplicit(rc, "mutex trylock error");
  }
 private:
  mutable pthread_mutex_t lock_;
};

}
