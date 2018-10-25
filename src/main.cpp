#include <iostream>

#include "CmdProcessor.h"
#include "ConsoleWriter.h"
#include "FileWriter.h"
#include "ver.h"

#include <iostream>

int main(int argc, char const *argv[])
{
  std::cout << "bulkmt version: "
            << ver_major() << "."
            << ver_minor() << "."
            << ver_patch() << std::endl;

  if(argc < 2) {
    std::cerr << "Wrong number of arguments.\n" << std::endl;
    return EXIT_FAILURE;
  }

  auto bulk_size = std::strtoll(argv[1], nullptr, 0);
  if (bulk_size <= 0) {
    std::cerr << "Bulk size must be greater than 0.\n";
    return EXIT_FAILURE;
  }

  bulk::CmdProcessor cmd_processor(static_cast<size_t>(bulk_size));

  auto console_writer = std::make_shared<bulk::ConsoleWriter>();
  auto file_writer = std::make_shared<bulk::FileWriter>();

  cmd_processor.subscribe(console_writer);
  cmd_processor.subscribe(file_writer);

  console_writer->start();
  file_writer->start();

  cmd_processor.process(std::cin);

  console_writer->stop();
  file_writer->stop();

  cmd_processor.print_metrics(std::cout);

  cmd_processor.unsubscribe(console_writer);
  cmd_processor.unsubscribe(file_writer);

  return 0;
}
