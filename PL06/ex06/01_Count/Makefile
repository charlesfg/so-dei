TARGET=count

CFLAGS=-g -Wall -Wextra -Werror

CC=gcc

SOURCES=count.c

OBJECTS=$(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for cleaning up.
# This removes files generated during compilation.
clean:
	rm -f $(TARGET) $(OBJECTS) $(SOURCES:.c=.o)

# A phony target is one that is not really the name of a file.
# It is just a name for a recipe to be executed when you make an explicit request.
.PHONY: all clean