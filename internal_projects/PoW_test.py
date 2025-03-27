import hashlib
import time


def proof_of_work(block_data, difficulty=4):
    """
    Performs proof-of-work by finding a valid nonce that produces 
    a hash with a given difficulty.
    """
    
    nonce = 0
    prefix = "0" * difficulty  # The required number of leading zeros
    start_time = time.time()  # Start timing

    while True:
        # Combine block data with nonce
        text = f"{block_data}{nonce}".encode()
        hash_result = hashlib.sha256(text).hexdigest()

        # Check if the hash meets the difficulty target
        if hash_result.startswith(prefix):
            elapsed_time = time.time() - start_time  # Compute elapsed time
            return (
                nonce,
                hash_result,
                elapsed_time,
                nonce,
            )  # Return nonce, hash, time spent, and number of operations

        nonce += 1  # Try the next nonce


def format_time(seconds):
    """Formats time in an appropriate scale."""
    if seconds < 1e-3:
        return f"{seconds * 1e6:.2f} µs"
    elif seconds < 1:
        return f"{seconds * 1e3:.2f} ms"
    elif seconds < 60:
        return f"{seconds:.2f} s"
    else:
        return f"{seconds / 60:.2f} min"


def test_proof_of_work(trials=5, difficulties=[3, 4, 5, 6]):
    """Runs proof-of-work tests for different difficulty levels and computes average statistics."""
    block_data = "Test Block"  # Fixed block content for comparison
    results = []

    for difficulty in difficulties:
        total_time = 0
        total_operations = 0

        print(f"\nTesting difficulty {difficulty} ({'0' * difficulty}... prefix)")

        for _ in range(trials):
            nonce, valid_hash, elapsed_time, operations = proof_of_work(
                block_data, difficulty
            )
            total_time += elapsed_time
            total_operations += operations

            print(
                f" - Nonce: {nonce}, Time: {format_time(elapsed_time)}, Ops: {operations}, Hash: {valid_hash[:10]}..."
            )

        avg_time = total_time / trials
        avg_operations = total_operations / trials
        hash_rate = avg_operations / avg_time if avg_time > 0 else 0

        results.append((difficulty, avg_operations, avg_time, hash_rate))
        print(
            f" -> Avg Time: {format_time(avg_time)}, Avg Ops: {avg_operations:.2f}, Hash Rate: {hash_rate:.2f} ops/sec"
        )

    return results


# Run the test program
if __name__ == "__main__":
    test_proof_of_work()
