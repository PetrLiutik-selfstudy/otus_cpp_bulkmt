#pragma once

#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <thread>

namespace bulk {

class Metrics {
  public:
    Metrics() = default;
    Metrics(size_t blocks, size_t commands) : blocks_{blocks}, commands_{commands} {
    }
    ~Metrics() = default;

    void operator += (const Metrics& other) {
      blocks_ += other.blocks_;
      commands_ += other.commands_;
    }

    size_t get_bloks() {
      return blocks_;
    }

    size_t get_commands_() {
      return commands_;
    }

  private:

    size_t blocks_{};
    size_t commands_{};
};

/**
 * @brief Интерфейс для классов выводящих блок команд в поток.
 */
class IStreamWriter {
  public:
    virtual ~IStreamWriter() = default;

    /**
     * @brief Запись блока команд в поток.
     * @param time - время начала блока команд.
     * @param bulk - блок команд.
     */
    virtual void write(const std::time_t& time, const std::vector<std::string>& bulk) = 0;

    /**
     * @brief Дать метрики работы потоков.
     * @return метрики по каждому из потоков.
     */
    virtual std::map<std::thread::id, Metrics> get_metrics() = 0;
};

} // namespace bulk.
