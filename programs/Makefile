
include ../make.defines
include ../make.rules

SRCS := main.c
OBJS := main.o
TARGETS  := hello_os

all: $(TARGETS)


hello_os: $(OBJS)
	@$(CC) -static -o $@ $^ ../kernel/kernel.a ../mlibc/libmc.a
	@$(OBJDUMP) -D hello_os > hello.dis
clean:
	@$(RM) -rf $(OBJS) $(TARGETS) hello.dis

