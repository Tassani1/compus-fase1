# Makefile for test and simulation targets (relative to test/)

CC=gcc
CFLAGS=-Wall -Wextra -I. -Ilib -I..

# Test runner
TEST_OBJS=../TAD_TIMER.c ../TAD_INTENSITY.c lib/mock_hw.c test_main.c
TEST_BIN=bin/test_runner

test: $(TEST_BIN)

$(TEST_BIN): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(TEST_OBJS) -o $(TEST_BIN)

# Simulation of main.c (without main.c itself, sim_main.c provides main)
SIM_OBJS=../TAD_TIMER.c ../TAD_INTENSITY.c ../TAD_SERIAL.c ../TAD_TECLAT.c ../TAD_CONTROLLER.c ../TAD_LEDS.c ../TAD_HALL.c ../TAD_SPEAKER.c lib/mock_hw.c sim_main.c
SIM_BIN=bin/sim_main

sim: $(SIM_BIN)

$(SIM_BIN): $(SIM_OBJS)
	$(CC) $(CFLAGS) $(SIM_OBJS) -o $(SIM_BIN)

.PHONY: test sim clean

clean:
	rm -f $(TEST_BIN) $(SIM_BIN)
