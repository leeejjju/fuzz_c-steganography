CC = clang
CFLAG = -fsanitize=fuzzer,address,undefined -g
INPUT_DIR = inputs
.PHONY : all rw rr clean

all: clean readMsg.out writeMsg.out

tr: readMsg.out

tw: writeMsg.out

readMsg.out: stego.c fuzz_readMsg.c 
	$(CC) $(CFLAG) -o $@ $^

writeMsg.out: stego.c fuzz_writeMsg.c
	$(CC) $(CFLAG) -o $@ $^

rw:
	./writeMsg.out $(INPUT_DIR)
rr:
	./readMsg.out $(INPUT_DIR)

clean:
	rm -f *.out
