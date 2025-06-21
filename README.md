# 🕒 Event Timer Library in C

A lightweight, scalable event timer library in C using **epoll** and a **min-heap**. Ideal for applications that require high-performance timer management without relying on `timerfd` or external dependencies.

## ✨ Features

- One-shot and repeating timers
- Millisecond precision
- Efficient scheduling with a min-heap (O(log n))
- Non-blocking event loop with `epoll`
- Clean and modular C interface


## 📁 Project Structure
├── event_timer.c       # Timer implementation
├── event_timer.h       # Timer interface
├── minheap.c           # Min-heap implementation
├── minheap.h           # Min-heap interface
├── main.c              # Sample usage/test
└── README.md           # Project documentation

---

## 🧩 API Usage

### 1. Define a timer callback
    void my_callback(void *data) {
    printf("Timer fired!\n");
    }

### 2. create and start time
    event_timer_watch_t timer;
    event_timer_init(&timer,    my_callback, 1000, 0);  // One-shot timer
    event_timer_start(&timer);
### 3. Run the event loop
    event_main_loop();  // Blocking loop that dispatches timers

 📦 Dependencies
	•	Linux (uses epoll and clock_gettime)
	•	C standard library
	•	No third-party libraries required

🛠 Build Instructions

make 

👤 Author

Pradeep Munakala
Feel free to fork, use, and contribute!
  

