TARGET = cutt

CC = gcc
CFLAGS = -Wall

ifdef DEBUG
CFLAGS += -g
else
CFLAGS += -O2
endif

.PHONY: all clean

all: ${TARGET}

clean:
	@rm -fv cutt

%: %.c
	${CC} ${CFLAGS} -o $@ $^
