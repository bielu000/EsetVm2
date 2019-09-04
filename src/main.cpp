//
// Created by pp on 04.09.19.
//

#include <EvmHeader.hpp>

using namespace esetvm2::file_format;

int main()
{
  const std::string exePath = "specs/samples/precompiled/fibonacci_loop.evm";

  auto evmExe = EvmHeader{exePath};
}

