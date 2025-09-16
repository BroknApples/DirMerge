#ifndef THREADS_HPP
#define THREADS_HPP


#include <iostream>
#include <thread>
#include <future>
#include <type_traits>


// TODO: Fix this code up, its ChatGPT-generated; make it fit the project better.


class ThreadRunner {
  public:
    // Delete constructor to make it static-only
    ThreadRunner() = delete;

    // Generic run function
    template <typename Func, typename... Args>
    static auto run(Func&& func, Args&&... args) {
      using RetType = std::invoke_result_t<Func, Args...>;

      // Case 1: Function returns something
      if constexpr (!std::is_void_v<RetType>) {
        std::packaged_task<RetType()> task(
          std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
        );
        std::future<RetType> fut = task.get_future();
        std::thread(std::move(task)).detach();
        return fut; // caller can fut.get()
      }
      // Case 2: Function returns void
      else {
        std::thread(
          std::forward<Func>(func),
          std::forward<Args>(args)...
        ).detach();
        return; // nothing to return
      }
    }
};


#endif // THREADS_HPP
