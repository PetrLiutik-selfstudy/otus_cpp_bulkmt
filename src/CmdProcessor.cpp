#include "../inc/CmdProcessor.h"

#include <algorithm>
#include <istream>

#include <iostream>

#include <thread>

namespace bulk {

void CmdProcessor::subscribe(const std::shared_ptr<IStreamWriter>& observer) {
  auto it = std::find_if(observers_.begin(), observers_.end(), [&observer](observer_t& p) {
      return p.lock() == observer;
  });
  if(it == observers_.end())
    observers_.emplace_back(observer);
}

void CmdProcessor::unsubscribe(const std::shared_ptr<IStreamWriter>& observer) {
  auto it = std::find_if(observers_.begin(), observers_.end(), [&observer](observer_t& p) {
    return p.lock() == observer;
  });
  if(it != observers_.end())
    observers_.erase(it);
}

void CmdProcessor::process(std::istream& is) {
  size_t rows{};
  for(bool is_eof = false; !is_eof;) {
    std::string input;
    is_eof = !std::getline(is, input);
    if(!is_eof)
      rows++;

    std::string cmd;
    bool is_bulk_end = interpreter_.interpret(input, cmd);
    bulk_.push(cmd);
    if(is_bulk_end) {
      metrics_.push(std::this_thread::get_id(), bulk_, rows);
      publish(bulk_);
      bulk_.clear();
      rows = 0;
    }
  }
}

void CmdProcessor::publish(const Bulk& bulk) {
  for(auto& it: observers_) {
    if(!it.expired()) {
      auto p = it.lock();
      p->write(bulk);
    }
  }
};

void CmdProcessor::print_metrics(std::ostream& os) {
  os << metrics_;
  for(auto& it: observers_) {
    if(!it.expired()) {
      auto p = it.lock();
      os << p->get_metrics();
    }
  }
}

} // namespace bulk.
