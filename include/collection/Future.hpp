  #pragma once

  #include <future>
  #include <thread>

  template <typename T>
  class Future {
  public:
    template <typename F>
    Future(F func) {
      _future = std::async(std::launch::async, func).share();
    }

    Future(const Future& other) {
      _future = other._future;
    }

    Future(Future&& other) {
      _future = std::move(other._future);
    }

    auto operator=(const Future& other) -> Future& {
      _future = other._future;
      return *this;
    }

    auto operator=(Future&& other) -> Future& {
      _future = std::move(other._future);
      return *this;
    }

    auto wait() -> void {
      _future.wait();
    }

    auto isReady() -> bool {
      auto status = _future.wait_for(std::chrono::seconds(0));
      return status == std::future_status::ready;
    }

    auto get() -> T {
      return _future.get();
    }
  private:
    std::shared_future<T> _future;
  };
