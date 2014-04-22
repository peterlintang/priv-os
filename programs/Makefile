
include ../make.defines
include ../make.rules

SRCS := main.c
OBJS := main.o
TARGETS  := hello_k9

all: $(TARGETS)


hello_k9: $(OBJS)
	@$(CC) -static -o $@ $^ ../kernel/kernel.a
	@$(OBJDUMP) -D hello_k9 > hello.dis
clean:
	@$(RM) -rf $(OBJS) $(TARGETS) hello.dis

