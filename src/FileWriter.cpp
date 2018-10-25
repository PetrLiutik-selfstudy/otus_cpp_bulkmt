#include "../inc/FileWriter.h"

#include <fstream>

namespace bulk {

void FileWriter::write(const Bulk& bulk) {
  add_job([this, bulk](){
    std::string file_name = "bulk" + std::to_string(bulk.time()) + "_" + std::to_string(get_job_id()) + ".log";
    std::fstream fs{file_name, std::ios::app};

    if(fs.is_open()) {
      fs << "ThreadId: " << std::this_thread::get_id() << bulk;
      fs.close();
    }

    // Добавление метрики.
    std::lock_guard<std::mutex> lock(metrics_guard_);
    threads_metrics_[std::this_thread::get_id()] += {1, bulk.size()};
  });
}

std::map<std::thread::id, Metrics> FileWriter::get_metrics() {
  std::lock_guard<std::mutex> lock(metrics_guard_);
  return threads_metrics_;
}


} // namespace bulk.
