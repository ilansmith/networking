CC=gcc
CFLAGS=-Wall -Werror -m64 -ansi -pedantic-errors
APP=showip
OBJS=showip.o

ifeq ($(CONFIG_DEBUG),y)
    CFLAGS+=-g -O0
else
    CFLAGS+=-O3
endif

%.o: %.c
	$(CC) -o $@ $(CFLAGS) -c $<

.PHONY: all clean cleanall

all: $(APP)

$(APP): $(OBJS)
	$(CC) -o $@ $^

clean:
	@echo "removing object files"
	@rm -f *.o

cleanall: clean
	@echo "removing tag file"
	@rm -f tags
	@echo "removing executables"
	@rm -f $(APP)

