//
// Created by pp on 27.10.19.
//

#pragma once

#include <cstdint>

#include "../inc/Instruction.hpp"
#include "../inc/Memory.hpp"

namespace esetvm2::core {
  class Decoder
  {
  public:
    explicit Decoder(Memory& memory)
      : memory_(memory) {}

    Instruction decode(uint16_t offset);

  private:
    Memory& memory_;
  };
}