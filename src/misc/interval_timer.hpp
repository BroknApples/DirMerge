/*
Simple timer class.

© 2025 BroknApples — modifications allowed; do not remove this notice.
*/

#ifndef INTERVAL_TIMER_HPP
#define INTERVAL_TIMER_HPP


#include <chrono>
#include <optional>

#include "utils.hpp"


/**
 * @brief Timer which tracks the time elapsed between two points in time.
 */
class IntervalTimer {
  private:
    std::optional<std::chrono::high_resolution_clock::time_point> _start;
    std::optional<std::chrono::high_resolution_clock::time_point> _end;


  public:
    /**
     * @brief Set params to default
     */
    IntervalTimer() = default;


    /**
     * @brief Set the starting point for time measuring
     * @returns bool: True/False of success
     */
    bool start() {
      // Do not allow the timer to be started without being reset
      if (_start.has_value()) {
        println("Please reset the timer before starting it.");
        return false;
      }

      _start = std::chrono::high_resolution_clock::now();
      return true;
    }


    /**
     * @brief Set the ending point for time measuring
     * @returns bool: True/False of success
     */
    bool end() {
      // Do not allow the timer to be started without being reset
      if (_end.has_value()) {
        println("Please reset the timer before ending.");
        return false;
      }

      _end = std::chrono::high_resolution_clock::now();
      return true;
    }

    /**
     * @brief Reset the start and end values
     */
    void reset() {
      _start.reset();
      _end.reset();
    }


    /**
     * @brief Get the difference between the recorded start and end values
     * 
     * Usage  ->   timer_name.diff(std::chrono::milliseconds{});
     * 
     * 
     * @tparam Duration: A 'std::chrono::' duration value such as 'milliseconds' or 'microseconds'
     * @returns double: Elapsed time
     */
    template <typename Duration>
    double diff(Duration) const {
      // In order to get the difference, the timer must have started and ended
      if (!_start.has_value() || !_end.has_value()) {
        println("Must start() and end() the timer before getting the time difference.");
        return 0.0;
      }

      return std::chrono::duration<double, typename Duration::period>(*_end - *_start).count();
    }

    /** Helper methods for seconds, milliseconds, and microseconds */

    double diff_s()  const { return diff(std::chrono::seconds{}); }
    double diff_ms() const { return diff(std::chrono::milliseconds{}); }
    double diff_us() const { return diff(std::chrono::microseconds{}); }
};

#endif // INTERVAL_TIMER_HPP
