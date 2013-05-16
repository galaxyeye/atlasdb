/*
 * buffer.h
 *
 *  Created on: May 15, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_BUFFER_H_
#define ATLASDB_BUFFER_H_

#include <array>
#include <cassert>

namespace atlasdb {

  namespace { struct useless {}; }

  template<size_t LowWater, size_t HighWater, size_t Capacity>
  class buffer {
  public:

    static_assert(LowWater <= HighWater, "bad low water");
    static_assert(HighWater < Capacity, "bad high water");

  public:

    buffer() : _i_low(LowWater), _i_high(HighWater) {}

    void swap(buffer& other) {
      _buffer.swap(other._buffer);
      std::swap(_i_low, other._i_low);
      std::swap(_i_high, other._i_high);
    }

    size_t dirty_space_size() const { return _i_high - _i_low; }

    size_t above_high_water_size() const { return _buffer.size() - _i_high; }

    size_t under_low_water_size() const { return _i_low; }

    template<typename T>
    void retrieve() {
      retrieve(sizeof(int32_t));
    }

    void retrieve(size_t len) {
      if (len < dirty_space_size()) {
        _i_low += len;
      }
      else {
        retrieve_all();
      }
    }

    void retrieve_all() {
      _i_low = 0;
      _i_high = 0;
    }

    template<typename T>
    T retrieve_as() { return retrieve_as<T>(dirty_space_size()); }

    template<typename T>
    T retrieve_as(size_t len) {
      assert(len <= dirty_space_size());

      T result(peek(), len);
      retrieve(len);
      return result;
    }

    template<typename T, typename std::enable_if<std::is_pod<T>::value, useless>::type = useless>
    void append(const T& t) {
      append(static_cast<const char*>(t), sizeof(t));
    }

    void append(const char* data, size_t len) {
      std::copy(data, data + len, high_water());
      raise_high_water(len);
    }

    void append(const void* data, size_t len) { append(static_cast<const char*>(data), len); }

    void prepend(const void* data, size_t len) {
      _i_low -= len;
      const char* d = static_cast<const char*>(data);
      std::copy(d, d + len, _buffer.begin() + _i_low);
    }

    template<typename T, typename std::enable_if<std::is_pod<T>::value, useless>::type = useless>
    T read() {
      T result = peek<T>();
      retrieve<T>();
      return result;
    }

    template<typename T, typename std::enable_if<std::is_pod<T>::value, useless>::type = useless>
    T peek() const {
      assert(dirty_space_size() >= sizeof(T));

      return static_cast<T*>(reinterpret_cast<T*>(_buffer.data() + _i_low));
    }

    const char* peek() const { return _buffer.data() + _i_low; }

    char* peek() { return _buffer.data() + _i_low; }

    const char* low_water() const { return _buffer.begin() + _i_low; }

    char* low_water() { return _buffer.begin() + _i_low; }

    const char* high_water() const { return _buffer.begin() + _i_high; }

    char* high_water() { return _buffer.begin() + _i_high; }

    void lower_low_water(size_t len) { _i_low += len; }

    void raise_high_water(size_t len) { _i_high += len; }

  private:

    size_t _i_low;
    size_t _i_high;
    std::array<char, Capacity> _buffer;
  };

} // atlasdb

#endif /* BUFFER_H_ */
