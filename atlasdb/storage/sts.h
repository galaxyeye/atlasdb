/*
* cursor.h
*
*  Created on: Oct 6, 2011
*      Author: vincent
*/

#ifndef STORAGE_STATE_H_
#define STORAGE_STATE_H_

#include <exception>
#include <string>

namespace atlasdb {
  namespace storage {

    enum _s_state {
      _s_goodbit     = 0,
      _s_badbit     = 1L << 0,
      _s_eofbit     = 1L << 1,
      _s_failbit    = 1L << 2,
      _s_state_end   = 1L << 16
    };

    inline _s_state operator&(_s_state __a, _s_state __b)
    { return _s_state(static_cast<int>(__a) & static_cast<int>(__b)); }

    inline _s_state operator|(_s_state __a, _s_state __b)
    { return _s_state(static_cast<int>(__a) | static_cast<int>(__b)); }

    inline _s_state operator^(_s_state __a, _s_state __b)
    { return _s_state(static_cast<int>(__a) ^ static_cast<int>(__b)); }

    inline _s_state operator~(_s_state __a)
    { return _s_state(~static_cast<int>(__a)); }

    inline const _s_state& operator|=(_s_state& __a, _s_state __b)
    { return __a = __a | __b; }

    inline const _s_state& operator&=(_s_state& __a, _s_state __b)
    { return __a = __a & __b; }

    inline const  _s_state& operator^=(_s_state& __a, _s_state __b)
    { return __a = __a ^ __b; }

    class sts {
    public:
      typedef _s_state sstate;

    public:
      sts() : _state(_s_goodbit), _exception(_s_goodbit) { }

    public:
      /**
      *  @brief These are thrown to indicate problems with put/get
      *
      */
      class failure : public std::exception
      {
      public:
        explicit failure(const std::string& msg) : _msg(msg) {}

        virtual ~failure() throw();

        virtual const char* what() const throw() { return _msg.c_str(); }

      private:
        std::string _msg;
      };

      /// Indicates a loss of integrity in an input or output sequence (such
      /// as an irrecoverable read error from a file).
      static const sstate badbit =  _s_badbit;

      /// Indicates that an input operation reached the end of an input sequence.
      static const sstate eofbit =  _s_eofbit;

      /// Indicates that an input operation failed to get the expected
      /// data node, or that an output operation failed to generate the
      /// data node.
      static const sstate failbit =  _s_failbit;

      /// Indicates all is well.
      static const sstate goodbit =  _s_goodbit;

    public:

      // ^-^, we can put/get more data node into the storage
      bool good() const { return this->rdstate() == 0; }

      // ^p^, we got the last data node when we use a cursor
      // to retrieve all matched nodes
      bool eof() const { return (this->rdstate() & eofbit) != 0; }

      // -.-, failed to put/get, please, try again
      bool fail() const { return (this->rdstate() & (badbit | failbit)) != 0; }

      // -!-, can not process any more, the storage is broken
      bool bad() const { return (this->rdstate() & badbit) != 0; }

      bool operator!() const { return this->fail(); }

      // read current status
      sstate rdstate() const { return _state; }

      // set additional status flag
      void setstate(sstate f) { this->clear(this->rdstate() | f); }

      void clear(sstate f = goodbit) {
        if (true) _state = f;
        else _state = f | badbit;

        if (this->exceptions() & this->rdstate())
          throw std::exception();
      }

      /**
      *  @brief  Throwing exceptions on errors.
      *  @return  The current exceptions mask.
      *
      *  This changes nothing in the storage.
      */
      sstate exceptions() const { return _exception; }

      void exceptions(sstate ex) {
        _exception = ex;
        clear(_state);
      }

    protected:
      sstate _state;
      sstate _exception;
    };

    /*void sts::clear(sstate f) {
      // TODO : This condition
      if (true) _state = f;
      else _state = f | badbit;

      if (this->exceptions() & this->rdstate())
        throw std::exception();
    } // end class sts*/
  } // end namespace storage
} // end namespace atlasdb

#endif /* STORAGE_STATE_H_ */
