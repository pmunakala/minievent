#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <event_timer.h>

event_timer_watch_t event_tm1[10];
event_timer_watch_t event_tm[10];
void timer_cb (void *data) {
    struct timespec ts;
    printf("Timer expiryt %p \n", data);
    // CLOCK_MONOTONIC - time since boot (not affected by system time changes)
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
//        printf("CLOCK_MONOTONIC: %ld.%09ld seconds\n", ts.tv_sec, ts.tv_nsec);
    } else {
        perror("clock_gettime (MONOTONIC)");
    }
}

int main() {
    // Create epoll instance
    // Nothing is added to the epoll set (it's empty)
    int i = 0;
    printf("Waiting for 3 seconds using epoll_wait() with no fds...\n");
	 for (i = 0; i < 10; i++) {
		  event_timer_init(&event_tm[i], timer_cb, 3000, 5000);
		  event_timer_start(&event_tm[i]);
	 }

    // epoll_wait with timeout = 3000 ms (3 seconds)
    event_main_loop();
    return 0;
}
