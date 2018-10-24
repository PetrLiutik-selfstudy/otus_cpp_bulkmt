﻿#include "../inc/CmdProcessor.h"

#include <algorithm>
#include <istream>

#include <iostream>

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
    bulk_pool_.push(cmd);
    if(is_bulk_end)
      bulk_pool_.flush(publish);
  }

  for(auto& it: observers_) {
    if(!it.expired()) {
      auto p = it.lock();
      auto metrics = p->get_metrics();
      for(auto metric: metrics) {
        std::cout << "Blocks: " << metric.second.get_bloks() << std::endl;
      }
    }
  }
}

} // namespace bulk.
