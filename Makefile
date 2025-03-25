CFILES = $(wildcard src/*.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Iinclude -Wall -O2 -Werror -Wextra

all: calc.exe

%.o: %.c
	gcc $(GCCFLAGS) -c $^ -o $@

calc.exe: $(OFILES)
	gcc $(GCCFLAGS) $^ -o calc.exe