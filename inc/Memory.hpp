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

    void loadByte(uint8_t bitsToLoad)
    {
      spdlog::trace("Loading by 8 bits long, bits to load: {0}", bitsToLoad);

      auto x = mem_->read<uint8_t>(memOffset_);
      memOffset_ += sizeof(uint8_t);
      data_ |= (x >> (std::numeric_limits<uint8_t>::digits - bitsToLoad)); // 6 = 8bit - bitsToLoad (2)

      sync = [this, x, bitsToLoad]() {
        auto d = static_cast<uint8_t>(x << bitsToLoad);
        bitsLeft = static_cast<uint8_t>(std::numeric_limits<uint8_t>::digits - bitsToLoad);
        data_ |= (d << (std::numeric_limits<value_type>::digits - bitsLeft - bitsToLoad));
      };
    };

    void loadWord(uint8_t bitsToLoad)
    {
      spdlog::trace("Loading by 16 bits long, bits to load: {0}", bitsToLoad);

      auto x = htobe16(mem_->read<uint16_t>(memOffset_));
      memOffset_ += sizeof(uint16_t);
      data_ |= (x >> (std::numeric_limits<uint16_t>::digits - bitsToLoad)); // 6 = 8bit - bitsToLoad (2)

      sync = [this, x, bitsToLoad]() {
        auto d = static_cast<uint16_t>(x << bitsToLoad);
        bitsLeft = static_cast<uint8_t>(std::numeric_limits<uint8_t>::digits - bitsToLoad);
        data_ |= (d << (std::numeric_limits<value_type>::digits - bitsLeft - bitsToLoad));
      };
    }


    void loadAtLeastBits(uint8_t bits)
    {

      auto bitsToLoad = static_cast<uint8_t >(std::abs(bitsLeft - bits));

      spdlog::info("Bits left: {0}, bits to load: {1}", bitsLeft, bitsToLoad);

      bitsLeft += bitsToLoad;
      auto mask = 0;

      for (size_t i = 0; i < bitsToLoad; i++) {
        mask = mask << 1;
        mask |= 1;
      }


      //Woks just for 8 bits!!!!!
      auto x = mem_->read<uint8_t>(memOffset_);
      memOffset_ += sizeof(uint8_t);
      data_ |= (x >> 6); // 6 = 8bit - bitsToLoad (2)

      sync = [this, x, bitsToLoad]() {
        auto d = static_cast<uint8_t>(x << bitsToLoad);
        bitsLeft = static_cast<uint8_t>(std::numeric_limits<uint8_t>::digits - bitsToLoad);
        data_ |= (d << (std::numeric_limits<value_type>::digits - bitsLeft - bitsToLoad));
      };
    };

    void loadBits(uint8_t bitsToLoad)
    {

      if (bitsToLoad <= 8) {
        loadByte(bitsToLoad);
      }
      else if (bitsToLoad <= 16) {
        loadWord(bitsToLoad);
      }
    }

    template<uint8_t T>
    auto get()
    {
      if ((bitsLeft - T) < 0) {
        auto bitsToLoad = static_cast<uint8_t >(std::abs(bitsLeft - T));
        loadBits(bitsToLoad);

      }

      bitsLeft -= T;

      if constexpr (T <= std::numeric_limits<uint8_t>::digits) {
        auto ret = static_cast<uint8_t>(data_ >> (std::numeric_limits<value_type>::digits - T));
        data_ = data_ << T;

        sync();

        return ret;

      }
      else if constexpr (T <= std::numeric_limits<uint16_t>::digits) {
        auto ret = static_cast<uint16_t>(data_ >> (std::numeric_limits<value_type>::digits - T));
        data_ = data_ << T;

        sync();

        return ret;

      }
      else if constexpr (T <= std::numeric_limits<uint32_t>::digits) {
        auto ret = static_cast<uint32_t>(data_ >> (std::numeric_limits<value_type>::digits - T));
        data_ = data_ << (T - 1);
        data_ = data_ << 1;

        sync();

        return ret;
      }
    }
    uint8_t bitsLeft { 0 };
    value_type data_{ 0 };
    uint16_t memOffset_{ 0 };
    Memory const * mem_;
  };
}