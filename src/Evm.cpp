//
// Created by pp on 10.10.19.
//

#include <Evm.hpp>
#include <EvmExecutable.hpp>
#include <spdlog/spdlog.h>

using namespace esetvm2::core;
using namespace esetvm2::file_format;

class ProcessMemory
{

};

class Loader
{
public:
  void fill(ProcessMemory& mem, EvmExecutable& exe)
  {
  }
};

class Memory
{
public:
  using ValueType = std::vector<uint32_t>;




private:
  ValueType data_;
};

void Evm::run(const std::string& exePath)
{
  auto loader =   std::make_unique<Loader>();
  auto cpu =      std::make_unique<Cpu>();
  auto memory =   std::make_unique<Memory>();

  try {
    auto evmExecutable = EvmExecutable{exePath};
    evmExecutable.loadSections();

    loader->load(*memory, evmExecutable);

    uint8_t d = 33;



    auto x = 10;

//    while(auto instruction = decoder->fetch(); ) {
//      cpu->process(instruction);
//    }

  } catch (const std::exception& e) {
    spdlog::error("Critical error occurred: {}", e.what());
  }
}