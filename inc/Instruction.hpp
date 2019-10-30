//
// Created by pp on 27.10.19.
//

#pragma once

#include <cstdint>
#include <string>

namespace esetvm2::core {
  class Instruction
  {
  public:
    Instruction(uint8_t code, uint8_t len, const std::string& mnemonic)
      : code_(code), len_(len), mnemonic_(mnemonic) {}

    using opcode = uint8_t;
    using opcode_length = uint8_t ;

    opcode getOpcode() { return code_; }
    opcode_length getLen() { return len_; }
    std::string getMnemonic() { return mnemonic_; }

  private:
    opcode code_;
    opcode_length len_;
    std::string mnemonic_;
  };
}

