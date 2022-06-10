CC=gcc
CFLAGS=-I.
DEPS = convert.h
OBJ = convert.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

convert: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

all: convert

clean:
	rm -f *.o *~ core convert image_coffee.spin2
