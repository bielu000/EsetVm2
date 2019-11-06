//
// Created by pp on 04.09.19.
//

#include <Decoder.hpp>
#include <Instruction.hpp>
#include <Evm.hpp>
#include <EvmExecutable.hpp>
#include <cassert>

using namespace esetvm2::file_format;

void test_8_bit()
{
  auto memory = esetvm2::core::Memory{};
  memory.alloc(32);

  std::array<std::byte, 10> arr {
    (std::byte)0xbf, (std::byte)0xff, (std::byte)0xff, (std::byte)0xff, (std::byte)0x7f, (std::byte)0x7f
  };

  std::copy(arr.begin(), arr.end(), memory.begin());

  auto stream = esetvm2::core::MemBitStream{&memory};

  auto x1 = stream.get<2>();
  assert(x1 == 0x02);

  auto x2 = stream.get<32>();
  assert(x2 == 0xFFFFFFFD);

  auto x3 = stream.get<6>();
  assert(x3 == 0x3f);
}

void test_16_bit()
{
  auto memory = esetvm2::core::Memory{};
  memory.alloc(32);

  std::array<std::byte, 10> arr {
    (std::byte)0xbf, (std::byte)0xff, (std::byte)0x80, (std::byte)0xff, (std::byte)0x7f, (std::byte)0xde
  };

  std::copy(arr.begin(), arr.end(), memory.begin());

  auto stream = esetvm2::core::MemBitStream{&memory};

  auto x1 = stream.get<14>();
  assert(x1 == 0x2fff);

  auto x2 = stream.get<32>();
  assert(x2 == 0xe03fdff7);

  auto x3 = stream.get<2>();
  assert(x3 == 2);
//
//  auto x3 = stream.get<6>();
//  assert(x3 == 0x3f);
}

int main()
{
  spdlog::set_level(spdlog::level::trace);
  const std::string exePath = "/home/pp/Projects/EsetVm2/specs/samples/precompiled/crc.evm";

//  auto vm = esetvm2::core::Evm{};
//  vm.run(exePath);

  auto file = EvmExecutable{exePath};
  file.loadSections();

  auto codeSection = file.getSection(EvmExecutable::Section::Type::Code);
  auto memory = esetvm2::core::Memory{};
  memory.alloc(codeSection->getSize());

  std::copy(codeSection->begin(), codeSection->end(), memory.begin());

  test_8_bit();
  test_16_bit();


//  auto decoder = esetvm2::core::Decoder{memory};
//  auto instr = decoder.decode(0);
  std::stringstream ss;

  for (std::size_t i = 0; i < 10; i++) {
    ss << std::to_string(i) << " ";
  }




  return 0;
}

/*
 * -> fill 32 bits
 * -> get  8 bits
 * -> get 32 bits
 *
 */

