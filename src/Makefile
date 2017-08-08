CC=gcc

TARGET_TEST=hasher

TARGET_OBJ=hasher.o

SRC=hasher.c

CFLAGS += -Wall

default: $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(TARGET_OBJ)

test: $(SRC)
	$(CC) $(CFLAGS) -DTEST_HASH $(SRC) -o $(TARGET_TEST)

clean:
	rm -rf $(TARGET_OBJ) $(TARGET_TEST)
