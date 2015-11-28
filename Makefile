HEADERS = functions.h
CC = clang
CFLAGS = -lpcap -lpthread -g -O0

default: s_switch

s_switch: main.c
	$(CC) $(CFLAGS) main.c -o bin/s_switch



debug:
	gdb -q --command=debug_commands.gdb bin/s_switch

clean:
	-rm -f bin/s_switch
