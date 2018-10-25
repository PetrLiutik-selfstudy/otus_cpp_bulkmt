#include "../inc/ConsoleWriter.h"

#include <iostream>
#include <sstream>

namespace bulk {

void ConsoleWriter::write(const Bulk& bulk) {
  add_job([this, bulk](){
    os_ << bulk;

    // Добавление метрики.
    std::lock_guard<std::mutex> lock(metrics_guard_);
    threads_metrics_[std::this_thread::get_id()] += {1, bulk.size()};
  });
}

std::map<std::thread::id, Metrics> ConsoleWriter::get_metrics() {
  std::lock_guard<std::mutex> lock(metrics_guard_);
  return threads_metrics_;
}

} // namespace bulk.
