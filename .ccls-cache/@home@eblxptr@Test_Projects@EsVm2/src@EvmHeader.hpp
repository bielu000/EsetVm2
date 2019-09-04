//
// Created by pp on 02.09.19.
//

#ifndef ESETVM2_EVM2HEADER_HPP
#define ESETVM2_EVM2HEADER_HPP
#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>

namespace esetvm2 {
  namespace file_format {

    enum class EvmFileError { FileNotFound, InvalidSignature, InvalidSize };

    class InvalidEvmFile : public std::exception
    {
    public:
      explicit InvalidEvmFile(EvmFileError errorCode)
        : errorCode_(errorCode) {}

    private:
      EvmFileError errorCode_;
    };

    class EvmHeader 
    {	    
    public:
      explicit EvmHeader(const std::string& fname) 
	: evmFname_(fname)
      {
        fillHeader();
      }

    private:
      void fillHeader()
      {
        std::ifstream file(evmFname_, std::ios_base::in | std::ios_base::binary);

        if (!file) {
          throw InvalidEvmFile(EvmFileError::FileNotFound);
        }

        file.read(magic_.data(), magic_.size());

	if (!signatureIsValid()) {
	  throw InvalidEvmFile(EvmFileError::InvalidSignature);
	}

        //Numbers within evm file are stored in little endian.
        //Assuming we are on little endian CPU.
        file.read(reinterpret_cast<char*>(&codeSize_), sizeof(codeSize_));
        file.read(reinterpret_cast<char*>(&dataSize_), sizeof(dataSize_));
        file.read(reinterpret_cast<char*>(&initialDataSize_), sizeof(initialDataSize_));

        if (!sizeIsValid()) {
	  throw InvalidEvmFile(EvmFileError::InvalidSize);
        }
      }

      bool signatureIsValid()
      {
	return (std::string(magic_.begin(), magic_.end()) == "ESET-VM2");
      }

      bool sizeIsValid()
      {
	auto fsize = std::filesystem::file_size(evmFname_);

	return fsize == (headerSize() + codeSize_ + initialDataSize_);
      }

      std::size_t constexpr headerSize()
      {
        return
          sizeof(magic_) +
          sizeof(codeSize_) +
          sizeof(dataSize_) +
          sizeof(initialDataSize_);
      }

    private:
      std::array<char, 8> magic_;
      uint32_t codeSize_;
      uint32_t dataSize_;
      uint32_t initialDataSize_;

      std::string evmFname_;
    };
  }
}

#endif //ESETVM2_EVM2HEADER_HPP
