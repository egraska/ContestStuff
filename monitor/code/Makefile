TARGET = monitor
LIBS = -lcrypto -lpthread -lgmp -lm
CC = gcc
CFLAGS = -g -Wall
VPATH = cbor commands


.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c)) $(patsubst %.c, %.o, $(wildcard cbor/*.c)) $(patsubst %.c, %.o, $(wildcard commands/*.c))
HEADERS = $(wildcard *.h) $(wildcard cbor/*.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f commands/*.o
	-rm -f cbor/*.o
