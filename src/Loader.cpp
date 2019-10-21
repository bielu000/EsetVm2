//
// Created by pp on 16.10.19.
//

#include <Loader.hpp>
#include <Memory.hpp>
#include <spdlog/spdlog.h>

using namespace esetvm2::file_format;
using namespace esetvm2::core;

void Loader::load(esetvm2::core::Memory& memory, esetvm2::file_format::EvmExecutable& exe)
{
  auto header = exe.getHeader();
  memory.alloc(header.codeSize + header.dataSize);
  auto memIter = memory.getData().begin();

  //Load code section
  auto codeSection = exe.getSection(EvmExecutable::Section::Type::Code);

  if (!codeSection) {
    spdlog::error("Cannot load code -> section error");

    return;
  }

  std::copy(codeSection->begin(), codeSection->end(), memIter);
  std::advance(memIter, codeSection->getSize());

  //Load data section
  auto dataSection = exe.getSection(EvmExecutable::Section::Type::Data);

  if (!dataSection) {
    spdlog::info("Data section is not present so it won't be compied into memory");

    return;
  }

  std::copy(dataSection->begin(), dataSection->end(), memIter);
}
