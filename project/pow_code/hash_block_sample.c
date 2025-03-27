/* main.c - Testing program */
#include <pthread.h>  // For pthread_self()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  // For getpid()

#include "deichain.h"
#include "pow.h"

#define LEDGER_SIZE 4

size_t transactions_per_block = 0;

TransactionBlock ledger[LEDGER_SIZE];

Transaction generate_random_transaction(int tx_number) {
  Transaction tx;

  // Generate a unique tx_id (e.g., PID + transaction number)
  snprintf(tx.tx_id, TX_ID_LEN, "TX-%d-%d", getpid(), tx_number);

  // Reward: initially from 1 to 3
  tx.reward = rand() % 3 + 1;

  // 3 % chance of aging, 1% of doubling aging
  int age_chance = rand() % 101;
  if (age_chance <= 3) tx.reward++;
  if (age_chance <= 1) tx.reward++;

  // Value: random float between 0.01 and 100.00
  tx.value = ((float)(rand() % 10000)) / 100.0f + 0.01f;

  // Timestamp: now
  tx.timestamp = time(NULL);

  return tx;
}

TransactionBlock generate_random_block(const char *prev_hash,
                                       int block_number) {
  TransactionBlock block;

  // Generate a unique block ID using thread ID + number
  pthread_t tid = pthread_self();
  snprintf(block.txb_id, TXB_ID_LEN, "BLOCK-%lu-%d", (unsigned long)tid,
           block_number);

  // Copy the previous hash
  strncpy(block.previous_block_hash, prev_hash, HASH_SIZE);

  block.transactions =
      (Transaction *)malloc(sizeof(Transaction) * transactions_per_block);

  // Fill with random transactions
  for (int i = 0; i < transactions_per_block; ++i) {
    block.transactions[i] = generate_random_transaction(i);
  }

  PoWResult r;
  do {
    // Timestamp: current time
    block.timestamp = time(NULL);
    printf("Computing the PoW with timestamp %ld\n", block.timestamp);
    r = proof_of_work(&block);

  } while (r.error == 1);

  return block;
}

void print_block_info(TransactionBlock *block) {
  // Print basic block info
  printf("Block ID: %s\n", block->txb_id);
  printf("Previous Hash:\n%s\n", block->previous_block_hash);
  printf("Block Timestamp: %ld\n", block->timestamp);
  printf("Nonce: %u\n", block->nonce);
  printf("Transactions:\n");

  for (int i = 0; i < transactions_per_block; ++i) {
    Transaction tx = block->transactions[i];
    printf("  [%d] ID: %s | Reward: %d | Value: %.2f | Timestamp: %ld\n", i,
           tx.tx_id, tx.reward, tx.value, tx.timestamp);
  }
}

void blkcpy(TransactionBlock *dest, TransactionBlock *src) {
  dest->nonce = src->nonce;
  dest->timestamp = src->timestamp;
  dest->transactions = src->transactions;
  strcpy(dest->txb_id, src->txb_id);
  strcpy(dest->previous_block_hash, src->previous_block_hash);
}

void dump_ledger() {
  printf("\n=================== Start Ledger ===================\n");
  for (int i = 0; i < LEDGER_SIZE; i++) {
    printf("||----  Block %03d -- \n", i);
    print_block_info(&(ledger[i]));
    printf("||------------------------------ \n");
  }
  printf("=================== End   Ledger ===================\n");
}

int main() {
  char hash_buffer[HASH_SIZE];

  srand(time(NULL));  // Seed RNG

  // Must set this value that is defined in the "deichain.h"
  transactions_per_block = 3;

  // create a zeroed block to generate the inital hash
  TransactionBlock empty_block;
  memset(&empty_block, 0, sizeof(TransactionBlock));
  empty_block.transactions =
      (Transaction *)calloc(transactions_per_block, sizeof(Transaction));
  PoWResult r;
  r = proof_of_work(&empty_block);
  if (r.error) {
    perror("Could not compute the Hash\n");
    exit(1);
  }
  strcpy(hash_buffer, r.hash);
  print_block_info(&empty_block);

  printf("Hash Inicial: %s\n", hash_buffer);

  for (int i = 0; i < LEDGER_SIZE; i++) {
    // Generate a sample block
    TransactionBlock test_block = generate_random_block(hash_buffer, i);
    // Check if the block is complient with difficult
    if (!verify_nonce(&test_block)) {
      perror("Block has an invalid nonce\n");
      exit(1);
    }
    // compute the hash to pass to the next block as previous hash
    compute_sha256(&test_block, hash_buffer);
    // put on ledger
    blkcpy(&ledger[i], &test_block);
    // print information
    print_block_info(&test_block);
  }

  dump_ledger();

  return 0;
}
