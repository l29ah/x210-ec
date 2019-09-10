all: checksum ec.bin
	./menu.sh
	./checksum ec.bin

checksum: checksum.o

.PHONY: clean

clean:
	rm -f *.o checksum
