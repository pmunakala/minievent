#ifndef __EVENT_TIMER_H_
#define __EVENT_TIMER_H_
typedef void (*timer_handler_t)(void *);
typedef struct {
    struct timespec event_ts;
    uint64_t expiry_ms;
    void *data;
    uint64_t repeat;
    timer_handler_t timer_cb;
} event_timer_watch_t;

void init_timerlib() __attribute__((constructor));

int
event_timer_init(event_timer_watch_t *ev_tm,
           timer_handler_t cb,
           uint64_t time,
           uint64_t repeat);
#endif
