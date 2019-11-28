#pragma once

#include <memory>

#include "envoy/common/pure.h"
#include "envoy/event/dispatcher.h"
#include "envoy/thread/thread.h"

namespace Envoy {
namespace Server {

/**
 * WatchDog objects are an individual thread's interface with the deadlock
 * GuardDog. A shared pointer to a WatchDog is obtained from the GuardDog at
 * thread startup. After this point the "touch" method must be called
 * periodically to avoid triggering the deadlock detector.
 * 监视程序对象是单个线程与死锁保护程序的接口。
 * 线程启动时，从GuardDog获取一个指向看门狗的共享指针。
 * 在此之后，必须定期调用“touch”方法，以避免触发死锁检测器。
 */
class WatchDog {
public:
  virtual ~WatchDog() = default;

  /**
   * Start a recurring touch timer in the dispatcher passed as argument.
   *
   * This will automatically call the touch() method at the interval specified
   * during construction.
   *
   * The timer object is stored within the WatchDog object. It will go away if
   * the object goes out of scope and stop the timer.
   */
  virtual void startWatchdog(Event::Dispatcher& dispatcher) PURE;

  /**
   * Manually indicate that you are still alive by calling this.
   *
   * This can be used if this is later used on a thread where there is no dispatcher.
   */
  virtual void touch() PURE;
  virtual Thread::ThreadId threadId() const PURE;
  virtual MonotonicTime lastTouchTime() const PURE;
};

using WatchDogSharedPtr = std::shared_ptr<WatchDog>;

} // namespace Server
} // namespace Envoy
