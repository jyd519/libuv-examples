# List of sources:
SOURCES = *.c
OBJECTS = $(SOURCES:.c=.o)

CFLAGS += `pkg-config --cflags libuv`
LDFLAGS += `pkg-config --libs libuv`

.c.o:
	$(CC) $(CFLAGS) $< -o $@

all:  dummy  test_uv_interface_addresses spawn-test spawn-detached


dummy : dummy.c
	$(CC) $^ -o $@ $(LDFLAGS)

signal: signal.c
	$(CC) $^ -o $@ $(LDFLAGS)

mkdir: mkdir.c
	$(CC) $^ -o $@ $(LDFLAGS)

test_interface_address: test_uv_interface_addresses.c
	$(CC) $^ -o $@ $(LDFLAGS)

spawn-test: spawn-test.c
	$(CC) $^ -o $@ $(LDFLAGS)


spawn-detached: spawn-detached.c
	$(CC) $^ -o $@ $(LDFLAGS)


clean:
	rm $(OBJECTS) $(EXECUTABLE)


#vim:set noet sts=2 sw=2 ts=2:
