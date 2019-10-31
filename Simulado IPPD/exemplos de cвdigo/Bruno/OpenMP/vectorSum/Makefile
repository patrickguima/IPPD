COMPILER = icc
FLAGS = -no-vec -qopenmp -xHost -Wall -Wextra

OBJSUM := $(patsubst %.c,%.o,$(wildcard src/*.c))
OBJSUMF := $(patsubst %.c,%.exec,$(wildcard src/*.c))

%.o: %.c
	$(COMPILER) $(FLAGS) -c -o $@ $<

all: main.o vsum
	@echo "Compiled!"

%.exec: %.o
	$(COMPILER) $(FLAGS) -o $@ $< main.o

vsum: $(OBJSUMF)

clean:
	rm -rf main.o $(OBJSUM) $(OBJSUMF)	