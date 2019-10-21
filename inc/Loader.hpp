//
// Created by pp on 10.10.19.
//

#pragma once

#include <EvmExecutable.hpp>
#include <Memory.hpp>

namespace esetvm2::core {
  class Loader
  {
  public:
    void load(Memory& memory, esetvm2::file_format::EvmExecutable& exe);
  };
}