//
// Created by pp on 10.10.19.
//

#pragma once

#include <memory>
#include <string>
#include <Memory.hpp>
#include <Cpu.hpp>
#include <Loader.hpp>

namespace esetvm2::core {
  class Evm
  {
  public:
    Evm() = default;
    ~Evm() = default;
    Evm(const Evm&) = delete;
    Evm(Evm&&) = default;

    Evm& operator= (Evm&&) = default;
    Evm& operator= (const Evm&) = delete;

    void run(const std::string& app);
  };
}