#pragma once

#include "../inc/IStreamWriter.h"
#include "../inc/ThreadPool.h"

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

  private:
    uint8_t cnt_;
};

} // namespace bulk.
