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
    explicit FileWriter() : metrics_{"file"} {
    }

    ~FileWriter() override {
      stop();
    }

    /**
     * @brief Запись блока команд в поток.
     * @param bulk - блок команд.
     */
    void write(const Bulk& bulk) final;

    /**
     * @brief Дать метрики работы потоков.
     * @return метрики по каждому из потоков.
     */
    Metrics& get_metrics() final;

  private:

    std::mutex metrics_mutex_{};
    Metrics metrics_;
};

} // namespace bulk.
