/*
 * transaction.h
 *
 *  Created on: May 16, 2013
 *      Author: vincent
 */

#ifndef ATLASDB_STORAGE_TRANSACTION_H_
#define ATLASDB_STORAGE_TRANSACTION_H_

namespace atlasdb {
  namespace storage {

    class transaction {
    public:

      transaction() {}
      transaction(const transaction& other) = delete;
      transaction(const transaction& other);

    public:

      void begin();
      void commit();
      void rollback();

      void undo();
      void redo();
      void roll();
      void purge();
    };

  } // storage
} // atlasdb

#endif /* ATLASDB_STORAGE_TRANSACTION_H_ */
