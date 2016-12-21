CC=gcc
CFLAGS=-Wall -Werror -m64
APP=  ### FILL IN ### 
OBJS=  ### FILL IN ### 

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
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

clean:
	@echo "removing object files"
	@rm -f *.o

cleanall: clean
	@echo "removing tag file"
	@rm -f tags
	@echo "removing executables"
	@rm -f $(APP)

