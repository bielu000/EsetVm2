//
// Created by pp on 10.10.19.
//

#pragma once

#include <vector>
#include <cstdint>
#include <endian.h>
#include <stdexcept>
#include <cmath>

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
      : mem_(mem) {}

    MemBitStream(const MemBitStream& str) = delete;
    MemBitStream& operator= (const MemBitStream& str) = delete;

    void loadAtLeastBits(uint8_t bits)
    {
      /*
       * 1)
       * bits_left   = 32
       * mem_offset  = 0
       *
       * 2) stream.get<8>()
       * bits_left   = 24
       * mem_offset  = 0
       *
       * 3) stream.get<32>()
       *
       *
       */
      auto bitsToLoad = static_cast<uint8_t >(std::abs(bitsLeft - bits));
      bitsLeft += bitsToLoad;
      auto mask = 0;

      for (size_t i = 0; i < bitsToLoad; i++) {
        mask = mask << 1;
        mask |= 1;
      }

      if (bitsToLoad <= 8) {
        auto reqData = mem_->read<uint8_t>(memOffset_);
        reqData &= mask; // mask need to be calculated in better way
        data_ |= reqData;
        memOffset_ += static_cast<uint16_t>(bitsToLoad / 8);
      }
      else if (bitsToLoad <= 16) {
        auto reqData = mem_->read<uint16_t>(memOffset_);
        reqData = htobe16(reqData);
        reqData &= mask;
        data_ |= reqData;
        memOffset_ += static_cast<uint16_t>(bitsToLoad / 8);
      }
      else if (bitsToLoad <= 32) {
        auto reqData = mem_->read<uint32_t>(memOffset_);
        reqData = htobe32(reqData);
        reqData &= mask;
        data_ |= reqData;
        memOffset_ += static_cast<uint16_t>(bitsToLoad / 8);
      }
    }

    template<uint8_t T>
    auto get()
    {
      if (data_ == 0) {
//        auto x = mem_->read<uint32_t>(memOffset_);
//        data_ = htobe32(x);
        loadAtLeastBits(32);
      }

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

        return ret;

      }
      else if constexpr (T <= 16) {
        auto ret = static_cast<uint16_t>(data_ >> (32 - T));
        data_ = data_ << T;

        return ret;

      }
      else if constexpr (T <= 32) {
        auto ret = static_cast<uint32_t>(data_ >> (32 - T));
        data_ = data_ << (T - 1);
        data_ = data_ << 1;

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