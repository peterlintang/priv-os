
include ../make.defines
include ../make.rules

INC_DIR := /home/john/lr33k/include
SRCS := printf.c
OBJS := printf.o
AR_FLAGS := crs
TARGETS  := libmc.a

all: $(TARGETS)


libmc.a: $(OBJS)
	@$(AR) $(AR_FLAGS) $@ $^ 
clean:
	@$(RM) -rf $(OBJS) $(TARGETS)

