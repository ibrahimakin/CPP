PWD = $(shell pwd)

CC		= gcc
LD		= ld

srctree		= $(PWD)/src

CFLAGS		:= -g -I$(srctree) -Wall
LDFLAGS		:= -static

src = main.c
obj = $(addprefix $(srctree)/, $(src:.c=.o))

target = swap.out

.PHONY: _all
_all: $(target)

$(target): $(obj)
	$(CC) $(LDFLAGS) -o $@ $^

%.o : %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	@rm -rf $(obj)
	@rm -rf $(target)