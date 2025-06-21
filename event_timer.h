/*
 * MIT License
 * 
 * File:        event_timer.h
 * Description: Interface file for timer library.
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

int event_timer_restart(event_timer_watch_t *ev_tm);
int event_timer_start(event_timer_watch_t *ev_tm);
int event_main_loop(void);
#endif
