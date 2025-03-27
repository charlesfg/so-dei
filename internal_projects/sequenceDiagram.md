sequenceDiagram
    participant User
    participant Controller
    participant TxGen as TransactionGenerators
    participant TransactionPool as TransactionPool
    participant Miner
    participant Validator
    participant Ledger as BlockchainLedger
    participant Log
    participant Statistics

    Controller->>Miner: (1) Spawn Miner (with M threads)
    Controller->>Validator: (1) Spawn Validator
    Controller->>Statistics: (1) Spawn Validator

    note over Miner,Validator: IPC, semaphores, message queues resources set up
    note over Controller, Statistics: Initialization complete
    
    loop Continuous (until termination condition)
        TxGen->>TransactionPool: (2) Send Transactions (via SharedMemory)
        Miner->>TransactionPool: (3) Check for transactions
        
        note over Miner: (3) Accumulate transactions

        Miner->>Miner: Perform simple PoW
        alt Proof Found
            Miner->>Validator: (4) Send new block for validation
            Validator->>Ledger: (5) Validate & Update BlockchainLedger
            Validator->>Log: (6) Safe the event (verified )
        else Proof Not Found
            Validator->>Log: (6) Safe the event (Rejection)
            Miner->>Miner: Retry PoW (loop)
        end
    end

        loop Verify (SIGUSR1 signal)
        User->>Statistics: (9) Send SIGUSR1 signal
        Log <<-->> Statistics: (9) Retrieve events
        note over Statistics: Perform statistic analysis
    end

    note over Controller: Termination reached
    Controller->>Miner: Send termination signal
    Controller->>Validator: Send termination signal
    Controller->>Statistics: Send termination signal
    note over TxGen: Cleanup and exit
    note over Miner: Cleanup and exit
    note over Validator: Cleanup and exit

    Controller->>Statistics: (7) Spawn statistics process
    Log <<-->> Statistics: (8) Retrieve events
    note over Statistics: Display metrics
    note over Statistics: Perform statistic analysis
    note over Statistics: Cleanup and exit
    note over Controller: Cleanup and exit


    3 100

    3  = reward + aging % 50

    0 -> 
    if (aging % 100 == 0) +=1

    0-50    -> 0     int(aging / 50)
    51-100  -> 1
    101-150 -> 2