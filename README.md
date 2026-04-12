# Mini Redis (In-Memory Key-Value Store in C++)

### Multithreaded TCP Server • LRU Cache • TTL (Heap Optimized) • WAL • Snapshot (RDB) • Replication • Initial Sync

This project implements a **Redis-like in-memory key-value store written in C++** that demonstrates advanced backend system design concepts.

It supports **O(1) key-value operations**, **multithreaded client handling**, **Write-Ahead Logging (WAL)**, **snapshot-based persistence**, **LRU eviction**, **heap-optimized TTL expiry**, and **master-slave replication with initial synchronization**.

The system is designed with **modular architecture, concurrency control, fault handling, and performance optimization**, making it a strong demonstration of **DSA, Operating Systems, Computer Networks, and DBMS concepts**.

---

# Demo

## Working Video
https://youtu.be/kNOoiKyg9EY

---

# Project Highlights

## Fast Key-Value Store
- Uses **unordered_map (hash map)** for near O(1) access
- Supports operations:
  - `SET key value`
  - `GET key`
  - `DEL key`
  - `EXISTS key`
  - `TTL key`
  - `INCR key`
  - `DECR key`

---

## TCP Server (Client-Server Architecture)
- Built using low-level **socket programming**
- Uses:
  - `socket()`, `bind()`, `listen()`, `accept()`
- Supports multiple clients over network

---

## Multithreaded Request Handling
- Each client handled in a **separate thread**
- Supports concurrent connections
- Improves responsiveness and throughput

---

## Connection Control
- Maximum client limit enforced
- Prevents server overload
- Returns `"Server busy"` when limit exceeded

---

## Graceful Shutdown
- Handles `Ctrl + C` using signal handling
- Stops accepting new connections
- Closes sockets cleanly
- Prevents abrupt termination issues

---

## Thread Safety
- Uses:
  - `std::mutex`
  - `std::lock_guard`
- Ensures safe concurrent access to shared data

---

## Write-Ahead Logging (WAL)
- All write operations logged to:
data.log
- Ensures durability
- Enables recovery after crash

---

## Snapshot Persistence (RDB)
- Periodically saves full database state to:
snapshot.rdb

- Runs every 10 seconds
- Improves startup time significantly

---

## WAL + Snapshot Integration
- Hybrid persistence model:
  - Snapshot → full database state
  - WAL → recent changes

Startup flow:
1. Load snapshot
2. Replay WAL

---

## WAL Compaction
- After snapshot:
  - WAL (`data.log`) is cleared
- Prevents unbounded log growth

---

## LRU Cache (Memory Management)
- Implements **Least Recently Used eviction**
- Uses:
  - `unordered_map` + `doubly linked list`
- Removes least recently accessed keys when capacity is exceeded

---

## TTL Expiry (Heap Optimized)
- Supports:
SET key value EX seconds

- Uses:
  - **Min-heap (priority queue)** for expiry tracking
- Complexity improved:
  - From O(n) → O(log n)
- Efficient expiration without full scan

---

## Background Expiry Thread
- Runs periodically
- Removes expired keys using heap
- Avoids blocking main operations

---

## Logging System
- Replaced raw `cout` with structured logging:
  - INFO
  - DEBUG
  - ERROR
- Includes timestamps
- Improves observability and debugging

---

## Replication (Master-Slave)

### Command-Based Replication
- Master forwards write commands:
  - SET
  - DEL
  - INCR
  - DECR

- Slave re-executes commands

---

## Initial Sync (State Synchronization)
- When slave connects:
  1. Master sends full snapshot
  2. Slave loads snapshot
  3. Then receives live updates

- Ensures consistency for late-joining slaves

---

## Fault Handling
- Detects:
  - Client disconnect
  - Slave disconnect
  - Socket errors
- Removes dead connections safely

---

# System Architecture
```
Client
↓
TCP Server
↓
Command Parser
↓
KV Store (RAM)
↓
Persistence Layer
├── WAL (data.log)
└── Snapshot (snapshot.rdb)
↓
Replication Layer
└── Slave Nodes
```



---

# Components

| Layer | Responsibility |
|------|---------------|
| Client | Sends commands |
| Server | Handles TCP connections and routing |
| CommandParser | Parses input into tokens |
| KVStore | Core data storage + LRU + TTL |
| Logger | Structured logging |
| WAL | Durable logging of writes |
| Snapshot | Full state persistence |
| Replication | Sync between master and slaves |

---

# Project Structure
```
MiniRedis/
│
├── store/
│ ├── KVStore.h
│ └── KVStore.cpp
│
├── parser/
│ ├── CommandParser.h
│ └── CommandParser.cpp
│
├── server/
│ ├── Server.h
│ └── Server.cpp
│
├── utils/
│ ├── Logger.h
│ └── Logger.cpp
│
├── main.cpp
├── client.cpp
├── data.log
└── snapshot.rdb
```



---

# How the System Works

## Request Flow

Example:
SET name Jayant



Steps:
1. Client sends command via TCP
2. Server receives raw input
3. CommandParser tokenizes input
4. KVStore executes operation
5. Response returned to client
6. WAL updated (if write operation)
7. Command forwarded to slaves (if master)

---

## Persistence Flow

### Write Operation:
SET / DEL / INCR / DECR


Steps:
1. Execute in memory
2. Append to WAL (`data.log`)
3. Forward to slaves

---

### Snapshot Flow:

1. Periodically save full state
2. Clear WAL
3. Maintain compact persistence

---

## TTL Expiry Flow
SET otp 1234 EX 10


Steps:
1. Store expiry timestamp
2. Push into min-heap
3. Background thread checks heap
4. Removes expired keys efficiently

---

## Replication Flow

1. Slave connects to master
2. Master sends snapshot
3. Slave loads snapshot
4. Master sends live commands
5. Slave replays commands

---

# Example Commands

### Basic Operations
SET name Jayant
GET name
DEL name

### TTL
SET otp 1234 EX 10
TTL otp



### Existence
EXISTS name


### Increment / Decrement

INCR counter
DECR counter




---

# Time Complexity

| Operation | Complexity |
|-----------|-----------|
| SET | O(1) |
| GET | O(1) |
| DEL | O(1) |
| EXISTS | O(1) |
| TTL | O(1) |
| INCR / DECR | O(1) |
| LRU Update | O(1) |
| TTL Expiry | O(log n) |

---

# Concepts Used

- HashMap (unordered_map)
- Doubly Linked List (LRU)
- Min Heap (priority queue)
- TCP Socket Programming
- Multithreading
- Mutex Synchronization
- Write-Ahead Logging
- Snapshot Persistence
- Replication Systems
- Event-driven Expiry
- System Design Principles

---

# Future Improvements

- Replication ACK system (reliability)
- Event-driven server (epoll / select)
- Pipelining support
- Distributed sharding
- REST API / Web dashboard
- Metrics and monitoring

---

# Key Learning

Built a **production-style backend system** combining:

- Networking
- Concurrency
- Memory management
- Persistence (WAL + Snapshot)
- Replication and synchronization
- Performance optimization

This project closely reflects the architecture of **real-world Redis and distributed systems**.

---

# Author

**Jayant Tomar**

Computer Science Engineering — Delhi Technological University

Focus Areas:
- Backend Systems
- Distributed Systems
- System Design
- Performance Optimization




















