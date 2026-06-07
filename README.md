# systems_monorepo

A collection of small-scale systems programming projects built for learning and exploration. Each project targets a specific low-level concept — implemented from scratch, minimal dependencies.

---

## Projects

### Event Broker — Observer Pattern

**`src/event_broker.cpp`**

A publish-subscribe event system. Producers publish typed payloads onto named topics via `std::any`; consumers register callbacks and get back an integer token they can later use to unsubscribe. The broker owns a topic map and cleans up empty topic entries automatically on unsubscribe.

---

### Inter-Process Pipeline

**`src/ipp.c`**

Two processes chained over a Unix pipe. The first child redirects its stdout into the write-end via `dup2`, the second redirects its stdin from the read-end, and both `execvp` into their respective commands. The parent closes both ends and waits. Mirrors what a shell does for `cmd1 | cmd2`.

---

### Thread-Safe Ring Buffer

**`src/ts_ring.c`**

A fixed-capacity (128-byte) lock-free circular buffer for single-producer single-consumer use. `push` and `pop` coordinate through `head` and `tail` atomics using acquire/release ordering — no mutex, no condition variables.

---

### Virtual File System

**`src/vfs.cpp`**

An in-memory filesystem tree built on a `Node` hierarchy — `File` holds a string, `Directory` owns a map of `unique_ptr<Node>` children. A `VFS` layer exposes `mkdir`, `touch`, `deleteNode`, and `get_size` over Unix-style paths, resolving them by splitting on `/` and walking to the parent directory.

---

### Intrusive Circular Doubly Linked List

**`src/instrusive_cdll.c`**

The `list_node` struct is embedded directly inside `task_struct` rather than holding a pointer to it. A `GET_ENTRY` macro (built on `GET_OFFSET`) recovers the parent struct from a node pointer by subtracting the member's offset — the same pattern as Linux's `container_of`. The list is circular: `init_list` points a sentinel node at itself, and `add_tail` splices new nodes in before the head.

---
