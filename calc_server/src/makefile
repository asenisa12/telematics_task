INCLUDE_DIR=/home/devteam/openwrt/staging_dir/target-i386_i486_uClibc-0.9.33.2/usr/include

CC=/home/devteam/openwrt/staging_dir/toolchain-i386_i486_gcc-4.8-linaro_uClibc-0.9.33.2/bin/i486-openwrt-linux-gcc
LD=/home/devteam/openwrt/staging_dir/toolchain-i386_i486_gcc-4.8-linaro_uClibc-0.9.33.2/bin/i486-openwrt-linux-ld

LD_LIBRARY_PATH=home/devteam/openwrt/staging_dir/target-i386_i486_uClibc-0.9.33.2/usr/lib



LDFLAGS=-L/home/devteam/openwrt/staging_dir/target-i386_i486_uClibc-0.9.33.2/usr/lib -lpthread -lprotobuf-c -lrt

SOURCES=message.pb-c.c message_utils.c main.c  server.c 

OBJS=$(SOURCES:.c=.o)

all: calc_server

server.o: server.c 
	$(CC) -c -I $(INCLUDE_DIR) $(LDFLAGS) -o $@ $<

%.o: %.c %.h
	$(CC) -c -I $(INCLUDE_DIR)  $(LDFLAGS) -o $@ $<

calc_server: $(OBJS)
	$(CC) $(LDFLAGS) -o calc_server $(OBJS)

clean:
	rm *.o calc_server
