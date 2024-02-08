all: clean readMsg writeMsg

readMsg: stego.c readMsg.c readMsg.h fuzz_readMsg.c 
	$(CC) -o readMsg stego.c readMsg.c fuzz_readMsg.c

writeMsg: stego.c writeMsg.c
	$(CC) -o writeMsg stego.c writeMsg.c

clean:
	rm -f readMsg writeMsg out.ppm
