/* main.c - Testing program */
#include "pow.h"
#include "pow.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TX_ID_LEN 64
#define TXB_ID_LEN 64
#define HASH_LEN 64
#define TRANSACTIONS_PER_BLOCK 10  // This should be set based on your config

// Transaction structure
typedef struct {
    char tx_id[TX_ID_LEN];     // Unique transaction ID (e.g., PID + #)
    int reward;                // Reward associated with PoW
    float value;               // Quantity or value transferred
    time_t timestamp;          // Creation time of the transaction
} Transaction;

// Transaction Block structure
typedef struct {
    char txb_id[TXB_ID_LEN];                 // Unique block ID (e.g., ThreadID + #)
    char previous_block_hash[HASH_LEN];      // Hash of the previous block
    time_t timestamp;                        // Time when block was created
    Transaction transactions[TRANSACTIONS_PER_BLOCK];  // Array of transactions
    unsigned int nonce;                      // PoW solution
} TransactionBlock;


const char *block_data_samples[] = {
    "Block A",
    "Transaction from Alice to Bob",
    "A much longer block containing multiple transactions and metadata",
    "Minimal",
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."};

const int num_samples = 50;
int difficulty = 6;
const int num_difficults = 6;
const int initial_difficult = 4;

/* Function to generate a random alphanumeric string */
void generate_random_string(int size, char *buffer)
{
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    int charset_size = sizeof(charset) - 1;

    for (int i = 0; i < size; i++)
    {
        buffer[i] = charset[rand() % charset_size];
    }
    buffer[size] = '\0';
}

void initial_test()
{
    for (int j = 0; j < num_difficults; j++)
    {
        difficulty = j + initial_difficult;

        for (int i = 0; i < num_samples; i++)
        {
            printf(
                "\nPoW, difficulty %d,  block: \"%s\"\n",
                difficulty, block_data_samples[i]);
            PoWResult result = proof_of_work(block_data_samples[i], difficulty);
            printf("Nonce: %d\n", result.nonce);
            if (!result.error)
                printf("Hash: %s\n", result.hash);
            else
                printf("Aborted\n");

            printf("Elapsed Time: %.4f seconds\n", result.elapsed_time);
            printf("Operations: %d\n", result.operations);
            printf("Hash Rate: %.2f ops/sec\n", result.operations / result.elapsed_time);
            fflush(stdin);
        }
    }
}

void test_difficult()
{
    // Will test difficult genreating many sample

    char block_test[250];

    for (DifficultyLevel d = HARD; d <= HARD; d++)
    {

        for (int i = 0; i < num_samples; i++)
        {
            generate_random_string((rand() % 100) + 150, block_test);
            printf(
                "\nPoW, difficulty %d,  block: \"%s\"\n",
                d, block_test);
            PoWResult result = proof_of_work(block_test, d);

            printf("Nonce: %d\n", result.nonce);
            printf("Hash: %s\n", result.hash);
            printf("Elapsed Time: %.4f seconds\n", result.elapsed_time);
            printf("Operations: %d\n", result.operations);
            printf("Hash Rate: %.2f ops/sec\n", result.operations / result.elapsed_time);
        }
    }
}

int main()
{
    srand(time(NULL));
    test_difficult();
    return 0;
}
