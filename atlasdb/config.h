#ifndef atlasdb_DATABASE_CONFIG_H_
#define atlasdb_DATABASE_CONFIG_H_

#define atlasdb_VERSION 0.1

#define atlasdb_HAS_NON_TRIVAL_STORAGE 1

#define CLUSTER_SINGLE_NODE 1

// For network buffer

namespace atlasdb {

  const size_t prepended_gap_size = sizeof(unsigned long int);

  const size_t reserved_buffer_size = 1024u * 1024u * 1024u + 100;

} // atlasdb

#endif // atlasdb_DATABASE_CONFIG_H_
