#ifndef __DEICHAIN_H__
#define __DEICHAIN_H__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TX_ID_LEN 64
#define TXB_ID_LEN 64
#define HASH_SIZE 65  // SHA256_DIGEST_LENGTH * 2 + 1

extern size_t transactions_per_block;

// Transaction structure
typedef struct {
  char tx_id[TX_ID_LEN];  // Unique transaction ID (e.g., PID + #)
  int reward;             // Reward associated with PoW
  float value;            // Quantity or value transferred
  time_t timestamp;       // Creation time of the transaction
} Transaction;

// Transaction Block structure
typedef struct {
  char txb_id[TXB_ID_LEN];              // Unique block ID (e.g., ThreadID + #)
  char previous_block_hash[HASH_SIZE];  // Hash of the previous block
  time_t timestamp;                     // Time when block was created
  Transaction *transactions;            // Array of transactions
  unsigned int nonce;                   // PoW solution
} TransactionBlock;

// Inline function to compute the size of a TransactionBlock
static inline size_t get_transaction_block_size() {
  if (transactions_per_block == 0) {
    perror("Must set the 'transactions_per_block' variable before using!\n");
    exit(-1);
  }
  return sizeof(TransactionBlock) +
         transactions_per_block * sizeof(Transaction);
}

#endif