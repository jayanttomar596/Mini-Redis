# Mini Redis (In-Memory Key-Value Store in C++)

### Multithreaded TCP Server • LRU Cache • TTL Expiry • WAL Persistence

This project implements a **Redis-like in-memory key-value store written in C++** that demonstrates several core backend system design concepts.

It supports **fast O(1) key-value operations**, **multithreaded client handling**, **Write-Ahead Logging (WAL)** for persistence, **LRU-based eviction**, and **TTL-based expiry**.

The system is designed with **clean modular architecture, concurrency control, and performance optimization**, making it a strong demonstration of **DSA, Operating Systems, Computer Networks, and DBMS concepts**.

---

# Demo

## Working Video
https://youtu.be/kNOoiKyg9EY

---

# Project Highlights

## Fast Key-Value Store
- Uses **unordered_map (hash map)** for near O(1) access
- Supports core operations:
  - `SET key value`
  - `GET key`
  - `DEL key`

## TCP Server (Client-Server Architecture)
- Built using low-level **socket programming**
- Uses:
  - `socket()`, `bind()`, `listen()`, `accept()`
- Enables remote command execution via clients

## Multithreaded Request Handling
- Each client handled in a **separate thread**
- Supports concurrent connections
- Improves system scalability

## Thread Safety
- Implemented using:
  - `std::mutex`
  - `lock_guard`
- Prevents race conditions in shared data access

## Write-Ahead Logging (Persistence)
- All write operations logged to:


data.log


- On restart:
- Server replays log
- Restores previous state

## LRU Cache (Memory Management)
- Implements **Least Recently Used eviction**
- Uses:
- `unordered_map` + `doubly linked list`
- Automatically removes least used keys when capacity is exceeded

## TTL Expiry (Time-To-Live)
- Supports key expiration:


SET key value EX seconds


- Uses:
- Timestamp-based expiry
- Lazy deletion during access

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
WAL (data.log)

```



---

## Components

| Layer | Responsibility |
|------|---------------|
| Client | Sends commands |
| Server | Handles TCP connections |
| CommandParser | Parses input commands |
| KVStore | Stores key-value data |
| Logger (WAL) | Logs operations for persistence |

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
└── data.log
```





---

# How the System Works

## Request Handling

When a client sends a command:


SET name Jayant


Steps:

1. Client sends request via TCP  
2. Server receives raw input  
3. CommandParser tokenizes input  
4. KVStore executes operation  
5. Response sent back to client  

---

## Persistence Flow

For write operations:


SET / DEL


Steps:

1. Operation executed in memory  
2. Logged in `data.log`  
3. On restart → log replay restores state  

---

## LRU Eviction

When capacity is exceeded:

1. Least recently used key identified  
2. Removed from cache  
3. New key inserted  

---

## TTL Expiry

For command:


SET otp 1234 EX 10


Steps:

1. Expiry timestamp stored  
2. On GET:
- Check expiry
- If expired → delete key  

---

# Example Commands

### Basic Operations
```
SET name Jayant
GET name
DEL name
```

### TTL Example
```
SET otp 1234 EX 10
```



---

# Time Complexity

| Operation | Complexity |
|-----------|-----------|
| SET | O(1) |
| GET | O(1) |
| DEL | O(1) |
| LRU Update | O(1) |
| TTL Check | O(1) |

---

# Concepts Used

- HashMap (unordered_map)
- Doubly Linked List (LRU)
- TCP Socket Programming
- Multithreading
- Mutex Synchronization
- Write-Ahead Logging
- Lazy Expiry Mechanism
- System Design Principles

---

# Future Improvements

- Master-Slave Replication
- Read scaling using replicas
- Snapshot-based persistence
- REST API interface
- Distributed architecture

---

# Key Learning

Built a **production-style backend system** combining:

- Networking
- Concurrency
- Memory management
- Persistence
- Caching strategies

This project closely mirrors the design principles of **real-world Redis**.

---

# Author

**Jayant Tomar**

Computer Science Engineering — Delhi Technological University

Focus Areas:
- Backend Systems
- Distributed Systems
- System Design
- Performance Optimization










