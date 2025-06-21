 # MIT License
 # 
 # File:         Makefile
 # Description:  Build script for the event timer library
 # Author:      Pradeep Kumar Munakala
 # Copyright:   (c) 2025 Pradeep Munakala
 # 
 # Permission is hereby granted, free of charge, to any person obtaining a copy
 # of this software and associated documentation files (the "Software"), to deal
 # in the Software without restriction, including without limitation the rights
 # to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 # copies of the Software, and to permit persons to whom the Software is
 # furnished to do so, subject to the following conditions:
 # 
 # The above copyright notice and this permission notice shall be included in
 # all copies or substantial portions of the Software.
 # 
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 # IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 # FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 # AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 # LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 # OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 # SOFTWARE.

CC := gcc
CFLAGS := -g -fpic -I./

SHARED_LIB := timerlib
DEMO_APP := DemoApp
BUILD_DIR := $(CURDIR) 
OBJS := event_timer.o  min_heap.o
APP_OBJS := test/test_timer.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(SHARED_LIB) $(DEMO_APP)

$(SHARED_LIB): $(OBJS)
	$(CC) -shared $(OBJS) -o lib$(SHARED_LIB).so

$(DEMO_APP): $(SHARED_LIB) $(APP_OBJS)
	$(CC) -o $(DEMO_APP) $(APP_OBJS) -L$(BUILD_DIR) -l$(SHARED_LIB)


.PHONY:  all clean

#clean up
clean: $(SHARED_LIB)
	rm -f *.o *.so test/*.o $(DEMO_APP)
