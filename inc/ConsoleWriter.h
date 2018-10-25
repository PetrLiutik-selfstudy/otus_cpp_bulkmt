#pragma once

#include "../inc/IStreamWriter.h"
#include "../inc/ThreadPool.h"

#include <iostream>
#include <map>
#include <ostream>

namespace bulk {

/**
 * @brief Класс вывода блока команд в консоль.
 */
class ConsoleWriter : public IStreamWriter, public ThreadPool<> {
  public:
    /**
     * @brief Консруктор.
     * @param os - поток для вывода.
     */
    explicit ConsoleWriter(std::ostream& os = std::cout) : os_(os), metrics_{"log"} {
    }

    ~ConsoleWriter() override {
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
    std::ostream& os_;
    std::mutex metrics_mutex_{};
    Metrics metrics_;
};

} // namespace bulk.
