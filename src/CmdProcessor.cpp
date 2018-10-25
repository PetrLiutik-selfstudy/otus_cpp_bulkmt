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
  for(bool is_eof = false; !is_eof;) {
    std::string input;
    is_eof = !std::getline(is, input);

    std::string cmd;
    bool is_bulk_end = interpreter_.interpret(input, cmd);
    bulk_.push(cmd);
    if(is_bulk_end) {
      publish(bulk_);
      bulk_.clear();
    }
  }

  std::this_thread::sleep_for(std::chrono::seconds(1));
  for(auto& it: observers_) {
    if(!it.expired()) {
      auto p = it.lock();
      auto metrics = p->get_metrics();
      for(auto metric: metrics) {
        std::cout << "Thread: " << metric.first << " blocks: " << metric.second.get_bloks() << " commands: " << metric.second.get_commands_() << std::endl;
      }
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

} // namespace bulk.
