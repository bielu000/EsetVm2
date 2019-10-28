//
// Created by pp on 10.10.19.
//

#pragma once

#include <vector>
#include <cstdint>
#include <endian.h>
#include <stdexcept>
#include <cmath>
#include <functional>
#include <spdlog/spdlog.h>

namespace esetvm2::core {
  class Memory
  {
  public:
    using ValueType = std::vector<std::byte>;

    void alloc(size_t size)
    {
      data_.resize(size);
    }

    auto getData() { return data_; }

    template <typename T>
    auto read(uint16_t offset) const
    {
      T value {};

      auto readBegin = data_.begin() + offset;
      auto readEnd = data_.begin() + offset + sizeof(T);

      std::copy(readBegin, readEnd, reinterpret_cast<std::byte*>(&value));

      return value;
    }

    auto begin() { return data_.begin(); }
    auto end() { return data_.end(); }

  private:
    ValueType data_;
  };

  class MemBitStream
  {
  public:
    using value_type = std::uint32_t;

    explicit MemBitStream(Memory const * mem)
      : mem_(mem) {
      auto x = mem_->read<uint32_t>(memOffset_);
      data_ = htobe32(x);
      memOffset_ = 4;
      bitsLeft = 32;
    }

    MemBitStream(const MemBitStream& str) = delete;
    MemBitStream& operator= (const MemBitStream& str) = delete;

    std::function<void()> sync = [](){};

    void loadAtLeastBits(uint8_t bits)
    {
      /*
       * 1)
       * bits_left   = 32
       * mem_offset  = 0
       *
       * 2) stream.get<2>()
       * bits_left   = 30
       * mem_offset  = 0
       *
       * 3) stream.get<32>()
       *
       *
       */

      uint8_t bitsToLoad = static_cast<uint8_t >(std::abs(bitsLeft - bits));

      spdlog::info("Bits left: {0}, bits to load: {1}", bitsLeft, bitsToLoad);

      bitsLeft += bitsToLoad;
      auto mask = 0;

      for (size_t i = 0; i < bitsToLoad; i++) {
        mask = mask << 1;
        mask |= 1;
      }

      auto x = mem_->read<uint8_t>(memOffset_);
      data_ |= (x >> 6);

      sync = [this, x, bitsToLoad]() {
        auto d = static_cast<uint8_t>(x << bitsToLoad);
        bitsLeft = static_cast<uint8_t>(8 - bitsToLoad);
        data_ |= (d << (32 - bitsLeft - bitsToLoad));
      };
    };

    template<uint8_t T>
    auto get()
    {
//      if (data_ == 0) {
////        auto x = mem_->read<uint32_t>(memOffset_);
////        data_ = htobe32(x);
////        memOffset_ = 4;
//      }

      if ((bitsLeft - T) < 0) {
        loadAtLeastBits(T);
      }

      if ((bitsLeft - T) < 0) {
        throw std::runtime_error("Not enough bits to fetch");
      }

      bitsLeft -= T;

      if constexpr (T <= 8) {
        auto ret = static_cast<uint8_t>(data_ >> (32 - T));
        data_ = data_ << T;

        sync();

        return ret;

      }
      else if constexpr (T <= 16) {
        auto ret = static_cast<uint16_t>(data_ >> (32 - T));
        data_ = data_ << T;

        sync();

        return ret;

      }
      else if constexpr (T <= 32) {
        auto ret = static_cast<uint32_t>(data_ >> (32 - T));
        data_ = data_ << (T - 1);
        data_ = data_ << 1;

        sync();

        return ret;
      }
    }
    uint8_t bitsLeft { 0 };
    value_type data_{ 0 };
    value_type mask_{ 0xffffffff };
    uint16_t memOffset_{ 0 };
    Memory const * mem_;
  };
}