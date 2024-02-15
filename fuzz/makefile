CC = clang
all: clean readMsg writeMsg

readMsg: stego.c fuzz_readMsg.c 
	$(CC) -o $@ $^

writeMsg: stego.c fuzz_writeMsg.c
	$(CC) -o $@ $^

clean:
	rm -f readMsg writeMsg out.ppm
