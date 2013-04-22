/*
 * ir_operate.h
 *
 *  Created on: 2012-9-29
 *      Author: vincent
 */

#ifndef atlasdb_QUERY_ALGORITHM_H_
#define atlasdb_QUERY_ALGORITHM_H_

#include <algorithm>

namespace atlasdb {
  namespace query {

    template<class KeySet, class Range>
    void keyset_and(KeySet& keys, Range& range) {
      typedef typename Range::iterator It;
      std::for_each(range.begin(), range.end(), [&keys](It& it) {
        keys.insert(it->second);
      });
    }

    template<class KeySet, class Range>
    void keyset_or(KeySet& keys, Range& range) {
      typedef typename Range::iterator It;
      std::for_each(range.begin(), range.end(), [&keys](It& it) {
        keys.insert(it->second);
      });
    }

    template<class KeySet, class KeySet2, class Range>
    void keyset_and_in(KeySet& output, KeySet2& input, Range& range) {
      typedef typename KeySet2::interator It;
      std::for_each(input.begin(), input.end(), [&range](It it) {
        if (std::any_of(input.begin(), input.end(), *it)) {
          output.insert(*it);
        }
      });
    }

    template<class KeySet, class KeySet2, class Range>
    void keyset_and_not_in(KeySet& output, KeySet2& input, Range& range) {
      typedef typename KeySet2::interator It;
      std::for_each(input.begin(), input.end(), [&range](It it) {
        if (!std::any_of(input.begin(), input.end(), *it)) {
          output.remove(*it);
        }
      });
    }

    template<class KeySet, class KeySet2, class Range>
    void keyset_or_in(KeySet& output, KeySet2& input, Range& range) {
      typedef typename KeySet2::interator It;
      std::for_each(input.begin(), input.end(), [&range](It it) {
        if (std::any_of(input.begin(), input.end(), *it)) {
          output.insert(*it);
        }
      });
    }

    template<class KeySet, class KeySet2, class Range>
    void keyset_or_not_in(KeySet& output, KeySet2& input, Range& range) {
      typedef typename KeySet2::interator It;
      std::for_each(input.begin(), input.end(), [&range](It it) {
        if (!std::any_of(input.begin(), input.end(), *it)) {
          output.insert(*it);
        }
      });
    }

    template<class KeySet, class KeySet2, class Range>
    void keyset_and_like(KeySet& output, KeySet2& input, Range& range) {
      typedef typename Range::interator It;
      std::for_each(range.begin(), range.end(), [&range](It it) {
        if (!std::any_of(input.begin(), input.end(), std::bind(like, *it))) {
          output.remove(*it);
        }
      });
    }

    template<class KeySet, class KeySet2, class Range>
    void keyset_or_like(KeySet& output, KeySet2& input, Range& range) {
      typedef typename KeySet2::interator It;
      std::for_each(input.begin(), input.end(), [&range](It it) {
        if (std::any_of(input.begin(), input.end(), std::bind(like, *it))) {
          output.insert(*it);
        }
      });
    }

  } // query
} // atlasdb

#endif /* atlasdb_QUERY_ALGORITHM_H_ */
