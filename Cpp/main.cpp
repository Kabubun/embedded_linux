#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <iomanip>
#ifdef RASPI
#include "pigpio.h"
#endif
typedef void (*Func)();

class Blink {
 public:
  Blink(Func on_func, Func off_func, double cycle_time, size_t limit)
      : led_status_(LedStatus::OFF),
        on_func_(nullptr),
        off_func_(nullptr),
        count_(0),
        limit_(0),
        cycle_time_(0.0) {
    assert(on_func != nullptr);
    assert(off_func != nullptr);
    on_func_ = on_func;
    off_func_ = off_func;
    limit_ = limit;
    cycle_time_ = cycle_time;
  }
  ~Blink() {}
  void loop() {
    while (true) {
      if (limit_ == count_) {
        return;
      }
      task();
      std::this_thread::sleep_for(
          std::chrono::milliseconds(static_cast<size_t>(cycle_time_ * 1000)));
      count_++;
    }
  }

 private:
  void task() {
    if (LedStatus::ON == led_status_) {
      (*off_func_)();
      led_status_ = LedStatus::OFF;
    } else {
      (*on_func_)();
      led_status_ = LedStatus::ON;
    }
  }
  enum class LedStatus { ON, OFF };
  LedStatus led_status_;
  void (*on_func_)(void);
  void (*off_func_)(void);
  size_t count_;
  size_t limit_;
  double cycle_time_;
};

std::string getTimeStamp()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(localtime(&now_c), "%H%M%S");
    return ss.str();
}

#ifdef RASPI
constexpr unsigned ON = 1;
constexpr unsigned OFF = 0;
constexpr unsigned PIN = 17;
void OnFunc() { gpioWrite(PIN, ON); };
void OffFunc() { gpioWrite(PIN, OFF); };
#else
void OnFunc() { std::cout << getTimeStamp() <<":LED ON" << std::endl; };
void OffFunc() { std::cout << getTimeStamp() << ":LED OFF" << std::endl; };
#endif
int main(void) {
#ifdef RASPI
  gpioInitialise();
#endif
  std::unique_ptr<Blink> blink;
  Func on_func = OnFunc;
  Func off_func = OffFunc;
  blink = std::make_unique<Blink>(on_func, off_func, 0.5, 10);
  blink->loop();
  return 0;
}