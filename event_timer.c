#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <event_timer.h>
#include <time.h>
#include <minheap.h>
MinHeap* heap = NULL;
struct timespec global_ts;

int event_timer_init (event_timer_watch_t *ev_tm,
                 timer_handler_t cb,
                 uint64_t time_ms, uint64_t repeat_ms) {
    if ((!ev_tm) || (!cb)) {
        printf("Invalid param");
        return -1;
    }
    //ev_tm->event_ts = 0;
    ev_tm->expiry_ms = time_ms;
    ev_tm->repeat = repeat_ms;
    ev_tm->data = ev_tm;
    ev_tm->timer_cb = cb;
    return 0;
}

void add_milliseconds(struct timespec *ts, uint64_t ms) {
    ts->tv_nsec += (ms % 1000) * 1000000L;
    ts->tv_sec  += ms / 1000;

    if (ts->tv_nsec >= 1000000000L) {
        ts->tv_sec += 1;
        ts->tv_nsec -= 1000000000L;
    }
}

int event_timer_restart (event_timer_watch_t *ev_tm)
{
    if ((!ev_tm)) {
        printf("Invalid param");
        return -1;
    }
    clock_gettime(CLOCK_MONOTONIC, &ev_tm->event_ts);
    add_milliseconds(&ev_tm->event_ts, ev_tm->repeat);
    heap_push(heap, ev_tm);
}


int event_timer_start (event_timer_watch_t *ev_tm)
{
    if ((!ev_tm)) {
        printf("Invalid param");
        return -1;
    }
    clock_gettime(CLOCK_MONOTONIC, &ev_tm->event_ts);
    add_milliseconds(&ev_tm->event_ts, ev_tm->expiry_ms);
    heap_push(heap, ev_tm);
}

int timespec_cmp(const struct timespec *a, const struct timespec *b) {
    if (a->tv_sec < b->tv_sec) return -1;
    if (a->tv_sec > b->tv_sec) return 1;
    if (a->tv_nsec < b->tv_nsec) return -1;
    if (a->tv_nsec > b->tv_nsec) return 1;
    return 0;  // equal
}

int event_main_loop (void) {
    int poll_fd = epoll_create1(0);
    if (poll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    int timeout_ms = 3000;
    struct epoll_event events[1]; // still need to provide an array
    while(1) {
        int nfds = epoll_wait(poll_fd, events, 1, timeout_ms);
        if (nfds == -1) {
            perror("epoll_wait");
            close(poll_fd);
            exit(EXIT_FAILURE);
        } else if (nfds == 0) {
            if (clock_gettime(CLOCK_MONOTONIC, &global_ts) == 0) {
                int count = 0;
                while (!heap_is_empty(heap)) {
                    HeapItem item = heap_peek(heap);
          
                    if (timespec_cmp(&((event_timer_watch_t *)item.data)->event_ts,
                              &global_ts) >= 0) {
                        break;
                    }

                    ((event_timer_watch_t *)item.data)->timer_cb(item.data);
                    item = heap_pop(heap);
                    if (((event_timer_watch_t *)item.data)->repeat > 0) { 
                        event_timer_restart(item.data);
                    } else {
                    }
                }
            }
              
        } else {
            printf("Unexpected: events returned even though epoll set is empty.\n");
        }
    }
    close(poll_fd);
}

int compare_ts (void *a, void *b)
{
    event_timer_watch_t *item_a = (event_timer_watch_t *)a; 
    event_timer_watch_t *item_b = (event_timer_watch_t *)b;
    return timespec_cmp(&item_a->event_ts, &item_b->event_ts);

}
void init_timerlib ()
{
    heap = heap_create(100, compare_ts);
}
