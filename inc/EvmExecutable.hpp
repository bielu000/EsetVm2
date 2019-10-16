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
#include <vector>
#include <iterator>
#include <optional>
#include <algorithm>
#include <list>

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
    struct Header
    {
      std::array<char, 8> magic;
      uint32_t codeSize;
      uint32_t dataSize;
      uint32_t initialDataSize;

      constexpr size_t size() {
        return sizeof(magic) +
          sizeof(codeSize) +
          sizeof(dataSize) +
          sizeof(initialDataSize);
      }
    };

    class Section
    {
    public:
      enum class Type { Code, Data };

      using iterator = std::vector<std::byte>::iterator;
      using value_type = std::vector<std::byte>::value_type;

      Section(Section::Type type, size_t size)
        : type_(type), size_(size)
        {
          data_.resize(size);
        }

      auto getType() const { return type_; }
      auto begin() { return data_.begin(); }
      auto end() { return data_.end(); }
      std::byte* data() { return data_.data(); }

      auto setSize(size_t size) { size_ = size; }
      auto getSize() const { return size_; }

    private:
      Section::Type type_;
      std::vector<std::byte> data_;
      size_t size_;
    };

  public:
    explicit EvmExecutable(const std::string& path)
      : header_{},
        fileSize_{},
        path_(path)
    {
      fillHeader(path);
    }

    const Section* const getSection(Section::Type type) const
    {
      auto iter = std::find_if(sections_.begin(), sections_.end(),
        [&](const auto& x){ return x.getType() == type; });

      if (iter == sections_.end()) {
        return nullptr;
      }

      return &(*iter);
    }

    void loadSections()
    {
      if (!sections_.empty()) {
        return;
      }

      //Skip header
      std::string temp;
      std::ifstream file(path_, std::ios_base::in | std::ios_base::binary);
      file.read(temp.data(), header_.size());

      //Code section
      auto codeSection = Section{Section::Type::Code, header_.codeSize};
      file.read(reinterpret_cast<char*>(codeSection.data()), header_.codeSize);
      sections_.push_back(codeSection);

      //Data section
      if (header_.initialDataSize != 0) {
        auto dataSection = Section(Section::Type::Data, header_.initialDataSize);
        file.read(reinterpret_cast<char*>(dataSection.data()), header_.initialDataSize);

        sections_.push_back(dataSection);
      }
    }

  private:
    void fillHeader(const std::string& path)
    {
      std::ifstream file(path, std::ios_base::in | std::ios_base::binary);

      if (!file) {
        throw InvalidEvmFile(EvmFileError::FileNotFound);
      }

      file.read(header_.magic.data(), header_.magic.size());

      if (auto magic = std::string(header_.magic.begin(), header_.magic.end()); magic != "ESET-VM2") {
        throw InvalidEvmFile(EvmFileError::InvalidSignature);
      }

      //Numbers within evm file are stored in little endian.
      //Assuming we are on little endian CPU.
      file.read(reinterpret_cast<char*>(&header_.codeSize), sizeof(header_.codeSize));
      file.read(reinterpret_cast<char*>(&header_.dataSize), sizeof(header_.dataSize));
      file.read(reinterpret_cast<char*>(&header_.initialDataSize), sizeof(header_.initialDataSize));

      fileSize_ = std::filesystem::file_size(path);

      if (!sizeIsValid()) {
        throw InvalidEvmFile(EvmFileError::InvalidSize);
      }
    }

    bool sizeIsValid()
    {
      return fileSize_ ==
        (header_.codeSize + header_.initialDataSize + header_.size());
    }

  private:
    Header header_{};
    std::list<Section> sections_;
    uintmax_t fileSize_{};
    std::string path_;
  };
}