CC     = gcc
CFLAGS = -Wall -Wextra -I./lib -I./input
SRCS   = main.c src/mongoose/mongoose.c src/files/input.c src/files/output.c src/text/text.c src/mongoose/mongoose_responses.c
OUT    = server

.PHONY: all run clean

all: $(OUT)

$(OUT): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(OUT)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
