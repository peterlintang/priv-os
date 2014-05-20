
include ../make.defines
include ../make.rules

INC_DIR := /home/john/lr33k/include
SRCS := printf.c	\
		malloc.c	\
		scanf.c
OBJS := printf.o	\
		malloc.o	\
		scanf.o
AR_FLAGS := crs
TARGETS  := libmc.a

all: $(TARGETS)


libmc.a: $(OBJS)
	@$(AR) $(AR_FLAGS) $@ $^ 
clean:
	@$(RM) -rf $(OBJS) $(TARGETS)

