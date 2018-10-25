﻿#pragma once

#include "../inc/Bulk.h"
#include "../inc/CmdInterpreter.h"
#include "../inc/IObservable.h"
#include "../inc/Metrics.h"

#include <functional>
#include <list>
#include <memory>

namespace bulk {

/**
 * @brief Класс обработчика команд.
 */
class CmdProcessor : public IObservable  {
  public:
    /**
     * @brief Конструктор.
     * @param bulk_size - макисмальный размер блока команд.
     */
    explicit CmdProcessor(size_t bulk_size) : interpreter_{bulk_size}, metrics_{"main"} {
    }

    ~CmdProcessor() override = default;

    /**
     * @brief Подписка на получение блоков команд на вывод.
     * @param observer - подписчик.
     */
    void subscribe(const std::shared_ptr<IStreamWriter>& observer) final;

    /**
     * @brief Отписка от получения блоков команд на вывод.
     * @param observer - подписчик.
     */
    void unsubscribe(const std::shared_ptr<IStreamWriter>& observer) final;

    /**
     * @brief Обработка входной команды.
     * @param is - входной поток, из которого будут вычитываться команды.
     */
    void process(std::istream& is);

  private:
    /// Тип обозревателя.
    using observer_t = std::weak_ptr<IStreamWriter>;

    /**
     * @brief Вывод блока команд.
     * @param bulk - блок команд.
     */
    void publish(const Bulk& bulk) final;

    /// Список обозревателей, ожидающих вывод содержимого пула.
    std::list<observer_t> observers_{};
    /// Блок команд.
    Bulk bulk_{};
    /// Интерпретатор команд.
    CmdInterpreter interpreter_;
    /// Метрики основного потока.
    Metrics metrics_;
};

} // namespace bulk.
