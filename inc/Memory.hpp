//
// Created by pp on 10.10.19.
//

#pragma once

#include <vector>

namespace esetvm2::core {
  class Memory {
  public:
    using iterator = std::vector<std::byte>::iterator;

    void alloc(size_t size)
    {
      data_.resize(size);
    }

    auto& getData() { return data_; } // just temp!!!

  private:
    std::vector<std::byte> data_;
  };
}