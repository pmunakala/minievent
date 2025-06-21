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
