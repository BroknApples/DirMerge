#ifndef CLOCK_HPP
#define CLOCK_HPP


#include <chrono>
#include <ctime>
#include <thread>


/**
 * @brief Class used to track the time of the program/computer.
 * 
 * ------------
 * 
 * STATIC ONLY CLASS
 */
class Clock {
  private:
    // TODO: Add runtime and local_time;
    //
    // runtime = time the program has been active
    // local_time = current time in your time zone


  public:
    /**
     * @brief Enforce Static Class
     */
    Clock() = delete;


    /**
     * @brief Setup the clock for usage
     */
    static void init();


    /**
     * @brief Equivalent to a sleep() function. | Applies on the current thread.
     * 
     * Usage  ->   Clock::wait(100, std::chrono::milliseconds{});
     * 
     * @param seconds: Time to sleep the current thread for in seconds
     * @tparam Duration: A 'std::chrono::' duration value such as 'milliseconds' or 'microseconds'
     */
    template <typename Duration>
    static void wait(double time, Duration) {
      std::this_thread::sleep_for(std::chrono::duration<double>(time));
    }

    /** Helper methods for seconds, milliseconds, and microseconds */

    static void wait_s(double time) { wait(time, std::chrono::seconds{}); }
    static void wait_ms(double time) { wait(time, std::chrono::milliseconds{}); }
    static void wait_us(double time) { wait(time, std::chrono::microseconds{}); }
};


#endif // CLOCK_HPP
