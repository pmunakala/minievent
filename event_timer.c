/*
 * MIT License
 * 
 * File:        event_timer.c
 * Description: Timer library implementation using epoll and min-heap.
 * Author:      Pradeep Kumar Munakala
 * Copyright:   (c) 2025 Pradeep Munakala
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <event_timer.h>
#include <time.h>
#include <min_heap.h>

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

int64_t compute_diff_millisecs (struct timespec *ev_ts, struct timespec *now)
{
    int64_t sec_diff = ev_ts->tv_sec - now->tv_sec;
    int64_t nsec_diff = ev_ts->tv_nsec - now->tv_nsec;
    if (sec_diff < 0 ) sec_diff = 0;
    if (nsec_diff < 0) nsec_diff = 0;
    if (!sec_diff &&  !nsec_diff) return 1; 
    return (sec_diff * 1000) + (nsec_diff / 1000000);
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
    return 0;
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
    return 0;
}

int timespec_cmp (const struct timespec *a, const struct timespec *b) {
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
    uint64_t timeout_ms = 1000;
    struct epoll_event events[1]; // still need to provide an array
    while(1) {
        int nfds = epoll_wait(poll_fd, events, 1, timeout_ms);
        if (nfds == -1) {
            perror("epoll_wait");
            close(poll_fd);
            exit(EXIT_FAILURE);
        } else if (nfds == 0) {
            if (clock_gettime(CLOCK_MONOTONIC, &global_ts) == 0) {
                while (!heap_is_empty(heap)) {
                    HeapItem item = heap_peek(heap);
          
                    if (timespec_cmp(&((event_timer_watch_t *)item.data)->event_ts,
                              &global_ts) > 0) {
			timeout_ms = compute_diff_millisecs(&((event_timer_watch_t *)item.data)->event_ts, &global_ts);
			//printf("Next timeout %ld\n", timeout_ms);
                        break;
                    }

                    item = heap_pop(heap);
                    ((event_timer_watch_t *)item.data)->timer_cb(item.data);
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
