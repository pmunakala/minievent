# minievent
# Event Timer Library in C

This project implements a scalable **event timer system** in C using:

- **`epoll`** for non-blocking event polling
- **Min-heap** for efficient timer scheduling
- **Monotonic clock** to ensure accurate timing, immune to system clock changes

---

## 🔧 Features

- One-shot and repeating timers
- Millisecond-precision scheduling
- Efficient O(log n) timer insertion and O(1) timer expiry check using a min-heap
- Non-blocking event loop using `epoll`
- Simple API for initializing, starting, and restarting timers

---

## 📦 Project Structure

```bash
.
├── event_timer.c          # Timer implementation
├── event_timer.h          # Timer API definitions
├── minheap.c              # Min-heap implementation
├── minheap.h              # Heap interface
├── main.c                 # Sample usage / test
└── README.md              # This file
