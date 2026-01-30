CC:= gcc-13
CFLAGS := -std=c11 -Wall -Wextra -pedantic -O2 -g
LDFLAGS:=

#Valgrind
VALGRIND?=valgrind
VGFLAGS?= --leak-check=full --show-leak-kinds=all --track-origins=yes --errors-for-leak
kinds=all --error-exitcode=1
.PHONY: all clean check valgrind vg

all: sched

sched: sched.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

sched.o:sched.c
	$(CC) $(CFLAGS) -c $<

#Run sample input under Valgrind; fails build on leaks/errors
valgrind: sched
	$(VALGRIND) $(VGFLAGS) ./sched < tests/input.txt

vg: valgrind

clean:
	rm -f sched sched.o