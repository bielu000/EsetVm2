//
// Created by pp on 02.09.19.
//

#pragma once

#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <numeric>

namespace esetvm2::file_format {
  enum class EvmFileError { FileNotFound, InvalidSignature, InvalidSize };

  class InvalidEvmFile : public std::exception
  {
  public:
    explicit InvalidEvmFile(EvmFileError errorCode)
      : errorCode_(errorCode) {}

  private:
    EvmFileError errorCode_;
  };

  class EvmExecutable {
  public:
    explicit EvmExecutable(const std::string& fname) {
      fillHeader(fname);
    }

  private:
    void fillHeader(const std::string& fname)
    {
      std::ifstream file(fname, std::ios_base::in | std::ios_base::binary);

      if (!file) {
        throw InvalidEvmFile(EvmFileError::FileNotFound);
      }

      file.read(magic_.data(), magic_.size());

      if (auto x = std::string(magic_.begin(), magic_.end()); x != "ESET-VM2") {
        throw InvalidEvmFile(EvmFileError::InvalidSignature);
      }

      //Numbers within evm file are stored in little endian.
      //Assuming we are on little endian CPU.
      file.read(reinterpret_cast<char*>(&codeSize_), sizeof(codeSize_));
      file.read(reinterpret_cast<char*>(&dataSize_), sizeof(dataSize_));
      file.read(reinterpret_cast<char*>(&initialDataSize_), sizeof(initialDataSize_));

      fileSize_ = std::filesystem::file_size(fname);

      if (!sizeIsValid()) {
        throw InvalidEvmFile(EvmFileError::InvalidSize);
      }
    }

    bool sizeIsValid()
    {
      return fileSize_ == (codeSize_ + initialDataSize_ + headerSize());
    }

    size_t constexpr headerSize()
    {
      return
        sizeof(magic_) +
        sizeof(codeSize_) +
        sizeof(dataSize_) +
        sizeof(initialDataSize_);
    }

  private:
    //Format specific
    std::array<char, 8> magic_;
    uint32_t codeSize_;
    uint32_t dataSize_;
    uint32_t initialDataSize_;
    // --------------------- //

    uintmax_t fileSize_;
  };
}