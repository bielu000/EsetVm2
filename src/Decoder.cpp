//
// Created by pp on 27.10.19.
//

#include <cstdint>
#include <list>
#include <algorithm>
#include <iostream>
#include <Instruction.hpp>
#include <Decoder.hpp>
#include <Memory.hpp>

using namespace esetvm2::core;

namespace {
  constexpr uint8_t INSTR_MASK_SHIFT = 8;

  static std::list<Instruction> instructions = {
    Instruction(0b000,    3, "mov"),
    Instruction(0b001,    3, "loadConst"),
    Instruction(0b010001, 6, "add"),
    Instruction(0b010010, 6, "sub"),
    Instruction(0b010011, 6, "div"),
    Instruction(0b010100, 6, "mod"),
    Instruction(0b010101, 6, "mul"),
    Instruction(0b01100,  5, "compare"),
    Instruction(0b01101,  5, "jump"),
    Instruction(0b01110,  5, "jumpEqual"),
    Instruction(0b10000,  5, "read"),
    Instruction(0b10001,  5, "write"),
    Instruction(0b10001,  5, "consoleRead"),
    Instruction(0b10011,  5, "consoleWrite"),
    Instruction(0b10100,  5, "createThread"),
    Instruction(0b10101,  5, "joinThread"),
    Instruction(0b10110,  5, "hlt"),
    Instruction(0b10111,  5, "sleep"),
    Instruction(0b1100,   5, "call"),
    Instruction(0b1101,   5, "ret"),
    Instruction(0b1110,   5, "lock"),
    Instruction(0b1111,   5, "unlock")
  };

  bool isInstructionValid(uint8_t opcode, uint8_t opcodeLen)
  {
    auto iter = std::find_if(instructions.begin(), instructions.end(), [&](auto& op) {
      return op.getOpcode() == opcode && op.getLen() == opcodeLen;
    });

    return iter != instructions.end();
  }

  Instruction getInstruction(uint8_t opcode, uint8_t opcodeLen)
  {
    auto it = std::find_if(instructions.begin(), instructions.end(), [&](auto& op)
    {
      return op.getOpcode() == opcode && op.getLen() == opcodeLen;
    });

    return *it;
  }
}

namespace esetvm2::core {
  Instruction Decoder::decode(uint16_t offset)
  {
    Instruction::opcode opcode{};
    Instruction::opcode_length opLen = 0;
    uint8_t opcodeBitShift = 7;

    auto stream = MemBitStream{&memory_, offset};

    do {
      opcode |= (stream.get<1>() << opcodeBitShift--);
      ++opLen;
    } while(!isInstructionValid((opcode >> (INSTR_MASK_SHIFT - opLen)), opLen));

    auto instruction = getInstruction((opcode >> (INSTR_MASK_SHIFT - opLen)), opLen);

    std::cout << instruction.getMnemonic() << " : " << instruction.getOpcode() << std::endl;

    return instruction;
  }
}