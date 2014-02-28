
include ../make.defines
include ../make.rules

SRCS := setjmp.s	\
		proc.c
OBJS := setjmp.o	\
		proc.o
AR_FLAGS := crs
TARGETS  := kernel.a

all: $(TARGETS)


kernel.a: $(OBJS)
	@$(AR) $(AR_FLAGS) $@ $^
clean:
	@rm -rf $(OBJS) $(TARGETS)

