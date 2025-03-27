/* pow.c - Proof-of-Work implementation */

#include "pow.h"

#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "deichain.h"

int get_max_transaction_reward(const TransactionBlock *block,
                               const int txs_per_block) {
  if (block == NULL) return 0;

  int max_reward = 0;
  for (int i = 0; i < txs_per_block; ++i) {
    int reward = block->transactions[i].reward;
    if (reward > max_reward) {
      max_reward = reward;
    }
  }

  return max_reward;
}

unsigned char *serialize_block(const TransactionBlock *block, size_t *sz_buf) {
  // We must subtract the size of the pointer, the static block does not have
  // the pointer
  *sz_buf = get_transaction_block_size() - sizeof(Transaction *);

  unsigned char *buffer = malloc(*sz_buf);
  if (!buffer) return NULL;

  unsigned char *p = buffer;

  memcpy(p, block->txb_id, TXB_ID_LEN);
  p += TXB_ID_LEN;

  memcpy(p, block->previous_block_hash, HASH_SIZE);
  p += HASH_SIZE;

  memcpy(p, &block->timestamp, sizeof(time_t));
  p += sizeof(time_t);

  for (size_t i = 0; i < transactions_per_block; ++i) {
    memcpy(p, &block->transactions[i], sizeof(Transaction));
    p += sizeof(Transaction);
  }

  memcpy(p, &block->nonce, sizeof(unsigned int));
  p += sizeof(unsigned int);

  return buffer;
}

/* Function to compute SHA-256 hash */
void compute_sha256(const TransactionBlock *block, char *output) {
  unsigned char hash[SHA256_DIGEST_LENGTH];
  // create a static buffer to copy data
  size_t buffer_sz;

  // since the Block has a pointer we must serialize the block to a buffer
  unsigned char *buffer = serialize_block(block, &buffer_sz);

  SHA256(buffer, buffer_sz, hash);
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    sprintf(output + (i * 2), "%02x", hash[i]);
  }
  output[SHA256_DIGEST_LENGTH * 2] = '\0';
  free(buffer);
}

/* Function to check difficulty using fractional levels */
int check_difficulty(const char *hash, const int reward) {
  int minimum = 4;  // minimum difficult

  int zeros = 0;
  DifficultyLevel difficulty = getDifficultFromReward(reward);

  while (hash[zeros] == '0') {
    zeros++;
  }

  // At least `minimum` zeros must exist
  if (zeros < minimum) return 0;

  char next_char = hash[zeros];

  switch (difficulty) {
    case EASY:  // 0000[0-b]
      if ((zeros == 4 && next_char <= 'b') || zeros > 4) return 1;
      break;
    case NORMAL:  // 00000
      if (zeros >= 5) return 1;
      break;
    case HARD:  // // 00000[0-b]
      if ((zeros == 5 && next_char <= 'b') || zeros > 5) return 1;
      break;
    default:
      fprintf(stderr, "Invalid Difficult\n");
      exit(2);
  }

  return 0;
}

/* Function to verify a nonce */
int verify_nonce(const TransactionBlock *block) {
  char hash[SHA256_DIGEST_LENGTH * 2 + 1];
  int reward = get_max_transaction_reward(block, transactions_per_block);
  compute_sha256(block, hash);
  return check_difficulty(hash, reward);
}

/* Proof-of-Work function */
PoWResult proof_of_work(TransactionBlock *block) {
  PoWResult result;

  result.elapsed_time = 0.0;
  result.operations = 0;
  result.error = 0;

  block->nonce = 0;

  int reward = get_max_transaction_reward(block, transactions_per_block);

  char hash[SHA256_DIGEST_LENGTH * 2 + 1];
  clock_t start = clock();

  while (1) {
    compute_sha256(block, hash);

    if (check_difficulty(hash, reward)) {
      result.elapsed_time = (double)(clock() - start) / CLOCKS_PER_SEC;
      strcpy(result.hash, hash);
      return result;
    }
    block->nonce++;
    if (block->nonce > POW_MAX_OPS) {
      fprintf(stderr, "Giving up\n");
      result.elapsed_time = (double)(clock() - start) / CLOCKS_PER_SEC;
      result.error = 1;
      return result;
    }
    if (DEBUG && block->nonce % 100000 == 0)
      printf("Nounce %d\n", block->nonce);
    result.operations++;
  }
}

DifficultyLevel getDifficultFromReward(const int reward) {
  if (reward <= 1)
    // 0000
    return EASY;

  if (reward == 2)
    // 00000
    return NORMAL;

  // For reward > 2
  // 00000[0-b]
  return HARD;
}
