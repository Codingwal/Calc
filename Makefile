CFILES = $(wildcard src/*.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Iinclude -Wall -O2 -Werror -Wextra

all: mycalc.exe

%.o: %.c
	gcc $(GCCFLAGS) -c $^ -o $@

mycalc.exe: $(OFILES)
	gcc $(GCCFLAGS) $^ -o $@

clean:
	del /S *.o