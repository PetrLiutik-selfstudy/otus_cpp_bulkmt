#pragma once

#include "../inc/IStreamWriter.h"
#include "../inc/ThreadPool.h"

#include <map>

namespace bulk {

/**
 * @brief Класс вывода блока команд в файл.
 */
class FileWriter : public IStreamWriter, public ThreadPool<2> {
  public:
    FileWriter() = default;
    ~FileWriter() override {
      stop();
    }

    /**
     * @brief Запись блока команд в поток.
     * @param time - время начала блока команд.
     * @param bulk - блок команд.
     */
    void write(const std::time_t& time, const std::vector<std::string>& bulk) final;

    /**
     * @brief Дать метрики работы потоков.
     * @return метрики по каждому из потоков.
     */
    std::map<std::thread::id, Metrics> get_metrics() final;

  private:

    std::mutex metrics_guard_;
    std::map<std::thread::id, Metrics> threads_metrics_;
};

} // namespace bulk.
