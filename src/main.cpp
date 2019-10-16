//
// Created by pp on 04.09.19.
//

#include "../inc/EvmExecutable.hpp"
#include "../inc/Evm.hpp"

using namespace esetvm2::file_format;

int main()
{
  const std::string exePath = "/home/pp/Projects/EsetVm2/specs/samples/precompiled/crc.evm";

  auto evmExe = EvmExecutable{exePath};



  evmExe.loadSections();

//  auto vm = esetvm2::core::Evm{};
//  vm.run(exePath);
}

