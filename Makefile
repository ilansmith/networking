CC=gcc
CFLAGS=-Wall -Werror -m64
APPS=server client
OBJS_SERVER=server.o
OBJS_CLIENT=client.o

ifeq ($(CONFIG_DEBUG),y)
    CFLAGS+=-g -O0
else
    CFLAGS+=-O3
endif

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: all clean cleanall

all: $(APPS)

$(APP): $(OBJS_`echo $@ | tr a-z A-Z`)
	$(CC) -o $@ $^

clean:
	@echo "removing object files"
	@rm -f *.o

cleanall: clean
	@echo "removing tag file"
	@rm -f tags
	@echo "removing executables"
	@rm -f $(APP)

