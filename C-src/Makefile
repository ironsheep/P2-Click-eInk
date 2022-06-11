CC=gcc
CFLAGS=-I.
TARGET=convert
DEPS = $(TARGET).h
OBJ = $(TARGET).o argparse.o


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

all: $(TARGET)

clean:
	rm -f *.o *~ core $(TARGET) *.spin2
