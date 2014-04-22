
include ../make.defines
include ../make.rules

SRCS := proc.c		\
#		setjmp.s	\
		start.s
OBJS := proc.o		\
#		setjmp.o	\
		start.o
AR_FLAGS := crs
TARGETS  := kernel.a

all: $(TARGETS)


kernel.a: $(OBJS)
	@$(AR) $(AR_FLAGS) $@ $^
clean:
	@rm -rf $(OBJS) $(TARGETS)

