
include make.defines
include make.rules

SRCS := main.c 	\
	malloc.c \
	ring_buf.c \
	proc.c
OBJS := main.o	\
	malloc.o \
	ring_buf.o \
	proc.o
TARGETS  := hello_os

all: $(TARGETS)


hello_os: $(OBJS)
	@$(CC) -static -o $@ $^ 
	@$(OBJDUMP) -D hello_os > hello.dis
clean:
	@$(RM) -rf $(OBJS) $(TARGETS) hello.dis

