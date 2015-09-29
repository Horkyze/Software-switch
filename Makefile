HEADERS = functions.h

default: s_switch

s_switch: main.c
	clang -lpcap -g main.c -o bin/s_switch

debug:
	gdb -q --command=debug_commands.gdb bin/s_switch

clean:
	-rm -f bin/s_switch
