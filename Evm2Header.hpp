//
// Created by pp on 02.09.19.
//

#ifndef ESETVM2_EVM2HEADER_HPP
#define ESETVM2_EVM2HEADER_HPP

#include <array>
#include <exception>

namespace file_format {
  enum class

  class InvalidEFile : public std::exception
  {

  private:

  };

  class EFile {
  public:
    explicit Evm2Header(const std::string& fname) {

    }

  private:
    void fillHeader()
    {

    }

  private:
    std::array<char, 8> magic_;
    uint32_t codeSize;
    uint32_t dataSize;
    uint32_t initialDataSize;
  };

}

#endif //ESETVM2_EVM2HEADER_HPP
