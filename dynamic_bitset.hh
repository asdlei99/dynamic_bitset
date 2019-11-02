//
// Simple dynamic bitset implementation in C++11
//

/*
The MIT License (MIT)

Copyright (c) 2019 Syoyo Fujita.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#pragma once

#include <cassert>
#include <cstdint>
#include <vector>
#include <string>
#include <sstream>

// TODO(syoyo): Consider endianness

///
/// @brief dynamically allocatable bitset
///
class dynamic_bitset {
 public:
  dynamic_bitset() = default;
  dynamic_bitset(dynamic_bitset &&) = default;
  dynamic_bitset(const dynamic_bitset &) = default;

  dynamic_bitset &operator=(const dynamic_bitset &) = default;

  ~dynamic_bitset() = default;

  ///
  /// @brief Construct dynamic_bitset with given number of bits.
  ///
  /// @param[in] nbits The number of bits to use.
  ///
  explicit dynamic_bitset(size_t nbits) {
    _num_bits = nbits;

    size_t num_words;
    if (nbits < 8) {
      num_words = 1;
    } else {
      num_words = 1 + (nbits - 8) / 8;
    }

    _data.resize(num_words);

    // init with zeros
    std::fill_n(_data.begin(), _data.size(), 0);
  }

  ///
  /// @brief Resize dynamic_bitset.
  ///
  /// @details Resize dynamic_bitset. Resize behavior is similar to std::vector::resize.
  ///
  /// @param[in] nbits The number of bits to use.
  ///
  void resize(size_t nbits) {

    _num_bits = nbits;

    size_t num_words;
    if (nbits < 8) {
      num_words = 1;
    } else {
      num_words = 1 + (nbits - 8) / 8;
    }

    _data.resize(num_words);
  }

  ///
  /// @return The number of bits that are set to `true`
  ///
  uint32_t count() {

    uint32_t c = 0;

    for (size_t i = 0; i < _num_bits; i++) {
      c += (*this)[i] ? 1 : 0;
    }

    return c;
  }

  void reset() {
    std::fill_n(_data.begin(), _data.size(), 0);
  }

  void set(size_t pos, bool value = true) {
    size_t byte_loc = pos / 8;
    uint8_t offset = pos % 8;

    uint8_t bitfield = uint8_t(1 << offset);

    if (value == true) {
      // bit on
      _data[byte_loc] |= bitfield;
    } else {
      // turn off bit
      _data[byte_loc] &= (~bitfield);
    }
  }

  std::string to_string() const {
    std::stringstream ss;

    for (size_t i = 0; i < _num_bits; i++) {
      ss << ((*this)[_num_bits - i - 1] ? "1" : "0");
    }

    return ss.str();
  }

  bool operator[](size_t pos) const {
    size_t byte_loc = pos / 8;
    size_t offset = pos % 8;

    return (_data[byte_loc] >> offset) & 0x1;
  }

 private:
  size_t _num_bits{0};

  // bitfields are reprentated as an array of bytes.
  std::vector<uint8_t> _data;
};