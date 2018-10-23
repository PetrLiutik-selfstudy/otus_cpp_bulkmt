#include "../inc/ConsoleWriter.h"

#include <iostream>
#include <sstream>

namespace bulk {

void ConsoleWriter::write(const std::time_t&, const std::vector<std::string>& bulk) {
  add_job([=](){
    os_ << "bulk: ";
    for(const auto &it : bulk) {
      os_ << it << (&it != &bulk.back() ? ", " : "\n");
    }
  });
}

} // namespace bulk.
